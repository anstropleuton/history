#include "source_code.hpp"
#include "token.hpp"
using namespace adobans;

struct tokenizer {
  private:
    std::weak_ptr<source_code> code;
    std::size_t                index = 0;

    std::vector<text_range> ranges;

    std::vector<std::pair<std::size_t, token>>  tokens;
    std::vector<std::pair<std::size_t, report>> reports;

    std::vector<text_range> range_stack;

  public:
    tokenizer(std::weak_ptr<source_code> code) : code(code) {}

    std::string_view get_code() const {
        if (auto lock = code.lock()) {
            return lock->content;
        }
        return std::string_view();
    }

    std::size_t size() const {
        return get_code().size();
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

    char peek(std::size_t offset = 0) const {
        if (tell() + offset >= size()) {
            return '\0';
        }
        return get_code()[tell() + offset];
    }

    std::string_view peek_range(std::size_t length = -1, std::size_t offset = 0) const {
        if (tell() + offset >= size()) {
            return std::string_view();
        }

        return get_code().substr(index + offset, length);
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

    std::size_t find(std::string_view string, std::size_t offset = 0) const {
        std::size_t result = peek_range(-1, offset).find(string);

        if (result == -1) {
            return -1;
        }

        return result + tell() + offset;
    }

    std::size_t find(char c, std::size_t offset = 0) const {
        return find(std::string_view(std::string(1, c)), offset);
    }

    std::size_t find_any_of(std::string_view string, std::size_t offset = 0) const {
        std::size_t result = peek_range(-1, offset).find_first_of(string);

        if (result == -1) {
            return -1;
        }

        return result + tell() + offset;
    }

    bool match(std::string_view string, std::size_t offset = 0) const {
        return peek_range(string.size(), offset) == string;
    }

    bool match(char c, std::size_t offset = 0) const {
        return match(std::string_view(std::string(1, c)), offset);
    }

    bool match_any_of(std::string_view string, std::size_t offset = 0) const {
        return string.contains(peek(offset));
    }

    void range_begin() {
        range_stack.push_back(text_range{.begin = index});
    }

    std::string_view range_end() {
        range_stack.back().length = index - range_stack.back().begin;
        ranges.emplace_back(range_stack.back());
        range_stack.pop_back();
        return get_code().substr(ranges.back().begin, ranges.back().length);
    }

    std::vector<token> get_tokens() const {
        std::vector<token> result;
        result.reserve(tokens.size());

        for (auto [index, token] : tokens) {
            token.range      = ranges[index];
            token.range.code = code;
            result.emplace_back(token);
        }

        return result;
    }

    std::vector<report> get_reports() const {
        std::vector<report> result;
        result.reserve(reports.size());

        for (auto [index, report] : reports) {
            report.range      = ranges[index];
            report.range.code = code;
            result.emplace_back(report);
        }

        return result;
    }

    void append_int(int int_value) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::int_lit,
                .value = int_value,
            }
        );
    }

    void append_float(float float_value) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::float_lit,
                .value = float_value,
            }
        );
    }

    void append_string(const std::string &string_value) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::string_lit,
                .value = string_value,
            }
        );
    }

    void append_char(char char_value) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::char_lit,
                .value = char_value,
            }
        );
    }

    void append_bool(bool bool_value) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::bool_lit,
                .value = bool_value,
            }
        );
    }

    void append_identifier(const std::string &identifier) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::identifier,
                .value = identifier,
            }
        );
    }

    void append_delimiter(char delimiter) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::delimiter,
                .value = delimiter,
            }
        );
    }

    void append_keyword(const std::string &keyword) {
        tokens.emplace_back(
            ranges.size() - 1,
            token{
                .type  = token_type::keyword,
                .value = keyword,
            }
        );
    }

    void append_note(const std::string &message) {
        reports.emplace_back(
            ranges.size() - 1,
            report{
                .sev     = severity::note,
                .message = message,
            }
        );
    }

    void append_warning(const std::string &message) {
        reports.emplace_back(
            ranges.size() - 1,
            report{
                .sev     = severity::warning,
                .message = message,
            }
        );
    }

    void append_error(const std::string &message) {
        reports.emplace_back(
            ranges.size() - 1,
            report{
                .sev     = severity::error,
                .message = message,
            }
        );
    }

    void append_help(const std::string &message) {
        reports.emplace_back(
            ranges.size() - 1,
            report{
                .sev     = severity::help,
                .message = message,
            }
        );
    }
};

static const std::string identifier_chars_ = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
static const std::string number_chars_     = "0123456789";

static const std::string identifier_start    = identifier_chars_;
static const std::string identifier_continue = identifier_chars_ + number_chars_;
static const std::string number_start        = number_chars_;
static const std::string number_continue     = identifier_chars_ + number_chars_ + "'.";
static const std::string whitespace          = " \n\r\t\v\b\f";
static const std::string delimiter           = "~!@$%^&*()-=+[]{}|:;,.<>/?";
static const std::string escape              = "abefnrtv\\\'\"#$";

static const std::vector<std::string> keywords = {
    "var",
    "print",
    "scan",
};

void tokenize_all(tokenizer &tokenizer);

void tokenize_whitespace(tokenizer &tokenizer) {
    if (tokenizer.match_any_of(whitespace)) {
        tokenizer.range_begin();

        while (tokenizer.match_any_of(whitespace)) {
            tokenizer.advance();
        }

        tokenizer.range_end();
    }
}

void tokenize_multi_line_comment(tokenizer &tokenizer) {
    if (tokenizer.match("#{")) {
        tokenizer.advance(2);
        tokenizer.range_begin();

        while (true) {
            if (tokenizer.eof()) {
                tokenizer.range_end();
                tokenizer.append_error("Unexpected end of file in multi-line comment.");
                tokenizer.append_help("You are missing a `}#` for `#{`");
                break;
            }

            if (tokenizer.match("#{")) {
                tokenize_multi_line_comment(tokenizer);
                continue;
            }
            if (tokenizer.match("}#")) {
                tokenizer.range_end();
                tokenizer.advance(2);
                break;
            }
            tokenizer.advance();
        }
    }
}

void tokenize_comment(tokenizer &tokenizer) {
    if (tokenizer.peek() == '#') {
        tokenizer.range_begin();

        if (tokenizer.match("#{")) {
            tokenize_multi_line_comment(tokenizer);
        }

        else {
            std::size_t pos = tokenizer.find('\n');

            if (pos != -1)
                tokenizer.seek(pos + 1);
            else
                tokenizer.seek(tokenizer.size());
        }

        tokenizer.range_end();
    }
}

void tokenize_identifier(tokenizer &tokenizer) {
    if (tokenizer.match_any_of(identifier_start)) {
        tokenizer.range_begin();

        while (tokenizer.match_any_of(identifier_continue)) {
            tokenizer.advance();
        }

        std::string identifier = std::string(tokenizer.range_end());
        bool        is_keyword = std::find(keywords.begin(), keywords.end(), identifier) != keywords.end();

        if (is_keyword) {
            tokenizer.append_keyword(identifier);
        } else if (identifier == "true" || identifier == "false") {
            tokenizer.append_bool(identifier == "true");
        } else {
            tokenizer.append_identifier(identifier);
        }
    }
}

void tokenize_delimiter(tokenizer &tokenizer) {
    while (tokenizer.match_any_of(delimiter)) {
        tokenizer.range_begin();
        tokenizer.advance();

        std::string delim = std::string(tokenizer.range_end());

        tokenizer.append_delimiter(delim[0]);
    }
}

void tokenize_number(tokenizer &tokenizer) {
    if (tokenizer.match_any_of(number_start)) {
        tokenizer.range_begin();

        while (tokenizer.match_any_of(number_continue)) {
            tokenizer.advance();
        }

        std::string number = std::string(tokenizer.range_end());

        std::size_t decimal_pos = number.find(".");

        if (number.find(".", decimal_pos + 1) != -1) {
            tokenizer.append_error("Number cannot contain multiple decimal point.");
            tokenizer.append_float(0.0f);
            return;
        }

        if (number.starts_with(".")) {
            tokenizer.append_error("Number cannot start with a decimal point.");
            tokenizer.append_help("Append a `0` before the number.");
            tokenizer.append_float(0.0f);
            return;
        }

        if (number.ends_with(".")) {
            tokenizer.append_error("Number cannot end with a decimal point.");
            tokenizer.append_help("Append a `0` after the number.");
            tokenizer.append_float(0.0f);
            return;
        }

        if (decimal_pos != -1) {
            tokenizer.append_float(std::stof(number));
        } else {
            tokenizer.append_int(std::stoi(number));
        }
    }
}

void tokenize_string(tokenizer &tokenizer) {
    if (tokenizer.match_any_of("\'\"")) {
        char enclosing = tokenizer.peek();

        tokenizer.range_begin();

        tokenizer.advance();
        while (tokenizer.peek() != enclosing) {
            if (tokenizer.eof()) {
                tokenizer.range_end();
                if (enclosing == '\'') {
                    tokenizer.append_error("Unexpected end of file in character literal.");
                    tokenizer.append_help("You are missing an ending `'`.");
                } else {
                    tokenizer.append_error("Unexpected end of file in string literal.");
                    tokenizer.append_help("You are missing an ending `\"`.");
                }
                return;
            }
            tokenizer.advance();
        }
        tokenizer.advance();

        std::string content = std::string(tokenizer.range_end());

        content = content.substr(1, content.size() - 2);

        if (enclosing == '\'') {
            if (content.size() > 1) {
                tokenizer.append_error("Character literal cannot contain more than one character.");
                return;
            }

            if (content.size() == 0) {
                tokenizer.append_error("Character literal cannot be empty.");
                return;
            }

            tokenizer.append_char(content[0]);
        } else {
            tokenizer.append_string(content);
        }
    }
}

void tokenize_all(tokenizer &tokenizer) {
    while (!tokenizer.eof()) {
        tokenize_whitespace(tokenizer);
        tokenize_comment(tokenizer);
        tokenize_identifier(tokenizer);
        tokenize_delimiter(tokenizer);
        tokenize_number(tokenizer);
        tokenize_string(tokenizer);
    }
}

tokenizer_result adobans::tokenize(std::weak_ptr<source_code> code) {
    tokenizer tokenizer(code);

    tokenize_all(tokenizer);

    tokenizer_result result = {
        .tokens  = tokenizer.get_tokens(),
        .reports = tokenizer.get_reports(),
    };

    return result;
}
