#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, Optional


# -----------------------------
# Tokenizer
# -----------------------------

@dataclass(frozen=True)
class Token:
    kind: str
    value: str
    line: int
    col: int


@dataclass(frozen=True)
class Span:
    start_line: int
    start_col: int
    end_line: int
    end_col: int


class LexError(Exception):
    pass


class ParseError(Exception):
    pass


class CodeGenError(Exception):
    pass


_KEYWORDS = {
    "use",
    "true",
    "false",
    "int",
    "float",
    "char",
    "bool",
    "string",
    "model",
    "node",
    "vec2",
    "vec3",
    "vec4",
    "auto",
    "set",
}

_CPP_KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel",
    "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char8_t", "char16_t",
    "char32_t", "class", "compl", "concept", "const", "consteval",
    "constexpr", "constinit", "const_cast", "continue", "co_await",
    "co_return", "co_yield", "decltype", "default", "delete", "do",
    "double", "dynamic_cast", "else", "enum", "explicit", "export",
    "extern", "false", "float", "for", "friend", "goto", "if", "inline",
    "int", "long", "mutable", "namespace", "new", "noexcept", "not",
    "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
    "public", "reflexpr", "register", "reinterpret_cast", "requires",
    "return", "short", "signed", "sizeof", "static", "static_assert",
    "static_cast", "struct", "switch", "template", "this", "thread_local",
    "throw", "true", "try", "typedef", "typeid", "typename", "union",
    "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while",
    "xor", "xor_eq",
}

_BUILTIN_TYPE_MAP = {
    "int": "std::int32_t",
    "float": "float",
    "char": "char",
    "bool": "bool",
    "string": "std::string",
    "model": "bppui::model",
    "node": "bppui::node",
    "vec2": "Vector2",
    "vec3": "Vector3",
    "vec4": "Vector4",
}


def sanitize_cpp_identifier(name: str) -> str:
    cleaned = re.sub(r"[^0-9A-Za-z_]", "_", name)
    if not cleaned:
        cleaned = "value"
    if cleaned[0].isdigit():
        cleaned = f"_{cleaned}"
    if cleaned in _CPP_KEYWORDS:
        cleaned += "_"
    return cleaned


def split_component_path(path: Path) -> tuple[list[str], str]:
    parts = list(path.with_suffix("").parts)
    if not parts:
        raise ValueError(f"invalid path: {path}")
    stem = parts[-1]
    namespace_parts = [sanitize_cpp_identifier(p) for p in parts[:-1] if p not in (".", "")]
    return namespace_parts, sanitize_cpp_identifier(stem)


class Lexer:
    def __init__(self, text: str):
        self.text = text
        self.i = 0
        self.line = 1
        self.col = 1
        self.n = len(text)

    def _peek(self, n: int = 0) -> str:
        j = self.i + n
        if j >= self.n:
            return ""
        return self.text[j]

    def _advance(self, n: int = 1) -> str:
        out = []
        for _ in range(n):
            if self.i >= self.n:
                return "".join(out)
            ch = self.text[self.i]
            out.append(ch)
            self.i += 1
            if ch == "\n":
                self.line += 1
                self.col = 1
            else:
                self.col += 1
        return "".join(out)

    def _skip_whitespace(self) -> None:
        while True:
            ch = self._peek()
            if ch and ch.isspace():
                self._advance()
                continue
            if ch == "/" and self._peek(1) == "/":
                self._advance(2)
                while self._peek() not in ("", "\n"):
                    self._advance()
                continue
            if ch == "/" and self._peek(1) == "*":
                self._advance(2)
                while True:
                    if self._peek() == "":
                        raise LexError("unterminated block comment")
                    if self._peek() == "*" and self._peek(1) == "/":
                        self._advance(2)
                        break
                    self._advance()
                continue
            break

    def _next_token(self) -> Token:
        self._skip_whitespace()
        line, col = self.line, self.col
        ch = self._peek()
        if not ch:
            return Token("EOF", "", line, col)

        if ch == "`":
            self._advance()
            buf = []
            while True:
                if self._peek() == "":
                    raise LexError(f"unterminated backtick code starting at {line}:{col}")
                if self._peek() == "`":
                    self._advance()
                    break
                buf.append(self._advance())
            return Token("BACKTICK", "".join(buf), line, col)

        if ch.isalpha() or ch == "_":
            buf = [self._advance()]
            while True:
                c = self._peek()
                if c.isalnum() or c == "_":
                    buf.append(self._advance())
                else:
                    break
            value = "".join(buf)
            kind = value if value in _KEYWORDS else "IDENT"
            return Token(kind, value, line, col)

        if ch.isdigit():
            buf = [self._advance()]
            has_dot = False
            has_exp = False
            while True:
                c = self._peek()
                if c.isdigit():
                    buf.append(self._advance())
                elif c == "." and not has_dot and self._peek(1) != ".":
                    has_dot = True
                    buf.append(self._advance())
                elif c in "eE" and not has_exp:
                    has_exp = True
                    buf.append(self._advance())
                    if self._peek() in "+-":
                        buf.append(self._advance())
                else:
                    break
            return Token("NUMBER", "".join(buf), line, col)

        if ch == '"' or ch == "'":
            quote = self._advance()
            buf = [quote]
            while True:
                c = self._peek()
                if c == "":
                    raise LexError("unterminated string literal")
                if c == quote:
                    buf.append(self._advance())
                    break
                if c == "\\":
                    buf.append(self._advance())
                    if self._peek() == "":
                        raise LexError("unterminated string escape")
                    buf.append(self._advance())
                    continue
                buf.append(self._advance())
            return Token("STRING" if quote == '"' else "CHAR", "".join(buf), line, col)

        two = ch + self._peek(1)
        three = two + self._peek(2)
        if three == "...":
            self._advance(3)
            return Token("ELLIPSIS", three, line, col)
        if two in {"==", "!=", "<=", ">=", "&&", "||", "::"}:
            self._advance(2)
            return Token(two, two, line, col)

        if ch in "(){}[];:,.=+-*/!?<>/":
            self._advance()
            return Token(ch, ch, line, col)

        raise LexError(f"unexpected character {ch!r} at {line}:{col}")

    def tokenize(self) -> list[Token]:
        tokens = []
        while True:
            t = self._next_token()
            tokens.append(t)
            if t.kind == "EOF":
                return tokens


# -----------------------------
# AST
# -----------------------------

@dataclass
class Module:
    span: Span
    imports: list["Import"] = field(default_factory=list)
    decls: list["Decl"] = field(default_factory=list)
    statements: list["Stmt"] = field(default_factory=list)


@dataclass
class Import:
    span: Span
    path: list[str]


@dataclass
class TypeSpec:
    span: Span
    name: str
    packed: bool = False


@dataclass
class Decl:
    span: Span
    name: str
    type_spec: TypeSpec


class Stmt:
    span: Span


@dataclass
class CodeStmt(Stmt):
    span: Span
    code: str


@dataclass
class NodeStmt(Stmt):
    span: Span
    name: str
    args: list["Arg"] = field(default_factory=list)
    children: list[Stmt] = field(default_factory=list)
    expanded: bool = False


@dataclass
class Arg:
    span: Span
    name: str
    value: "Expr"
    setter: bool = False


class Expr:
    span: Span


@dataclass
class Ident(Expr):
    span: Span
    name: str


@dataclass
class Number(Expr):
    span: Span
    text: str


@dataclass
class String(Expr):
    span: Span
    text: str


@dataclass
class Char(Expr):
    span: Span
    text: str


@dataclass
class RawCode(Expr):
    span: Span
    code: str


@dataclass
class Unary(Expr):
    span: Span
    op: str
    expr: Expr


@dataclass
class Binary(Expr):
    span: Span
    op: str
    left: Expr
    right: Expr


@dataclass
class Conditional(Expr):
    span: Span
    cond: Expr
    yes: Expr
    no: Expr


@dataclass
class CallExpr(Expr):
    span: Span
    func: Expr
    args: list[Expr]


@dataclass
class ListExpr(Expr):
    span: Span
    items: list[Expr]


@dataclass
class ObjectExpr(Expr):
    span: Span
    fields: list[tuple[str, Expr]]


# -----------------------------
# Parser
# -----------------------------

class Parser:
    def __init__(self, tokens: list[Token]):
        self.tokens = tokens
        self.i = 0

    def _current(self) -> Token:
        return self.tokens[self.i]

    def _previous(self) -> Token:
        if self.i <= 0:
            return self.tokens[0]
        return self.tokens[self.i - 1]

    def _peek(self, n: int = 0) -> Token:
        j = self.i + n
        if j >= len(self.tokens):
            return self.tokens[-1]
        return self.tokens[j]

    def _match(self, *kinds: str) -> bool:
        return self._peek().kind in kinds

    def _accept(self, *kinds: str) -> Optional[Token]:
        if self._match(*kinds):
            t = self._peek()
            self.i += 1
            return t
        return None

    def _expect(self, kind: str, msg: str = "") -> Token:
        t = self._accept(kind)
        if t is None:
            cur = self._peek()
            detail = msg or f"expected {kind}, got {cur.kind} at {cur.line}:{cur.col}"
            raise ParseError(detail)
        return t

    @staticmethod
    def _is_name_token(tok: Token) -> bool:
        return tok.kind == "IDENT" or (
                tok.kind in _KEYWORDS and tok.kind not in {"use", "true", "false"}
        )

    def _accept_name(self) -> Optional[Token]:
        tok = self._peek()
        if self._is_name_token(tok):
            self.i += 1
            return tok
        return None

    def _expect_name(self, msg: str = "expected name") -> Token:
        t = self._accept_name()
        if t is None:
            cur = self._peek()
            raise ParseError(f"{msg} at {cur.line}:{cur.col}")
        return t

    @staticmethod
    def _span(start: Token, end: Token) -> Span:
        return Span(start.line, start.col, end.line, end.col)

    @staticmethod
    def _span_from_exprs(start: Expr, end: Expr) -> Span:
        return Span(start.span.start_line, start.span.start_col, end.span.end_line, end.span.end_col)

    @staticmethod
    def _is_type_name_token(tok: Token) -> bool:
        return tok.kind == "IDENT" or (
                tok.kind in _KEYWORDS and tok.kind not in {"use", "true", "false"}
        )

    def _is_decl_start(self) -> bool:
        return self._is_name_token(self._peek()) and self._peek(1).kind == ":"

    def parse(self) -> Module:
        start = self._peek()
        module = Module(span=Span(start.line, start.col, start.line, start.col))
        while not self._match("EOF"):
            if self._match("use"):
                module.imports.append(self._parse_import())
                continue
            if self._is_decl_start():
                module.decls.append(self._parse_decl())
                continue
            module.statements.append(self._parse_stmt())
        end = self._previous() if self.i > 0 else self._peek()
        module.span = self._span(start, end)
        return module

    def _parse_import(self) -> Import:
        start = self._expect("use")
        parts = []
        last = start
        while True:
            t = self._expect("IDENT", "expected import path segment")
            parts.append(t.value)
            last = t
            if self._accept("/"):
                last = self._previous()
                continue
            if self._accept("::"):
                last = self._previous()
                continue
            break
        semi = self._expect(";")
        return Import(span=self._span(start, semi), path=parts)

    def _parse_decl(self) -> Decl:
        start = self._peek()
        name = self._expect_name("expected declaration name")
        self._expect(":")
        t = self._parse_type_spec()
        semi = self._expect(";")
        return Decl(span=self._span(start, semi), name=name.value, type_spec=t)

    def _parse_type_spec(self) -> TypeSpec:
        start = self._peek()
        if self._is_type_name_token(start):
            name = self._peek().value
            self.i += 1
        else:
            raise ParseError(f"expected type name at {self._peek().line}:{self._peek().col}")
        packed = bool(self._accept("ELLIPSIS"))
        end = self._previous()
        return TypeSpec(span=self._span(start, end), name=name, packed=packed)

    def _parse_stmt(self) -> Stmt:
        if self._match("BACKTICK"):
            t = self._expect("BACKTICK")
            self._accept(";")
            return CodeStmt(span=self._span(t, self._previous()), code=t.value)
        if self._is_name_token(self._peek()):
            return self._parse_node_stmt()
        cur = self._peek()
        raise ParseError(f"unexpected token {cur.kind} at {cur.line}:{cur.col}")

    def _parse_arg(self) -> Arg:
        start = self._peek()
        setter = False

        if self._match("set"):
            self._accept("set")
            setter = True

        arg_name = self._expect_name("expected argument name")
        self._expect("=")
        arg_value = self._parse_expr()
        return Arg(span=self._span(start, self._previous()), name=arg_name.value, value=arg_value, setter=setter)

    def _parse_node_stmt(self) -> NodeStmt:
        start = self._peek()
        name = self._expect_name("expected node name")
        args: list[Arg] = []
        children: list[Stmt] = []
        expanded = False

        if self._accept("("):
            if not self._match(")"):
                while True:
                    args.append(self._parse_arg())
                    if self._accept(","):
                        if self._match(")"):
                            break
                        continue
                    break
            self._expect(")")

        if self._accept("{"):
            while not self._match("}"):
                children.append(self._parse_stmt())
            self._expect("}")

        if self._accept("ELLIPSIS"):
            expanded = True

        semi = self._accept(";")
        end = semi or self._previous()
        return NodeStmt(span=self._span(start, end), name=name.value, args=args, children=children, expanded=expanded)

    # ---- expressions ----

    def _parse_expr(self) -> Expr:
        return self._parse_conditional()

    def _parse_conditional(self) -> Expr:
        expr = self._parse_logical_or()
        if self._accept("?"):
            yes = self._parse_expr()
            self._expect(":")
            no = self._parse_expr()
            return Conditional(span=self._span_from_exprs(expr, no), cond=expr, yes=yes, no=no)
        return expr

    def _parse_logical_or(self) -> Expr:
        expr = self._parse_logical_and()
        while self._match("||"):
            op = self._expect("||")
            rhs = self._parse_logical_and()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_logical_and(self) -> Expr:
        expr = self._parse_equality()
        while self._match("&&"):
            op = self._expect("&&")
            rhs = self._parse_equality()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_equality(self) -> Expr:
        expr = self._parse_relational()
        while self._match("==", "!="):
            op = self._peek()
            self.i += 1
            rhs = self._parse_relational()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_relational(self) -> Expr:
        expr = self._parse_additive()
        while self._match("<", ">", "<=", ">="):
            op = self._peek()
            self.i += 1
            rhs = self._parse_additive()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_additive(self) -> Expr:
        expr = self._parse_multiplicative()
        while self._match("+", "-"):
            op = self._peek()
            self.i += 1
            rhs = self._parse_multiplicative()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_multiplicative(self) -> Expr:
        expr = self._parse_unary()
        while self._match("*", "/"):
            op = self._peek()
            self.i += 1
            rhs = self._parse_unary()
            expr = Binary(span=self._span_from_exprs(expr, rhs), op=op.kind, left=expr, right=rhs)
        return expr

    def _parse_unary(self) -> Expr:
        if self._match("!", "-"):
            op = self._peek()
            self.i += 1
            expr = self._parse_unary()
            return Unary(span=self._span(op, self._previous()), op=op.kind, expr=expr)
        return self._parse_postfix()

    def _parse_postfix(self) -> Expr:
        expr = self._parse_primary()
        while self._accept("("):
            args: list[Expr] = []
            if not self._match(")"):
                while True:
                    args.append(self._parse_expr())
                    if self._accept(","):
                        if self._match(")"):
                            break
                        continue
                    break
            close = self._expect(")")
            expr = CallExpr(
                span=Span(expr.span.start_line, expr.span.start_col, close.line, close.col),
                func=expr,
                args=args,
            )
        return expr

    def _parse_primary(self) -> Expr:
        name = self._accept_name()
        if name is not None:
            return Ident(span=self._span(name, name), name=name.value)
        if self._match("true", "false"):
            t = self._expect(self._peek().kind)
            return Ident(span=self._span(t, t), name=t.value)
        if self._match("NUMBER"):
            t = self._expect("NUMBER")
            return Number(span=self._span(t, t), text=t.value)
        if self._match("STRING"):
            t = self._expect("STRING")
            return String(span=self._span(t, t), text=t.value)
        if self._match("CHAR"):
            t = self._expect("CHAR")
            return Char(span=self._span(t, t), text=t.value)
        if self._match("BACKTICK"):
            t = self._expect("BACKTICK")
            return RawCode(span=self._span(t, t), code=t.value)
        if self._accept("("):
            expr = self._parse_expr()
            self._expect(")")
            return expr
        if self._accept("{"):
            items: list[Expr] = []
            fields: list[tuple[str, Expr]] = []
            start = self._previous()
            if not self._match("}"):
                if self._match("IDENT") and self._peek(1).kind == "=":
                    while True:
                        key = self._expect_name("expected field name").value
                        self._expect("=")
                        value = self._parse_expr()
                        fields.append((key, value))
                        if self._accept(","):
                            if self._match("}"):
                                break
                            continue
                        break
                else:
                    while True:
                        items.append(self._parse_expr())
                        if self._accept(","):
                            if self._match("}"):
                                break
                            continue
                        break
            end = self._expect("}")
            if fields:
                return ObjectExpr(span=self._span(start, end), fields=fields)
            return ListExpr(span=self._span(start, end), items=items)
        cur = self._peek()
        raise ParseError(f"unexpected token {cur.kind} at {cur.line}:{cur.col}")


# -----------------------------
# Code generation
# -----------------------------

@dataclass
class GenerationContext:
    component_name: str
    namespace: list[str]
    imports: dict[str, str] = field(default_factory=dict)
    decl_cpp_names: dict[str, str] = field(default_factory=dict)
    temp_counter: int = 0
    root_type: str = "bppui::node"

    def fresh_name(self, base: str) -> str:
        base = sanitize_cpp_identifier(base)
        self.temp_counter += 1
        return f"{base}_{self.temp_counter}"

    def resolve_name(self, name: str) -> str:
        if name not in self.decl_cpp_names:
            raise CodeGenError(f"unresolved identifier {name!r}")
        return self.decl_cpp_names[name]

    def resolve_component(self, name: str) -> str:
        if name in self.imports:
            return self.imports[name]
        if name == self.component_name:
            return name
        return f"bppui::{sanitize_cpp_identifier(name)}"


def cpp_type_name(type_spec: TypeSpec) -> str:
    base = _BUILTIN_TYPE_MAP.get(type_spec.name, type_spec.name)
    if type_spec.packed:
        return f"std::vector<{base}>"
    return base


def expr_to_cpp(expr: Expr, ctx: GenerationContext) -> str:
    if isinstance(expr, Ident):
        if expr.name in {"true", "false"}:
            return expr.name
        return ctx.resolve_name(expr.name)
    if isinstance(expr, Number):
        return expr.text
    if isinstance(expr, String):
        return expr.text
    if isinstance(expr, Char):
        return expr.text
    if isinstance(expr, RawCode):
        return expr.code
    if isinstance(expr, Unary):
        return f"({expr.op}{expr_to_cpp(expr.expr, ctx)})"
    if isinstance(expr, Binary):
        return f"({expr_to_cpp(expr.left, ctx)} {expr.op} {expr_to_cpp(expr.right, ctx)})"
    if isinstance(expr, Conditional):
        return f"({expr_to_cpp(expr.cond, ctx)} ? {expr_to_cpp(expr.yes, ctx)} : {expr_to_cpp(expr.no, ctx)})"
    if isinstance(expr, CallExpr):
        func = expr_to_cpp(expr.func, ctx)
        args = ", ".join(expr_to_cpp(a, ctx) for a in expr.args)
        return f"{func}({args})"
    if isinstance(expr, ListExpr):
        return "{ " + ", ".join(expr_to_cpp(a, ctx) for a in expr.items) + " }"
    if isinstance(expr, ObjectExpr):
        parts = [f".{sanitize_cpp_identifier(k)} = {expr_to_cpp(v, ctx)}" for k, v in expr.fields]
        return "{ " + ", ".join(parts) + " }"
    raise TypeError(f"unsupported expr: {type(expr)!r}")


def node_ctor_expr(stmt: NodeStmt, ctx: GenerationContext) -> str:
    ctor_name = ctx.resolve_component(stmt.name)
    return f"bppui::make<{ctor_name}>()"


def render_indent(level: int) -> str:
    return "    " * level


def emit_node_arg_assignments(
        stmt: NodeStmt,
        var: str,
        ctx: GenerationContext,
        indent: int,
        value_renderer: Optional[Callable[[Expr], str]] = None,
) -> list[str]:
    lines: list[str] = []
    pad = render_indent(indent)
    render = value_renderer or (lambda expr: expr_to_cpp(expr, ctx))
    for arg in stmt.args:
        cpp_name = sanitize_cpp_identifier(arg.name)
        cpp_value = render(arg.value)
        if arg.setter:
            lines.append(f"{pad}{var}->set_{cpp_name}({cpp_value});")
        else:
            lines.append(f"{pad}{var}->{cpp_name} = {cpp_value};")
    return lines


def emit_code_stmt(stmt: CodeStmt, indent: int) -> list[str]:
    lines: list[str] = []
    pad = render_indent(indent)
    code = stmt.code.strip("\n")
    for line in code.splitlines():
        if line.strip():
            lines.append(f"{pad}{line}")
        else:
            lines.append("")
    return lines


def emit_stmt(stmt: Stmt, ctx: GenerationContext, indent: int, parent_var: Optional[str]) -> list[str]:
    if isinstance(stmt, CodeStmt):
        return emit_code_stmt(stmt, indent)
    if isinstance(stmt, NodeStmt):
        lines, _ = emit_node_tree(stmt, ctx, indent, parent_var)
        return lines
    raise TypeError(f"unknown statement: {type(stmt)!r}")


def emit_expanded_node(stmt: NodeStmt, ctx: GenerationContext, indent: int, parent_var: str) -> list[str]:
    lines: list[str] = []
    pad = render_indent(indent)
    resolved_name = ctx.resolve_component(stmt.name)

    if not stmt.args:
        raise ParseError(f"expanded node '{stmt.name}' needs at least one argument")

    arg_exprs = [arg.value for arg in stmt.args]

    size_vars = []
    for i, expr in enumerate(arg_exprs):
        sv = ctx.fresh_name(f"size_{i}")
        size_vars.append(sv)
        lines.append(f"{pad}const auto {sv} = {expr_to_cpp(expr, ctx)}.size();")

    max_size_expr = size_vars[0]
    for sv in size_vars[1:]:
        lines.append(f"{pad}if ({sv} != {max_size_expr}) throw std::runtime_error(\"packed expansion size mismatch\");")

    index_var = ctx.fresh_name("i")
    lines.append(f"{pad}for (std::size_t {index_var} = 0; {index_var} < {max_size_expr}; ++{index_var}) {{")
    inner_pad = render_indent(indent + 1)
    child_var = ctx.fresh_name(stmt.name)
    lines.append(f"{inner_pad}auto {child_var} = bppui::make<{resolved_name}>();")

    lines.extend(
        emit_node_arg_assignments(
            stmt,
            child_var,
            ctx,
            indent + 1,
            value_renderer=lambda expr: f"{expr_to_cpp(expr, ctx)}[{index_var}]",
            )
    )

    for child in stmt.children:
        lines.extend(emit_stmt(child, ctx, indent + 1, child_var))

    lines.append(f"{inner_pad}{parent_var}->append(std::move({child_var}));")
    lines.append(f"{pad}}}")
    return lines


def emit_node_tree(stmt: NodeStmt, ctx: GenerationContext, indent: int, parent_var: Optional[str]) -> tuple[list[str], str]:
    lines: list[str] = []
    pad = render_indent(indent)

    if stmt.expanded:
        if parent_var is None:
            synthetic = ctx.fresh_name("expanded")
            lines.append(f"{pad}auto {synthetic} = bppui::make<{ctx.root_type}>();")
            lines.extend(emit_expanded_node(stmt, ctx, indent, synthetic))
            return lines, synthetic
        lines.extend(emit_expanded_node(stmt, ctx, indent, parent_var))
        return lines, parent_var

    var = ctx.fresh_name(stmt.name)
    lines.append(f"{pad}auto {var} = {node_ctor_expr(stmt, ctx)};")
    lines.extend(emit_node_arg_assignments(stmt, var, ctx, indent))

    for child in stmt.children:
        lines.extend(emit_stmt(child, ctx, indent + 1, var))

    if parent_var is not None:
        lines.append(f"{pad}{parent_var}->append(std::move({var}));")
    return lines, var


def collect_imports(module: Module, ctx: GenerationContext) -> None:
    for imp in module.imports:
        if not imp.path:
            continue
        leaf = sanitize_cpp_identifier(imp.path[-1])
        qualified = "::".join(sanitize_cpp_identifier(p) for p in imp.path)
        existing = ctx.imports.get(leaf)
        if existing is not None and existing != qualified:
            raise ParseError(f"duplicate import alias '{leaf}' resolves to both '{existing}' and '{qualified}'")
        ctx.imports[leaf] = qualified


def declaration_param(decl: Decl, ctx: GenerationContext) -> tuple[str, Optional[str]]:
    cpp_name = sanitize_cpp_identifier(decl.name)
    if cpp_name in ctx.decl_cpp_names.values():
        raise ParseError(f"duplicate declaration name after sanitization: {decl.name!r} -> {cpp_name!r}")
    ctx.decl_cpp_names[decl.name] = cpp_name

    if decl.type_spec.name == "auto":
        template_name = sanitize_cpp_identifier(f"{decl.name}_t")
        if decl.type_spec.packed:
            return f"const std::vector<{template_name}>& {cpp_name}", template_name
        return f"const {template_name}& {cpp_name}", template_name

    cpp_type = cpp_type_name(decl.type_spec)
    if decl.type_spec.packed:
        return f"const {cpp_type}& {cpp_name}", None
    if decl.type_spec.name in {"int", "float", "char", "bool"}:
        return f"{cpp_type} {cpp_name}", None
    return f"const {cpp_type}& {cpp_name}", None


def component_signature(module: Module, ctx: GenerationContext) -> tuple[list[str], list[str], list[str]]:
    template_names: list[str] = []
    params: list[str] = []
    template_decl_lines: list[str] = []

    for decl in module.decls:
        param, template_name = declaration_param(decl, ctx)
        params.append(param)
        if template_name is not None:
            template_names.append(template_name)

    if template_names:
        template_decl_lines.append("template <" + ", ".join(f"typename {name}" for name in template_names) + ">")

    return template_names, params, template_decl_lines


def emit_component_body(module: Module, ctx: GenerationContext) -> list[str]:
    stmts = module.statements
    node_stmts = [s for s in stmts if isinstance(s, NodeStmt)]
    code_stmts = [s for s in stmts if isinstance(s, CodeStmt)]

    lines: list[str] = []

    if len(node_stmts) == 1 and not code_stmts and len(stmts) == 1:
        root = node_stmts[0]
        root_lines, root_var = emit_node_tree(root, ctx, 1, None)
        lines.extend(root_lines)
        lines.append(f"    return {root_var};")
        return lines

    root_var = ctx.fresh_name("root")
    lines.append(f"    auto {root_var} = bppui::make<{ctx.root_type}>();")
    for stmt in stmts:
        lines.extend(emit_stmt(stmt, ctx, 1, root_var))
    lines.append(f"    return {root_var};")
    return lines


def generate_cpp(module: Module, source_path: Path, root_dir: Path) -> tuple[str, str]:
    try:
        rel_source = source_path.relative_to(root_dir)
    except ValueError as exc:
        raise ValueError(f"input path '{source_path}' must be under root dir '{root_dir}'") from exc

    ns_parts, component_name = split_component_path(rel_source)
    ctx = GenerationContext(component_name=component_name, namespace=ns_parts)
    collect_imports(module, ctx)

    template_params, params, template_decl_lines = component_signature(module, ctx)

    header_lines: list[str] = [
        "#pragma once",
        "#include <cstddef>",
        "#include <cstdint>",
        "#include <memory>",
        "#include <stdexcept>",
        "#include <string>",
        "#include <utility>",
        "#include <vector>",
        '#include "ui/ui.hpp"',
        "",
    ]

    for imp in module.imports:
        include_target = "/".join(imp.path) + ".hpp"
        header_lines.append(f'#include "{include_target}"')
    if module.imports:
        header_lines.append("")

    for ns in ns_parts:
        header_lines.append(f"namespace {ns} {{")
    if ns_parts:
        header_lines.append("")

    signature = f"auto {component_name}({', '.join(params)}) -> std::unique_ptr<bppui::node>"

    body_lines = emit_component_body(module, ctx)

    if template_decl_lines:
        header_lines.extend(template_decl_lines)
        header_lines.append(signature + " {")
        header_lines.extend(body_lines)
        header_lines.append("}")
        header_lines.append("")
    else:
        header_lines.append(signature + ";")
        header_lines.append("")

    for ns in reversed(ns_parts):
        header_lines.append(f"}} // namespace {ns}")

    source_lines: list[str] = []
    header_include = rel_source.with_suffix(".hpp").as_posix()
    source_lines.append(f'#include "{header_include}"')
    source_lines.append('#include "ui/ui.hpp"')
    source_lines.append("")

    if not template_decl_lines:
        for ns in ns_parts:
            source_lines.append(f"namespace {ns} {{")
        source_lines.append("")
        source_lines.append(signature + " {")
        ctx2 = GenerationContext(component_name=component_name, namespace=ns_parts)
        collect_imports(module, ctx2)
        for decl in module.decls:
            ctx2.decl_cpp_names[decl.name] = sanitize_cpp_identifier(decl.name)
        source_lines.extend(emit_component_body(module, ctx2))
        source_lines.append("}")
        source_lines.append("")
        for ns in reversed(ns_parts):
            source_lines.append(f"}} // namespace {ns}")
    else:
        source_lines.append("// Template component definitions are emitted inline in the header.")
        source_lines.append("")

    return "\n".join(header_lines).rstrip() + "\n", "\n".join(source_lines).rstrip() + "\n"


def compile_bppui_file(input_path: Path, root_dir: Path, output_dir: Optional[Path] = None) -> tuple[Path, Path]:
    source_path = Path(input_path).resolve()
    root_dir = Path(root_dir).resolve()
    text = source_path.read_text(encoding="utf-8")
    tokens = Lexer(text).tokenize()
    module = Parser(tokens).parse()

    if output_dir is None:
        output_dir = source_path.parent.resolve()
    else:
        output_dir = Path(output_dir).resolve()

    try:
        out_rel = source_path.relative_to(root_dir).with_suffix("")
    except ValueError as exc:
        raise ValueError(f"input path '{source_path}' must be under root dir '{root_dir}'") from exc

    header_path = output_dir / out_rel.with_suffix(".hpp")
    source_out_path = output_dir / out_rel.with_suffix(".cpp")

    header_path.parent.mkdir(parents=True, exist_ok=True)
    source_out_path.parent.mkdir(parents=True, exist_ok=True)

    header_text, source_text = generate_cpp(module, source_path, root_dir)
    header_path.write_text(header_text, encoding="utf-8")
    source_out_path.write_text(source_text, encoding="utf-8")
    return header_path, source_out_path


def _run_self_test() -> None:
    # Regression: declaration parsing must work.
    decl_mod = Parser(Lexer("value: int;\n").tokenize()).parse()
    assert len(decl_mod.decls) == 1
    assert decl_mod.decls[0].name == "value"
    assert decl_mod.decls[0].type_spec.name == "int"

    # Regression: expansion should index arguments rather than reuse the original expression.
    with tempfile.TemporaryDirectory() as td:
        root = Path(td) / "root"
        root.mkdir()
        src = root / "demo.bppui"
        src.write_text("values: int...;\nmore: int...;\nItem(items = values, extra = more) ...;\n", encoding="utf-8")
        module = Parser(Lexer(src.read_text(encoding="utf-8")).tokenize()).parse()
        hpp, cpp = generate_cpp(module, src, root)
        assert "[i_" in cpp or "[i]" in cpp
        assert "packed expansion size mismatch" in cpp or "packed expansion size mismatch" in hpp

    # Regression: unresolved identifiers should now fail instead of being silently sanitized.
    with tempfile.TemporaryDirectory() as td:
        root = Path(td) / "root"
        root.mkdir()
        src = root / "demo.bppui"
        src.write_text("Button(text = title);\n", encoding="utf-8")
        module = Parser(Lexer(src.read_text(encoding="utf-8")).tokenize()).parse()
        try:
            generate_cpp(module, src, root)
        except CodeGenError:
            pass
        else:
            raise AssertionError("expected CodeGenError for unresolved identifier")


def main() -> int:
    ap = argparse.ArgumentParser(description="Compile .bppui files to C++ header/source pairs.")
    ap.add_argument("input", type=Path, nargs="?", help="Input .bppui file")
    ap.add_argument("-o", "--output-dir", type=Path, default=None, help="Output directory")
    ap.add_argument("-r", "--root-dir", type=Path, required=False, help="File base root directory")
    ap.add_argument("--self-test", action="store_true", help="Run a small regression test suite and exit")
    args = ap.parse_args()

    if args.self_test:
        _run_self_test()
        print("self-test ok")
        return 0

    if args.input is None:
        ap.error("the following arguments are required: input")
    if args.root_dir is None:
        ap.error("the following arguments are required: -r/--root-dir")

    hpp, cpp = compile_bppui_file(args.input, args.root_dir, args.output_dir)
    print(hpp)
    print(cpp)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
