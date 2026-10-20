# Adobans — Formal Language Specification

This document is a complete, self-contained formal specification for **Adobans**, the small, operational programming language described and implemented in the interpreter you already have. It defines lexical conventions, syntax (EBNF), static & dynamic semantics, evaluation order, runtime behavior, standard builtins, and precise semantics for the language’s special features (`break`/`cont`, `once`/`onward`, arrays, classes, `include`, `scan`/`print`, conversions, etc.).

Where the spec says “runtime error”, the reference implementation raises an error and halts execution of the current program (possibly printing a short message). All unspecified behavior is considered an implementation detail; the spec documents the intended semantics implemented by the reference interpreter.

---

# Table of contents

1. Overview
2. Lexical grammar (tokens)
3. Syntactic grammar (EBNF)
4. Operator precedence and associativity
5. Types and values
6. Expressions — static & dynamic semantics
7. Statements — semantics & control flow
8. Functions & closures
9. Arrays — representation & methods
10. Classes & objects
11. Special constructs: `include`, `once` / `onward`
12. I/O: `scan` and `print`
13. Conversions & coercion rules (`as`)
14. Error model & diagnostics
15. Implementation notes and evaluation strategy
16. Examples (compact)
17. Annex: sample AST node shapes

---

# 1 — Overview

Adobans is an imperative, statically-annotatable, dynamically-executed language with the following core properties:

* Typed values drawn from a small set of primitive types plus arrays, functions, objects.
* Static type annotations are optional. When present, `var` declarations and function parameters use them, and the interpreter enforces conversions where required at runtime.
* Execution model: single-threaded, lexical scoping, dynamic allocation for arrays and objects, first-class functions (as `Val('fn', ...)` values).
* Control flow: `if`, `while`, `break N`, `cont N`, `once`, `onward`.
* File inclusion: `include "path";` executes the included file in the same global environment.
* REPL mode preserves a persistent environment across inputs.

---

# 2 — Lexical grammar (tokens)

Whitespace and comments separate tokens.

## 2.1 Whitespace

Whitespace characters: space `U+0020`, horizontal tab `U+0009`, carriage-return `U+000D`, line feed `U+000A`. These are token separators and otherwise ignored.

## 2.2 Comments

* Single-line comment: begins with `#` and runs to (but not including) end-of-line.

  ```
  # this is a single-line comment
  ```
* Multi-line comment: begins with `#{` and ends with `}#`. Multi-line comments nest arbitrarily: every `#{` increases nest-depth by one; every `}#` decreases it. Unclosed nested comments are a lexical error.

## 2.3 Tokens (terminal categories)

* **IDENT**: `[A-Za-z_][A-Za-z0-9_]*` except reserved keywords (see below).
* **NUMBER**: integer or decimal: `\d+(\.\d+)?`. If literal contains `.`, it's a `float` literal; otherwise `int`.
* **STRING**: double-quoted string supporting C-style escapes: `"` then characters then `"`. Supports `\"`, `\\`, `\n`, `\t`, etc. (interpreted with standard escape semantics).
* **CHAR**: single-quoted single character with escapes: `'a'`, `'\n'`.
* **OP**: operators and punctuation: multi-character operators are tokenized first: `**`, `//`, `%%`, `==`, `!=`, `<=`, `>=`, `&&`, `||`, `->`. Single-character operators include `+ - * / % ( ) [ ] { } = < > ! ; : , .`.

## 2.4 Keywords

The following reserved words are token kinds (they are not IDENTs):

```
var int float char bool string print scan as true false
if else while fn return include break cont once onward class
```

(They appear as their lowercase spellings in tokens.)

---

# 3 — Syntactic grammar (EBNF)

Below is a compact grammar describing the language. Terminals are in quotes; non-terminals are bare names. `|` is alternation, `{ }` is repetition (0 or more), `[ ]` is optional.

```
program         ::= { statement }

statement       ::= var_decl ";" 
                  | assignment ";" 
                  | print_stmt ";" 
                  | include_stmt ";" 
                  | return_stmt ";" 
                  | break_stmt ";" 
                  | cont_stmt ";" 
                  | once_stmt 
                  | onward_stmt 
                  | if_stmt 
                  | while_stmt 
                  | fn_def 
                  | class_def 
                  | expr_stmt ";" 

var_decl        ::= "var" IDENT ( ":" type )? ( "=" expression )?
assignment      ::= target "=" expression
target          ::= IDENT { "." IDENT | "[" expression "]" }   # var, field access, or index
print_stmt      ::= "print" expression
include_stmt    ::= "include" STRING
return_stmt     ::= "return" [ expression ]
break_stmt      ::= "break" [ NUMBER ]
cont_stmt       ::= "cont" [ NUMBER ]

once_stmt       ::= "once" block                     # braces required
onward_stmt     ::= "onward" block                   # braces required

if_stmt         ::= "if" ( "(" expression ")" | expression ) block_or_stmt [ "else" block_or_stmt ]
while_stmt      ::= "while" ( "(" expression ")" | expression ) block_or_stmt [ "else" block ]

block_or_stmt   ::= block | statement   # when no braces, a single statement is allowed (it must include its `;`)
block           ::= "{" { statement } "}"

fn_def          ::= "fn" IDENT "(" [ fn_params ] ")" [ "->" type ] block
fn_params       ::= fn_param { "," fn_param } [ "," ]
fn_param        ::= IDENT ":" type

class_def       ::= "class" IDENT "{" { class_member } "}"
class_member    ::= var_decl ";" | fn_def

expression      ::= logic_or
logic_or        ::= logic_and { "||" logic_and }
logic_and       ::= equality { "&&" equality }
equality        ::= relational { ( "==" | "!=" ) relational }
relational      ::= add { ( "<" | "<=" | ">" | ">=" ) add }
add             ::= mul { ( "+" | "-" ) mul }
mul             ::= power { ( "*" | "/" | "//" | "%" | "%%" ) power }
power           ::= unary [ "**" power ]    # right associative
unary           ::= ( "+" | "-" | "!" ) unary | postfix
postfix         ::= primary { postfix_op }
postfix_op      ::= "!" | "(" [ args ] ")" | "." IDENT | "[" expression "]"
primary         ::= literal | IDENT | "(" expression ")" | array_literal | "scan"
array_literal   ::= "[" [ args ] "]"
args            ::= expression { "," expression } [ "," ]
literal         ::= NUMBER | STRING | CHAR | "true" | "false"
type            ::= "int" | "float" | "char" | "bool" | "string" | IDENT "[" "]"
expr_stmt       ::= expression
```

Notes:

* The grammar allows optional trailing commas in parameter lists, argument lists, and array literals (see `args` and `fn_params`).
* `block_or_stmt` permits omission of braces `{}` if and only if parentheses or braces are not also omitted according to the rules described later; parser enforces that **not both** parentheses and braces are omitted for `if`/`while`.
* `class` body contains only `var` declarations and `fn` method definitions (no nested classes).

---

# 4 — Operator precedence and associativity

Operators from highest precedence to lowest:

1. Postfix: factorial `a!` (left-to-right if multiple `!` applied)
2. Exponentiation `**` — **right-associative**
3. Unary: `+ - !` — right-associative (unary binds to its operand)
4. Multiplicative: `* / // % %%` — left-associative

   * `//` — floor-division (result type depends on numeric promotion)
   * `%` — remainder `%` (language-defined remainder; follows Python-like semantics for numbers)
   * `%%` — wrapping modulo (positive modulo in [0, b-1] if b > 0)
5. Additive: `+ -` — left-associative

   * `+` handles both numeric addition and string concatenation (string concatenation if either operand is string)
6. Relational: `< <= > >=`
7. Equality: `== !=`
8. Logical AND: `&&` — left-associative, short-circuit
9. Logical OR: `||` — left-associative, short-circuit
10. `as` conversion — parsed as a low-precedence postfix (lowest expression-level operator; binds weaker than `||`), evaluated after the conversion target expression.

Evaluation order of subexpressions: **left-to-right** for binary operators and function call arguments; short-circuiting for `&&` and `||` implies the right operand may not be evaluated.

---

# 5 — Types and values

## 5.1 Primitive types

* `int` — arbitrarily large integer in the implementation (or typical host-language integer). Arithmetic on ints uses exact integer semantics when possible.
* `float` — double-precision floating-point number.
* `char` — single-character value (string of length 1).
* `bool` — boolean `true`/`false`.
* `string` — sequence of characters.

## 5.2 Composite / runtime types

* `array` — runtime values with metadata: element-type (optional/inferred), data (sequence of `Val`), capacity (power-of-two).
* `fn` — function value with metadata: parameter list (names + optional types), return type (optional), body AST, closure environment.
* `class` / `object` — class values and object instances:

  * `class` is a runtime value representing class metadata (fields, methods).
  * `object` is an instance with a map of fields (name→Val) and reference to class metadata.

## 5.3 Default values

When a variable is declared with a type but no initializer, it receives a default value:

* `int` → `0`
* `float` → `0.0`
* `bool` → `false`
* `string` → `""`
* `char` → `"\0"`
* `T[]` → empty array with element-type `T` (capacity at least 1)

## 5.4 Runtime representation

All values are wrapped as `Val(typ, v)` where `typ` is a string describing the value’s type (`int`, `float`, `array`, `fn`, `object`, etc.) and `v` is the host representation (python numbers, dicts for arrays/objects/functions).

---

# 6 — Expressions — static & dynamic semantics

## 6.1 Literals

* Integer literal `42` → `Val('int', 42)`
* Float literal `3.14` → `Val('float', 3.14)`
* String literal `"hi\n"` → `Val('string', "hi\n")` with escape processing
* Char literal `'x'` → `Val('char', 'x')`
* Boolean `true` / `false` → `Val('bool', True|False)`

## 6.2 Variables and name lookup

* `IDENT` resolves to the nearest binding in the lexical (dynamic) environment stack, searching from the most recent local scope outward to module/global scope.
* Unbound `IDENT` is a runtime error: `Undefined variable X`.

## 6.3 Attribute access and indexing

* `expr.attr`:

  * If `expr` is `array`, `attr` may be `count`, `capacity` (read-only) or an array method name; reading those returns values:

    * `array.count` → `Val('int', length)`
    * `array.capacity` → `Val('int', capacity)`
    * `array.insert` etc. return function-like bound-method values which when called dispatch to array-method semantics.
  * If `expr` is `object`, field lookup:

    * If `attr` is a field, returns the field value (as `Val`).
    * If `attr` is a method, returns a **bound method** value (a `fn`-like `Val`) that, when called, executes the method body with the instance’s fields visible as a local scope (see Methods & Classes).
  * Attribute access on other types is an error.

* `expr[index]`:

  * Valid only for `array` values.
  * `index` must evaluate to `int`. Negative indices allowed and interpreted as `len + index`. Out-of-bounds is runtime error.
  * Index expression is evaluated before the array expression (left-to-right evaluation of postfix ops).

## 6.4 Calls

* `f(a1, a2, ...)`:

  * Evaluate callee `f` first, then evaluate argument expressions left-to-right.
  * If callee value is `fn`, perform a call:

    * If `fn` is a builtin binding (e.g., array method), dispatch accordingly.
    * Else create a new runtime frame: closure env (captured at function definition) is used as base, push a new local scope containing parameters bound to argument values. Convert args to parameter types if declared.
    * Execute function body; `return` inside throws a `Return` event with a value. If `return` value present, coerce to declared return type if present. If no `return` executed:

      * If return type declared: return default value for that type.
      * Otherwise: return `int 0`.
  * If callee value is `class`, it constructs a new object:

    * Evaluate default initializers in calling environment; then positional args override fields in declaration order; the new object is returned as `Val('object', {...})`.
  * Calling a non-callable value is a runtime error.

## 6.5 Unary/Factorial

* `+x` / `-x` apply to numeric types; result type preserves numeric type (int remains int unless operations force float).
* `!x` is boolean not; operand must be `bool`.
* `x!` factorial: operand must be non-negative `int`; returns `int` factorial.

## 6.6 Binary arithmetic & string concat

* If either operand is `string` and operator is `+`, perform concatenation: convert the other operand to string using serialization rules, then concatenate.
* Numeric operators (`+ - * / // % %% **`) require numeric operands (`int`/`float`).

  * Numeric promotion: `int` + `float` → `float` result (`int` promotes to `float`).
  * `/` always yields `float`.
  * `//` floor division: `floor(a / b)`. For mixed types prefer float arithmetic but result of `//` is `int`.
  * `%` remainder uses the host-defined remainder semantics (consistent with Python `%` for mixes of signs).
  * `%%` wrapping modulo ensures non-negative result in `[0, |b|-1]` (i.e., `((a % b) + b) % b`).
  * `**` exponentiation: accept numeric operands; follows host-language exponent semantics (float result if float involved).
* Comparisons:

  * Numeric comparisons allow mixing int/float by numeric promotion; yields `bool`.
  * Equality `==`/`!=`:

    * If both numeric: compare numerically.
    * If same runtime type: compare by value.
    * If different non-numeric types: `==` is `false`, `!=` is `true`.
* Logical `&&` and `||`:

  * Operands must be `bool`.
  * Short-circuiting semantics: left operand is evaluated; right operand is evaluated only if needed.

## 6.7 Conversions `as`

* Syntax `expr as Type`:

  * Evaluate `expr` then attempt to convert to `Type`.
  * Conversion rules:

    * `int -> float`: implicit (but `as` also allowed).
    * `float -> int`: explicit `as int` (truncation toward zero / host cast).
    * `char -> string`: implicit conversion to one-character string.
    * `any -> string`: serialization (convert `Val` into a string form).
    * `string -> numeric/bool/char`: parse; if parse fails → runtime error. `string -> object` and `string -> array` parse not supported (runtime error).
    * Conversions between incompatible types raise runtime error.
  * `as` is a runtime operator (dynamic check/convert occurs at evaluation).

---

# 7 — Statements — semantics & control flow

## 7.1 General execution model

* Program executes sequentially.
* Each `block` creates a new lexical scope: variables declared with `var` are bound to the current top scope.
* Lookup resolves to the nearest lexical scope containing the name (search from top-of-stack to bottom).
* `env` is modeled as a stack of dictionaries representing nested scopes; the global environment is `env[0]`.

## 7.2 `var` declaration

* Syntax: `var name [: Type] [= expression] ;`
* If an initializer is present, evaluate it and assign to the variable.

  * If a `Type` is present, convert the initializer value to that type (runtime `convert`) before binding.
* If no initializer:

  * If `Type` is present → bind default value for that type.
  * If no type → runtime error (spec enforces that `var` without type requires initializer).

Redeclaration:

* Declaring a variable with the same name in the same scope **overwrites** that name in that scope (current interpreter behavior). Shadowing across scopes is allowed.

## 7.3 Assignment

* `target = expression;` where target can be:

  * variable identifier
  * dotted field access (e.g., `obj.field`)
  * array index (e.g., `arr[0]`)
* Semantics:

  * Evaluate right-hand-side expression.
  * For variable target: find the nearest containing scope with that name and assign; if not found → runtime error.
  * For `obj.field`: `obj` must be object; assign to the field in instance’s field dictionary; if field does not exist → runtime error.
  * For `arr[index]`: `arr` must be array; index must be `int`; negative allowed as `len+index`; assign into array data at resolved index; out-of-range → runtime error.

## 7.4 `print` and `scan`

* `print expr;` → evaluate `expr`, convert to string using `to_string` rules, and write to standard output with trailing newline (or no trailing newline depending on implementation choice — the reference interpreter prints newline).
* `scan` is an expression that reads one input line (until newline) from stdin and yields a `string` value (without the trailing newline).

## 7.5 `if` statement

* Syntax allows optional parentheses around condition and optional braces for body (one may omit parentheses or braces, but **not both**).

  * Allowed:

    * `if (cond) { ... }`
    * `if (cond) stmt;`
    * `if cond { ... }`
    * `if cond stmt;`
  * Disallowed: both parentheses and braces omitted (that would be ambiguous).
* Semantics:

  * Evaluate condition; it must be `bool` (otherwise runtime error).
  * If true → execute the `then` block/statement; else execute optional `else` block/statement.

## 7.6 `while` statement

* Same optional parentheses/braces rule as `if`.
* Semantics:

  * Evaluate condition; must be `bool`.
  * If true, execute body; repeat until condition is false.
  * Optional `else` block runs only if loop body **never executed** (i.e., loop test was false at the first evaluation).
* Loop context tracking:

  * The implementation maintains a **loop stack**, pushing an entry for each `while` execution context before the first iteration, with metadata `first_iteration: True`. After the first successful body execution, `first_iteration` is set to `False`.
  * `once` and `onward` statements read that flag.

## 7.7 `break` and `cont` (continue)

* Syntax: `break [ N ] ;`, `cont [ N ] ;` where `N` is a positive integer literal; default `N=1`.
* Semantics:

  * `break` with `N=1` immediately exit the currently-executing `while` loop. Execution resumes in the next statement after the loop.
  * `cont` with `N=1` immediately end the current iteration and begin the next iteration of the current `while` loop (re-evaluate condition).
  * For `N > 1`:

    * The `break N` / `cont N` is intended to target an outer loop: it pops (i.e., exits) the current loop and applies `break`/`cont` to the outer loop with `N-1`.
    * Implementation: `break` and `cont` are implemented via exceptions carrying `count`; an inner loop’s handler, on catching `BreakException`/`ContinueException` with `count > 1`, rethrows with `count - 1` so the event bubbles outward until it is consumed by the targeted outer loop, or until none remain → runtime error if it escapes program-level.
  * If `break N` / `cont N` escapes all loops (i.e., N exceeds number of nested loops), it becomes an unhandled exception → runtime error.

## 7.8 `once` / `onward`

* Syntax:

  * `once { ... }` — block runs only **on the first iteration** of the immediate enclosing `while`.
  * `onward { ... }` — block runs only on **subsequent iterations** (i.e., not the first) of the immediate enclosing `while`.
* Braces are required: single-statement omission is not supported for `once`/`onward`.
* Semantics:

  * `once` and `onward` consult the top of the loop stack (`env[0].__loops__[-1]['first_iteration']`):

    * `once`: if `first_iteration` is true, execute block; otherwise do nothing.
    * `onward`: if `first_iteration` is false, execute block; otherwise do nothing.
  * If they appear outside a `while` (no loop context), runtime error.

---

# 8 — Functions & closures

## 8.1 Definition

* `fn name(p1: T1, p2: T2, ...) -> TRet { ... }`
* Return type is optional.
* Functions capture their **defining environment** (a shallow copy of scope list) and store it in the function value as closure. That closure is used as the base environment for calls.

## 8.2 Calling semantics

* Evaluate callee expression then evaluate arguments left-to-right.
* Create a new frame whose base environment is the captured closure and whose topmost scope is the call-local scope for parameters and local `var` declarations executed in the function body.
* Parameter typing:

  * If the parameter has a type, the argument value is converted to that type at call time; conversion failure → runtime error.
* Return:

  * Executing `return expression;` raises a return event inside the function and unwinds to the call site.
  * If returned value exists and function has return type declared → convert value to that type (runtime conversion).
  * If no `return` executed:

    * If return type declared → return the default value for that type.
    * Else → return `int 0`.

## 8.3 Methods & bound methods

* Class methods are functions declared inside class body.
* Accessing `obj.method` yields a bound function value that, when called, sets up an environment where:

  * The class closure environment (captured at class definition time) is available,
  * the object’s instance fields are presented as a local scope (so method bodies can read/write fields by name),
  * then a call-local scope for parameters.
* Methods can mutate instance fields directly by assignment to a field name in the method body.

## 8.4 First-class functions

* Functions are values; you can assign them to variables, print a compact signature, and call them via a variable name (`var f = add; f(1,2);`).
* Functions carry metadata: params, optional return type, body AST, closure.

---

# 9 — Arrays — representation & methods

## 9.1 Literal & type

* Array literal: `[ e1, e2, ..., en ]` (optional trailing comma allowed). Each `ei` is an expression evaluated left-to-right.
* Arrays maintain:

  * `data`: list of `Val` elements (preserve values as `Val`).
  * `elem`: optional element type (if all literal elements have the same static runtime type, this is inferred; otherwise `None`).
  * `cap`: capacity (always a power-of-two ≥ len(data)). On initialization, `cap` = next power of two ≥ len(data) (min 1).

## 9.2 Indexing & negative indices

* `a[i]`:

  * `i` must be an `int`.
  * Negative `i` interpreted as `len + i`.
  * `index out of range` if resolved index not in `[0, len-1]`.

## 9.3 Mutability

* Arrays are mutable: assignment to `a[idx] = v` updates element at index.

## 9.4 Methods & properties

* `a.count` — `int`, number of elements in the array (read-only).
* `a.capacity` — `int`, internal capacity.
* `a.insert(value, index)` — inserts `value` before `index` position. Negative index allowed; if index < 0, treated as `len + index + 1`. Returns the insertion position as `int`. Adjusts `cap` to next power-of-two if needed.
* `a.remove(index)` — removes and returns the element at `index`. Negative index allowed. `Index out of range` → runtime error.
* `a.insert_front(value)` — insert at beginning; returns 0.
* `a.insert_back(value)` — append; returns new index.
* `a.remove_front()` — remove and return first element; error if empty.
* `a.remove_back()` — remove and return last element; error if empty.
* `a.reserve(count)` — set capacity to next power-of-two ≥ `count` (no change in `data` contents). Returns new capacity.

## 9.5 Method binding and calling

* `a.insert` etc. accessed via `a.insert` returns a bound builtin function value. Calling it invokes the array method with the array object as implicit receiver. Implementation ensures arguments are validated.

---

# 10 — Classes & objects

## 10.1 Definition

```
class Name {
    var field1: Type1 = default1;
    var field2: Type2;
    fn method1(...) { ... }
}
```

* Fields declared inside class body define instance fields and optional default initializers (expressions evaluated when an instance is created).
* Methods declared inside class body have access to fields as local variables (see binding semantics).

## 10.2 Instantiation

* `Name(arg1, arg2, ...)` constructs a new object:

  * Evaluate arguments left-to-right.
  * For each field in declaration order:

    * If the class declaration provided a default expression, evaluate it in the **current calling environment** (not the class definition environment) to produce the initializer; then override with positional argument if provided.
    * If no default expression and no argument provided, use type default for declared type, or `int 0` if no type declared.
  * The object stores field values and a reference to method bodies and class metadata.
* If more positional args than fields → runtime error.

## 10.3 Field access and assignment

* `obj.field` reads field value; `obj.field = expr;` assigns into the object field. Accessing or assigning a non-existent field is a runtime error.

## 10.4 Methods

* `obj.method(...)` returns and calls a **bound method** that sets up an environment where:

  * The method's closure (captured at class definition time) is available as base,
  * The instance’s fields are presented as a local scope (so methods can read / write fields by name),
  * The call-local scope for parameters follows.
* Calling a method uses normal function return semantics.

---

# 11 — Special constructs

## 11.1 `include "file";`

* At evaluation time, `include "path";`:

  * Reads the file `path` (relative to current working directory).
  * Lexes, parses, and executes the included file’s AST **in the same environment** (same `env` stack), i.e., included file can define functions and variables visible afterward.
  * Inclusion is executed immediately and not cached (multiple includes re-run the included file).
  * File-not-found or parse error in included file is a runtime error in the including program.

## 11.2 `once` / `onward`

* See §7.8; they require braces and must appear within a `while` loop block.

---

# 12 — I/O: `scan` and `print`

* `scan` reads a line from standard input when evaluated. It yields a `string` (without trailing newline).
* `print` prints a string (converted via `to_string`) followed by newline (reference implementation prints newline).

`to_string` rules:

* `bool` → `"true"` / `"false"`
* `char` → single char
* `string` → its content
* `int` → decimal representation
* `float` → host float `repr`
* `array` → printable representation `[e1, e2, ...]` (implementation may limit depth)
* `fn` → compact signature `<fn (p1:t1, ...) -> ret>`
* `object` → compact `<obj ClassName>` representation (fields not expanded unless explicitly printed via helper)

---

# 13 — Conversions & coercion rules (`as`)

### 13.1 Implicit vs explicit

* Implicit:

  * `int` → `float` when needed in arithmetic with a float operand.
  * `char` → `string` when context requires string.
  * String concatenation: non-strings are converted to string implicitly within `+` when other operand is string.
* Explicit (`as`):

  * `float as int` truncates fractional part via host integer cast.
  * `int as string`, `float as string`, `bool as string`, `char as string` → serialization.
  * `string as int/float/bool/char` → parsing; parse failure → runtime error.
  * `string as array/object` not supported.

### 13.2 Conversion error behavior

* On conversion failure at runtime, an error is raised and execution halts.

---

# 14 — Error model & diagnostics

* **Lexical errors**: unrecognized tokens, unterminated string/char, unterminated nested comment → lexer error with line number.
* **Parse errors**: unexpected token, mismatched parentheses/braces → syntax error with line number.
* **Runtime errors**: undefined variable, type mismatch, conversion failure, index out-of-range, division-by-zero, calling non-callable, object field not found, invalid `once` outside loop, unmatched `break/cont` depth cause unhandled exception → interpreter prints a short message and halts.
* **Control exceptions**: `return` is implemented as an internal unwind (not visible as user exception). `break` and `cont` are implemented as control exceptions that bubble out to target loops.
* Error messages should include line numbers and a compact description (reference interpreter prints minimal messages; you may extend with stack traces).

---

# 15 — Implementation notes and evaluation strategy

* **Evaluation order**:

  * Left-to-right for function callee then arguments, for binary operator operands, and inside array literals.
  * Short-circuit evaluation for `&&` and `||`.
* **Scoping**:

  * Lexical scoping implemented by a stack of environments. A function’s closure captures the current environment stack shallowly at definition time.
  * When calling functions or entering blocks, the interpreter creates new local maps (scopes) appended to copies of closure env as appropriate.
* **Performance**:

  * Arrays are backed by host lists; capacity maintained as power-of-two; reserve uses next power-of-two.
* **Storage model**:

  * All values are boxed as `Val(typ, v)`.
* **Control-flow events**:

  * `ReturnException`, `BreakException(count)`, `ContinueException(count)` are internal exceptions used to implement control transfer across nested constructs.
* **Include path resolution**:

  * `include` uses file system path relative to process CWD. No module system or import caching is provided in basic interpreter.

---

# 16 — Examples (concise)

**Factorial**

```adb
fn fact(n: int) -> int {
    if n <= 1 { return 1; }
    return n * fact(n - 1);
}
print fact(5);  # prints 120
```

**While with once/onward**

```adb
var i = 0;
while i < 3 {
  once { print "first"; }
  onward { print "next"; }
  print i;
  i = i + 1;
}
# Output:
# first
# 0
# next
# 1
# next
# 2
```

**Arrays**

```adb
var a: int[] = [1, 2, 3];
print a.count;       # 3
a.insert(99, 1);
print a[1];          # 99
a[1] = 42;
print a[1];          # 42
var x = a.remove_back();
print x;             # 3
a.reserve(100);
print a.capacity;    # 128 (next power-of-two)
```

**Classes**

```adb
class Foo {
  var value: int = 0;
  fn inc() { value = value + 1; }
  fn get() -> int { return value; }
}

var o = Foo(10);
o.inc();
print o.get();  # 11
o.value = 99;
print o.value;  # 99
```

**Break/Continue depth**

```adb
var i = 0;
while i < 3 {
  var j = 0;
  while j < 5 {
    if j == 2 { j = j + 1; cont 2; }   # continue outer loop (skip to next i)
    if j == 4 { break; }               # break inner loop
    print i * 10 + j;
    j = j + 1;
  }
  i = i + 1;
}
```

---

# 17 — Annex: sample AST node shapes

(Reference shapes used by the interpreter.)

* Program: `('program', [stmt, ...])`
* Statements:

  * `('var_decl', name, type_or_tuple_or_None, expr_or_None)`
  * `('assign', target_ast, expr_ast)`
  * `('print', expr_ast)`
  * `('include', path_string)`
  * `('return', expr_or_None)`
  * `('break', depth_int)`
  * `('cont', depth_int)`
  * `('once', block_ast)`
  * `('onward', block_ast)`
  * `('if', cond_ast, then_block_ast, else_block_or_None)`
  * `('while', cond_ast, body_block_ast, else_block_or_None)`
  * `('fn_def', name, params_list, ret_type_or_None, body_block_ast)`
  * `('class_def', name, fields_list, methods_map)`
  * `('expr', expr_ast)`
* Expressions:

  * `('lit', value, type_string)`
  * `('var', name)`
  * `('unary', op, expr)`
  * `('fact', expr)`
  * `('as', expr, type_string)`
  * `('call', callee_expr, [arg_exprs])` — callee_expr may be `('var', name)` or any expression
  * `('get', expr, attr_name)`
  * `('index', expr, index_expr)`
  * Binary operators: `('+', left, right)`, `('-', left, right)`, `('*', left, right)`, etc.
  * `('array_lit', [exprs])`

---

# Closing notes, implementation conformance & extensions

* This specification describes the semantics implemented by the reference interpreter and is intended to be precise enough for:

  * writing programs,
  * implementing additional tooling (type checker, bytecode compiler),
  * and porting the language to another host language.
* **Extensions** that fit naturally:

  * `for` loops, iterators, comprehensions
  * static type checking & optional ahead-of-time checks
  * first-class continuations or proper tail calls
  * named constructor args & defaulting in classes
  * module system & include caching
* **Design decisions recorded**:

  * `once` / `onward` require braces to avoid ambiguous parsing.
  * `break` / `cont` numbered semantics use bubbling exceptions for clarity and simplicity.
  * Arrays use power-of-two capacity for simple amortized performance.

If you want, I can:

* produce a formal type system (`typing judgments`) for annotated programs, or
* write a formal small-step operational semantics (in inference-rule form) for expressions and statements, or
* produce a compact language reference card (two pages) suitable for printing.

(You didn’t ask for any of those explicitly; I’m listing them so you can pick one if you want further formalization.)
