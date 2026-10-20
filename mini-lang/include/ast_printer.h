#ifndef MINI_LANG_AST_PRINTER_H
#define MINI_LANG_AST_PRINTER_H

#include "ast.h"
#include "typechecker.h"
#include <ostream>

struct ASTPrinter : public ExprVisitor, public StmtVisitor {
    std::ostream &os;
    TypeChecker *tc;
    int indent;

    ASTPrinter(std::ostream &o, TypeChecker *tcm = nullptr);

    void print_program(const std::vector<StmtPtr> &program);
    void print_indent();

    // ExprVisitor
    void visit(IntLiteral&) override;
    void visit(FloatLiteral&) override;
    void visit(CharLiteral&) override;
    void visit(StringLiteral&) override;
    void visit(BoolLiteral&) override;
    void visit(VarExpr&) override;
    void visit(BinaryExpr&) override;
    void visit(UnaryExpr&) override;
    void visit(CastExpr&) override;
    void visit(ScanExpr&) override;
    void visit(CallExpr&) override;
    void visit(ArrayLiteralExpr&) override;
    void visit(TupleLiteralExpr&) override;
    void visit(MapLiteralExpr&) override;
    void visit(IndexExpr&) override;
    void visit(MemberExpr&) override;

    // StmtVisitor
    void visit(VarDeclStmt&) override;
    void visit(AssignStmt&) override;
    void visit(PrintStmt&) override;
    void visit(BlockStmt&) override;
    void visit(IfStmt&) override;
    void visit(WhileStmt&) override;
    void visit(ForStmt&) override;
    void visit(ReturnStmt&) override;
    void visit(FuncDeclStmt&) override;
    void visit(ExprStmt&) override;
};

#endif // MINI_LANG_AST_PRINTER_H
