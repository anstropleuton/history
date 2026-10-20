#include "interpreter.h"
#include "diag.h"
#include "ast.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <functional>
#include <cstdint>

using namespace std;

Interpreter::Interpreter(Diagnostics *d) : diag(d) { runtime_.push_scope(); }

// MapKey used as key in map hash
struct MapKey {
    enum KKind { K_STR, K_INT, K_CHAR, K_BOOL, K_TUPLE } kind;
    string s;
    long long i = 0;
    char c = 0;
    bool b = false;
    vector<MapKey> tuple;

    bool operator==(MapKey const &o) const {
        if (kind != o.kind) return false;
        switch (kind) {
            case K_STR: return s == o.s;
            case K_INT: return i == o.i;
            case K_CHAR: return c == o.c;
            case K_BOOL: return b == o.b;
            case K_TUPLE: return tuple == o.tuple;
        }
        return false;
    }
};

struct MapKeyHash {
    size_t operator()(MapKey const &k) const noexcept {
        size_t h = 1469598103934665603ull;
        auto mix = [&](size_t x){ h ^= x; h *= 1099511628211ull; };
        switch (k.kind) {
            case MapKey::K_STR: mix(std::hash<string>{}(k.s)); break;
            case MapKey::K_INT: mix(std::hash<long long>{}(k.i)); break;
            case MapKey::K_CHAR: mix(std::hash<char>{}(k.c)); break;
            case MapKey::K_BOOL: mix(std::hash<bool>{}(k.b)); break;
            case MapKey::K_TUPLE:
                for (auto &sub : k.tuple) mix(operator()(sub));
                break;
        }
        return h;
    }
};

// Full MapVal
struct MapVal {
    Type keyType;
    Type valType;
    unordered_map<MapKey, RuntimeValue, MapKeyHash> m;
};

// Helpers
static string runtime_val_to_string(const RuntimeValue &v);

static MapKey make_key_from_runtime(const RuntimeValue &v) {
    if (holds_alternative<string>(v)) return MapKey{MapKey::K_STR, get<string>(v),0,0,false,{}};
    if (holds_alternative<long long>(v)) return MapKey{MapKey::K_INT,"",get<long long>(v),0,false,{}};
    if (holds_alternative<char>(v)) return MapKey{MapKey::K_CHAR,"",0,get<char>(v),false,{}};
    if (holds_alternative<bool>(v)) return MapKey{MapKey::K_BOOL,"",0,0,get<bool>(v),{}};
    if (holds_alternative<shared_ptr<TupleVal>>(v)) {
        auto tvptr = get<shared_ptr<TupleVal>>(v);
        MapKey mk; mk.kind = MapKey::K_TUPLE;
        for (auto &elem : tvptr->elems) mk.tuple.push_back(make_key_from_runtime(elem));
        return mk;
    }
    return MapKey{MapKey::K_STR, "", 0,0,false,{}};
}

static RuntimeValue zero_for_type(const Type &t) {
    switch (t.kind) {
        case TypeKind::INT: return (long long)0;
        case TypeKind::FLOAT: return (double)0.0;
        case TypeKind::CHAR: return (char)0;
        case TypeKind::BOOL: return false;
        case TypeKind::STRING: return string();
        case TypeKind::FUNC: return RFuncPtr();
        case TypeKind::DYN_ARRAY:
        case TypeKind::STATIC_ARRAY: {
            auto a = make_shared<ArrayVal>();
            a->is_static = (t.kind == TypeKind::STATIC_ARRAY);
            if (a->is_static && t.array_size) a->static_size = *t.array_size;
            if (a->is_static && t.array_size) {
                for (size_t i = 0; i < *t.array_size; ++i) a->elems.push_back(zero_for_type(t.subs[0]));
            }
            return a;
        }
        case TypeKind::TUPLE: {
            auto tv = make_shared<TupleVal>();
            tv->names = t.tuple_field_names;
            for (size_t i=0;i<t.subs.size();++i) tv->elems.push_back(zero_for_type(t.subs[i]));
            return tv;
        }
        case TypeKind::MAP: {
            auto mv = make_shared<MapVal>();
            mv->keyType = t.subs[0];
            mv->valType = t.subs[1];
            return mv;
        }
        default:
            return string();
    }
}

static string runtime_val_to_string(const RuntimeValue &v) {
    if (holds_alternative<long long>(v)) return to_string(get<long long>(v));
    if (holds_alternative<double>(v)) { ostringstream os; os << get<double>(v); return os.str(); }
    if (holds_alternative<char>(v)) { string s; s.push_back(get<char>(v)); return s; }
    if (holds_alternative<bool>(v)) return get<bool>(v) ? "true" : "false";
    if (holds_alternative<string>(v)) return get<string>(v);
    if (holds_alternative<RFuncPtr>(v)) return "<function>";
    if (holds_alternative<shared_ptr<ArrayVal>>(v)) {
        auto arr = get<shared_ptr<ArrayVal>>(v);
        ostringstream os; os << "[";
        for (size_t i=0;i<arr->elems.size();++i) {
            if (i) os << ", ";
            os << runtime_val_to_string(arr->elems[i]);
        }
        os << "]";
        return os.str();
    }
    if (holds_alternative<shared_ptr<TupleVal>>(v)) {
        auto tv = get<shared_ptr<TupleVal>>(v);
        ostringstream os; os << "(";
        for (size_t i=0;i<tv->elems.size();++i) {
            if (i) os << ", ";
            os << runtime_val_to_string(tv->elems[i]);
        }
        os << ")";
        return os.str();
    }
    if (holds_alternative<shared_ptr<MapVal>>(v)) {
        auto mv = get<shared_ptr<MapVal>>(v);
        ostringstream os; os << "{";
        bool first = true;
        for (auto &kv : mv->m) {
            if (!first) os << ", ";
            first = false;
            os << "<key>: " << runtime_val_to_string(kv.second);
        }
        os << "}";
        return os.str();
    }
    return "<unknown>";
}

RuntimeValue Interpreter::cast_runtime(const RuntimeValue &v, const Type &from, const Type &to, const Loc &loc) {
    if (from == to) return v;
    if (holds_alternative<long long>(v) && to.kind == TypeKind::FLOAT) return (double)get<long long>(v);
    if (holds_alternative<double>(v) && to.kind == TypeKind::INT) return (long long)get<double>(v);
    if (holds_alternative<string>(v)) {
        const string &s = get<string>(v);
        try {
            if (to.kind == TypeKind::INT) { size_t pos=0; long long val = stoll(s,&pos); if (pos!=s.size()) throw invalid_argument("trailing"); return val; }
            if (to.kind == TypeKind::FLOAT) { size_t pos=0; double val = stod(s,&pos); if (pos!=s.size()) throw invalid_argument("trailing"); return val; }
            if (to.kind == TypeKind::BOOL) { if (s=="true") return true; if (s=="false") return false; throw invalid_argument("invalid bool"); }
            if (to.kind == TypeKind::CHAR) { if (s.size()==1) return s[0]; throw invalid_argument("invalid char"); }
        } catch (...) { Diagnostic d{Diagnostic::Severity::Error, loc, string("cannot parse string '") + s + "' as " + type_to_string(to.kind), "RT001"}; if (diag) diag->report(d); throw DiagnosticError(d); }
    }
    if (to.kind == TypeKind::STRING) return runtime_val_to_string(v);
    Diagnostic d{Diagnostic::Severity::Error, loc, "unsupported runtime cast", "RT002"};
    if (diag) diag->report(d);
    throw DiagnosticError(d);
}

struct ReturnException { RuntimeValue value; ReturnException(RuntimeValue v): value(std::move(v)) {} };

RuntimeValue Interpreter::eval_expr(Expr &e) {
    struct EvalVisitor : ExprVisitor {
        Interpreter &interp;
        RuntimeValue result;
        EvalVisitor(Interpreter &i): interp(i) {}

        void visit(IntLiteral &i) override { result = i.value; }
        void visit(FloatLiteral &f) override { result = f.value; }
        void visit(CharLiteral &c) override { result = c.value; }
        void visit(StringLiteral &s) override { result = s.value; }
        void visit(BoolLiteral &b) override { result = b.value; }

        void visit(VarExpr &v) override {
            auto *p = interp.runtime_lookup(v.name);
            if (!p) { Diagnostic d{Diagnostic::Severity::Error, v.loc, "runtime use of undeclared variable '" + v.name + "'", "RT003"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            result = p->second;
        }

        void visit(BinaryExpr &b) override {
            RuntimeValue lv = interp.eval_expr(*b.left);
            RuntimeValue rv = interp.eval_expr(*b.right);
            bool Lnum = holds_alternative<long long>(lv) || holds_alternative<double>(lv);
            bool Rnum = holds_alternative<long long>(rv) || holds_alternative<double>(rv);
            if (b.op == BinOp::Add || b.op == BinOp::Sub || b.op == BinOp::Mul || b.op == BinOp::Div) {
                if (Lnum && Rnum) {
                    bool eitherDouble = holds_alternative<double>(lv) || holds_alternative<double>(rv);
                    if (eitherDouble) {
                        double a = holds_alternative<double>(lv) ? get<double>(lv) : (double)get<long long>(lv);
                        double bb = holds_alternative<double>(rv) ? get<double>(rv) : (double)get<long long>(rv);
                        if (b.op == BinOp::Add) result = a + bb;
                        if (b.op == BinOp::Sub) result = a - bb;
                        if (b.op == BinOp::Mul) result = a * bb;
                        if (b.op == BinOp::Div) { if (bb==0.0) { Diagnostic d{Diagnostic::Severity::Error, b.loc, "division by zero", "RT004"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); } result = a / bb; }
                        return;
                    } else {
                        long long a = get<long long>(lv);
                        long long bb = get<long long>(rv);
                        if (b.op == BinOp::Add) result = a + bb;
                        if (b.op == BinOp::Sub) result = a - bb;
                        if (b.op == BinOp::Mul) result = a * bb;
                        if (b.op == BinOp::Div) { if (bb==0) { Diagnostic d{Diagnostic::Severity::Error, b.loc, "division by zero", "RT005"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); } result = a / bb; }
                        return;
                    }
                }
                if (holds_alternative<string>(lv) && holds_alternative<string>(rv) && b.op == BinOp::Add) {
                    result = get<string>(lv) + get<string>(rv); return;
                }
                Diagnostic d{Diagnostic::Severity::Error, b.loc, "invalid binary operands at runtime", "RT006"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
            }
            if (b.op == BinOp::Lt || b.op == BinOp::Gt || b.op == BinOp::Le || b.op == BinOp::Ge || b.op == BinOp::Eq || b.op == BinOp::Ne) {
                if (Lnum && Rnum) {
                    double a = holds_alternative<double>(lv) ? get<double>(lv) : (double)get<long long>(lv);
                    double bb = holds_alternative<double>(rv) ? get<double>(rv) : (double)get<long long>(rv);
                    if (b.op == BinOp::Lt) result = a < bb;
                    if (b.op == BinOp::Gt) result = a > bb;
                    if (b.op == BinOp::Le) result = a <= bb;
                    if (b.op == BinOp::Ge) result = a >= bb;
                    if (b.op == BinOp::Eq) result = a == bb;
                    if (b.op == BinOp::Ne) result = a != bb;
                    return;
                }
                if (lv.index() == rv.index()) {
                    if (b.op == BinOp::Eq) result = (lv == rv);
                    if (b.op == BinOp::Ne) result = (lv != rv);
                    return;
                }
                Diagnostic d{Diagnostic::Severity::Error, b.loc, "invalid comparison at runtime", "RT007"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
            }
            Diagnostic d{Diagnostic::Severity::Error, b.loc, "unknown binary operator at runtime", "RT008"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
        }

        void visit(UnaryExpr &u) override {
            RuntimeValue v = interp.eval_expr(*u.expr);
            if (u.op == UnOp::Neg) {
                if (holds_alternative<double>(v)) result = -get<double>(v);
                else if (holds_alternative<long long>(v)) result = -get<long long>(v);
                else { Diagnostic d{Diagnostic::Severity::Error, u.loc, "unary - applied to non-numeric", "RT009"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                return;
            }
            Diagnostic d{Diagnostic::Severity::Error, u.loc, "unknown unary op", "RT010"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
        }

        void visit(CastExpr &c) override {
            RuntimeValue v = interp.eval_expr(*c.expr);
            Type from;
            if (holds_alternative<long long>(v)) from = Type(TypeKind::INT);
            else if (holds_alternative<double>(v)) from = Type(TypeKind::FLOAT);
            else if (holds_alternative<char>(v)) from = Type(TypeKind::CHAR);
            else if (holds_alternative<bool>(v)) from = Type(TypeKind::BOOL);
            else if (holds_alternative<string>(v)) from = Type(TypeKind::STRING);
            else { Diagnostic d{Diagnostic::Severity::Error, c.loc, "unknown value type to cast", "RT011"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            result = interp.cast_runtime(v, from, c.target, c.loc);
        }

        void visit(ScanExpr &s) override { (void)s; string line; if (!getline(cin, line)) line=""; result = line; }

        void visit(ArrayLiteralExpr &ae) override {
            auto av = make_shared<ArrayVal>();
            for (auto &el : ae.elems) {
                RuntimeValue rv = interp.eval_expr(*el);
                av->elems.push_back(rv);
            }
            result = av;
        }

        void visit(TupleLiteralExpr &te) override {
            auto tv = make_shared<TupleVal>();
            for (auto &el : te.elems) {
                RuntimeValue rv = interp.eval_expr(*el);
                tv->elems.push_back(rv);
                tv->names.push_back("");
            }
            result = tv;
        }

        void visit(MapLiteralExpr &me) override {
            auto mv = make_shared<MapVal>();
            for (auto &kv : me.entries) {
                RuntimeValue k = interp.eval_expr(*kv.first);
                RuntimeValue v = interp.eval_expr(*kv.second);
                MapKey mk = make_key_from_runtime(k);
                mv->m.emplace(mk, v);
            }
            result = mv;
        }

        void visit(IndexExpr &ie) override {
            RuntimeValue targ = interp.eval_expr(*ie.target);
            RuntimeValue idxv = interp.eval_expr(*ie.index);
            if (!holds_alternative<long long>(idxv)) { Diagnostic d{Diagnostic::Severity::Error, ie.loc, "index not integer at runtime", "RT070"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            long long ii = get<long long>(idxv);
            if (holds_alternative<shared_ptr<ArrayVal>>(targ)) {
                auto av = get<shared_ptr<ArrayVal>>(targ);
                if (ii < 0 || (size_t)ii >= av->elems.size()) { Diagnostic d{Diagnostic::Severity::Error, ie.loc, "array index out of bounds", "RT071"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                result = av->elems[(size_t)ii];
                return;
            }
            if (holds_alternative<shared_ptr<TupleVal>>(targ)) {
                auto tv = get<shared_ptr<TupleVal>>(targ);
                if (ii < 0 || (size_t)ii >= tv->elems.size()) { Diagnostic d{Diagnostic::Severity::Error, ie.loc, "tuple index out of bounds", "RT072"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                result = tv->elems[(size_t)ii];
                return;
            }
            if (holds_alternative<shared_ptr<MapVal>>(targ)) {
                auto mv = get<shared_ptr<MapVal>>(targ);
                MapKey mk = make_key_from_runtime(idxv);
                auto it = mv->m.find(mk);
                if (it == mv->m.end()) { Diagnostic d{Diagnostic::Severity::Error, ie.loc, "map key not present", "RT073"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                result = it->second; return;
            }
            Diagnostic d{Diagnostic::Severity::Error, ie.loc, "indexing unsupported at runtime", "RT074"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
        }

        void visit(MemberExpr &me) override {
            RuntimeValue targ = interp.eval_expr(*me.target);
            if (!holds_alternative<shared_ptr<TupleVal>>(targ)) { Diagnostic d{Diagnostic::Severity::Error, me.loc, "member access on non-tuple at runtime", "RT080"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            auto tv = get<shared_ptr<TupleVal>>(targ);
            size_t idx = SIZE_MAX;
            for (size_t i=0;i<tv->names.size();++i) if (tv->names[i] == me.name) { idx = i; break; }
            if (idx == SIZE_MAX) { Diagnostic d{Diagnostic::Severity::Error, me.loc, "tuple has no field named '" + me.name + "'", "RT081"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            result = tv->elems[idx];
        }

        void visit(CallExpr &ce) override {
            // builtin runtime implementations first (detect by callee AST being VarExpr)
            if (auto ve = dynamic_cast<VarExpr*>(ce.callee.get())) {
                string name = ve->name;
                if (name == "len") {
                    if (ce.args.size() != 1) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "len() expects 1 argument", "RT200"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue a = interp.eval_expr(*ce.args[0]);
                    if (holds_alternative<string>(a)) { result = (long long)get<string>(a).size(); return; }
                    if (holds_alternative<shared_ptr<ArrayVal>>(a)) { result = (long long)get<shared_ptr<ArrayVal>>(a)->elems.size(); return; }
                    Diagnostic d{Diagnostic::Severity::Error, ce.loc, "len() expects string or array", "RT201"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d);
                }
                if (name == "push") {
                    if (ce.args.size() != 2) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() expects 2 args", "RT210"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue a = interp.eval_expr(*ce.args[0]);
                    RuntimeValue val = interp.eval_expr(*ce.args[1]);
                    if (!holds_alternative<shared_ptr<ArrayVal>>(a)) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() first arg must be array", "RT211"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    auto arr = get<shared_ptr<ArrayVal>>(a);
                    if (arr->is_static) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "push() on static array", "RT212"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    arr->elems.push_back(val);
                    result = (long long)arr->elems.size();
                    return;
                }
                if (name == "pop") {
                    if (ce.args.size() != 1) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "pop() expects 1 arg", "RT220"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue a = interp.eval_expr(*ce.args[0]);
                    if (!holds_alternative<shared_ptr<ArrayVal>>(a)) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "pop() first arg must be array", "RT221"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    auto arr = get<shared_ptr<ArrayVal>>(a);
                    if (arr->elems.empty()) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "pop() on empty array", "RT222"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue v = arr->elems.back();
                    arr->elems.pop_back();
                    result = v;
                    return;
                }
                if (name == "get") {
                    if (ce.args.size() != 2) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "get() expects 2 args", "RT230"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue mval = interp.eval_expr(*ce.args[0]);
                    RuntimeValue k = interp.eval_expr(*ce.args[1]);
                    if (!holds_alternative<shared_ptr<MapVal>>(mval)) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "get() first arg must be map", "RT231"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    auto mv = get<shared_ptr<MapVal>>(mval);
                    MapKey mk = make_key_from_runtime(k);
                    auto it = mv->m.find(mk);
                    if (it == mv->m.end()) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "map key not present", "RT232"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    result = it->second; return;
                }
                if (name == "contains") {
                    if (ce.args.size() != 2) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "contains() expects 2 args", "RT240"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    RuntimeValue mval = interp.eval_expr(*ce.args[0]);
                    RuntimeValue k = interp.eval_expr(*ce.args[1]);
                    if (!holds_alternative<shared_ptr<MapVal>>(mval)) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "contains() first arg must be map", "RT241"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                    auto mv = get<shared_ptr<MapVal>>(mval);
                    MapKey mk = make_key_from_runtime(k);
                    result = mv->m.find(mk) != mv->m.end();
                    return;
                }
            }

            // default: call user-defined function
            RuntimeValue calleeVal = interp.eval_expr(*ce.callee);
            if (!holds_alternative<RFuncPtr>(calleeVal)) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "attempt to call non-function value", "RT020"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            RFuncPtr rf = get<RFuncPtr>(calleeVal);
            if (ce.args.size() != rf->param_names.size()) { Diagnostic d{Diagnostic::Severity::Error, ce.loc, "wrong number of arguments in call", "RT021"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            vector<RuntimeValue> evalArgs;
            for (size_t i=0;i<ce.args.size();++i) evalArgs.push_back(interp.eval_expr(*ce.args[i]));
            interp.runtime_.push_scope();
            for (size_t i=0;i<rf->param_names.size();++i) interp.runtime_.scopes.back()[rf->param_names[i]] = { rf->param_types[i], evalArgs[i] };
            try {
                try {
                    interp.exec_stmt(*rf->body);
                    interp.runtime_.pop_scope();
                    RuntimeValue zero = zero_for_type(rf->ret_type);
                    result = zero; return;
                } catch (const ReturnException &re) {
                    interp.runtime_.pop_scope();
                    result = re.value; return;
                }
            } catch (...) { interp.runtime_.pop_scope(); throw; }
        }
    } ev(*this);
    e.accept(ev);
    return ev.result;
}

void Interpreter::declare_runtime(const std::string &name, Type t, bool initialized, const Loc &loc) {
    (void)initialized;
    auto &cur = runtime_.scopes.back();
    if (cur.count(name)) { Diagnostic d{Diagnostic::Severity::Error, loc, "runtime redeclaration", "RT012"}; if (diag) diag->report(d); throw DiagnosticError(d); }
    RuntimeValue defaultVal = zero_for_type(t);
    cur[name] = {t, defaultVal};
}

pair<Type, RuntimeValue>* Interpreter::runtime_lookup(const string &name) {
    for (int i=(int)runtime_.scopes.size()-1;i>=0;--i) {
        auto &m = runtime_.scopes[i];
        auto it = m.find(name);
        if (it != m.end()) return &it->second;
    }
    return nullptr;
}

void Interpreter::exec_stmt(Stmt &s) {
    struct ExecVisitor : StmtVisitor {
        Interpreter &interp;
        ExecVisitor(Interpreter &i): interp(i) {}
        void visit(VarDeclStmt &vd) override {
            Type targetType;
            if (vd.declared_type) targetType = *vd.declared_type;
            else {
                assert(vd.initializer);
                RuntimeValue v = interp.eval_expr(*vd.initializer);
                if (holds_alternative<long long>(v)) targetType = Type(TypeKind::INT);
                else if (holds_alternative<double>(v)) targetType = Type(TypeKind::FLOAT);
                else if (holds_alternative<char>(v)) targetType = Type(TypeKind::CHAR);
                else if (holds_alternative<bool>(v)) targetType = Type(TypeKind::BOOL);
                else if (holds_alternative<string>(v)) targetType = Type(TypeKind::STRING);
                else if (holds_alternative<shared_ptr<TupleVal>>(v)) {
                    auto tv = get<shared_ptr<TupleVal>>(v);
                    vector<Type> subs(tv->elems.size(), Type(TypeKind::INT));
                    targetType = make_tuple_type(subs, tv->names);
                } else if (holds_alternative<shared_ptr<ArrayVal>>(v)) {
                    auto av = get<shared_ptr<ArrayVal>>(v);
                    vector<Type> subs; subs.push_back(Type(TypeKind::INT));
                    targetType = make_dyn_array(subs[0]);
                } else targetType = Type(TypeKind::STRING);
            }
            interp.declare_runtime(vd.name, targetType, false, vd.loc);
            if (vd.initializer) {
                RuntimeValue v = interp.eval_expr(*vd.initializer);
                if (vd.declared_type && vd.declared_type->kind == TypeKind::TUPLE && holds_alternative<shared_ptr<TupleVal>>(v)) {
                    auto tv = get<shared_ptr<TupleVal>>(v);
                    tv->names = vd.declared_type->tuple_field_names;
                }
                auto *p = interp.runtime_lookup(vd.name);
                p->second = v;
            }
        }

        void visit(AssignStmt &as) override {
            auto *p = interp.runtime_lookup(as.name);
            if (!p) { Diagnostic d{Diagnostic::Severity::Error, as.loc, "assignment to undeclared variable", "RT013"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            RuntimeValue v = interp.eval_expr(*as.expr);
            p->second = v;
        }

        void visit(PrintStmt &p) override {
            RuntimeValue v = interp.eval_expr(*p.expr);
            cout << runtime_val_to_string(v) << "\n";
        }

        void visit(BlockStmt &b) override {
            interp.runtime_.push_scope();
            for (auto &st : b.stmts) st->accept(*this);
            interp.runtime_.pop_scope();
        }

        void visit(IfStmt &ifs) override {
            RuntimeValue c = interp.eval_expr(*ifs.cond);
            if (!holds_alternative<bool>(c)) { Diagnostic d{Diagnostic::Severity::Error, ifs.loc, "if condition not bool at runtime", "RT014"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
            if (get<bool>(c)) ifs.thenBlock->accept(*this);
        }

        void visit(WhileStmt &w) override {
            while (true) {
                RuntimeValue c = interp.eval_expr(*w.cond);
                if (!holds_alternative<bool>(c)) { Diagnostic d{Diagnostic::Severity::Error, w.loc, "while condition not bool at runtime", "RT015"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                if (!get<bool>(c)) break;
                w.body->accept(*this);
            }
        }

        void visit(ForStmt &f) override {
            interp.runtime_.push_scope();
            f.init->accept(*this);
            while (true) {
                RuntimeValue c = interp.eval_expr(*f.condition);
                if (!holds_alternative<bool>(c)) { Diagnostic d{Diagnostic::Severity::Error, f.loc, "for condition not bool at runtime", "RT016"}; if (interp.diag) interp.diag->report(d); throw DiagnosticError(d); }
                if (!get<bool>(c)) break;
                f.body->accept(*this);
                f.update->accept(*this);
            }
            interp.runtime_.pop_scope();
        }

        void visit(ReturnStmt &r) override {
            RuntimeValue v;
            if (r.value) v = interp.eval_expr(*r.value);
            else v = (long long)0;
            throw ReturnException(v);
        }

        void visit(FuncDeclStmt &fd) override {
            vector<string> pnames;
            vector<Type> ptypes;
            for (auto &p: fd.params) { pnames.push_back(p.name); ptypes.push_back(p.type); }
            Type ret = fd.ret_type ? *fd.ret_type : Type(TypeKind::VOID);
            StmtPtr bodyClone = std::move(fd.body);
            fd.body = make_unique<BlockStmt>(vector<StmtPtr>{}, fd.loc);
            RFuncPtr rf = make_shared<RuntimeFunction>(pnames, ptypes, ret, std::move(bodyClone), fd.loc);
            interp.declare_runtime(fd.name, make_func_type(ptypes, ret), false, fd.loc);
            auto *entry = interp.runtime_lookup(fd.name);
            if (entry) entry->second = rf;
        }

        void visit(ExprStmt &es) override {
            interp.eval_expr(*es.expr);
        }
    } vis(*this);
    s.accept(vis);
}

void Interpreter::run_program(const std::vector<StmtPtr> &program) {
    for (auto &s : program) exec_stmt(*s);
}
