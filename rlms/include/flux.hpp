/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Main header file for Flux (Expressed float).
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

#pragma once

#include <functional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <raylib.h>

#ifndef FLUX_EXTERN
/**
 *  @brief  Define this to define global variables.
 */
#define FLUX_EXTERN extern
#endif // ifndef FLUX_EXTERN

namespace flux {

/**
 *  @brief  Flux custom function type, taking n parameters and returning one
 *          value.
 */
using flux_function = std::function<float (std::vector<float>)>;

/**
 *  @brief  List of all the custom functions.
 */
FLUX_EXTERN std::unordered_map<std::string, flux_function> functions;

/**
 *  @brief  List of all the custom constants.
 */
FLUX_EXTERN std::unordered_map<std::string, float> constants;

/**
 *  @brief  Add some built-in functions to @c flux_functions .
 *
 *  Appends to @c flux_functions , some built-in functions, such as min, max,
 *  sqrt, cbrt, round, floor, ceil, clamp, exp, log, sin, cos, tan, abs, etc.
 *  functions.  There are currently 73 functions, but this text might not be
 *  updated.  To see the full list, see @c src/minui_flux_builtin.cpp .  All
 *  the functions are C++ standard library functions, such as from <cmath>,
 *  <algorithm>, <numeric>, etc.
 */
auto add_builtin_functions() -> void;

/**
 *  @brief  Append some built-in constants to @c flux_constants .
 *
 *  Appends to @c flux_constants , some built-in functions, such as pi, e,
 *  phi, etc.  There are currently 13 constants, but this text might not be
 *  updated.  To see the full list, see @c src/minui_flux_builtin.cpp .  All
 *  the constants are C++ standard library constants, from <numbers>.
 */
auto add_builtin_constants() -> void;

/**
 *  @brief  Flux parser token type.
 */
enum class token_type {
    /**
     *  @brief  Unspecified.
     */
    unknown = -1,
    /**
     *  @brief  An identifier starts with `a`-`z`, `A`-`Z` or `_`, and can be
     *          followed by the mentioned characters, with `0`-`9` or `.`.
     */
    identifier,
    /**
     *  @brief  A number starts with `0`-`9` and can be followed by the
     *          mentioned characters, with `.` (for decimal)
     *          and `'` (for separating digits).
     */
    number,
    /**
     *  @brief  An operator can contain any of these following characters:
     *          `+`, `-`, `*`, `/`, `%`, `^`, `=`, `!`, `~`, `&`, `|`, `<`, `>`,
     *          `?`, `:`, `[` and `]`.
     */
    _operator,
    /**
     *  @brief  A punctuation can contain any of these following characters:
     *          `@`, `#`, `$`, `(`, `)`, `{`, `}`, `\`, `;` and `,`.
     */
    punctuation,
    /**
     *  @brief  Max.
     */
    max
};

/**
 *  @brief  A single token.
 */
struct token {

    /**
     *  @brief  Type of token.
     */
    token_type type;

    /**
     *  @brief  Value of the token.
     */
    std::string value;
};

/**
 *  @brief   Tokenize the code.
 *
 *  @param   code  Code to tokenize.
 *  @return  Tokens of code.
 */
[[nodiscard]] auto tokenize(std::string_view code)
-> std::vector<token>;

/**
 *  @brief  AST node base.
 */
struct ast_node {

    /**
     *  @brief  Parent node for reference.
     */
    std::weak_ptr<ast_node> parent;

    /**
     *  @brief  Default constructor.
     */
    ast_node() = default;

    /**
     *  @brief  Virtual destructor.
     */
    virtual ~ast_node() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float = 0;
};

/**
 *  @brief   Create an AST node.
 *
 *  @tparam  ast_node_type  Type of node to create.
 *  @return  Shared pointer to newly created AST node.
 */
template<typename ast_node_type>
[[nodiscard]] inline constexpr auto make_ast_node()
{
    auto shared = std::make_shared<ast_node_type>();
    return shared;
}

/**
 *  @brief   Create an AST node.
 *
 *  @tparam  ast_node_type  Type of node to create.
 *  @param   parent         Parent node reference.
 *  @return  Shared pointer to newly created AST node.
 */
template<typename ast_node_type>
[[nodiscard]] inline constexpr auto make_ast_node(
    std::weak_ptr<ast_node> parent
)
{
    auto shared = std::make_shared<ast_node_type>();

    shared->parent = parent;
    if (auto locked = shared->parent.lock())
    {
        locked->children.emplace_back(shared);
    }

    return shared;
}

/**
 *  @brief  Node that has number.
 */
struct number_ast : ast_node {

    /**
     *  @brief  Value of the number.
     */
    float value;

    /**
     *  @brief  Default constructor.
     */
    number_ast() = default;

    /**
     *  @brief  Virtual destructor.
     */
    ~number_ast() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float override;
};

/**
 *  @brief  Node that has variable (or constant).
 */
struct variable_ast : ast_node {

    /**
     *  @brief  Name of the variable.
     */
    std::string name;

    /**
     *  @brief  Default constructor.
     */
    variable_ast() = default;

    /**
     *  @brief  Virtual destructor.
     */
    ~variable_ast() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float override;
};

/**
 *  @brief  Node that is a function call.
 */
struct function_ast : ast_node {

    /**
     *  @brief  Function name.
     */
    std::string name;

    /**
     *  @brief  Function's arguments.
     */
    std::vector<std::shared_ptr<ast_node>> args;

    /**
     *  @brief  Default constructor.
     */
    function_ast() = default;

    /**
     *  @brief  Virtual destructor.
     */
    ~function_ast() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float override;
};

/**
 *  @brief  Node that is an operator with operands.
 */
struct operator_ast : ast_node {

    /**
     *  @brief  Operator string.
     */
    std::string op;

    /**
     *  @brief  Operator's operands.
     */
    std::vector<std::shared_ptr<ast_node>> operands;

    /**
     *  @brief  Default constructor.
     */
    operator_ast() = default;

    /**
     *  @brief  Virtual destructor.
     */
    ~operator_ast() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float override;
};

/**
 *  @brief  Node that for conditional operator (? and :).
 */
struct conditional_ast : ast_node {

    /**
     *  @brief  Condition node.
     */
    std::shared_ptr<ast_node> condition;

    /**
     *  @brief  True node.
     */
    std::shared_ptr<ast_node> _true;

    /**
     *  @brief  False node.
     */
    std::shared_ptr<ast_node> _false;

    /**
     *  @brief  Default constructor.
     */
    conditional_ast() = default;

    /**
     *  @brief  Virtual destructor.
     */
    ~conditional_ast() = default;

    /**
     *  @brief   Evaluate this node for value.
     *
     *  @param   variables  Map of local variables.
     *  @return  Value after evaluating.
     */
    [[nodiscard]] virtual auto evaluate(
        const std::unordered_map<std::string, float> &variables
    ) -> float override;
};

/**
 *  @brief   Parse tokens into AST.
 *
 *  @param   tokens  Tokens to parse.
 *  @return  Root AST node for parsed code.
 */
[[nodiscard]] auto parse(
    const std::vector<token> &tokens
) -> std::shared_ptr<ast_node>;

/**
 *  @brief  Flux.
 *
 *  Flux is a flexible way to represent float with expression.
 *
 *  This supports:
 *  - Numbers (duh)
 *    - Both integers and reals are interpreted as float.
 *    - Don't use the 'f' suffix for real numbers, as you would do in C++.
 *  - Operators: +, -, *, /, % (modulo/remainder), %% (wrapping modulo),
 *    ** (exponent/power), // (flooring division)
 *  - Parenthesis: ( and )
 *  - Relational operators: ==, !=, <, >, <=, >=, !<, !>, !<=, !>=
 *  - Boolean conditional operators: &&, ||, !, =>
 *  - Integral bitwise operators: &, |, ^, ~, <<, >>
 *  - Min/max operators: <? (min), >? (max)
 *  - Absolute difference operator: !!
 *  - Conditional operator: condition ? true_value : false_value
 *  - Null-coalescing-like operator: value ?? value_if_zero
 *  - Custom variables: variable_name
 *    - Append your variables in the local list of variables in `variables`
 *      member.
 *    - There are built-in variables depending on the context, such as `psx` for
 *      parent size X, or `csy` for current size Y (this is dependent on
 *      context, see documentation in corresponding layout).
 *  - Custom constants: constant_name
 *    - Append your constants in the global list of constants in
 *      `flux_constants`.
 *  - Custom function call: function_name(comma_separated_parameters)
 *    - Append your function in the global list of functions in
 *      `flux_functions`.  The function must return one value.
 */
struct flux {

    /**
     *  @brief  Expression to use to evaluate.
     */
    std::string expr;

    /**
     *  @brief  Cached AST node.
     */
    std::shared_ptr<ast_node> ast;

    /**
     *  @brief  List of variables.
     */
    std::unordered_map<std::string, float> variables;

    /**
     *  @brief  Default constructor.
     */
    inline flux() = default;

    /**
     *  @brief  Creates Flux using string expression.
     *  @param  expr  Expression.
     */
    inline flux(std::string expr) : expr(expr) {}

    /**
     *  @brief  Creates Flux using string expression.
     *  @param  expr  Expression.
     */
    inline flux(const char *expr) : expr(expr) {}

    /**
     *  @brief  Creates Flux using value.
     *
     *  @param  type   Type of value.
     *  @param  value  Convert value to string.
     */
    template<typename type>
    inline flux(const type &value) : expr(std::to_string(value)) {}

    /**
     *  @brief  Re-tokenize and re-parse the cache.
     */
    [[nodiscard]] inline constexpr auto refresh()
    {
        // Empty expression does nothing
        if (expr == "")
        {
            return;
        }

        auto tokens = tokenize(expr);
        ast         = parse(tokens);
    }

    /**
     *  @brief   Evaluate expression.
     *  @return  Float of evaluated expression.
     */
    [[nodiscard]] inline constexpr auto evaluate()
    {
        // Empty expression returns 0
        if (expr == "")
        {
            return 0.0f;
        }

        if (!ast) refresh();
        return ast->evaluate(variables);
    }

    /**
     *  @brief   Evaluate expression.
     *  @return  Float of evaluated expression.
     */
    [[nodiscard]] inline constexpr operator float () {
        return evaluate();
    }
};

/**
 *  @brief  Flux2 is a Vector2 of flux components.
 */
struct flux2 {

    /**
     *  @brief  Flux2 X component.
     */
    flux x;

    /**
     *  @brief  Flux2 Y component.
     */
    flux y;

    /**
     *  @brief  Default constructor.
     */
    flux2() = default;

    /**
     *  @brief  Creates a Flux2 with string expressions.
     *
     *  @param  x_expr  X component expression.
     *  @param  y_expr  Y component expression.
     */
    flux2(std::string x_expr, std::string y_expr)
        : x(x_expr), y(y_expr) {}

    /**
     *  @brief  Creates a Flux2 with one expression (as `{}`) for both X and Y
     *          components.
     *
     *  This constructor creates a Flux2 with one provided expression for all
     *  components. You can use `{}` as a placeholder, which is substituted with
     *  `"x"` for X, and `"y"` for Y.
     *
     *  @param  expr  Expression.
     */
    flux2(std::string expr)
    {
        x = flux(std::regex_replace(expr, std::regex("\\{\\}"), "x"));
        y = flux(std::regex_replace(expr, std::regex("\\{\\}"), "y"));
    }

    /**
     *  @brief  Creates Flux2 using values.
     *
     *  @param  type_x   Type of X component value.
     *  @param  type_y   Type of Y component value.
     *  @param  value_x  X component value.
     *  @param  value_y  Y component value.
     */
    template<typename type_x, typename type_y>
    inline flux2(const type_x &value_x, const type_y &value_y)
        : x(value_x), y(value_y) {}

    /**
     *  @brief  Evaluate all flux components.
     */
    [[nodiscard]] inline constexpr operator Vector2 () {
        return { x, y };
    }
};

/**
 *  @brief  Flux3 is a Vector3 of flux components.
 */
struct flux3 {

    /**
     *  @brief  Flux3 X component.
     */
    flux x;

    /**
     *  @brief  Flux3 Y component.
     */
    flux y;

    /**
     *  @brief  Flux3 Z component.
     */
    flux z;

    /**
     *  @brief  Default constructor.
     */
    flux3() = default;

    /**
     *  @brief  Creates a Flux3 with string expressions.
     *
     *  @param  x_expr  X component expression.
     *  @param  y_expr  Y component expression.
     *  @param  z_expr  Z component expression.
     */
    flux3(std::string x_expr, std::string y_expr, std::string z_expr)
        : x(x_expr), y(y_expr), z(z_expr) {}

    /**
     *  @brief  Creates a Flux3 with one expression (as `{}`) for all components.
     *
     *  This constructor creates a Flux3 with one provided expression for all
     *  components. You can use `{}` as a placeholder, which is substituted with
     *  `"x"` for X, `"y"` for Y, and `"z"` for Z.
     *
     *  @param  expr  Expression.
     */
    flux3(std::string expr)
    {
        x = flux(std::regex_replace(expr, std::regex("\\{\\}"), "x"));
        y = flux(std::regex_replace(expr, std::regex("\\{\\}"), "y"));
        z = flux(std::regex_replace(expr, std::regex("\\{\\}"), "z"));
    }

    /**
     *  @brief  Creates Flux2 using values.
     *
     *  @param  type_x   Type of X component value.
     *  @param  type_y   Type of Y component value.
     *  @param  type_z   Type of Z component value.
     *  @param  value_x  X component value.
     *  @param  value_y  Y component value.
     *  @param  value_z  Z component value.
     */
    template<typename type_x, typename type_y, typename type_z>
    inline flux3(
        const type_x &value_x, const type_y &value_y, const type_z &value_z
    ) : x(value_x), y(value_y), z(value_z) {}

    /**
     *  @brief  Evaluate all flux components.
     */
    [[nodiscard]] inline constexpr operator Vector3 () {
        return { x, y, z };
    }
};

/**
 *  @brief  Flux4 is a Vector4 of flux components.
 */
struct flux4 {

    /**
     *  @brief  Flux4 X component.
     */
    flux x;

    /**
     *  @brief  Flux4 Y component.
     */
    flux y;

    /**
     *  @brief  Flux4 Z component.
     */
    flux z;

    /**
     *  @brief  Flux4 W component.
     */
    flux w;

    /**
     *  @brief  Default constructor.
     */
    flux4() = default;

    /**
     *  @brief  Creates a flux with string expressions.
     *
     *  @param  x_expr  X component expression.
     *  @param  y_expr  Y component expression.
     *  @param  z_expr  Z component expression.
     *  @param  w_expr  W component expression.
     */
    flux4(
        std::string x_expr, std::string y_expr,
        std::string z_expr, std::string w_expr
    ) : x(x_expr), y(y_expr), z(z_expr), w(w_expr) {}

    /**
     *  @brief  Creates a flux with one expression (as `{}`) for all components.
     *
     *  This constructor creates a flux with one provided expression for all
     *  components. You can use `{}` as a placeholder, which is substituted with
     *  `"x"` for X, `"y"` for Y, `"z"` for Z, and `"w"` for W.
     *
     *  @param  expr  Expression.
     */
    flux4(std::string expr)
    {
        x = flux(std::regex_replace(expr, std::regex("\\{\\}"), "x"));
        y = flux(std::regex_replace(expr, std::regex("\\{\\}"), "y"));
        z = flux(std::regex_replace(expr, std::regex("\\{\\}"), "z"));
        w = flux(std::regex_replace(expr, std::regex("\\{\\}"), "w"));
    }

    /**
     *  @brief  Creates a flux with one expression (as `{}`) for all components.
     *
     *  This constructor creates a flux with one provided expression for all
     *  components. You can use `{}` as a placeholder, which is substituted with
     *  `"x"` for X, `"y"` for Y, `"z"` for Z, and `"w"` for W.
     *
     *  @param  expr  Expression.
     */
    flux4(const char *expr) : flux4(std::string (expr)) {}

    /**
     *  @brief  Creates Flux2 using values.
     *
     *  @param  type_x   Type of X component value.
     *  @param  type_y   Type of Y component value.
     *  @param  type_z   Type of Z component value.
     *  @param  type_w   Type of W component value.
     *  @param  value_x  X component value.
     *  @param  value_y  Y component value.
     *  @param  value_z  Z component value.
     *  @param  value_w  W component value.
     */
    template<typename type_x, typename type_y, typename type_z, typename type_w>
    inline flux4(
        const type_x &value_x, const type_y &value_y,
        const type_z &value_z, const type_w &value_w
    ) : x(value_x), y(value_y), z(value_z), w(value_w) {}

    /**
     *  @brief  Evaluate all flux components.
     */
    [[nodiscard]] inline constexpr operator Vector4 ()
    {
        return { x, y, z, w };
    }
};

}
