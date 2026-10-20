import argparse
from dataclasses import dataclass
from enum import StrEnum, auto
from pathlib import Path


class TokenType(StrEnum):
    IDENT = auto()
    CODE = auto()
    NUMBER = auto()
    CHAR = auto()
    STRING = auto()

    # Delimiters
    OPEN_PAREN = auto()
    CLOSE_PAREN = auto()
    OPEN_BRACE = auto()
    CLOSE_BRACE = auto()
    EQUAL = auto()

    # Keywords
    SET = auto()
    DEF = auto()
    CALL = auto()


DELIMITERS = {
    '(': TokenType.OPEN_PAREN,
    ')': TokenType.CLOSE_PAREN,
    '{': TokenType.OPEN_BRACE,
    '}': TokenType.CLOSE_BRACE,
    '=': TokenType.EQUAL,
}

KEYWORDS = {
    str(TokenType.SET): TokenType.SET,
    str(TokenType.DEF): TokenType.DEF,
    str(TokenType.CALL): TokenType.CALL,
}


class Severity(StrEnum):
    ERROR = auto()
    WARN = auto()
    INFO = auto()
    HELP = auto()


@dataclass
class Span:
    begin: int
    end: int


@dataclass
class SourceCode:
    name: str
    content: str
    lines: list[str]


@dataclass
class Report:
    severity: Severity
    message: str
    span: Span


def load_source_code(filename: str):
    with open(filename) as file:
        content = file.read()
        lines = content.split('\n')
        return SourceCode(filename, content, lines)


@dataclass
class Token:
    token_type: TokenType
    value: str
    span: Span


class Lexer:
    def __init__(self, source_code: SourceCode):
        self.source_code_ = source_code
        self.index_ = 0

    def peek_(self, offset=0):
        return self.source_code_.content[self.index_ + offset] if self.index_ + offset in range(len(self.source_code_.content)) else ''

    def advance_(self, offset=1):
        self.index_ += offset
        if self.index_ + offset not in range(len(self.source_code_.content)):
            self.index_ = len(self.source_code_.content)

    def skip_whitespace_(self):
        if self.peek_().isspace():
            while self.peek_().isspace():
                self.advance_()
            return True
        return False

    def skip_line_comment_(self):
        if self.peek_() == '/' and self.peek_(1) == '/':


@dataclass
class File:
    functions: list[Function]


@dataclass
class Function:
    name: str
    params: list[str]
    body: Body


@dataclass
class Body:
    items: list[Node | Call | str]


@dataclass
class Node:
    name: str
    typename: str
    args: list[NodeArgs]
    children: Body


@dataclass
class NodeArgs:
    name: str
    value: str
    setter: bool


@dataclass
class Call:
    name: str
    args: list[str]


class Parser:
    def __init__(self, tokens: list[Token]):
        pass


def gen_bppui_file(input_file: Path, output_dir: Path):
    pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate C++ header and source file from BPP UI files')
    parser.add_argument("-i", "--input", required=True, help="Input BPP UI file")
    parser.add_argument("-o", "--output", required=True, help="Output directory")

    args = parser.parse_args()

    input_file = Path(args.input)
    output_dir = Path(args.output)

    print("Processing BPP UI file")
    print(f"Input file: {input_file}")
    print(f"Output directory: {output_dir}")

    gen_bppui_file(input_file, output_dir)
