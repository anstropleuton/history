# Sitroxia - Programming Language

Sitroxia is a statically typed, compiled and interpreted programming language written in Rust. It is extremely small and simple, with only a few features. It is designed to be easy to implement.

Sitroxia is educational programming language, and is not intended for production use. It is designed to help me learn about programming language design and implementation.

For simplicity and ease of implementation, Sitroxia compiler compiles to standard C++, and Sitroxia interpreter interprets in-memory. This allows for easy translation into C++ utilizing the C++ standard library, and in-memory execution without the need for a separate runtime environment or complex infrastructure, at the cost of requiring a C++ compiler and slower interpretation.

This repository contains the source code for the Sitroxia programming language, as well as some example programs and documentation.

## Features

- Statically typed
- Compiled and interpreted
- Simple syntax
- Small standard library
- Easy to implement

## Specification

This section covers the specification of the Sitroxia programming language in an informal way. It can also be used as a tour of the language.

### File Extension

Sitroxia source files have the extension `.stx`.

### Naming Conventions

All entities in Sitroxia (variables, functions, classes, etc.) must be named using snake_case. This means that all letters must be lowercase and words must be separated by underscores.

```stx
let my_variable = 5;
fn my_function() {};
class my_class {};
```

### Comments

Comments in Sitroxia start with `#` and continue until the end of the line. They are ignored by the engine (compiler and interpreter).

```stx
# This is a comment
```

### Variables

Variables in Sitroxia are declared using the `let` keyword, followed by the variable name and an optional type annotation. The type annotation is not required, as Sitroxia has type inference.

```stx
let x = 5; # Type inferred as int
let y: float = 3.14; # Type explicitly annotated as float
```

Unlike Rust, variables in Sitroxia are mutable by default. To make a variable immutable, you can use the `!` modifier after the type.

```stx
let x: int! = 5; # Immutable variable
```

Variable shadowing is allowed in Sitroxia, which means that you can declare a new variable with the same name as an existing variable. The new variable will shadow the old variable within its scope.

```stx
let x = 5; # x is an int with the value 5
let x = "hello"; # x is now a string with the value "hello", shadowing the previous x variable
```

### Type Checking & Conversion

Type checking in Sitroxia allows inspecting the type of a value or expression. This can be done using the `is` keyword.

```stx
let x = 5;
if x is int {
    # Code to execute if x is of type int
}
```

Type conversion in Sitroxia is done implicitly or by using the `as` keyword. This allows you to convert a value from one type to another.

```stx
let x = 5; # x is an int
let y = x as float; # y is a float with the value 5.0
```

### `if` Statements

`if` statements in Sitroxia are used for conditional execution. They consist of an `if` keyword, a condition, and a block of code to execute if the condition is true. An optional `else` block can be included for the case when the condition is false.

```stx
if x > 0 {
    # Code to execute if x is greater than 0
} else {
    # Code to execute if x is not greater than 0
}
```

`if` statments can also contain arbitrary statement before the condition, including variable declarations and function calls.

```stx
if let y = some_function(); y > 0 {
    # Code to execute if y is greater than 0, with y being the result of some_function()
} else {
    # Code to execute if y is not greater than 0, with y being the result of some_function()
}
```

### `while` Loops

`while` loops in Sitroxia are used for repeated execution of a block of code as long as a condition is true. They consist of a `while` keyword, a condition, and a block of code to execute.

Like Python, `while` loops can also contain an optional `else` block that executes if the loop was never entered.

```stx
while x > 0 {
    # Code to execute while x is greater than 0
    x = x - 1; # Decrement x to eventually break the loop
} else {
    # Code to execute if the loop was never entered
}
```

`while` also supports arbitrary statements before the condition, including variable declarations and function calls.

```stx
while let y = some_function(); y > 0 {
    # Code to execute while y is greater than 0, with y being the result of some_function() (executed only once)
} else {
    # Code to execute if the loop was never entered, with y being the result of some_function() (executed only once)
}
```

### `for` Loops

`for` loops in Sitroxia are used for iterating over a range of values. They consist of a `for` keyword, a loop variable, the `in` keyword, and a range expression. The block of code to execute for each value is enclosed in curly braces.

Like Python, `for` loops can also contain an optional `else` block that executes if the loop was never entered (i.e., if the range was empty).

```stx
for let i in 0:10 {
    # Code to execute for each value of i from 0 to 9
} else {
    # Code to execute if the loop was never entered (e.g., if the range was empty)
}
```

Traditional C-style `for` loops is also supported, which consist of an initialization statement, a condition, and an increment statement.

```stx
for let i = 0; i < 10; i++ {
    # Code to execute for each value of i from 0 to 9
}
```

### `break` and `cont`

The `break` statement in Sitroxia is used to exit a loop immediately, while the `cont` statement is used to skip the rest of the current iteration and move on to the next iteration of the loop.

```stx
while x > 0 {
    if x == 5 {
        break; # Exit the loop if x is equal to 5
    }
    if x % 2 == 0 {
        cont; # Skip the rest of the loop body if x is even
    }
}
```

### Scoping

Sitroxia uses lexical scoping, which means that the scope of a variable is determined by its position in the source code. Variables declared inside a block (e.g., inside a function or an `if` statement) are only accessible within that block and its nested blocks.

```stx
fn example() {
    let x = 5; # x is only accessible within the example function
    if x > 0 {
        let y = 10; # y is only accessible within this if block
    }
    # y is not accessible here, outside the if block
};
```

### Functions

Functions in Sitroxia are declared using the `fn` keyword, followed by the function name, parameters, and an optional return type. The function body is enclosed in curly braces. The return type is not required, as Sitroxia has type inference.

```stx
fn add(a: int, b: int) -> int {
    return a + b;
}; # The semicolon is required after the function declaration
```

### Classes

Classes in Sitroxia are declared using the `class` keyword, followed by the class name and an optional list of fields. The class body is enclosed in curly braces.

```stx
class point {
    let x: int;
    let y: int;
    
    fn distance() -> float {
        return (x * x + y * y) as float;
    };
}; # The semicolon is required after the class declaration
```

Creating an instance of the class is done using the class name followed by parentheses like a function call.

```stx
let p = point(3, 4); # Create an instance of the point class with x=3 and y=4
```

### Namespaces

Namespaces in Sitroxia are declared using the `ns` keyword, followed by the namespace name. The namespace body is enclosed in curly braces.

```stx
ns math {
    fn add(a: int, b: int) -> int {
        return a + b;
    };
}; # The semicolon is required after the namespace declaration
```

### Importing & Exporting

Modules in Sitroxia are imported using the `import` keyword, followed by the module name. The module name can be a path to a file or a namespace.

```stx
import math; # Import the math namespace
import math.add; # Import the add function from the math namespace

ns utils {
    # Export the helper function to be used in other modules
    export fn helper() {
    };
};

# Export the entire config namespace to be used in other modules
export ns config {
    let setting: string = "default";
};
```

Imports and Exports are not path based; they are name based. This means that you can import and export entities from any module, regardless of their location in the file system.

### Nullable Types

Nullable types in Sitroxia are represented using the `?` operator. This allows you to indicate that a value may be null.

```stx
let x: int? = 5; # x is an optional int with the value Some(5)
let y: int? = null; # y is an optional int with the value None

if x != null {
    # Code to execute if x is not null
}
```

### Arrays

Arrays in Sitroxia are declared using square brackets `[]` and can hold elements of any type. The type of the elements can be specified using a type annotation.

```stx
let arr: int[] = [1, 2, 3, 4, 5]; # An array of integers

for let elm in arr {
    # Code to execute for each element in the array
}
```

An untyped array can also be declared, which can hold elements of any type.

```stx
let arr = [1, "two", 3.0]; # An untyped array

for let elm in arr {
    if elm is int {
        # Code to execute if elm is an integer
    } else if elm is string {
        # Code to execute if elm is a string
    } else if elm is float {
        # Code to execute if elm is a float
    }
}
```

A typed array can also be declared without an initializer, in which case it will be initialized with default values (e.g., `0` for `int`, `0.0` for `float`, `false` for `bool`, and empty string for `string`).

```stx
let arr: int[5]; # An array of 5 integers, initialized with default values
```

A static-size array can be declared by specifying the size in square brackets.

```stx
let arr: int[5] = [1, 2, 3, 4, 5]; # A fixed-size array of 5 integers
```

A dynamic-size array can be declared by omitting the size in square brackets.

```stx
let arr: int[] = [1, 2, 3]; # A dynamic-size array of integers, which can be resized later

arr.insert(4, 3); # Insert the value 4 at index 3, resizing the array to accommodate the new element
arr.remove(1); # Remove the element at index 1, resizing the array to fill the gap
arr.size(); # Get the current size of the array
```

### String Interpolation

String interpolation in Sitroxia is done using the `${}` syntax. This allows you to embed expressions inside string literals, which will be evaluated and included in the resulting string.

```stx
let name = "Alice";
let greeting = "Hello, ${name}!"; # The value of greeting will be "Hello, Alice!"
```

### Generics

Generics in Sitroxia are declared using angle brackets `<>` and can be used to define functions and classes that can work with any type.

```stx
fn identity:<t>(value: t) -> t {
    return value;
};
```

Using the generic function:

```stx
let x = identity:<int>(5); # x will be of type int with the value 5
let y = identity:<string>("hello"); # y will be of type string with the value "hello"
```

Type inference can also be used with generics, allowing the type to be inferred from the context.

```stx
let x = identity(5); # x will be of type int with the value 5
let y = identity("hello"); # y will be of type string with the value "hello"
```

### Appendix A - List of types

- `int`: A signed integer type.
- `float`: A floating-point number type.
- `bool`: A boolean type that can be either `true` or `false`.
- `string`: A string type for representing text.
- `nullable`: A type that can be either value or `null`.
- `void`: A type that represents the absence of a value, used for functions that do not return anything.

### Appendix B - List of operators

- Arithmetic Operators: `+`, `-`, `*`, `/`, `%`, `**`
- Comparison Operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Logical Operators: `&&`, `||`, `!`
- Bitwise Operators: `&`, `|`, `^`, `~`
- Assignment Operators: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
- Increment and Decrement Operators: `++`, `--`

### Appendix C - List of keywords

- `as`
- `break`
- `class`
- `cont`
- `else`
- `export`
- `false`
- `fn`
- `for`
- `if`
- `import`
- `is`
- `let`
- `null`
- `ns`
- `return`
- `true`
- `while`

## Standard Library

The standard library of Sitroxia is small and simple. It provides basic functionality for working in the language, such as input/output, string manipulation, and basic data structures.

### Input/Output

The `io` module provides functions for reading from and writing to the console.

- `io.println(value: string)`: Prints a line of text to the console.
- `io.scanln() -> string`: Reads a line of input from the console and returns it as a string.
- `io.file`: Provides functions for reading from and writing to files.
- `io.open(file: string, mode: string) -> file`: Opens a file with the specified `mode` and returns a file handle.
- `io.close(file: file)`: Closes the specified file handle.
- `io.file.read(file: file) -> string`: Reads the contents of the specified file and returns it as a string.
- `io.file.write(file: file, content: string)`: Writes the specified `content` to the specified file.

### String Manipulation

The `str` module provides functions for working with strings.

- `str.upper(s: string) -> string`: Returns a new string with all characters in `s` converted to uppercase.
- `str.lower(s: string) -> string`: Returns a new string with all characters in `s` converted to lowercase.
- `str.trim(s: string) -> string`: Returns a new string with leading and trailing whitespace removed from `s`.
- `str.split(s: string, delimiter: string) -> string[]`: Splits the string `s` into an array of substrings using the specified `delimiter`.
- `str.join(arr: string[], delimiter: string) -> string`: Joins an array of strings into a single string, with the specified `delimiter` between each element.
- `str.find(s: string, substring: string) -> int`: Returns the index of the first occurrence of `substring` in `s`, or `-1` if `substring` is not found.
- `str.replace(s: string, old: string, new: string) -> string`: Returns a new string with all occurrences of `old` in `s` replaced with `new`.
- `str.contains(s: string, substring: string) -> bool`: Returns `true` if `s` contains `substring`, and `false` otherwise.

### Array Manipulation

The `arr` module provides functions for working with arrays.

- `arr.find:<t>(arr: t[], value: t) -> int`: Returns the index of the first occurrence of `value` in `arr`, or `-1` if `value` is not found.
- `arr.replace:<t>(arr: t[], old: t, new: t) -> T[]`: Returns a new array with all occurrences of `old` in `arr` replaced with `new`.
- `arr.contains:<t>(arr: t[], value: t) -> bool`: Returns `true` if `arr` contains `value`, and `false` otherwise.

### Math

The `math` module provides functions for working with numbers.

- `math.abs(x: float) -> float`: Returns the absolute value of `x`.
- `math.sqrt(x: float) -> float`: Returns the square root of `x`.
- `math.cbrt(x: float) -> float`: Returns the cube root of `x`.
- `math.pow(base: float, exponent: float) -> float`: Returns `base` raised to the power of `exponent`.
- `math.sin(x: float) -> float`: Returns the sine of `x` (where `x` is in radians).
- `math.cos(x: float) -> float`: Returns the cosine of `x` (where `x` is in radians).
- `math.tan(x: float) -> float`: Returns the tangent of `x` (where `x` is in radians).
- `math.pi`: A constant representing the value of π (approximately 3.14159).
- `math.e`: A constant representing the value of e (approximately 2.71828).
- `math.max(a: float, b: float) -> float`: Returns the maximum of `a` and `b`.
- `math.min(a: float, b: float) -> float`: Returns the minimum of `a` and `b`.
- `math.round(x: float) -> int`: Rounds `x` to the nearest integer and returns it as an `int`.
- `math.floor(x: float) -> int`: Rounds `x` down to the nearest integer and returns it as an `int`.
- `math.ceil(x: float) -> int`: Rounds `x` up to the nearest integer and returns it as an `int`.

### Date and Time

The `dt` module provides functions for working with dates and times.

- `dt.now() -> datetime`: Returns the current date and time as a `datetime` (date & time) object.
- `dt.year(date: datetime) -> int`: Returns the year component of the given `date`.
- `dt.month(date: datetime) -> int`: Returns the month component of the given `date`.
- `dt.day(date: datetime) -> int`: Returns the day component of the given `date`.
- `dt.hour(date: datetime) -> int`: Returns the hour component of the given `date`.
- `dt.minute(date: datetime) -> int`: Returns the minute component of the given `date`.
- `dt.second(date: datetime) -> int`: Returns the second component of the given `date`.
- `dt.millisecond(date: datetime) -> int`: Returns the millisecond component of the given `date`.
- `dt.weekday(date: datetime) -> int`: Returns the weekday component of the given `date`.
- `dt.week(date: datetime) -> int`: Returns the week component of the given `date`.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
