#include "ast_printer.h"
#include <iostream>

ASTPrinter::ASTPrinter(std::ostream &o, TypeChecker *tcm): os(o), tc(tcm), indent(0) {}

void ASTPrinter::print_indent() {
    for (int i = 0; i < indent; ++i) os << "  ";
}

void ASTPrinter::print_program(const std::vector<StmtPtr> &program) {
    os << "Program:\n";
    indent++;
    for (auto &s : program) s->accept(*this);
    indent--;
}

// Expr visitors
void ASTPrinter::visit(IntLiteral &i) { print_indent(); os << "Int(" << i.value << ")\n"; }
void ASTPrinter::visit(FloatLiteral &f) { print_indent(); os << "Float(" << f.value << ")\n"; }
void ASTPrinter::visit(CharLiteral &c) { print_indent(); os << "Char('" << c.value << "')\n"; }
void ASTPrinter::visit(StringLiteral &s) { print_indent(); os << "String(\"" << s.value << "\")\n"; }
void ASTPrinter::visit(BoolLiteral &b) { print_indent(); os << "Bool(" << (b.value ? "true" : "false") << ")\n"; }
void ASTPrinter::visit(VarExpr &v) { print_indent(); os << "Var(" << v.name << ")\n"; }
void ASTPrinter::visit(BinaryExpr &b) { print_indent(); os << "Binary\n"; indent++; b.left->accept(*this); b.right->accept(*this); indent--; }
void ASTPrinter::visit(UnaryExpr &u) { print_indent(); os << "Unary\n"; indent++; u.expr->accept(*this); indent--; }
void ASTPrinter::visit(CastExpr &c) { print_indent(); os << "Cast to " << type_to_string(c.target.kind) << "\n"; indent++; c.expr->accept(*this); indent--; }
void ASTPrinter::visit(ScanExpr &s) { print_indent(); os << "Scan()\n"; }
void ASTPrinter::visit(CallExpr &ce) { print_indent(); os << "Call\n"; indent++; ce.callee->accept(*this); for (auto &a : ce.args) a->accept(*this); indent--; }
void ASTPrinter::visit(ArrayLiteralExpr &ae) { print_indent(); os << "ArrayLiteral\n"; indent++; for (auto &e : ae.elems) e->accept(*this); indent--; }
void ASTPrinter::visit(TupleLiteralExpr &te) { print_indent(); os << "TupleLiteral\n"; indent++; for (auto &e : te.elems) e->accept(*this); indent--; }
void ASTPrinter::visit(MapLiteralExpr &me) { print_indent(); os << "MapLiteral\n"; indent++; for (auto &kv : me.entries) { print_indent(); os << "Key:\n"; indent++; kv.first->accept(*this); indent--; print_indent(); os << "Value:\n"; indent++; kv.second->accept(*this); indent--; } indent--; }
void ASTPrinter::visit(IndexExpr &ie) { print_indent(); os << "IndexExpr\n"; indent++; ie.target->accept(*this); ie.index->accept(*this); indent--; }
void ASTPrinter::visit(MemberExpr &me) { print_indent(); os << "MemberExpr(" << me.name << ")\n"; indent++; me.target->accept(*this); indent--; }

// Stmt visitors
void ASTPrinter::visit(VarDeclStmt &vd) {
    print_indent();
    os << "VarDecl " << vd.name;
    if (vd.declared_type) os << " : " << type_to_string(vd.declared_type->kind);
    os << "\n";
    if (vd.initializer) { indent++; vd.initializer->accept(*this); indent--; }
}
void ASTPrinter::visit(AssignStmt &as) { print_indent(); os << "Assign " << as.name << "\n"; indent++; as.expr->accept(*this); indent--; }
void ASTPrinter::visit(PrintStmt &p) { print_indent(); os << "Print\n"; indent++; p.expr->accept(*this); indent--; }
void ASTPrinter::visit(BlockStmt &b) { print_indent(); os << "Block\n"; indent++; for (auto &s : b.stmts) s->accept(*this); indent--; }
void ASTPrinter::visit(IfStmt &ifs) { print_indent(); os << "If\n"; indent++; ifs.cond->accept(*this); ifs.thenBlock->accept(*this); indent--; }
void ASTPrinter::visit(WhileStmt &w) { print_indent(); os << "While\n"; indent++; w.cond->accept(*this); w.body->accept(*this); indent--; }
void ASTPrinter::visit(ForStmt &f) { print_indent(); os << "For\n"; indent++; if (f.init) f.init->accept(*this); if (f.condition) f.condition->accept(*this); if (f.update) f.update->accept(*this); if (f.body) f.body->accept(*this); indent--; }
void ASTPrinter::visit(ReturnStmt &r) { print_indent(); os << "Return\n"; if (r.value) { indent++; r.value->accept(*this); indent--; } }
void ASTPrinter::visit(FuncDeclStmt &fd) {
    print_indent();
    os << "FuncDecl " << fd.name << "\n";
    indent++;
    for (auto &p : fd.params) {
        print_indent();
        os << "Param " << p.name << " : " << type_to_string(p.type.kind) << "\n";
    }
    if (fd.body) fd.body->accept(*this);
    indent--;
}
void ASTPrinter::visit(ExprStmt &es) {
    print_indent();
    os << "ExprStmt\n";
    indent++;
    if (es.expr) es.expr->accept(*this);
    indent--;
}
