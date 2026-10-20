#ifndef MINI_LANG_PARSER_H
#define MINI_LANG_PARSER_H

#include "token.h"
#include "ast.h"
#include "diag.h"
#include <vector>
#include <string>

struct Parser {
    Parser(std::vector<Token> toks, Diagnostics *diag = nullptr);
    std::vector<StmtPtr> parse_program();

private:
    std::vector<Token> toks_;
    size_t idx_;
    Diagnostics *diag_;

    const Token& peek() const;
    const Token& get();
    bool accept(TokenKind k);
    void expect(TokenKind k, const std::string &msg="");
    bool is_type_name(const Token &t) const;
    Type parse_type(); // NEW: parse a type (handles arrays, tuples, maps)

    StmtPtr parse_statement();
    StmtPtr parse_vardecl();
    StmtPtr parse_print();
    StmtPtr parse_if();
    StmtPtr parse_while();
    StmtPtr parse_for();
    StmtPtr parse_block();
    StmtPtr parse_func_decl();
    StmtPtr parse_return();

    ExprPtr parse_expression();
    ExprPtr parse_cast();
    ExprPtr parse_equality();
    ExprPtr parse_relational();
    ExprPtr parse_add();
    ExprPtr parse_mul();
    ExprPtr parse_unary();
    ExprPtr parse_primary();

    void recover_to_sync();
};

#endif
