#ifndef MINI_LANG_TYPECHECKER_H
#define MINI_LANG_TYPECHECKER_H

#include "ast.h"
#include "diag.h"
#include <vector>
#include <unordered_map>
#include <string>

struct TypeChecker {
    struct Symbol { Type type; bool initialized; };
    std::vector<std::unordered_map<std::string, Symbol>> scopes_;
    std::unordered_map<const Expr*, Type> type_map_;
    Diagnostics *diag;

    explicit TypeChecker(Diagnostics *d = nullptr);
    void push_scope();
    void pop_scope();
    void declare(const std::string &name, Type type, bool initialized, const Loc &loc);
    Symbol* lookup(const std::string &name);

    static bool is_numeric(TypeKind k);
    static bool can_assign_implicit(const Type &target, const Type &value);
    static bool cast_allowed(const Type &from, const Type &to);

    bool is_hashable_type(const Type &t) const;

    void type_check_program(const std::vector<StmtPtr> &program);
    Type type_of_expr(Expr &e);
    void type_check_stmt(Stmt &s);
};

#endif
