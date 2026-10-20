#ifndef MINI_LANG_AST_H
#define MINI_LANG_AST_H

#include "common.h"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <sstream>

enum class TypeKind { INT, FLOAT, CHAR, BOOL, STRING, VOID, FUNC, STATIC_ARRAY, DYN_ARRAY, TUPLE, MAP };

struct Type {
    TypeKind kind;
    std::vector<Type> subs; // used for func params/ret, tuple element types, map key/value, array element
    std::optional<size_t> array_size; // only used for STATIC_ARRAY
    std::vector<std::string> tuple_field_names; // for TUPLE: empty strings for unnamed
    Type(): kind(TypeKind::VOID){}
    explicit Type(TypeKind k): kind(k){}
    bool operator==(const Type &o) const {
        if (kind != o.kind) return false;
        if (array_size != o.array_size) return false;
        if (tuple_field_names != o.tuple_field_names) return false;
        if (subs.size() != o.subs.size()) return false;
        for (size_t i=0;i<subs.size();++i) if (!(subs[i] == o.subs[i])) return false;
        return true;
    }
};

inline Type make_func_type(const std::vector<Type> &params, const Type &ret) {
    Type t(TypeKind::FUNC);
    t.subs = params;
    t.subs.push_back(ret);
    return t;
}
inline Type make_static_array(Type elem, size_t n) {
    Type t(TypeKind::STATIC_ARRAY);
    t.subs = {elem};
    t.array_size = n;
    return t;
}
inline Type make_dyn_array(Type elem) {
    Type t(TypeKind::DYN_ARRAY);
    t.subs = {elem};
    return t;
}
// renamed helper to avoid conflict with std::make_tuple
inline Type make_tuple_type(const std::vector<Type>& elems, const std::vector<std::string>& names = {}) {
    Type t(TypeKind::TUPLE);
    t.subs = elems;
    t.tuple_field_names = names;
    return t;
}
inline Type make_map(Type key, Type val) {
    Type t(TypeKind::MAP);
    t.subs = {key, val};
    return t;
}

inline std::string type_to_string(TypeKind k) {
    switch (k) {
        case TypeKind::INT: return "int";
        case TypeKind::FLOAT: return "float";
        case TypeKind::CHAR: return "char";
        case TypeKind::BOOL: return "bool";
        case TypeKind::STRING: return "string";
        case TypeKind::VOID: return "void";
        case TypeKind::FUNC: return "func";
        case TypeKind::STATIC_ARRAY: return "array";
        case TypeKind::DYN_ARRAY: return "array";
        case TypeKind::TUPLE: return "tuple";
        case TypeKind::MAP: return "map";
    }
    return "unknown";
}
inline std::string type_to_string(const Type &t) {
    switch (t.kind) {
        case TypeKind::STATIC_ARRAY:
            return type_to_string(t.subs[0].kind) + "[" + (t.array_size ? std::to_string(*t.array_size) : "?") + "]";
        case TypeKind::DYN_ARRAY:
            return type_to_string(t.subs[0].kind) + "[]";
        case TypeKind::TUPLE: {
            std::string s = "(";
            for (size_t i=0;i<t.subs.size();++i) {
                if (i) s += ", ";
                if (!t.tuple_field_names.empty() && !t.tuple_field_names[i].empty()) s += t.tuple_field_names[i] + ": ";
                s += type_to_string(t.subs[i]);
            }
            s += ")";
            return s;
        }
        case TypeKind::MAP:
            return type_to_string(t.subs[0]) + " => " + type_to_string(t.subs[1]);
        case TypeKind::FUNC:
            return "fn(...)";
        default:
            return type_to_string(t.kind);
    }
}

// Param struct (used by FuncDeclStmt)
struct Param {
    std::string name;
    Type type;
    Param() = default;
    Param(std::string n, Type t) : name(std::move(n)), type(std::move(t)) {}
};

// Forward declarations and aliases
struct Expr;
struct Stmt;
struct ExprVisitor;
struct StmtVisitor;

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

// Expr base
struct Expr {
    Loc loc;
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor &v) = 0;
};

// Basic expressions (literals, var, binary, unary, cast, scan)
struct IntLiteral : Expr { long long value; IntLiteral(long long v, Loc l): value(v){ loc = l;} void accept(ExprVisitor &v) override; };
struct FloatLiteral : Expr { double value; FloatLiteral(double v, Loc l): value(v){ loc = l;} void accept(ExprVisitor &v) override; };
struct CharLiteral : Expr { char value; CharLiteral(char v, Loc l): value(v){ loc = l;} void accept(ExprVisitor &v) override; };
struct StringLiteral : Expr { std::string value; StringLiteral(std::string v, Loc l): value(std::move(v)){ loc = l;} void accept(ExprVisitor &v) override; };
struct BoolLiteral : Expr { bool value; BoolLiteral(bool v, Loc l): value(v){ loc = l;} void accept(ExprVisitor &v) override; };
struct VarExpr : Expr { std::string name; VarExpr(std::string n, Loc l): name(std::move(n)){ loc = l;} void accept(ExprVisitor &v) override; };
enum class BinOp { Add, Sub, Mul, Div, Lt, Gt, Le, Ge, Eq, Ne };
struct BinaryExpr : Expr { BinOp op; ExprPtr left, right; BinaryExpr(BinOp o, ExprPtr l, ExprPtr r, Loc loc_): op(o), left(std::move(l)), right(std::move(r)) { loc = loc_; } void accept(ExprVisitor &v) override; };
enum class UnOp { Neg };
struct UnaryExpr : Expr { UnOp op; ExprPtr expr; UnaryExpr(UnOp o, ExprPtr e, Loc loc_): op(o), expr(std::move(e)){ loc = loc_; } void accept(ExprVisitor &v) override; };
struct CastExpr : Expr { ExprPtr expr; Type target; CastExpr(ExprPtr e, Type t, Loc loc_): expr(std::move(e)), target(std::move(t)){ loc = loc_; } void accept(ExprVisitor &v) override; };
struct ScanExpr : Expr { ScanExpr(Loc l){ loc = l; } void accept(ExprVisitor &v) override; };

// New expr nodes for collections and access
struct ArrayLiteralExpr : Expr {
    std::vector<ExprPtr> elems;
    ArrayLiteralExpr(std::vector<ExprPtr> e, Loc l): elems(std::move(e)) { loc = l; }
    void accept(ExprVisitor &v) override;
};
struct TupleLiteralExpr : Expr {
    std::vector<ExprPtr> elems;
    TupleLiteralExpr(std::vector<ExprPtr> e, Loc l): elems(std::move(e)) { loc = l; }
    void accept(ExprVisitor &v) override;
};
struct MapLiteralExpr : Expr {
    std::vector<std::pair<ExprPtr, ExprPtr>> entries;
    MapLiteralExpr(std::vector<std::pair<ExprPtr, ExprPtr>> e, Loc l): entries(std::move(e)) { loc = l; }
    void accept(ExprVisitor &v) override;
};
struct IndexExpr : Expr {
    ExprPtr target;
    ExprPtr index;
    IndexExpr(ExprPtr t, ExprPtr i, Loc l): target(std::move(t)), index(std::move(i)) { loc = l; }
    void accept(ExprVisitor &v) override;
};
struct MemberExpr : Expr {
    ExprPtr target;
    std::string name;
    MemberExpr(ExprPtr t, std::string n, Loc l): target(std::move(t)), name(std::move(n)) { loc = l; }
    void accept(ExprVisitor &v) override;
};
struct CallExpr : Expr {
    ExprPtr callee;
    std::vector<ExprPtr> args;
    CallExpr(ExprPtr c, std::vector<ExprPtr> a, Loc l): callee(std::move(c)), args(std::move(a)) { loc = l; }
    void accept(ExprVisitor &v) override;
};

// Stmt base
struct Stmt {
    Loc loc;
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor &v) = 0;
};

struct VarDeclStmt : Stmt {
    std::string name;
    std::optional<Type> declared_type;
    ExprPtr initializer;
    VarDeclStmt(std::string n, std::optional<Type> dt, ExprPtr init, Loc l): name(std::move(n)), declared_type(std::move(dt)), initializer(std::move(init)){ loc = l; }
    void accept(StmtVisitor &v) override;
};
struct AssignStmt : Stmt { std::string name; ExprPtr expr; AssignStmt(std::string n, ExprPtr e, Loc l): name(std::move(n)), expr(std::move(e)){ loc = l; } void accept(StmtVisitor &v) override; };
struct PrintStmt : Stmt { ExprPtr expr; PrintStmt(ExprPtr e, Loc l): expr(std::move(e)){ loc = l; } void accept(StmtVisitor &v) override; };
struct BlockStmt : Stmt { std::vector<StmtPtr> stmts; BlockStmt(std::vector<StmtPtr> s, Loc l): stmts(std::move(s)){ loc = l; } void accept(StmtVisitor &v) override; };
struct IfStmt : Stmt { ExprPtr cond; StmtPtr thenBlock; IfStmt(ExprPtr c, StmtPtr t, Loc l): cond(std::move(c)), thenBlock(std::move(t)){ loc = l; } void accept(StmtVisitor &v) override; };
struct WhileStmt : Stmt { ExprPtr cond; StmtPtr body; WhileStmt(ExprPtr c, StmtPtr b, Loc l): cond(std::move(c)), body(std::move(b)){ loc = l; } void accept(StmtVisitor &v) override; };
struct ForStmt : Stmt { StmtPtr init; ExprPtr condition; StmtPtr update; StmtPtr body; ForStmt(StmtPtr i, ExprPtr c, StmtPtr u, StmtPtr b, Loc l): init(std::move(i)), condition(std::move(c)), update(std::move(u)), body(std::move(b)){ loc = l; } void accept(StmtVisitor &v) override; };
struct ReturnStmt : Stmt { ExprPtr value; ReturnStmt(ExprPtr v, Loc l): value(std::move(v)){ loc = l; } void accept(StmtVisitor &v) override; };
struct FuncDeclStmt : Stmt { std::string name; std::vector<Param> params; std::optional<Type> ret_type; StmtPtr body; FuncDeclStmt(std::string n, std::vector<Param> p, std::optional<Type> r, StmtPtr b, Loc l): name(std::move(n)), params(std::move(p)), ret_type(std::move(r)), body(std::move(b)){ loc = l; } void accept(StmtVisitor &v) override; };
struct ExprStmt : Stmt {
    ExprPtr expr;
    ExprStmt(ExprPtr e, Loc l) : expr(std::move(e)) { loc = l; }
    void accept(StmtVisitor &v) override;
};

// Visitors
struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual void visit(IntLiteral&) = 0;
    virtual void visit(FloatLiteral&) = 0;
    virtual void visit(CharLiteral&) = 0;
    virtual void visit(StringLiteral&) = 0;
    virtual void visit(BoolLiteral&) = 0;
    virtual void visit(VarExpr&) = 0;
    virtual void visit(BinaryExpr&) = 0;
    virtual void visit(UnaryExpr&) = 0;
    virtual void visit(CastExpr&) = 0;
    virtual void visit(ScanExpr&) = 0;
    virtual void visit(CallExpr&) = 0;
    virtual void visit(ArrayLiteralExpr&) = 0;
    virtual void visit(TupleLiteralExpr&) = 0;
    virtual void visit(MapLiteralExpr&) = 0;
    virtual void visit(IndexExpr&) = 0;
    virtual void visit(MemberExpr&) = 0;
};

struct StmtVisitor {
    virtual ~StmtVisitor() = default;
    virtual void visit(VarDeclStmt&) = 0;
    virtual void visit(AssignStmt&) = 0;
    virtual void visit(PrintStmt&) = 0;
    virtual void visit(BlockStmt&) = 0;
    virtual void visit(IfStmt&) = 0;
    virtual void visit(WhileStmt&) = 0;
    virtual void visit(ForStmt&) = 0;
    virtual void visit(ReturnStmt&) = 0;
    virtual void visit(FuncDeclStmt&) = 0;
    virtual void visit(ExprStmt&) = 0;
};

// accept implementations
inline void IntLiteral::accept(ExprVisitor &v) { v.visit(*this); }
inline void FloatLiteral::accept(ExprVisitor &v) { v.visit(*this); }
inline void CharLiteral::accept(ExprVisitor &v) { v.visit(*this); }
inline void StringLiteral::accept(ExprVisitor &v) { v.visit(*this); }
inline void BoolLiteral::accept(ExprVisitor &v) { v.visit(*this); }
inline void VarExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void BinaryExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void UnaryExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void CastExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void ScanExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void CallExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void ArrayLiteralExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void TupleLiteralExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void MapLiteralExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void IndexExpr::accept(ExprVisitor &v) { v.visit(*this); }
inline void MemberExpr::accept(ExprVisitor &v) { v.visit(*this); }

inline void VarDeclStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void AssignStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void PrintStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void BlockStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void IfStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void WhileStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void ForStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void ReturnStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void FuncDeclStmt::accept(StmtVisitor &v) { v.visit(*this); }
inline void ExprStmt::accept(StmtVisitor &v) { v.visit(*this); }

#endif // MINI_LANG_AST_H
