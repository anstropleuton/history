/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Implementation for flux parser from @c flux.hpp .
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   _____ _
 *  |  ___| |_   ___  __
 *  | |_  | | | | \ \/ /
 *  |  _| | | |_| |>  <
 *  |_|   |_|\__,_/_/\_\
 *
 *  Flux is an expression-storing data type that evaluates the expression as a
 *  float when required its value.
 *
 *  This software is licensed under the terms of MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Credits where credit's due:
 *  - ASCII Art generated using https://www.patorjk.com/software/taag with font
 *    "Standard".
 */

#define FLUX_EXTERN
#include "flux.hpp"

#include <cmath>
#include <stdexcept>

/**
 *  @brief   Convert float to int using @c std::round .
 *
 *  @param   value  Floating-point value.
 *  @return  @c value converted to int by rounding it.
 */
[[nodiscard]] static inline constexpr auto rint(float value)
{
    return (int)std::round(value);
}

/**
 *  @brief  Binary operator precedences, where each outer vector's index is the
 *          precedence for all the operators in the inner vector.
 */
std::vector<std::vector<std::string>> bin_op_prec = {
    // uncrustify:off
    { "&", "|", "^" },  // Bitwise
    { "<<", ">>" },     // Bitshift
    { "+", "-" },       // Additive
    { "*", "/" },       // Multiplicative
    { "%", "%%" },      // Modulos
    { "**", "//" },     // Exponential and flooring division
    { "<?", ">?" },     // Min and max
    { "==", "!=", "<", ">", "<=", ">=", "!<", "!>", "!<=", "!>=" }, // Relational
    { "&&", "||" },     // Logical
    { "=>" },           // Implication
    { "??" }            // Null-coalescing
    // uncrustify:on
};

/**
 *  @brief   Tokenize the code.
 *
 *  @param   code  Code to tokenize.
 *  @return  Tokens of code.
 */
[[nodiscard]] auto flux::tokenize(std::string_view code)
-> std::vector<token>
{
    std::vector<token> tokens = {};
    std::size_t        pos    = 0;
    std::string        tmp    = "";

    const std::string ident_start = "abcdefghijklmnopqrstuvwxyz"
                                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "_";
    const std::string number_start = "0123456789";
    const std::string ident_cont   = ident_start + number_start + ".";
    const std::string number_cont  = number_start + ".'";

    const std::string op_chars    = "+-*/%^=!~&|<>?:[]";
    const std::string punct_chars = "@#$(){}\\;,";

    // Sadly I am not using Alcelin, it would have been so much better tho...
    while (pos < code.size())
    {
        // Skip whitespace
        if (pos < code.size() && std::isspace(code[pos]))
        {
            while (pos < code.size() && std::isspace(code[pos]))
            {
                pos++;
            }
        }
        // Parse identifier
        else if (pos < code.size() && ident_start.contains(code[pos]))
        {
            while (pos < code.size() && ident_cont.contains(code[pos]))
            {
                tmp += code[pos++];
            }

            tokens.emplace_back(
                token_type::identifier,
                tmp
            );

            tmp = "";
        }
        // Parse number
        else if (pos < code.size() && number_start.contains(code[pos]))
        {
            while (pos < code.size() && number_cont.contains(code[pos]))
            {
                tmp += code[pos++];
            }

            tokens.emplace_back(
                token_type::number,
                tmp
            );

            tmp = "";
        }
        // Parse operator
        else if (pos < code.size() && op_chars.contains(code[pos]))
        {
            while (pos < code.size() && op_chars.contains(code[pos]))
            {
                tmp += code[pos++];
            }

            tokens.emplace_back(
                token_type::_operator,
                tmp
            );

            tmp = "";
        }
        // Parse punctuation
        else if (pos < code.size() && punct_chars.contains(code[pos]))
        {
            while (pos < code.size() && punct_chars.contains(code[pos]))
            {
                tmp += code[pos++];

                tokens.emplace_back(
                    token_type::punctuation,
                    tmp
                );

                tmp = "";
            }
        }
        // Invalid character
        else
        {
            throw std::runtime_error(std::format(
                "Invalid character in token: {} (at {})", code[pos], pos));
        }
    }

    return tokens;
}

/**
 *  @brief   Parse primary expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_primary(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>;

/**
 *  @brief   Parse unary operator expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_unary(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in unary operator expression");
    }

    // Parse primary expression if not unary operator
    if (tokens[pos].type != flux::token_type::_operator)
    {
        return parse_primary(tokens, pos);
    }

    auto copy    = tokens[pos++];
    auto operand = parse_unary(tokens, pos);
    auto node    = flux::make_ast_node<flux::operator_ast>();

    node->op = copy.value;
    node->operands.emplace_back(operand);

    return node;
}

/**
 *  @brief   Parse expression at a precedence level.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @param   lvl     Precedence level.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_expr_lvl(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos,
    std::size_t                     lvl
) -> std::shared_ptr<flux::ast_node>;

/**
 *  @brief   Parse numeric expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_number(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in numeric expression");
    }

    if (tokens[pos].type != flux::token_type::number)
    {
        throw std::runtime_error("Expected number in numeric expression");
    }

    // Replace all "'" with nothing
    std::string num_str = tokens[pos].value;
    num_str = std::regex_replace(num_str, std::regex("'"), "");

    auto node   = flux::make_ast_node<flux::number_ast>();
    node->value = std::stof(num_str);
    pos++;
    return node;
}

/**
 *  @brief   Parse function call expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @param   ident   Function identifier.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_func(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos,
    std::string                     ident
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in function call expression");
    }

    if (tokens[pos].type != flux::token_type::punctuation
     || tokens[pos].value != "(")
    {
        throw std::runtime_error(
            "Expected \"(\" in function call expression");
    }

    pos++; // Consume "("

    auto node  = flux::make_ast_node<flux::function_ast>();
    node->name = ident;

    while (pos < tokens.size()
        && !(tokens[pos].type == flux::token_type::punctuation
          && tokens[pos].value == ")"))
    {
        node->args.emplace_back(parse_expr_lvl(tokens, pos, 0));
        if (pos < tokens.size()
         && tokens[pos].type == flux::token_type::punctuation
         && tokens[pos].value == ",")
        {
            pos++; // Consume ","
        }
        else
        {
            break;
        }
    }

    if (pos >= tokens.size()
     || !(tokens[pos].type == flux::token_type::punctuation
          && tokens[pos].value == ")"))
    {
        throw std::runtime_error("Expected \")\" in function call");
    }

    pos++; // Consume ")"

    return node;
}

/**
 *  @brief   Parse variable expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @param   ident   Function identifier.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_var(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos,
    std::string                     ident
) -> std::shared_ptr<flux::ast_node>
{
    auto node  = flux::make_ast_node<flux::variable_ast>();
    node->name = ident;
    return node;
}

/**
 *  @brief   Parse identifier expression (variable or function call).
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_ident(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in identifier expression");
    }

    std::string ident = tokens[pos++].value;

    // Parse function
    if (pos < tokens.size()
     && tokens[pos].type == flux::token_type::punctuation
     && tokens[pos].value == "(")
    {
        return parse_func(tokens, pos, ident);
    }
    else
    {
        return parse_var(tokens, pos, ident);
    }
}

/**
 *  @brief   Parse expression with parenthesis.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_paren(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in parenthesis expression");
    }

    // Parse function
    if (tokens[pos].type != flux::token_type::punctuation
     || tokens[pos].value != "(")
    {
        throw std::runtime_error(
            "Expected \"(\" in parenthesis expression");
    }

    pos++; // Consume "("

    auto expr = parse_expr_lvl(tokens, pos, 0);

    if (pos >= tokens.size()
     || !(tokens[pos].type == flux::token_type::punctuation
          && tokens[pos].value == ")"))
    {
        throw std::runtime_error("Expected \")\" in parenthesis expression");
    }

    pos++; // Consume ")"

    return expr;
}

/**
 *  @brief   Parse primary expression.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_primary(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in primary expression");
    }

    // Parse number
    if (tokens[pos].type == flux::token_type::number)
    {
        return parse_number(tokens, pos);
    }
    else if (tokens[pos].type == flux::token_type::identifier)
    {
        return parse_ident(tokens, pos);
    }
    else if (tokens[pos].type == flux::token_type::punctuation
          && tokens[pos].value == "(")
    {
        return parse_paren(tokens, pos);
    }
    else
    {
        throw std::runtime_error(std::format(
            "Unexpected token (\"{}\") in primary expression",
            tokens[pos].value));
    }
}

/**
 *  @brief   Parse expression at a precedence level.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @param   lvl     Precedence level.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_expr_lvl(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos,
    std::size_t                     lvl
) -> std::shared_ptr<flux::ast_node>
{
    if (lvl == bin_op_prec.size())
    {
        return parse_unary(tokens, pos);
    }

    auto left = parse_expr_lvl(tokens, pos, lvl + 1);
    while (pos < tokens.size()
        && tokens[pos].type == flux::token_type::_operator
        && std::ranges::find(bin_op_prec[lvl], tokens[pos].value)
        != bin_op_prec[lvl].end())
    {
        std::string op = tokens[pos++].value;

        auto right = parse_expr_lvl(tokens, pos, lvl + 1);
        auto node  = flux::make_ast_node<flux::operator_ast>();
        node->op   = op;
        node->operands.emplace_back(left);
        node->operands.emplace_back(right);
        left = node;
    }

    return left;
}

/**
 *  @brief   Parse conditional expressions.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @param   cond    Condition expression.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_cond(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos,
    std::shared_ptr<flux::ast_node> cond
) -> std::shared_ptr<flux::ast_node>
{
    if (pos >= tokens.size())
    {
        throw std::runtime_error(
            "Unexpected end of tokens in conditional expression");
    }

    if (tokens[pos].type != flux::token_type::_operator
     || tokens[pos].value != "?")
    {
        throw std::runtime_error(
            "Expected \"?\" in conditional expression");
    }
    pos++; // Consume "?"

    auto true_expr = parse_expr_lvl(tokens, pos, 0);

    if (pos >= tokens.size()
     || !(tokens[pos].type == flux::token_type::_operator
       && tokens[pos].value == ":"))
    {
        throw std::runtime_error("Expected \":\" in conditional expression");
    }

    pos++; // Consume ":"

    auto false_expr = parse_expr_lvl(tokens, pos, 0);

    auto node       = flux::make_ast_node<flux::conditional_ast>();
    node->condition = cond;
    node->_true     = true_expr;
    node->_false    = false_expr;

    return node;
}

/**
 *  @brief   Parse all expressions.
 *
 *  @param   tokens  Tokens to parse.
 *  @param   pos     Current token position.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] static inline constexpr auto parse_all(
    const std::vector<flux::token> &tokens,
    std::size_t                    &pos
) -> std::shared_ptr<flux::ast_node>
{
    auto expr = parse_expr_lvl(tokens, pos, 0);

    // Parse conditional operator
    if (pos < tokens.size()
     && tokens[pos].type == flux::token_type::_operator
     && tokens[pos].value == "?")
    {
        expr = parse_cond(tokens, pos, expr);
    }

    if (pos != tokens.size())
    {
        throw std::runtime_error("Unexpected tokens at the end of expression");
    }

    return expr;
}

/**
 *  @brief   Parse tokens into AST.
 *
 *  @param   tokens  Tokens to parse.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] auto flux::parse(
    const std::vector<token> &tokens
) -> std::shared_ptr<ast_node>
{
    std::size_t pos = 0;
    return parse_all(tokens, pos);
}

/**
 *  @brief   Evaluate this node for value.
 *
 *  @param   variables  Map of local variables.
 *  @return  Value after evaluating.
 */
[[nodiscard]] auto flux::number_ast::evaluate(
    const std::unordered_map<std::string, float> &variables
) -> float
{
    return value;
}

/**
 *  @brief   Evaluate this node for value.
 *
 *  @param   variables  Map of local variables.
 *  @return  Value after evaluating.
 */
[[nodiscard]] auto flux::variable_ast::evaluate(
    const std::unordered_map<std::string, float> &variables
) -> float
{
    if (variables.contains(name))
    {
        return variables.at(name);
    }

    if (constants.contains(name))
    {
        return constants.at(name);
    }

    throw std::runtime_error(std::format("Invalid variable \"{}\"", name));
}

/**
 *  @brief   Evaluate this node for value.
 *
 *  @param   variables  Map of local variables.
 *  @return  Value after evaluating.
 */
[[nodiscard]] auto flux::function_ast::evaluate(
    const std::unordered_map<std::string, float> &variables
) -> float
{
    if (!functions.contains(name))
    {
        throw std::runtime_error(std::format("Invalid function: \"{}\"", name));
    }

    std::vector<float> values;
    values.reserve(args.size());
    for (auto &arg : args)
    {
        values.emplace_back(arg->evaluate(variables));
    }

    return functions.at(name)(values);
}

/**
 *  @brief   Evaluate this node for value.
 *
 *  @param   variables  Map of local variables.
 *  @return  Value after evaluating.
 */
[[nodiscard]] auto flux::operator_ast::evaluate(
    const std::unordered_map<std::string, float> &variables
) -> float
{
    if (operands.size() == 1)
    {
        float operand = operands[0]->evaluate(variables);

        if (op == "+") return operand;
        if (op == "-") return -operand;
        if (op == "*") return operand;
        if (op == "/") return 1.0f / operand;

        if (op == "!") return operand == 0.0f;
        if (op == "~") return ~rint(operand);

        throw std::runtime_error(std::format(
            "Invalid unary operator \"{}\"", op));
    }
    else if (operands.size() == 2)
    {
        float left  = operands[0]->evaluate(variables);
        float right = operands[1]->evaluate(variables);

        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/") return left / right;
        if (op == "%") return std::fmod(left, right);
        if (op == "%%")
        {
            float result = std::fmod(left, right);
            return result < 0 ? result + right : result;
        }
        if (op == "**") return std::pow(left, right);
        if (op == "//") return std::floor(left / right);

        if (op == "==") return left == right;
        if (op == "!=") return left != right;
        if (op == "<") return left < right;
        if (op == ">") return left > right;
        if (op == "<=") return left <= right;
        if (op == ">=") return left >= right;
        if (op == "!<") return !(left < right);
        if (op == "!>") return !(left > right);
        if (op == "!<=") return !(left <= right);
        if (op == "!>=") return !(left >= right);

        if (op == "&&") return (left != 0.0f) && (right != 0.0f);
        if (op == "||") return (left != 0.0f) || (right != 0.0f);
        if (op == "=>") return !(left != 0.0f) || (right != 0.0f);

        if (op == "&") return rint(left) & rint(right);
        if (op == "|") return rint(left) | rint(right);
        if (op == "^") return rint(left) ^ rint(right);
        if (op == "<<") return rint(left) << rint(right);
        if (op == ">>") return rint(left) >> rint(right);

        if (op == "<?") return std::min(left, right);
        if (op == ">?") return std::max(left, right);
        if (op == "!!") return std::abs(left - right);

        if (op == "??") return left != 0.0f ? left : right;

        throw std::runtime_error(std::format(
            "Invalid binary operator \"{}\"", op));
    }

    throw std::runtime_error("Invalid number of operands");
}

/**
 *  @brief   Evaluate this node for value.
 *
 *  @param   variables  Map of local variables.
 *  @return  Value after evaluating.
 */
[[nodiscard]] auto flux::conditional_ast::evaluate(
    const std::unordered_map<std::string, float> &variables
) -> float
{
    return condition->evaluate(variables) != 0.0f
         ? _true->evaluate(variables)
         : _false->evaluate(variables);
}
