# Adobans Language - Production Implementation Plan

## Overview

This plan outlines the complete production-ready implementation of the Adobans programming language in Rust. The implementation will follow idiomatic Rust practices with zero hacks or shortcuts, comprehensive test coverage, and clear separation of concerns across multiple phases.

## Project Structure

```
adobans/
├── src/
│   ├── main.rs                 # CLI entry point
│   ├── lib.rs                  # Library root
│   ├── lexer/
│   │   ├── mod.rs
│   │   ├── token.rs
│   │   ├── scanner.rs
│   │   └── errors.rs
│   ├── parser/
│   │   ├── mod.rs
│   │   ├── ast.rs
│   │   ├── builder.rs
│   │   └── errors.rs
│   ├── analyzer/
│   │   ├── mod.rs
│   │   ├── semantic.rs
│   │   ├── type_checker.rs
│   │   ├── symbol_table.rs
│   │   ├── type_system.rs
│   │   └── errors.rs
│   ├── ir/
│   │   ├── mod.rs
│   │   ├── instruction.rs
│   │   ├── builder.rs
│   │   ├── optimizer.rs
│   │   └── lowering.rs
│   ├── codegen/
│   │   ├── mod.rs
│   │   ├── c_backend.rs
│   │   └── emitter.rs
│   ├── interpreter/
│   │   ├── mod.rs
│   │   ├── executor.rs
│   │   ├── runtime.rs
│   │   ├── value.rs
│   │   ├── memory.rs
│   │   └── builtins.rs
│   ├── stdlib/
│   │   ├── mod.rs
│   │   ├── io.adb
│   │   ├── math.adb
│   │   └── builtin_loader.rs
│   ├── diagnostics/
│   │   ├── mod.rs
│   │   ├── error.rs
│   │   ├── warning.rs
│   │   └── reporter.rs
│   └── cli/
│       ├── mod.rs
│       └── options.rs
├── tests/
│   ├── integration/
│   │   ├── lexer_tests.rs
│   │   ├── parser_tests.rs
│   │   ├── semantic_tests.rs
│   │   ├── codegen_tests.rs
│   │   └── interpreter_tests.rs
│   ├── e2e/
│   │   ├── hello_world.rs
│   │   ├── control_flow.rs
│   │   ├── functions.rs
│   │   ├── classes.rs
│   │   ├── stdlib.rs
│   │   ├── string_interpolation.rs
│   │   └── namespaces.rs
│   └── fixtures/
│       ├── valid/
│       │   ├── simple.adb
│       │   ├── complex.adb
│       │   └── ...
│       ├── invalid/
│       │   ├── syntax_error.adb
│       │   ├── type_error.adb
│       │   └── ...
│       └── stdlib/
│           ├── io_tests.adb
│           └── math_tests.adb
├── Cargo.toml
├── Cargo.lock
└── docs/
    ├── architecture.md
    ├── type_system.md
    └── ir_spec.md
```

## Phase 1: Foundation & Core Infrastructure

### 1.1 Project Setup & Tooling
- [x] Initialize Cargo project with `lib` and `bin` targets
- [x] Configure `Cargo.toml` with appropriate dependencies:
  - `thiserror` or `anyhow` for error handling
  - `clap` for CLI argument parsing
  - `strum` for enum serialization (if needed)
- [x] Set up clippy and rustfmt configuration
- [x] Create directory structure as outlined above
- [x] Document development guidelines

### 1.2 Error Handling & Diagnostics

**Goal**: Establish robust, zero-cost error handling with excellent error messages.

#### Error Types
- Create `diagnostics/error.rs` with proper error types:
  - `LexerError`: Invalid characters, unterminated strings, invalid escape sequences
  - `ParserError`: Unexpected tokens, missing semicolons, mismatched braces
  - `AnalyzerError`: Undefined variables, type mismatches, duplicate definitions
  - `IRError`: Invalid IR construction, unreachable code
  - `CodegenError`: C backend generation failures
  - `RuntimeError`: Division by zero, out of bounds, null pointer dereference

#### Error Reporting System
- Implement source location tracking (`Span`) with file path and line/column numbers
- Create `Diagnostic` type with severity levels (Error, Warning, Note)
- Implement pretty error printing with context display
- No string-based error passing; all errors must be typed
- All errors must be recoverable or clearly terminal

#### Test Coverage
- Unit tests for each error variant creation
- Integration tests for error message formatting
- Tests for multi-error reporting (collecting all errors in a compilation)
- Ensure all public error APIs are tested

### 1.3 Token & Lexical Analysis

**Goal**: Implement a complete, well-tested lexer that handles all Adobans syntax.

#### Token Definition (`lexer/token.rs`)
```rust
pub enum TokenKind {
    // Literals
    Integer(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Char(char),
    
    // Keywords
    Let, Fn, Return, If, Else, While, For, In, Class, Ns, Import, Export, True, False,
    
    // Identifiers
    Ident(String),
    
    // Operators
    Plus, Minus, Star, Slash, Percent, Eq, NotEq, Lt, Le, Gt, Ge, Assign, And, Or, Not,
    PlusAssign, MinusAssign, StarAssign, SlashAssign,
    Increment, Decrement,
    
    // Punctuation
    LBrace, RBrace, LParen, RParen, LBracket, RBracket,
    Comma, Dot, Colon, Semicolon, Arrow, DotDotDot,
    StringInterpolStart, StringInterpolEnd,
    
    // Special
    Eof,
}

pub struct Token {
    pub kind: TokenKind,
    pub span: Span,
}
```

#### Lexer Implementation (`lexer/scanner.rs`)
- Implement stateful lexer with proper lookahead
- Support for:
  - Single-line comments (`#`)
  - String literals with escape sequences
  - String interpolation tokenization (`${...}`)
  - Character literals
  - Multi-character operators
  - Integer and floating-point number parsing
  - Keyword recognition with proper identifier handling
- No regex-based tokenization; hand-written scanner for performance and clarity
- Single-pass scanning with O(n) complexity
- Proper handling of all edge cases:
  - Unterminated strings
  - Invalid escape sequences
  - Invalid unicode
  - Numeric overflow detection

#### Test Coverage
- Property-based tests for all valid token types
- Unit tests for each token kind
- Tests for boundary cases (very large numbers, empty strings, etc.)
- Tests for all error conditions
- Regression tests for edge cases found during development
- Benchmarks for lexer performance

---

## Phase 2: Parsing & Abstract Syntax Tree

### 2.1 AST Definition

**Goal**: Create a comprehensive, type-safe AST that accurately represents all Adobans constructs.

#### AST Nodes (`parser/ast.rs`)
```rust
pub struct Program {
    pub items: Vec<TopLevelItem>,
}

pub enum TopLevelItem {
    FunctionDef(FunctionDef),
    ClassDef(ClassDef),
    NamespaceDef(NamespaceDef),
    ImportDecl(ImportDecl),
    ExportDecl(ExportDecl),
}

pub struct FunctionDef {
    pub name: String,
    pub params: Vec<Parameter>,
    pub return_type: Option<Type>,
    pub body: Block,
    pub span: Span,
}

pub struct Parameter {
    pub name: String,
    pub ty: Type,
    pub span: Span,
}

pub enum Type {
    Int,
    Float,
    String,
    Bool,
    Char,
    Custom(String), // For classes
    // No generic types initially
}

pub struct Block {
    pub statements: Vec<Statement>,
    pub span: Span,
}

pub enum Statement {
    VariableDecl(VariableDecl),
    ExpressionStmt(Expression),
    Return(Option<Box<Expression>>, Span),
    If(IfStmt),
    While(WhileStmt),
    For(ForStmt),
}

pub struct VariableDecl {
    pub name: String,
    pub ty: Type,
    pub init: Option<Box<Expression>>,
    pub span: Span,
}

pub enum Expression {
    Literal(Literal),
    Variable(String, Span),
    Binary(Box<Expression>, BinaryOp, Box<Expression>, Span),
    Unary(UnaryOp, Box<Expression>, Span),
    Call(Box<Expression>, Vec<Expression>, Span),
    FieldAccess(Box<Expression>, String, Span),
    Assignment(Box<Expression>, Box<Expression>, Span),
    CompoundAssignment(Box<Expression>, CompoundOp, Box<Expression>, Span),
    IncDec(Box<Expression>, IncDecOp, Span),
    StringInterpolation(Vec<InterpolPart>),
    Block(Block),
}

pub enum Literal {
    Integer(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Char(char),
}

pub enum BinaryOp {
    Add, Sub, Mul, Div, Mod,
    Eq, NotEq, Lt, Le, Gt, Ge,
    And, Or,
}

pub enum UnaryOp {
    Neg, Not,
}

pub enum CompoundOp {
    AddAssign, SubAssign, MulAssign, DivAssign,
}

pub enum IncDecOp {
    Increment, Decrement,
}

pub struct ClassDef {
    pub name: String,
    pub fields: Vec<Field>,
    pub methods: Vec<FunctionDef>,
    pub span: Span,
}

pub struct Field {
    pub name: String,
    pub ty: Type,
    pub span: Span,
}

pub struct NamespaceDef {
    pub name: String,
    pub items: Vec<TopLevelItem>,
    pub span: Span,
}

pub struct ImportDecl {
    pub alias: Option<String>,
    pub path: Vec<String>, // e.g., ["adb", "io"]
    pub span: Span,
}

pub struct ExportDecl {
    pub item: Box<TopLevelItem>,
    pub span: Span,
}
```

Requirements:
- All nodes must include `Span` for error reporting
- Use strong typing to prevent invalid AST construction
- No null values; use `Option` for optional fields
- Immutable by default
- Serializable for debugging (derive Debug, Clone where appropriate)

### 2.2 Parser Implementation (`parser/builder.rs`)

**Goal**: Implement a recursive descent parser with proper error recovery.

#### Parser Structure
```rust
pub struct Parser {
    tokens: Vec<Token>,
    current: usize,
    errors: Vec<ParseError>,
}

impl Parser {
    pub fn parse(tokens: Vec<Token>) -> Result<Program, Vec<ParseError>>;
    
    // Parsing methods follow standard recursive descent pattern
    fn parse_program(&mut self) -> Result<Program, ...>;
    fn parse_top_level_item(&mut self) -> Result<TopLevelItem, ...>;
    fn parse_function_def(&mut self) -> Result<FunctionDef, ...>;
    fn parse_statement(&mut self) -> Result<Statement, ...>;
    fn parse_expression(&mut self) -> Result<Expression, ...>;
    fn parse_primary(&mut self) -> Result<Expression, ...>;
    // ... etc
}
```

#### Parser Capabilities
- Proper operator precedence handling for all binary operators
- Left-associative binary operators
- Right-associative assignment operators
- String interpolation handling (complex token stream manipulation)
- Error recovery: skip to synchronization points (semicolons, braces) on error
- Lookahead of 1-2 tokens (no complex lookahead)
- Precedence climbing for expression parsing

#### Grammar (Formal Specification)
```
program = topLevelItem*

topLevelItem = functionDef | classDef | namespaceDef | importDecl | exportDecl

functionDef = "fn" ident "(" paramList? ")" ("->" type)? "{" statement* "}" ";"

paramList = parameter ("," parameter)*
parameter = ident ":" type

classDef = "class" ident "{" classBody "}"
classBody = (fieldDecl | methodDef)*
fieldDecl = "let" ident ":" type ";"
methodDef = "fn" ident "(" paramList? ")" ("->" type)? "{" statement* "}" ";"

type = "int" | "float" | "string" | "bool" | "char" | ident

statement = varDecl | exprStmt | returnStmt | ifStmt | whileStmt | forStmt | blockStmt

varDecl = "let" ident ":" type ("=" expression)? ";"

exprStmt = expression ";"

returnStmt = "return" expression? ";"

ifStmt = "if" expression "{" statement* "}" ("else" "{" statement* "}")?

whileStmt = "while" expression "{" statement* "}"

forStmt = ("for" "let" ident "in" expression ":" expression) | 
          ("for" "let" ident "=" expression ";" expression ";" exprStmt) "{" statement* "}"

blockStmt = "{" statement* "}"

expression = assignment

assignment = ternary (("=" | "+=" | "-=" | "*=" | "/=") ternary)*

ternary = logicalOr

logicalOr = logicalAnd ("||" logicalAnd)*

logicalAnd = equality ("&&" equality)*

equality = comparison (("==" | "!=") comparison)*

comparison = term (("<" | "<=" | ">" | ">=") term)*

term = factor (("+" | "-") factor)*

factor = unary (("*" | "/" | "%") unary)*

unary = ("!" | "-") unary | postfix

postfix = primary ("++" | "--")?

primary = literal | ident | "(" expression ")" | callExpr | fieldAccessExpr | stringInterp
```

#### Test Coverage
- Unit tests for each parsing rule
- Property-based tests for operator precedence
- Tests for all error recovery scenarios
- Tests for string interpolation parsing
- Regression tests for complex nested structures
- Stress tests with deeply nested expressions
- Tests ensuring all parse errors are properly reported

### 2.3 Error Recovery

- Implement panic mode error recovery
- Synchronization tokens: semicolons, braces, keywords
- Collect multiple errors per compilation attempt
- Clear error messages with suggestions

---

## Phase 3: Semantic Analysis & Type System

### 3.1 Type System Design (`analyzer/type_system.rs`)

**Goal**: Implement a complete, predictable type system with zero surprising coercions.

#### Type Representation
```rust
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Type {
    Int,
    Float,
    String,
    Bool,
    Char,
    Class(ClassId),
}

#[derive(Clone, Debug)]
pub struct ClassInfo {
    pub id: ClassId,
    pub name: String,
    pub fields: HashMap<String, (Type, usize)>, // (type, index)
    pub methods: HashMap<String, FunctionSignature>,
}

#[derive(Clone, Debug)]
pub struct FunctionSignature {
    pub params: Vec<Type>,
    pub return_type: Type,
}
```

#### Type Compatibility Rules
- `int` and `float` are NOT compatible (no implicit conversions)
- `string` concatenation with `+` requires both operands to be `string`
- Comparison operators (`==`, `!=`, `<`, etc.) must have compatible types
- Boolean operators (`&&`, `||`) accept only `bool`
- Function arguments must match parameter types exactly
- Assignment must have identical types

#### Predefined Types
- Primitive types: `int`, `float`, `string`, `bool`, `char`
- User-defined class types
- No `null` or `undefined` type
- All variables must be initialized before use

### 3.2 Symbol Table (`analyzer/symbol_table.rs`)

**Goal**: Implement a scoped symbol table with proper lifetime management.

#### Symbol Table Structure
```rust
pub struct SymbolTable {
    scopes: Vec<Scope>,
}

pub struct Scope {
    symbols: HashMap<String, Symbol>,
    parent: Option<usize>,
}

#[derive(Clone, Debug)]
pub enum Symbol {
    Variable(VariableSymbol),
    Function(FunctionSymbol),
    Class(ClassSymbol),
}

#[derive(Clone, Debug)]
pub struct VariableSymbol {
    pub name: String,
    pub ty: Type,
    pub mutable: bool,
    pub defined_at: Span,
}

#[derive(Clone, Debug)]
pub struct FunctionSymbol {
    pub name: String,
    pub signature: FunctionSignature,
    pub is_extern: bool,
    pub defined_at: Span,
}

#[derive(Clone, Debug)]
pub struct ClassSymbol {
    pub name: String,
    pub info: ClassInfo,
    pub defined_at: Span,
}
```

#### Operations
- `push_scope()` / `pop_scope()` for block entry/exit
- `define_symbol()` with duplicate checking
- `lookup_symbol()` with scope traversal
- Namespace handling with qualified lookup
- No use of `RefCell` or internal mutability; use `&mut self` for modifications

### 3.3 Semantic Analysis (`analyzer/semantic.rs`)

**Goal**: Perform type checking and semantic validation with comprehensive error reporting.

#### Analysis Phases

**Phase 3.3.1: Declaration Collection**
- Traverse AST to collect all declarations (functions, classes, variables)
- Detect duplicate definitions
- Build symbol table for each scope
- Record all `Span`s for error reporting
- Validate that exported items are defined

**Phase 3.3.2: Type Checking**
- Visit each expression and statement
- Check function arguments match signatures
- Check assignment types match variable types
- Check all binary operations have compatible operands
- Check function return types match declared types
- Check that all code paths return (for non-void functions)
- Check variable initialization before use
- Type-check field access on classes

**Phase 3.3.3: Semantic Validation**
- No undefined variable/function/class references
- No unreachable code (after unconditional return)
- All loop variables properly scoped
- All class fields initialized during construction
- No method calls on non-class types
- Validate import paths exist

#### Analyzer Errors
- `UndefinedVariable(name, Span)`
- `UndefinedFunction(name, Span)`
- `UndefinedClass(name, Span)`
- `TypeMismatch { expected, actual, span }`
- `DuplicateDefinition { name, first_def, second_def }`
- `ArgumentCountMismatch { expected, actual, span }`
- `FieldAccessOnNonClass { type_name, span }`
- `UninstantiableClass { name, span }`
- `InvalidOperands { op, left, right, span }`
- `MissingReturn { function, span }`
- `UnreachableCode { span }`

### 3.4 Type Checker (`analyzer/type_checker.rs`)

**Goal**: Provide utilities for type compatibility checking.

```rust
pub struct TypeChecker;

impl TypeChecker {
    pub fn types_compatible(left: &Type, right: &Type) -> bool;
    pub fn type_of_expr(expr: &Expression, context: &AnalysisContext) -> Result<Type, TypeError>;
    pub fn can_assign(target: &Type, value: &Type) -> bool;
    pub fn binary_op_type(op: BinaryOp, left: &Type, right: &Type) -> Result<Type, TypeError>;
}
```

### 3.5 Test Coverage

- Unit tests for symbol table operations
- Tests for type compatibility
- Tests for all semantic error cases
- Tests for forward references (if supported)
- Tests for class method resolution
- Tests for complex nested scopes
- Integration tests combining parsing and analysis

---

## Phase 4: Intermediate Representation (IR)

### 4.1 IR Design (`ir/instruction.rs`)

**Goal**: Design a clean, lower-level IR that can be easily compiled to C or interpreted.

#### IR Instructions

```rust
pub enum Instruction {
    // Constants
    LoadConst { dest: Register, value: Constant },
    
    // Arithmetic
    Add { dest: Register, left: Register, right: Register },
    Sub { dest: Register, left: Register, right: Register },
    Mul { dest: Register, left: Register, right: Register },
    Div { dest: Register, left: Register, right: Register },
    Mod { dest: Register, left: Register, right: Register },
    Neg { dest: Register, operand: Register },
    
    // Comparison
    Eq { dest: Register, left: Register, right: Register },
    NotEq { dest: Register, left: Register, right: Register },
    Lt { dest: Register, left: Register, right: Register },
    Le { dest: Register, left: Register, right: Register },
    Gt { dest: Register, left: Register, right: Register },
    Ge { dest: Register, left: Register, right: Register },
    
    // Logical
    And { dest: Register, left: Register, right: Register },
    Or { dest: Register, left: Register, right: Register },
    Not { dest: Register, operand: Register },
    
    // Control Flow
    Jump { target: Label },
    JumpIfTrue { cond: Register, target: Label },
    JumpIfFalse { cond: Register, target: Label },
    Return { value: Option<Register> },
    
    // Memory & Variables
    Store { var: VarId, value: Register },
    Load { dest: Register, var: VarId },
    
    // Function Calls
    Call { dest: Option<Register>, func: FuncId, args: Vec<Register> },
    
    // String Operations
    StringConcat { dest: Register, left: Register, right: Register },
    StringInterpolate { dest: Register, parts: Vec<InterpolPart> },
    
    // Field Access (for classes)
    FieldLoad { dest: Register, obj: Register, field: FieldId },
    FieldStore { obj: Register, field: FieldId, value: Register },
    
    // Object Construction
    NewObject { dest: Register, class: ClassId, fields: HashMap<FieldId, Register> },
}

pub struct BasicBlock {
    pub id: BasicBlockId,
    pub instructions: Vec<Instruction>,
    pub label: Label,
}

pub struct Function {
    pub id: FuncId,
    pub name: String,
    pub blocks: Vec<BasicBlock>,
    pub signature: FunctionSignature,
    pub register_count: usize,
}

pub struct Constant {
    pub ty: Type,
    pub value: ConstValue,
}

pub enum ConstValue {
    Integer(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Char(char),
}
```

#### Design Principles
- Register-based IR (like LLVM IR)
- Each instruction has explicit operands
- All variables represented as registers
- Control flow through labels and jumps
- No implicit type conversions
- No undefined behavior

### 4.2 IR Generation from AST (`ir/lowering.rs`)

**Goal**: Convert type-checked AST to IR with proper code generation.

#### Code Generation Strategy

```rust
pub struct IRGenerator {
    functions: Vec<Function>,
    current_block: BasicBlockId,
    symbol_table: SymbolTable,
    register_allocator: RegisterAllocator,
}

impl IRGenerator {
    pub fn generate(program: &Program) -> Result<IRModule, IRError>;
    
    fn gen_function(&mut self, func: &FunctionDef) -> Result<(), IRError>;
    fn gen_statement(&mut self, stmt: &Statement) -> Result<(), IRError>;
    fn gen_expression(&mut self, expr: &Expression) -> Result<Register, IRError>;
}
```

Process:
1. Allocate register for each variable
2. Generate instructions for each statement
3. Handle control flow with labels
4. Inline type information for codegen backends

#### Register Allocation
- Simple allocation strategy: one register per variable
- Function parameters get dedicated registers
- Temporary registers for expression evaluation
- No register reuse across variable lifetimes (simplicity first)

### 4.3 IR Optimization (`ir/optimizer.rs`)

**Goal**: Implement basic IR optimizations for performance.

#### Optimization Passes

**Dead Code Elimination**
- Remove unreachable basic blocks
- Remove unused variable stores
- Requires proof of unreachability

**Constant Folding**
- Evaluate constant expressions at compile time
- Store results as `LoadConst` instructions

**Jump Threading**
- Eliminate jumps to jumps
- Simplify control flow graph

**Common Subexpression Elimination (Optional for Phase 4)**
- Identify identical expressions
- Reuse computed values

#### Implementation Pattern
```rust
pub trait Optimizer {
    fn optimize(ir: &mut IRModule) -> Result<(), OptimizeError>;
}

pub struct DeadCodeEliminator;
pub struct ConstantFolder;
pub struct JumpThreader;
```

### 4.4 Test Coverage

- Unit tests for instruction generation
- Tests for each statement type
- Tests for each expression type
- Integration tests with semantic analyzer
- Register allocation tests
- Optimization tests

---

## Phase 5: Code Generation (C Backend)

### 5.1 C Backend Design (`codegen/c_backend.rs`)

**Goal**: Generate correct, readable C code from IR.

#### Target C Version
- ANSI C89 compatible (maximum portability)
- No C99 features required
- No variable length arrays
- Explicit type declarations

#### Generated Code Structure
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Type definitions
typedef long long int_value;
typedef double float_value;
typedef char char_value;
typedef int bool_value;

// String type: (length, data)
typedef struct {
    int length;
    char* data;
} string_value;

// Forward declarations for functions

// Function implementations

// Runtime library functions
int_value adb_max(int_value a, int_value b) { ... }
// ... etc

// Main entry point
int main(int argc, char* argv[]) {
    // Generated code
    return 0;
}
```

#### Code Generation Strategy

```rust
pub struct CCodeGenerator;

impl CCodeGenerator {
    pub fn generate(ir: &IRModule) -> Result<String, CodegenError>;
    
    fn gen_function(&self, func: &Function) -> String;
    fn gen_instruction(&self, instr: &Instruction) -> String;
    fn gen_type(&self, ty: &Type) -> String;
}
```

#### Generated Code Features
- Each variable gets a C declaration
- Each register gets a local C variable
- Each label becomes a goto target
- String interpolation becomes sprintf calls
- Object construction initializes struct fields
- Field access becomes struct member access
- Function calls become C function calls

#### Runtime Library
- String manipulation functions
- Math library wrapping
- Memory management helpers
- Type conversion utilities

### 5.2 String Handling

- Strings represented as `string_value` struct
- String concatenation via `strcpy` and `strcat`
- String interpolation via `sprintf`
- Safe buffer sizes (no stack overflows)

### 5.3 Test Coverage

- Unit tests for type mapping
- Integration tests for each IR instruction
- Integration tests for complete programs
- Compile-ability tests (generated C must compile)
- Functional correctness tests (generated C produces correct output)

---

## Phase 6: Interpreter Implementation

### 6.1 Runtime Value System (`interpreter/value.rs`)

**Goal**: Create a type-safe runtime value representation.

#### Runtime Values
```rust
pub enum Value {
    Int(i64),
    Float(f64),
    String(String),
    Bool(bool),
    Char(char),
    Object(ObjectRef),
}

pub type ObjectRef = Rc<RefCell<Object>>;

pub struct Object {
    pub class_id: ClassId,
    pub fields: HashMap<FieldId, Value>,
}
```

Requirements:
- Use `Rc<RefCell<Object>>` only for objects (necessary for reference semantics)
- Primitive types stay on stack
- No boxing of primitives
- `Display` and `Debug` implementations for all values
- Proper `Clone` for value copying

### 6.2 Runtime Environment (`interpreter/runtime.rs`)

**Goal**: Manage execution environment and variable storage.

```rust
pub struct Runtime {
    globals: HashMap<VarId, Value>,
    call_stack: Vec<StackFrame>,
    return_value: Option<Value>,
}

pub struct StackFrame {
    locals: HashMap<VarId, Value>,
    return_addr: usize,
}

impl Runtime {
    pub fn push_frame(&mut self);
    pub fn pop_frame(&mut self);
    pub fn store(&mut self, var: VarId, value: Value) -> Result<(), RuntimeError>;
    pub fn load(&self, var: VarId) -> Result<Value, RuntimeError>;
}
```

### 6.3 Executor (`interpreter/executor.rs`)

**Goal**: Execute IR instructions with proper semantics.

```rust
pub struct Interpreter {
    runtime: Runtime,
    ir: IRModule,
    pc: usize, // Program counter
}

impl Interpreter {
    pub fn new(ir: IRModule) -> Self;
    pub fn execute(&mut self) -> Result<Value, RuntimeError>;
    
    fn execute_instruction(&mut self, instr: &Instruction) -> Result<(), RuntimeError>;
}
```

#### Execution Rules
- Evaluate expressions left-to-right
- Strict type checking (no implicit conversions)
- Division by zero produces runtime error
- String operations produce runtime errors on invalid input
- Object access to non-existent fields produces error
- Stack overflow detection

### 6.4 Built-in Functions (`interpreter/builtins.rs`)

Implement standard library functions:
- `io.println(string) -> void`
- `io.print(string) -> void`
- `io.scanln() -> string`
- `io.scan() -> string`
- `math.sqrt(float) -> float`
- `math.pow(float, float) -> float`
- `math.abs(int) -> int`
- `math.max(int, int) -> int`
- `math.min(int, int) -> int`
- `math.sin(float) -> float`
- `math.cos(float) -> float`
- `math.tan(float) -> float`

All built-ins must:
- Check argument types strictly
- Validate preconditions (e.g., sqrt of negative)
- Return appropriate `RuntimeError` on failure
- Have documented behavior

### 6.5 Test Coverage

- Unit tests for each Value type
- Tests for stack frame operations
- Tests for each IR instruction execution
- Tests for all built-in functions
- Integration tests for complete programs
- Edge case tests (division by zero, etc.)
- Performance benchmarks

---

## Phase 7: Standard Library Implementation

### 7.1 Builtin Modules

Each stdlib module is a `.adb` file that gets embedded in the binary.

#### `io.adb`
```adb
export fn println(message: string) { ... };
export fn print(message: string) { ... };
export fn scanln() -> string { ... };
export fn scan() -> string { ... };
```

#### `math.adb`
```adb
export fn sqrt(x: float) -> float { ... };
export fn pow(base: float, exponent: float) -> float { ... };
export fn abs(x: float) -> int { ... };
export fn max(a: int, b: int) -> int { ... };
export fn min(a: int, b: int) -> int { ... };
export fn sin(x: float) -> float { ... };
export fn cos(x: float) -> float { ... };
export fn tan(x: float) -> float { ... };
```

### 7.2 Stdlib Loading (`stdlib/builtin_loader.rs`)

```rust
pub struct StdlibLoader;

impl StdlibLoader {
    pub fn load_io() -> String;
    pub fn load_math() -> String;
}
```

Process:
1. Stdlib functions are written in Adobans
2. Embedded as strings in the binary
3. Parsed and compiled alongside user code
4. Made available through import system

### 7.3 Test Coverage

- Tests for each stdlib function
- Tests for math accuracy
- Tests for I/O operations
- End-to-end tests using stdlib

---

## Phase 8: CLI & Integration

### 8.1 CLI Options (`cli/options.rs`)

```bash
adobans [OPTIONS] <FILE> [ARGS...]

OPTIONS:
    -h, --help              Print help
    -V, --version           Print version
    -c, --compile           Compile to C (output to FILE.c)
    -o, --output <FILE>     Output filename
    -i, --interpret         Interpret directly (default if no -c)
    --check                 Type-check only, don't run
    --emit-ir               Emit IR to FILE.ir
    --emit-ast              Emit AST to FILE.ast
    -O, --optimize <LEVEL>  Optimization level (0-2, default 1)
    --verbose               Verbose output
```

### 8.2 Main Entry Point (`main.rs`)

```rust
fn main() {
    // Parse CLI arguments
    let opts = cli::Options::parse();
    
    // Read source file
    let source = fs::read_to_string(&opts.input)?;
    
    // Compile
    let (ast, warnings) = compile(&source)?;
    
    // Handle output based on flags
    match opts.mode {
        Mode::Interpret => interpret(&ast)?,
        Mode::Compile => codegen(&ast, &opts.output)?,
        Mode::Check => { /* success */ },
    }
}
```

### 8.3 Error Reporting to User

- Beautiful, colored error messages
- Show source code context
- Suggest fixes when possible
- Separate errors from warnings
- Exit with appropriate status codes

---

## Phase 9: Testing & Quality Assurance

### 9.1 Unit Testing Strategy

#### Test Organization
```
tests/unit/
├── lexer/
│   ├── tokens.rs
│   ├── numbers.rs
│   ├── strings.rs
│   └── errors.rs
├── parser/
│   ├── expressions.rs
│   ├── statements.rs
│   ├── functions.rs
│   ├── classes.rs
│   └── errors.rs
├── analyzer/
│   ├── types.rs
│   ├── symbols.rs
│   ├── validation.rs
│   └── errors.rs
├── ir/
│   ├── generation.rs
│   ├── optimization.rs
│   └── correctness.rs
├── codegen/
│   ├── c_generation.rs
│   └── correctness.rs
└── interpreter/
    ├── execution.rs
    ├── builtins.rs
    └── errors.rs
```

#### Test Patterns
- Arrange-Act-Assert pattern
- Property-based tests using `quickcheck`
- Parametrized tests for variants
- Snapshot tests for large outputs (AST, IR)
- Mock objects where appropriate (avoid for this project)

### 9.2 Integration Testing

#### Test Categories

**Syntax Tests**
```
tests/fixtures/valid/
├── hello_world.adb
├── variables.adb
├── arithmetic.adb
├── strings.adb
├── string_interpolation.adb
├── functions.adb
├── function_overloading.adb (if supported)
├── classes.adb
├── namespaces.adb
├── control_flow.adb
├── nested_blocks.adb
└── stdlib_usage.adb
```

**Error Tests**
```
tests/fixtures/invalid/
├── syntax/
│   ├── missing_semicolon.adb
│   ├── mismatched_braces.adb
│   └── invalid_token.adb
├── semantic/
│   ├── undefined_variable.adb
│   ├── type_mismatch.adb
│   ├── duplicate_definition.adb
│   └── invalid_access.adb
└── runtime/
    ├── division_by_zero.adb
    └── invalid_operation.adb
```

### 9.3 End-to-End Testing

Full compilation and execution tests:
- Read `.adb` file
- Compile to AST
- Analyze semantics
- Generate IR
- Generate C code or interpret
- Compare output to expected results

Example test:
```rust
#[test]
fn test_hello_world() {
    let source = r#"
        fn main() {
            io.println("Hello, World!");
        };
        main();
    "#;
    
    let output = execute(source).unwrap();
    assert_eq!(output.trim(), "Hello, World!");
}
```

### 9.4 Test Coverage Goals

- Line coverage: ≥ 95% for all modules
- Branch coverage: ≥ 90% for complex logic
- All error paths tested
- All success paths tested
- Edge cases for each function

### 9.5 Test Execution

```bash
# Run all tests
cargo test

# Run specific test suite
cargo test --test integration tests::lexer

# Run with detailed output
cargo test -- --nocapture

# Run benchmarks
cargo bench

# Check test coverage
tarpaulin --out Html
```

---

## Phase 10: Documentation & Polish

### 10.1 Code Documentation

#### Inline Documentation
- Module-level `//!` docs explaining purpose
- Function-level `///` docs with examples
- Complex algorithm explanation
- Safety considerations for `unsafe` code (minimal)
- Examples in doc comments

#### Example Documentation Format
```rust
/// Parses a complete Adobans program from tokens.
///
/// This is the main entry point for parsing. It handles:
/// - Top-level item parsing (functions, classes, imports)
/// - Error recovery for partial programs
/// - Span tracking for error reporting
///
/// # Errors
/// Returns `ParseError` if syntax is invalid.
/// Multiple errors may be collected.
///
/// # Examples
/// ```
/// let tokens = lexer::scan(source)?;
/// let program = parse(tokens)?;
/// ```
pub fn parse(tokens: Vec<Token>) -> Result<Program, Vec<ParseError>> {
    // ...
}
```

### 10.2 Architecture Documentation

- `docs/architecture.md`: High-level system design
- `docs/type_system.md`: Detailed type semantics
- `docs/ir_spec.md`: IR instruction set specification
- `docs/stdlib.md`: Standard library documentation
- `docs/testing.md`: Testing strategy and coverage

### 10.3 Contributor Guide

- `CONTRIBUTING.md`: How to contribute
- Code style guide (enforced by clippy/rustfmt)
- Adding new features checklist
- Common pitfalls and solutions

### 10.4 Binary Release

- Ensure all tests pass
- Update version in `Cargo.toml`
- Create tagged release
- Build optimized binary: `cargo build --release`
- Generate changelog

---

## Phase 11: Validation & Launch

### 11.1 Pre-Release Checklist

- [ ] All tests pass (100% pass rate)
- [ ] No compiler warnings (clippy clean)
- [ ] Code formatted (rustfmt)
- [ ] Documentation complete
- [ ] Benchmarks show acceptable performance
- [ ] SAFETY.md documents any unsafe code (should be zero)
- [ ] Dependencies audited (cargo-audit)
- [ ] No TODO/FIXME comments (except documented limitations)

### 11.2 Testing Matrix

Test with all combinations:
- Both lexer and parser
- Semantic analysis enabled
- IR generation
- C code generation and compilation
- Interpretation

All combinations must work correctly.

### 11.3 Performance Benchmarks

Measure and document:
- Lexer throughput (tokens/ms)
- Parse time for large programs
- Analysis time
- IR generation time
- Interpretation overhead
- Generated C code performance

Targets:
- Lexing: >100KB/ms
- Parsing: >50KB/ms
- Full compilation: >10KB/ms

### 11.4 Known Limitations Document

Document:
- No generic types
- No operator overloading
- No inheritance
- No interfaces/traits
- No error handling (try/catch)
- No null safety guarantees
- Platform-specific behaviors

---

## Implementation Constraints & Best Practices

### Rust Idioms Required

1. **Error Handling**
   - Use `Result<T, E>` for fallible operations
   - Never use `.unwrap()` in library code
   - Use `?` operator exclusively for error propagation
   - Custom error types with `thiserror` crate
   - No panics except in tests

2. **Ownership & Borrowing**
   - No unnecessary cloning
   - Use `&str` for string references
   - Use references for large data structures
   - No circular references (use Rc/Arc sparingly)

3. **Type Safety**
   - Use strong types (no `String` for multiple purposes)
   - Use `newtype` pattern for distinct types (e.g., `VarId`, `FuncId`)
   - No `as` casts unless absolutely necessary
   - Exhaustive pattern matching

4. **RefCell & Interior Mutability**
   - Use ONLY for objects needing shared mutability
   - Never use `RefCell<T>` for local state
   - Use `&mut self` for mutable access in normal cases
   - Document why interior mutability is needed

5. **Lifetimes**
   - Only annotate lifetimes when necessary (Rust inference handles most cases)
   - Use lifetimes for borrowed data relationships
   - Use named lifetimes in public APIs only when meaningful
   - Prefer owned data over complex lifetime annotations

6. **Module Organization**
   - One public item per module (usually)
   - Private implementation details in submodules
   - Clear public API boundaries
   - No wildcard imports in code

7. **Testing**
   - Unit tests in same file (`#[cfg(test)]` module)
   - Integration tests in `tests/` directory
   - Test naming: `test_<feature>_<scenario>`
   - Descriptive assertions with context

### Code Quality Standards

1. **Complexity Limits**
   - Functions ≤ 100 lines (soft limit)
   - Cyclomatic complexity ≤ 10
   - No deeply nested structures

2. **Documentation**
   - Public items must have doc comments
   - Examples in doc comments for complex functions
   - Safety documentation for unsafe code

3. **Performance**
   - No premature optimizations
   - Benchmark before optimizing
   - Document performance characteristics
   - Avoid unnecessary allocations

4. **Maintainability**
   - Clear variable names (no abbreviations)
   - Single responsibility per function
   - Consistent naming conventions
   - No magic numbers (use named constants)

### Forbidden Patterns

- ❌ `unsafe` code (unless absolutely justified with documentation)
- ❌ `unwrap()` or `expect()` outside tests
- ❌ `RefCell` for non-objects
- ❌ String-based error handling
- ❌ Global mutable state
- ❌ Implicit type conversions
- ❌ Panic in library code
- ❌ TODO/FIXME without issue numbers
- ❌ Commented-out code
- ❌ Multiple return types for same operation

### Required Patterns

- ✅ `Result<T, E>` for errors
- ✅ `Option<T>` for optionals
- ✅ Explicit type annotations in public APIs
- ✅ Custom error types
- ✅ Builder pattern for complex construction
- ✅ Comprehensive tests
- ✅ Documentation with examples
- ✅ Performance benchmarks

---

## Development Workflow

### Repository Structure
```
.
├── src/                  # Source code
├── tests/                # Integration tests
├── benches/              # Performance benchmarks
├── docs/                 # Documentation
├── examples/             # Example programs
├── Cargo.toml            # Package manifest
├── Cargo.lock            # Dependency lock file
├── .gitignore            # Git ignore rules
├── .github/
│   └── workflows/
│       ├── test.yml      # CI tests
│       └── clippy.yml    # Clippy checks
├── README.md             # Project overview
├── CONTRIBUTING.md       # Contribution guide
└── PLAN.md              # This file
```

### Git Workflow

**Commit Message Format**
```
<type>: <subject>

<body>

<footer>
```

Types: feat, fix, refactor, test, docs, chore, perf, style

Example:
```
feat: implement string interpolation in lexer

- Add StringInterpolStart and StringInterpolEnd tokens
- Properly handle nested braces in interpolation
- Track span for error reporting

Fixes: #42
```

### Code Review Checklist

- [ ] Code follows Rust conventions
- [ ] Tests are comprehensive
- [ ] No compiler warnings
- [ ] Documentation is clear
- [ ] Error messages are helpful
- [ ] Performance is acceptable
- [ ] No breaking changes to public API

---

## Success Criteria

### Correctness
- ✅ All language features implemented per specification
- ✅ All test cases pass
- ✅ No undefined behavior
- ✅ Error messages are helpful and accurate
- ✅ Generated C code compiles with gcc/clang
- ✅ Interpreter produces same results as C backend

### Quality
- ✅ Code compiles without warnings (clippy + rustc)
- ✅ Code is formatted with rustfmt
- ✅ Test coverage ≥ 95%
- ✅ Documentation complete
- ✅ No security vulnerabilities
- ✅ No memory leaks (Valgrind clean)

### Performance
- ✅ Compilation completes in reasonable time
- ✅ Generated code runs efficiently
- ✅ Interpreter has acceptable performance
- ✅ Memory usage within bounds

### Maintainability
- ✅ Code is idiomatic Rust
- ✅ Architecture is clear and extensible
- ✅ Tests serve as documentation
- ✅ Easy to debug errors

---

## Timeline Estimates

| Phase | Duration | Dependencies |
|-------|----------|--------------|
| 1. Foundation | 2-3 weeks | None |
| 2. Parsing | 3-4 weeks | Phase 1 |
| 3. Semantic Analysis | 3-4 weeks | Phase 2 |
| 4. IR Generation | 2-3 weeks | Phase 3 |
| 5. C Code Generation | 2-3 weeks | Phase 4 |
| 6. Interpreter | 3-4 weeks | Phase 4 |
| 7. Standard Library | 1-2 weeks | Phase 6 |
| 8. CLI & Integration | 1 week | Phase 7 |
| 9. Testing & QA | 2-3 weeks | Phases 1-8 |
| 10. Documentation | 1-2 weeks | Phases 1-8 |
| 11. Validation | 1 week | Phases 1-10 |

**Total: 22-34 weeks** (5-8 months) for a single developer

---

## Risk Mitigation

### Technical Risks

1. **String Interpolation Complexity**
   - Risk: Handling nested expressions in string interpolation
   - Mitigation: Design clear tokenization strategy; extensive testing

2. **Type System Edge Cases**
   - Risk: Missing type compatibility rules
   - Mitigation: Formal specification; comprehensive test matrix

3. **C Code Generation Correctness**
   - Risk: Generated C code has subtle bugs
   - Mitigation: Compare interpreter output with generated code; Valgrind testing

4. **Performance Regression**
   - Risk: Inefficient compilation or slow generated code
   - Mitigation: Early performance benchmarking; optimize critical paths

### Mitigation Strategies

- Regular testing against reference implementations
- Peer review of all code
- Automated testing in CI/CD
- Performance monitoring throughout development
- Documentation of design decisions

---

## Maintenance & Future Work

### Future Enhancements (Post-Launch)

1. **Language Features**
   - Generic types
   - Enums
   - Pattern matching
   - Traits/interfaces
   - Error handling (try/catch)
   - Modules system

2. **Performance**
   - JIT compilation
   - LLVM IR backend
   - Advanced optimizations
   - Parallelization

3. **Tooling**
   - Language server protocol (LSP)
   - Debugger
   - Profiler
   - Package manager

4. **Community**
   - Contribution guidelines
   - Community standards
   - Roadmap transparency
   - Feature requests process

### Maintenance Tasks

- Dependency updates (quarterly)
- Security audits (quarterly)
- Performance monitoring
- Bug fixes
- Documentation updates

---

## Conclusion

This plan provides a comprehensive roadmap for implementing a production-ready Adobans compiler and interpreter in Rust. The phased approach ensures correctness at each step, with extensive testing and validation throughout.

The emphasis on Rust best practices, comprehensive testing, and clear documentation will result in a maintainable, reliable implementation that can serve as a foundation for future enhancements and community contributions.

Key success factors:
- Strict adherence to phase dependencies
- Zero tolerance for hacks or shortcuts
- Comprehensive testing at each phase
- Regular architectural reviews
- Clear communication through documentation
