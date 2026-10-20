#ifndef MINI_LANG_LEXER_H
#define MINI_LANG_LEXER_H
#include "token.h"
#include "diag.h"

#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(std::string src, Diagnostics *diag = nullptr);
    // tokenize whole input; returns tokens (including End token)
    std::vector<Token> tokenize();

private:
    std::string src_;
    size_t i_;
    Loc loc_;
    Diagnostics *diag_;
    char peek() const;
    char get();
    void skip_ws_and_comments();
    bool starts_with(const std::string &s) const;
    Token next_token();
};

#endif
