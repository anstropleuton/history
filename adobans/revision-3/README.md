## Adobans Programming Language

Adobans is an experimental programming language primarily intended for compiler and interpreter writing practice. Do not use in production.

### Features

- Simple syntax inspired by C++ and Rust
- Statically typed
- Both compiled and interpreted modes
- Basic data types: integers, floats, strings, booleans, and characters
- Basic control flow constructs
- Function definitions and calls
- Class and object support

### Architecture

- **Adobans Processor**: Convert source code to an intermediate representation.
  - **Lexer**: Recognize tokens in the source code, skip whitespace and comments.
  - **Parser**: Generate an abstract syntax tree from the tokens.
  - **Analyzer**: Analyze the abstract syntax tree by filling metadata, perform type checking, etc.
  - **IR Generator**: Generate an IR from the analyzed tree.
- **Adobans Compiler**: Convert IR into C code.
  - **C Generator**: Generate C code from IR.
- **Adobans Interpreter**: Interpret IR in-memory.
  - **In-memory Interpreter**: Interpret each instructions of IR.

### Comments

Comments start with `#` and continue to the end of the line.

```adb
# This is a comment
```

### Variables

Variables are declared using the `let` keyword.

```adb
let x: int = 10;
let name = "Adobans"; # Type inference
```

### Data Types

- `int`: Integer numbers
- `float`: Floating-point numbers
- `string`: Text strings
- `bool`: Boolean values (`true` or `false`)
- `char`: Single characters

### Control Flow

#### If Statements

```adb
if x > 0 {
    io.println("Positive");
} else {
    io.println("Non-positive");
}
```

#### While Loops

```adb
while x < 10 {
    x = x + 1;
}
```

#### For Loops

```adb
for let i in 0:10 {
    io.println(i);
}

for let i = 0; i < 10; i++ {
    io.println(i);
}
```

### String Interpolation

String interpolation is done using the `${}` syntax within double-quoted strings.

```adb
let name: string = "Adobans";
let greeting: string = "Hello, ${name}!";
io.println(greeting); # Outputs: Hello, Adobans!
```

### Functions

Functions are defined using the `fn` keyword.

```adb
fn add(a: int, b: int) -> int {
    return a + b;
}; # Note the semicolon at the end of function definitions is mandatory
```

Calling a function:

```adb
let result: int = add(5, 10);
```

Supports return type inference:

```adb
fn welcome_message(name: string) {
    return "Welcome, ${name}!";
};
```

### Classes

Classes are defined using the `class` keyword.

```adb
class Person {
    let name: string;
    let age: int;

    fn greet() {
        io.println("Hello, my name is ${name} and I am ${age} years old.");
    };
}; # Note the semicolon at the end of class definitions is mandatory
```

Creating an instance of a class:

```adb
let person = Person(.name = "Alice", .age = 30);
person.greet();
```

### Namespaces

Namespaces are used to organize code and prevent naming conflicts.

```adb
ns math {
    fn square(x: int) -> int {
        return x * x;
    };
};
```

### Import/Export

`utils.adb`:
```adb
import io: adb.io; # Import as alias, so we can use io.println rather than adb.io.println

export fn helper() {
    io.println("This is a helper function.");
};
```

`main.adb`:
```adb
import helper; # Symbol based import, has nothing to do with file names

helper();
```

### Standard Library

- `io.adb`: Defines:
    - `println(message: string)`: Prints a message to the console with a newline.
    - `print(message: string)`: Prints a message to the console without a newline.
    - `scanln() -> string`: Reads a line of input from the console.
    - `scan() -> string`: Reads input from the console until whitespace.
- `math.adb`: Defines:
    - `sqrt(x: float) -> float`: Returns the square root of a number.
    - `pow(base: float, exponent: float) -> float`: Returns the base raised to the exponent power.
    - `abs(x: float) -> int`: Returns the absolute value of an integer.
    - `max(a: int, b: int) -> int`: Returns the maximum of two integers.
    - `min(a: int, b: int) -> int`: Returns the minimum of two integers.
    - `sin(x: float) -> float`: Returns the sine of an angle in radians.
    - `cos(x: float) -> float`: Returns the cosine of an angle in radians.
    - `tan(x: float) -> float`: Returns the tangent of an angle in radians.

### Conclusion

That's about it! Adobans is just a small experimental language for learning purposes.

### License

This project is licensed under the MIT License. See the LICENSE file for details.
