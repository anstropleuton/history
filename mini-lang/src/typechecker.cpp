#include "typechecker.h"
#include "diag.h"
#include "ast.h"
#include <cassert>
#include <algorithm>

using namespace std;

TypeChecker::TypeChecker(Diagnostics *d) : diag(d) { push_scope(); }

void TypeChecker::push_scope() { scopes_.emplace_back(); }
void TypeChecker::pop_scope() { scopes_.pop_back(); }

void TypeChecker::declare(const string &name, Type type, bool initialized, const Loc &loc) {
    auto &cur = scopes_.back();
    if (cur.count(name)) {
        Diagnostic diagrec{Diagnostic::Severity::Error, loc, "redeclaration of variable '" + name + "'", "TC001"};
        if (diag) diag->report(diagrec); else throw DiagnosticError(diagrec);
        return;
    }
    cur[name] = Symbol{std::move(type), initialized};
}

TypeChecker::Symbol* TypeChecker::lookup(const string &name) {
    for (int i = (int)scopes_.size() - 1; i >= 0; --i) {
        auto &m = scopes_[i];
        auto it = m.find(name);
        if (it != m.end()) return &it->second;
    }
    return nullptr;
}

bool TypeChecker::is_numeric(TypeKind k) { return k == TypeKind::INT || k == TypeKind::FLOAT; }

bool TypeChecker::can_assign_implicit(const Type &target, const Type &value) {
    if (target == value) return true;
    // int -> float allowed implicitly
    if (value.kind == TypeKind::INT && target.kind == TypeKind::FLOAT) return true;

    // Arrays: element-wise compatibility; static size must match for STATIC_ARRAY->STATIC_ARRAY
    if ((target.kind == TypeKind::DYN_ARRAY || target.kind == TypeKind::STATIC_ARRAY) &&
        (value.kind == TypeKind::DYN_ARRAY || value.kind == TypeKind::STATIC_ARRAY)) {
        if (!can_assign_implicit(target.subs[0], value.subs[0])) return false;
        if (target.kind == TypeKind::STATIC_ARRAY && value.kind == TypeKind::STATIC_ARRAY) {
            return target.array_size == value.array_size;
        }
        return true;
    }

    // Tuples: same arity and element-wise compatibility
    if (target.kind == TypeKind::TUPLE && value.kind == TypeKind::TUPLE) {
        if (target.subs.size() != value.subs.size()) return false;
        for (size_t i = 0; i < target.subs.size(); ++i)
            if (!can_assign_implicit(target.subs[i], value.subs[i])) return false;
        return true;
    }

    // Maps: key/value compatibility
    if (target.kind == TypeKind::MAP && value.kind == TypeKind::MAP) {
        return can_assign_implicit(target.subs[0], value.subs[0]) && can_assign_implicit(target.subs[1], value.subs[1]);
    }

    return false;
}

bool TypeChecker::cast_allowed(const Type &from, const Type &to) {
    if (from == to) return true;
    if (from.kind == TypeKind::INT && to.kind == TypeKind::FLOAT) return true;
    return true;
}

bool TypeChecker::is_hashable_type(const Type &t) const {
    switch (t.kind) {
        case TypeKind::INT:
        case TypeKind::CHAR:
        case TypeKind::BOOL:
        case TypeKind::STRING:
            return true;
        case TypeKind::TUPLE:
            for (auto &sub : t.subs) if (!is_hashable_type(sub)) return false;
            return true;
        default:
            return false;
    }
}

void TypeChecker::type_check_program(const vector<StmtPtr> &program) {
    push_scope();
    for (auto &s : program) type_check_stmt(*s);
    pop_scope();
}

Type TypeChecker::type_of_expr(Expr &e) {
    struct V : public ExprVisitor {
        TypeChecker &tc;
        Type ret;
        V(TypeChecker &t) : tc(t), ret(TypeKind::VOID) {}

        Type result() const { return ret; }

        void visit(IntLiteral &i) override { (void)i; ret = Type(TypeKind::INT); }
        void visit(FloatLiteral &f) override { (void)f; ret = Type(TypeKind::FLOAT); }
        void visit(CharLiteral &c) override { (void)c; ret = Type(TypeKind::CHAR); }
        void visit(StringLiteral &s) override { (void)s; ret = Type(TypeKind::STRING); }
        void visit(BoolLiteral &b) override { (void)b; ret = Type(TypeKind::BOOL); }

        void visit(VarExpr &v) override {
            Symbol *sym = tc.lookup(v.name);
            if (!sym) {
                // do not error for builtin names here; CallExpr handling will special-case builtins
                // but to avoid excessive noise, only emit error if name isn't a known builtin function
                if (v.name == "len" || v.name == "push" || v.name == "pop" || v.name == "get" || v.name == "contains") {
                    // give a placeholder function type to avoid crashing later; CallExpr will check signature
                    ret = make_func_type(vector<Type>{}, Type(TypeKind::VOID));
                    return;
                }
                Diagnostic d{Diagnostic::Severity::Error, v.loc, "use of undeclared variable '" + v.name + "'", "TC002"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            if (!sym->initialized) {
                Diagnostic d{Diagnostic::Severity::Error, v.loc, "use of uninitialized variable '" + v.name + "'", "TC003"};
                if (tc.diag) tc.diag->report(d);
            }
            ret = sym->type;
        }

        void visit(BinaryExpr &b) override {
            Type L = tc.type_of_expr(*b.left);
            Type R = tc.type_of_expr(*b.right);
            if (b.op == BinOp::Add || b.op == BinOp::Sub || b.op == BinOp::Mul || b.op == BinOp::Div) {
                if (tc.is_numeric(L.kind) && tc.is_numeric(R.kind)) {
                    ret = (L.kind == TypeKind::FLOAT || R.kind == TypeKind::FLOAT) ? Type(TypeKind::FLOAT) : Type(TypeKind::INT);
                    return;
                }
                if (L.kind == TypeKind::STRING && R.kind == TypeKind::STRING && b.op == BinOp::Add) {
                    ret = Type(TypeKind::STRING);
                    return;
                }
                Diagnostic d{Diagnostic::Severity::Error, b.loc, "invalid operand types for operator", "TC004"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }

            if (b.op == BinOp::Lt || b.op == BinOp::Gt || b.op == BinOp::Le || b.op == BinOp::Ge || b.op == BinOp::Eq || b.op == BinOp::Ne) {
                if (L == R || (tc.is_numeric(L.kind) && tc.is_numeric(R.kind))) {
                    ret = Type(TypeKind::BOOL);
                    return;
                }
                Diagnostic d{Diagnostic::Severity::Error, b.loc, "cannot compare the given operand types", "TC005"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::BOOL); return;
            }

            Diagnostic d{Diagnostic::Severity::Error, b.loc, "unknown binary operator", "TC006"};
            if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT);
        }

        void visit(UnaryExpr &u) override {
            Type t = tc.type_of_expr(*u.expr);
            if (u.op == UnOp::Neg) {
                if (tc.is_numeric(t.kind)) { ret = t; return; }
                Diagnostic d{Diagnostic::Severity::Error, u.loc, "unary '-' applied to non-numeric type", "TC007"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            Diagnostic d{Diagnostic::Severity::Error, u.loc, "unknown unary operator", "TC008"};
            if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT);
        }

        void visit(CastExpr &c) override {
            Type from = tc.type_of_expr(*c.expr);
            Type to = c.target;
            if (!tc.cast_allowed(from, to)) {
                Diagnostic d{Diagnostic::Severity::Error, c.loc, "cast from " + type_to_string(from.kind) + " to " + type_to_string(to.kind) + " not allowed", "TC009"};
                if (tc.diag) tc.diag->report(d); ret = to; return;
            }
            ret = to;
        }

        void visit(ScanExpr &s) override {
            (void)s;
            ret = Type(TypeKind::STRING);
        }

        void visit(ArrayLiteralExpr &ae) override {
            if (ae.elems.empty()) {
                ret = make_dyn_array(Type(TypeKind::INT));
                return;
            }
            vector<Type> elemTypes;
            for (auto &e : ae.elems) elemTypes.push_back(tc.type_of_expr(*e));
            bool allSame = true;
            for (size_t i = 1; i < elemTypes.size(); ++i) if (!(elemTypes[i] == elemTypes[0])) { allSame = false; break; }
            if (allSame) {
                ret = make_dyn_array(elemTypes[0]);
                return;
            }
            bool numericMix = true; bool seenFloat = false, seenInt = false;
            for (auto &et : elemTypes) {
                if (et.kind == TypeKind::FLOAT) seenFloat = true;
                else if (et.kind == TypeKind::INT) seenInt = true;
                else { numericMix = false; break; }
            }
            if (numericMix && seenFloat && seenInt) {
                ret = make_dyn_array(Type(TypeKind::FLOAT));
                return;
            }
            Diagnostic d{Diagnostic::Severity::Error, ae.loc, "cannot infer array element type (heterogeneous)", "TC040"};
            if (tc.diag) tc.diag->report(d);
            ret = make_dyn_array(Type(TypeKind::INT));
        }

        void visit(TupleLiteralExpr &te) override {
            vector<Type> elems;
            for (auto &e : te.elems) elems.push_back(tc.type_of_expr(*e));
            ret = make_tuple_type(elems, {});
        }

        void visit(MapLiteralExpr &me) override {
            if (me.entries.empty()) {
                ret = make_map(Type(TypeKind::STRING), Type(TypeKind::INT));
                return;
            }
            vector<Type> keyTypes, valTypes;
            for (auto &kv : me.entries) {
                keyTypes.push_back(tc.type_of_expr(*kv.first));
                valTypes.push_back(tc.type_of_expr(*kv.second));
            }
            bool kSame = true, vSame = true;
            for (size_t i = 1; i < keyTypes.size(); ++i) if (!(keyTypes[i] == keyTypes[0])) { kSame = false; break; }
            for (size_t i = 1; i < valTypes.size(); ++i) if (!(valTypes[i] == valTypes[0])) { vSame = false; break; }
            if (!kSame || !vSame) {
                Diagnostic d{Diagnostic::Severity::Error, me.loc, "map literal with heterogeneous key/value types", "TC041"};
                if (tc.diag) tc.diag->report(d);
                ret = make_map(Type(TypeKind::STRING), Type(TypeKind::INT));
                return;
            }
            if (!tc.is_hashable_type(keyTypes[0])) {
                Diagnostic d{Diagnostic::Severity::Error, me.loc, "map keys must be hashable types", "TC042"};
                if (tc.diag) tc.diag->report(d);
            }
            ret = make_map(keyTypes[0], valTypes[0]);
        }

        void visit(IndexExpr &ie) override {
            Type target = tc.type_of_expr(*ie.target);
            Type idx = tc.type_of_expr(*ie.index);
            if (idx.kind != TypeKind::INT) {
                Diagnostic d{Diagnostic::Severity::Error, ie.loc, "index must be int", "TC050"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            if (target.kind == TypeKind::DYN_ARRAY || target.kind == TypeKind::STATIC_ARRAY) {
                ret = target.subs[0];
                return;
            }
            if (target.kind == TypeKind::TUPLE) {
                if (auto lit = dynamic_cast<IntLiteral*>(ie.index.get())) {
                    long long v = lit->value;
                    if (v < 0 || (size_t)v >= target.subs.size()) {
                        Diagnostic d{Diagnostic::Severity::Error, ie.loc, "tuple index out of range", "TC051"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    ret = target.subs[(size_t)v];
                    return;
                } else {
                    Diagnostic d{Diagnostic::Severity::Error, ie.loc, "tuple index must be constant integer literal", "TC052"};
                    if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                }
            }
            if (target.kind == TypeKind::MAP) {
                if (!can_assign_implicit(target.subs[0], idx)) {
                    Diagnostic d{Diagnostic::Severity::Error, ie.loc, "map index type mismatch", "TC053"};
                    if (tc.diag) tc.diag->report(d);
                }
                ret = target.subs[1];
                return;
            }
            Diagnostic d{Diagnostic::Severity::Error, ie.loc, "indexing not supported on this type", "TC054"};
            if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT);
        }

        void visit(MemberExpr &me) override {
            Type target = tc.type_of_expr(*me.target);
            if (target.kind != TypeKind::TUPLE) {
                Diagnostic d{Diagnostic::Severity::Error, me.loc, "member access on non-tuple type", "TC060"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            if (target.tuple_field_names.empty()) {
                Diagnostic d{Diagnostic::Severity::Error, me.loc, "tuple has no named fields", "TC061"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            bool found = false;
            size_t idx = 0;
            for (size_t i = 0; i < target.tuple_field_names.size(); ++i) {
                if (target.tuple_field_names[i] == me.name) { found = true; idx = i; break; }
            }
            if (!found) {
                Diagnostic d{Diagnostic::Severity::Error, me.loc, "tuple has no field named '" + me.name + "'", "TC062"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            ret = target.subs[idx];
        }

        void visit(CallExpr &ce) override {
            // builtin short-circuits: handle builtins without requiring the callee to be declared
            if (auto ve = dynamic_cast<VarExpr*>(ce.callee.get())) {
                string name = ve->name;
                if (name == "len") {
                    if (ce.args.size() != 1) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "len() expects 1 argument", "TC100"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    Type at = tc.type_of_expr(*ce.args[0]);
                    if (at.kind == TypeKind::STRING) { ret = Type(TypeKind::INT); return; }
                    if (at.kind == TypeKind::DYN_ARRAY || at.kind == TypeKind::STATIC_ARRAY) { ret = Type(TypeKind::INT); return; }
                    Diagnostic d{Diagnostic::Severity::Error, ce.loc, "len() argument must be array or string", "TC101"};
                    if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                }
                if (name == "push") {
                    if (ce.args.size() != 2) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() expects 2 arguments", "TC110"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::VOID); return;
                    }
                    Type at = tc.type_of_expr(*ce.args[0]);
                    Type val = tc.type_of_expr(*ce.args[1]);
                    if (at.kind != TypeKind::DYN_ARRAY) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() first argument must be dynamic array", "TC111"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::VOID); return;
                    }
                    if (!can_assign_implicit(at.subs[0], val)) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() value type mismatch", "TC112"};
                        if (tc.diag) tc.diag->report(d);
                    }
                    ret = Type(TypeKind::VOID); return;
                }
                if (name == "pop") {
                    if (ce.args.size() != 1) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "pop() expects 1 argument", "TC120"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    Type at = tc.type_of_expr(*ce.args[0]);
                    if (at.kind != TypeKind::DYN_ARRAY) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "pop() first argument must be dynamic array", "TC121"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    ret = at.subs[0]; return;
                }
                if (name == "get") {
                    if (ce.args.size() != 2) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "get() expects 2 arguments", "TC130"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    Type mt = tc.type_of_expr(*ce.args[0]);
                    Type keyt = tc.type_of_expr(*ce.args[1]);
                    if (mt.kind != TypeKind::MAP) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "get() first argument must be map", "TC131"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
                    }
                    if (!can_assign_implicit(mt.subs[0], keyt)) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "get() key type mismatch", "TC132"};
                        if (tc.diag) tc.diag->report(d);
                    }
                    ret = mt.subs[1]; return;
                }
                if (name == "contains") {
                    if (ce.args.size() != 2) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "contains() expects 2 arguments", "TC140"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::BOOL); return;
                    }
                    Type mt = tc.type_of_expr(*ce.args[0]);
                    Type keyt = tc.type_of_expr(*ce.args[1]);
                    if (mt.kind != TypeKind::MAP) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "contains() first argument must be map", "TC141"};
                        if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::BOOL); return;
                    }
                    if (!can_assign_implicit(mt.subs[0], keyt)) {
                        Diagnostic d{Diagnostic::Severity::Error, ce.loc, "contains() key type mismatch", "TC142"};
                        if (tc.diag) tc.diag->report(d);
                    }
                    ret = Type(TypeKind::BOOL); return;
                }
            }

            // default call behavior: evaluate callee type normally
            Type calleeT = tc.type_of_expr(*ce.callee);
            if (calleeT.kind != TypeKind::FUNC) {
                Diagnostic d{Diagnostic::Severity::Error, ce.loc, "attempt to call a non-function value", "TC020"};
                if (tc.diag) tc.diag->report(d); ret = Type(TypeKind::INT); return;
            }
            size_t paramCount = calleeT.subs.size() - 1;
            if (ce.args.size() != paramCount) {
                Diagnostic d{Diagnostic::Severity::Error, ce.loc, "function called with wrong number of arguments", "TC021"};
                if (tc.diag) tc.diag->report(d);
            }
            for (size_t i = 0; i < ce.args.size() && i < paramCount; ++i) {
                Type at = tc.type_of_expr(*ce.args[i]);
                Type pt = calleeT.subs[i];
                if (!tc.can_assign_implicit(pt, at)) {
                    Diagnostic d{Diagnostic::Severity::Error, ce.loc, "argument type mismatch in call", "TC022"};
                    if (tc.diag) tc.diag->report(d);
                }
            }
            ret = calleeT.subs.back();
        }
    };

    V vis(*this);
    e.accept(vis);
    type_map_[&e] = vis.result();
    return vis.result();
}

void TypeChecker::type_check_stmt(Stmt &s) {
    struct SV : public StmtVisitor {
        TypeChecker &tc;
        SV(TypeChecker &t) : tc(t) {}

        void visit(VarDeclStmt &vd) override {
            if (vd.declared_type && vd.initializer) {
                Type initT = tc.type_of_expr(*vd.initializer);
                if (!tc.can_assign_implicit(*vd.declared_type, initT)) {
                    Diagnostic d{Diagnostic::Severity::Error, vd.loc, "cannot initialize variable with incompatible type", "TC010"};
                    if (tc.diag) tc.diag->report(d);
                }
                tc.declare(vd.name, *vd.declared_type, true, vd.loc);
            } else if (vd.declared_type) {
                tc.declare(vd.name, *vd.declared_type, false, vd.loc);
            } else if (vd.initializer) {
                Type initT = tc.type_of_expr(*vd.initializer);
                tc.declare(vd.name, initT, true, vd.loc);
            } else {
                Diagnostic d{Diagnostic::Severity::Error, vd.loc, "variable declaration must have type or initializer", "TC011"};
                if (tc.diag) tc.diag->report(d);
            }
        }

        void visit(AssignStmt &as) override {
            auto *sym = tc.lookup(as.name);
            if (!sym) {
                Diagnostic d{Diagnostic::Severity::Error, as.loc, "assignment to undeclared variable '" + as.name + "'", "TC012"};
                if (tc.diag) tc.diag->report(d); return;
            }
            Type rhs = tc.type_of_expr(*as.expr);
            if (!tc.can_assign_implicit(sym->type, rhs)) {
                Diagnostic d{Diagnostic::Severity::Error, as.loc, "cannot assign expression of incompatible type to variable", "TC013"};
                if (tc.diag) tc.diag->report(d);
            }
            sym->initialized = true;
        }

        void visit(PrintStmt &p) override { tc.type_of_expr(*p.expr); }

        void visit(BlockStmt &b) override {
            tc.push_scope();
            for (auto &st : b.stmts) st->accept(*this);
            tc.pop_scope();
        }

        void visit(IfStmt &ifs) override {
            Type t = tc.type_of_expr(*ifs.cond);
            if (t.kind != TypeKind::BOOL) {
                Diagnostic d{Diagnostic::Severity::Error, ifs.loc, "if condition must be bool", "TC014"};
                if (tc.diag) tc.diag->report(d);
            }
            ifs.thenBlock->accept(*this);
        }

        void visit(WhileStmt &w) override {
            Type t = tc.type_of_expr(*w.cond);
            if (t.kind != TypeKind::BOOL) {
                Diagnostic d{Diagnostic::Severity::Error, w.loc, "while condition must be bool", "TC015"};
                if (tc.diag) tc.diag->report(d);
            }
            w.body->accept(*this);
        }

        void visit(ForStmt &f) override {
            tc.push_scope();
            f.init->accept(*this);
            Type condt = tc.type_of_expr(*f.condition);
            if (condt.kind != TypeKind::BOOL) {
                Diagnostic d{Diagnostic::Severity::Error, f.loc, "for condition must be bool", "TC016"};
                if (tc.diag) tc.diag->report(d);
            }
            f.update->accept(*this);
            f.body->accept(*this);
            tc.pop_scope();
        }

        void visit(ReturnStmt &r) override {
            if (r.value) tc.type_of_expr(*r.value);
        }

        void visit(FuncDeclStmt &fd) override {
            vector<Type> params;
            for (auto &p : fd.params) params.push_back(p.type);
            Type declaredRet = fd.ret_type.has_value() ? *fd.ret_type : Type(TypeKind::VOID);
            Type funcType = make_func_type(params, declaredRet);
            tc.declare(fd.name, funcType, true, fd.loc);

            tc.push_scope();
            for (auto &p : fd.params) tc.declare(p.name, p.type, true, fd.loc);

            vector<Type> retTypes;
            struct ReturnCollector : public StmtVisitor {
                TypeChecker &tc;
                vector<Type> &retTypes;
                ReturnCollector(TypeChecker &t, vector<Type> &r) : tc(t), retTypes(r) {}
                void visit(VarDeclStmt &vd) override { tc.type_check_stmt(vd); }
                void visit(AssignStmt &as) override { tc.type_check_stmt(as); }
                void visit(PrintStmt &p) override { tc.type_check_stmt(p); }
                void visit(BlockStmt &b) override { for (auto &st : b.stmts) st->accept(*this); }
                void visit(IfStmt &ifs) override { tc.type_of_expr(*ifs.cond); ifs.thenBlock->accept(*this); }
                void visit(WhileStmt &w) override { tc.type_of_expr(*w.cond); w.body->accept(*this); }
                void visit(ForStmt &f) override { f.init->accept(*this); tc.type_of_expr(*f.condition); f.update->accept(*this); f.body->accept(*this); }
                void visit(ReturnStmt &r) override { if (r.value) retTypes.push_back(tc.type_of_expr(*r.value)); else retTypes.push_back(Type(TypeKind::VOID)); }
                void visit(FuncDeclStmt &fd) override { tc.type_check_stmt(fd); }
                void visit(ExprStmt &rs) override { tc.type_check_stmt(rs); }
            } rc(*&tc, retTypes);

            if (fd.body) fd.body->accept(rc);

            Type finalRet = declaredRet;
            if (!fd.ret_type.has_value()) {
                if (retTypes.empty()) finalRet = Type(TypeKind::VOID);
                else {
                    bool allSame = true;
                    for (size_t i = 1; i < retTypes.size(); ++i) if (!(retTypes[i] == retTypes[0])) { allSame = false; break; }
                    if (allSame) finalRet = retTypes[0];
                    else {
                        bool allNumericMix = true; bool seenFloat = false, seenInt = false;
                        for (auto &rt : retTypes) {
                            if (rt.kind == TypeKind::FLOAT) seenFloat = true;
                            else if (rt.kind == TypeKind::INT) seenInt = true;
                            else { allNumericMix = false; break; }
                        }
                        if (allNumericMix && seenFloat && seenInt) finalRet = Type(TypeKind::FLOAT);
                        else {
                            Diagnostic d{Diagnostic::Severity::Error, fd.loc, "cannot infer function return type (inconsistent return types)", "TC030"};
                            if (tc.diag) tc.diag->report(d);
                            finalRet = Type(TypeKind::INT);
                        }
                    }
                }
            }

            fd.ret_type = finalRet;
            auto *sym = tc.lookup(fd.name);
            if (sym) sym->type = make_func_type(params, finalRet);
            tc.pop_scope();
        }
        void visit(ExprStmt &es) override { tc.type_of_expr(*es.expr); }
    };

    SV vis(*this);
    s.accept(vis);
}
