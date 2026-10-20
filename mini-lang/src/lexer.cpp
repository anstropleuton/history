#include "lexer.h"
#include "diag.h"
#include <cctype>
#include <stdexcept>

using namespace std;

Lexer::Lexer(string src, Diagnostics *diag) : src_(move(src)), i_(0), loc_{1,1}, diag_(diag) {}

char Lexer::peek() const { return (i_ < src_.size()) ? src_[i_] : '\0'; }
char Lexer::get() {
    char c = peek();
    if (c == '\n') { loc_.line++; loc_.col = 1; ++i_; }
    else { ++i_; ++loc_.col; }
    return c;
}
bool Lexer::starts_with(const string &s) const {
    return src_.compare(i_, s.size(), s) == 0;
}
void Lexer::skip_ws_and_comments() {
    while (true) {
        char c = peek();
        if (c == '\0') return;
        if (isspace((unsigned char)c)) { get(); continue; }
        if (c == '#') {
            while (peek() && peek() != '\n') get();
            continue;
        }
        break;
    }
}

Token Lexer::next_token() {
    skip_ws_and_comments();
    Loc start = loc_;
    char c = peek();
    if (c == '\0') return Token{TokenKind::End, "", start};

    // multi-char operators (check longest first)
    if (starts_with("==")) { get(); get(); return {TokenKind::EqualEqual, "==", start}; }
    if (starts_with("!=")) { get(); get(); return {TokenKind::NotEqual, "!=", start}; }
    if (starts_with("<=")) { get(); get(); return {TokenKind::LessEqual, "<=", start}; }
    if (starts_with(">=")) { get(); get(); return {TokenKind::GreaterEqual, ">=", start}; }
    if (starts_with("->")) { get(); get(); return {TokenKind::Arrow, "->", start}; }
    if (starts_with("=>")) { get(); get(); return {TokenKind::MapArrow, "=>", start}; }

    if (isalpha((unsigned char)c) || c == '_') {
        string id;
        while (isalnum((unsigned char)peek()) || peek() == '_') id.push_back(get());
        if (id == "let") return {TokenKind::KeywordLet, id, start};
        if (id == "print") return {TokenKind::KeywordPrint, id, start};
        if (id == "as") return {TokenKind::KeywordAs, id, start};
        if (id == "if") return {TokenKind::KeywordIf, id, start};
        if (id == "while") return {TokenKind::KeywordWhile, id, start};
        if (id == "for") return {TokenKind::KeywordFor, id, start};
        if (id == "true") return {TokenKind::KeywordTrue, id, start};
        if (id == "false") return {TokenKind::KeywordFalse, id, start};
        if (id == "scan") return {TokenKind::KeywordScan, id, start};
        if (id == "fn") return {TokenKind::KeywordFn, id, start};
        if (id == "return") return {TokenKind::KeywordReturn, id, start};
        return {TokenKind::Identifier, id, start};
    }

    if (isdigit((unsigned char)c)) {
        string num;
        bool seenDot = false;
        while (isdigit((unsigned char)peek()) || (!seenDot && peek() == '.')) {
            if (peek() == '.') { seenDot = true; num.push_back(get()); }
            else num.push_back(get());
        }
        if (seenDot) return {TokenKind::FloatLiteral, num, start};
        return {TokenKind::IntLiteral, num, start};
    }

    if (c == '\'') {
        get(); // '
        char val = 0;
        if (peek() == '\\') {
            get(); char esc = get();
            switch (esc) {
                case 'n': val = '\n'; break;
                case 't': val = '\t'; break;
                case '\\': val = '\\'; break;
                case '\'': val = '\''; break;
                default: val = esc;
            }
        } else {
            if (peek() == '\0') { emit_report({Diagnostic::Severity::Error, start, "unterminated char literal", "LEX001"}, diag_); }
            val = get();
        }
        if (peek() != '\'') emit_report({Diagnostic::Severity::Error, start, "unterminated char literal", "LEX001"}, diag_);
        if (peek() == '\'') get();
        string s; s.push_back(val);
        return {TokenKind::CharLiteral, s, start};
    }

    if (c == '"') {
        get();
        string out;
        while (peek() && peek() != '"') {
            if (peek() == '\\') {
                get(); char esc = get();
                switch (esc) {
                    case 'n': out.push_back('\n'); break;
                    case 't': out.push_back('\t'); break;
                    case '\\': out.push_back('\\'); break;
                    case '"': out.push_back('"'); break;
                    default: out.push_back(esc);
                }
            } else out.push_back(get());
        }
        if (peek() != '"') emit_report({Diagnostic::Severity::Error, start, "unterminated string literal", "LEX002"}, diag_);
        if (peek() == '"') get();
        return {TokenKind::StringLiteral, out, start};
    }

    // single char tokens
    char ch = get();
    switch (ch) {
        case ':': return {TokenKind::Colon, ":", start};
        case ';': return {TokenKind::Semicolon, ";", start};
        case '=': return {TokenKind::Equal, "=", start};
        case '{': return {TokenKind::LBrace, "{", start};
        case '}': return {TokenKind::RBrace, "}", start};
        case '(' : return {TokenKind::LParen, "(", start};
        case ')' : return {TokenKind::RParen, ")", start};
        case '[' : return {TokenKind::LBracket, "[", start};
        case ']' : return {TokenKind::RBracket, "]", start};
        case '.' : return {TokenKind::Dot, ".", start};
        case '+' : return {TokenKind::Plus, "+", start};
        case '-' : return {TokenKind::Minus, "-", start};
        case '*' : return {TokenKind::Star, "*", start};
        case '/' : return {TokenKind::Slash, "/", start};
        case '<' : return {TokenKind::Less, "<", start};
        case '>' : return {TokenKind::Greater, ">", start};
        case ',' : return {TokenKind::Comma, ",", start};
        default:
            emit_report({Diagnostic::Severity::Error, start, std::string("unexpected character '") + ch + "'", "LEX003"}, diag_);
            return Token{TokenKind::End, "", start};
    }
}

vector<Token> Lexer::tokenize() {
    vector<Token> out;
    while (true) {
        Token tk = next_token();
        out.push_back(tk);
        if (tk.kind == TokenKind::End) break;
    }
    return out;
}
