#include "ast.hpp"
#include "operator.hpp"
#include "source_code.hpp"
#include "token.hpp"
#include <optional>

using namespace adobans;

struct parser {
  private:
    std::vector<token> tokens;
    std::size_t        index = 0;

  public:
    const std::unique_ptr<operator_config> &op_config;
    std::vector<report>                     reports;

    parser(const std::vector<token> &tokens, const std::unique_ptr<operator_config> &op_config) : tokens(tokens), op_config(op_config) {}

    std::size_t size() const {
        return tokens.size();
    }

    void seek(std::size_t new_index) {
        index = new_index;
        if (index > size()) {
            index = size();
        }
    }

    std::size_t tell() const {
        return index;
    }

    token peek(std::size_t offset = 0) const {
        if (tell() + offset >= size()) {
            return token{
                .range = {
                    .begin = (std::size_t)-1,
                }
            };
        }

        return tokens[tell() + offset];
    }

    void advance(std::size_t amount = 1) {
        seek(tell() + amount);
    }

    void recede(std::size_t amount = 1) {
        if (amount > tell()) amount = tell();
        seek(tell() - amount);
    }

    std::size_t remaining() const {
        return size() - tell();
    }

    bool eof() const {
        return remaining() == 0;
    }

    bool is(token_type type) const {
        if (eof()) return false;
        return peek().type == type;
    }

    bool is(token_type type, token_value value) const {
        if (eof()) return false;
        return peek().type == type && peek().value == value;
    }

    void append_note(std::string_view message) {
        reports.emplace_back(report{
            .sev     = severity::note,
            .range   = peek().range,
            .message = std::string(message),
        });
    }

    void append_note(std::string_view message, text_range range) {
        reports.emplace_back(report{
            .sev     = severity::note,
            .range   = range,
            .message = std::string(message),
        });
    }

    void append_warning(std::string_view message) {
        reports.emplace_back(report{
            .sev     = severity::warning,
            .range   = peek().range,
            .message = std::string(message),
        });
    }

    void append_warning(std::string_view message, text_range range) {
        reports.emplace_back(report{
            .sev     = severity::warning,
            .range   = range,
            .message = std::string(message),
        });
    }

    void append_error(std::string_view message) {
        reports.emplace_back(report{
            .sev     = severity::error,
            .range   = peek().range,
            .message = std::string(message),
        });
    }

    void append_error(std::string_view message, text_range range) {
        reports.emplace_back(report{
            .sev     = severity::error,
            .range   = range,
            .message = std::string(message),
        });
    }

    void append_help(std::string_view message) {
        reports.emplace_back(report{
            .sev     = severity::help,
            .range   = peek().range,
            .message = std::string(message),
        });
    }

    void append_help(std::string_view message, text_range range) {
        reports.emplace_back(report{
            .sev     = severity::help,
            .range   = range,
            .message = std::string(message),
        });
    }

    bool is_int_lit() const {
        return is(token_type::int_lit);
    }

    bool is_int_lit(int int_value) const {
        return is(token_type::int_lit, int_value);
    }

    bool is_float_lit() const {
        return is(token_type::float_lit);
    }

    bool is_float_lit(float float_value) const {
        return is(token_type::float_lit, float_value);
    }

    bool is_string_lit() const {
        return is(token_type::string_lit);
    }

    bool is_string_lit(const std::string &string_value) const {
        return is(token_type::string_lit, string_value);
    }

    bool is_char_lit() const {
        return is(token_type::char_lit);
    }

    bool is_char_lit(char char_value) const {
        return is(token_type::char_lit, char_value);
    }

    bool is_bool_lit() const {
        return is(token_type::bool_lit);
    }

    bool is_bool_lit(bool bool_value) const {
        return is(token_type::bool_lit, bool_value);
    }

    bool is_identifier() const {
        return is(token_type::identifier);
    }

    bool is_identifier(const std::string &identifier) const {
        return is(token_type::identifier, identifier);
    }

    bool is_delimiter() const {
        return is(token_type::delimiter);
    }

    bool is_delimiter(char delimiter) const {
        return is(token_type::delimiter, delimiter);
    }

    bool is_keyword() const {
        return is(token_type::keyword);
    }

    bool is_keyword(const std::string &keyword) const {
        return is(token_type::keyword, keyword);
    }

    int get_int_lit() {
        return std::get<int>(peek().value);
    }

    float get_float_lit() {
        return std::get<float>(peek().value);
    }

    std::string get_string_lit() {
        return std::get<std::string>(peek().value);
    }

    char get_char_lit() {
        return std::get<char>(peek().value);
    }

    bool get_bool_lit() {
        return std::get<bool>(peek().value);
    }

    std::string get_identifier() {
        return std::get<std::string>(peek().value);
    }

    char get_delimiter() {
        return std::get<char>(peek().value);
    }

    std::string get_keyword() {
        return std::get<std::string>(peek().value);
    }
};

std::unique_ptr<ast_node> parse_expression(parser &parser, int minimum_precedence);

std::unique_ptr<ast_node> parse_primary(parser &parser) {
    if (parser.is_int_lit()) {
        auto node = make_int(parser.get_int_lit(), parser.peek().range);
        parser.advance();
        return std::move(node);
    }

    if (parser.is_delimiter('(')) {
        auto lparen_range = parser.peek().range;
        parser.advance();

        auto node = parse_expression(parser, 0);

        if (parser.is_delimiter(')')) {
            parser.advance();
        } else {
            parser.append_error("Expected ')'");
            parser.append_note("To match '('", lparen_range);
        }

        return node;
    }

    return nullptr;
}

std::unique_ptr<ast_node> parse_expression(parser &parser, int min_prec) {
    std::unique_ptr<ast_node> left = nullptr;

    if (parser.is_delimiter()) {
        std::string op(1, parser.get_delimiter());
        text_range  op_range = parser.peek().range;

        auto info = parser.op_config->get_prefix_info(op);
        if (info) {
            parser.advance();

            auto right = parse_expression(parser, info->prec);

            left = make_prefix_op(info.value(), std::move(right), op_range);
        }
    }

    if (!left) {
        left = parse_primary(parser);
        if (!left) {
            parser.append_error("Expected an expression");
            return nullptr;
        }
    }

    while (true) {
        if (!parser.is_delimiter()) {
            break;
        }

        std::string op(1, parser.get_delimiter());
        text_range  op_range = parser.peek().range;

        auto postfix_info = parser.op_config->get_postfix_info(op);
        auto infix_info   = parser.op_config->get_infix_info(op);

        // Hack: sets options to nullopt to choose higher precedence operator
        if (postfix_info && infix_info) {
            if (postfix_info->prec > infix_info->prec) {
                infix_info = std::nullopt;
            } else {
                postfix_info = std::nullopt;
            }
        }

        if (postfix_info) {
            if (postfix_info->prec < min_prec) {
                break;
            }

            parser.advance();
            left = make_postfix_op(postfix_info.value(), std::move(left), op_range);
            continue;
        }

        if (infix_info) {
            if (infix_info->prec < min_prec) {
                break;
            }

            parser.advance();

            std::size_t next_min_prec = infix_info->prec + (std::get<infix_operator>(infix_info->kind).assoc == operator_assoc::left);

            auto right = parse_expression(parser, next_min_prec);
            left       = make_infix_op(infix_info.value(), std::move(left), std::move(right), op_range);
            continue;
        }

        break;
    }

    return left;
}

parser_result adobans::parse(const std::vector<token> &tokens, const std::unique_ptr<operator_config> &op_config) {
    parser parser(tokens, op_config);

    auto ast = parse_expression(parser, 0);

    parser_result result{
        .ast     = std::move(ast),
        .reports = std::move(parser.reports)
    };

    return result;
}
