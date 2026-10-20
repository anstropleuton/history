#ifndef MINI_LANG_TOKEN_H
#define MINI_LANG_TOKEN_H

#include "common.h"
#include <string>

enum class TokenKind {
    End,
    Identifier,
    IntLiteral,
    FloatLiteral,
    CharLiteral,
    StringLiteral,

    // two-char operators
    EqualEqual,
    NotEqual,
    LessEqual,
    GreaterEqual,

    // keywords
    KeywordLet,
    KeywordPrint,
    KeywordAs,
    KeywordIf,
    KeywordWhile,
    KeywordFor,
    KeywordTrue,
    KeywordFalse,
    KeywordScan,
    KeywordFn,
    KeywordReturn,

    // punctuation
    LBrace, RBrace,
    LParen, RParen,
    LBracket, RBracket,
    Colon, Semicolon,
    Comma,
    Dot,

    // operators
    Equal, // =
    Plus, Minus, Star, Slash,
    Less, Greater,

    Arrow,    // '->' (function arrow)
    MapArrow, // '=>' (map arrow)
};

struct Token {
    TokenKind kind;
    std::string text;
    Loc loc;
};

#endif // MINI_LANG_TOKEN_H
