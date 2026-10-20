#ifndef MINI_LANG_INTERPRETER_H
#define MINI_LANG_INTERPRETER_H

#include "ast.h"
#include "diag.h"
#include <variant>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

struct RuntimeFunction {
    std::vector<std::string> param_names;
    std::vector<Type> param_types;
    Type ret_type;
    StmtPtr body;
    Loc loc;
    RuntimeFunction(std::vector<std::string> pn, std::vector<Type> pt, Type rt, StmtPtr b, Loc l)
      : param_names(std::move(pn)), param_types(std::move(pt)), ret_type(std::move(rt)), body(std::move(b)), loc(l) {}
};
using RFuncPtr = std::shared_ptr<RuntimeFunction>;

// forward-declare compound runtime containers; full MapVal defined in .cpp
struct ArrayVal;
struct TupleVal;
struct MapVal;

using RuntimeValue = std::variant<
    long long,
    double,
    char,
    bool,
    std::string,
    RFuncPtr,
    std::shared_ptr<ArrayVal>,
    std::shared_ptr<TupleVal>,
    std::shared_ptr<MapVal>,
    std::shared_ptr<void>
>;

struct ArrayVal {
    std::vector<RuntimeValue> elems;
    bool is_static = false;
    size_t static_size = 0;
};

struct TupleVal {
    std::vector<RuntimeValue> elems;
    std::vector<std::string> names; // empty if positional
};

// Note: MapVal is intentionally not defined here; it is implementation-private in src/interpreter.cpp

struct Interpreter {
    struct Runtime {
        std::vector<std::unordered_map<std::string, std::pair<Type, RuntimeValue>>> scopes;
        void push_scope() { scopes.emplace_back(); }
        void pop_scope() { scopes.pop_back(); }
    } runtime_;

    Diagnostics *diag;

    explicit Interpreter(Diagnostics *d = nullptr);
    RuntimeValue eval_expr(Expr &e);
    void exec_stmt(Stmt &s);
    void run_program(const std::vector<StmtPtr> &program);

    void declare_runtime(const std::string &name, Type t, bool initialized, const Loc &loc);
    std::pair<Type, RuntimeValue>* runtime_lookup(const std::string &name);
    RuntimeValue cast_runtime(const RuntimeValue &v, const Type &from, const Type &to, const Loc &loc);
};

#endif // MINI_LANG_INTERPRETER_H
