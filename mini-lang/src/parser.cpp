#include "parser.h"
#include <stdexcept>
#include <cassert>

using namespace std;

Parser::Parser(vector<Token> toks, Diagnostics *diag) : toks_(move(toks)), idx_(0), diag_(diag) {}

const Token& Parser::peek() const {
    if (idx_ < toks_.size()) return toks_[idx_];
    static Token e{TokenKind::End,"",Loc{0,0}};
    return e;
}
const Token& Parser::get() { return toks_[idx_++]; }
bool Parser::accept(TokenKind k) { if (peek().kind == k) { get(); return true; } return false; }
void Parser::expect(TokenKind k, const string &msg) {
    if (peek().kind == k) { get(); return; }
    Diagnostic d{Diagnostic::Severity::Error, peek().loc, msg.empty() ? "unexpected token" : msg, "PAR001"};
    if (diag_) { diag_->report(d); recover_to_sync(); } else throw DiagnosticError(d);
}

bool Parser::is_type_name(const Token &t) const {
    if (t.kind != TokenKind::Identifier) return false;
    string s = t.text;
    return (s=="int" || s=="float" || s=="char" || s=="bool" || s=="string" || s=="void" );
}

// parse_type: handles base types, array suffixes, tuple types and map types
Type Parser::parse_type() {
    Token t = peek();
    if (t.kind == TokenKind::LParen) {
        // tuple type
        get(); // (
        vector<Type> elems;
        vector<string> names;
        if (peek().kind != TokenKind::RParen) {
            while (true) {
                // optional name: IDENT COLON TYPE
                if (peek().kind == TokenKind::Identifier && (idx_ + 1 < toks_.size()) && toks_[idx_+1].kind == TokenKind::Colon) {
                    string fname = peek().text; get(); // name
                    expect(TokenKind::Colon, "expected ':' after field name");
                    Type ft = parse_type();
                    elems.push_back(ft);
                    names.push_back(fname);
                } else {
                    Type ft = parse_type();
                    elems.push_back(ft);
                    names.push_back("");
                }
                if (accept(TokenKind::Comma)) continue;
                break;
            }
        }
        expect(TokenKind::RParen, "expected ')' to close tuple type");
        Type tup = make_tuple_type(elems, names);
        // allow trailing map arrow like: (int,int) => int or -> int
        if (peek().kind == TokenKind::Arrow || peek().kind == TokenKind::MapArrow) { get(); Type val = parse_type(); return make_map(tup, val); }
        return tup;
    }
    if (t.kind == TokenKind::Identifier) {
        get();
        Type base;
        if (t.text == "int") base = Type(TypeKind::INT);
        else if (t.text == "float") base = Type(TypeKind::FLOAT);
        else if (t.text == "char") base = Type(TypeKind::CHAR);
        else if (t.text == "bool") base = Type(TypeKind::BOOL);
        else if (t.text == "string") base = Type(TypeKind::STRING);
        else if (t.text == "void") base = Type(TypeKind::VOID);
        else {
            // unknown type name -> error
            Diagnostic d{Diagnostic::Severity::Error, t.loc, "unknown type name", "PAR002"};
            if (diag_) { diag_->report(d); base = Type(TypeKind::INT); } else throw DiagnosticError(d);
        }
        // suffixes: [N] or [] or => (map) or -> (map)
        while (true) {
            if (peek().kind == TokenKind::LBracket) {
                get();
                if (peek().kind == TokenKind::RBracket) {
                    // dynamic array
                    get();
                    base = make_dyn_array(base);
                    continue;
                } else {
                    Token num = peek();
                    if (num.kind != TokenKind::IntLiteral) {
                        Diagnostic d{Diagnostic::Severity::Error, num.loc, "expected integer in array size", "PAR020"};
                        if (diag_) { diag_->report(d); recover_to_sync(); return base; } else throw DiagnosticError(d);
                    }
                    size_t n = (size_t)stoull(num.text);
                    get();
                    expect(TokenKind::RBracket, "expected ']'");
                    base = make_static_array(base, n);
                    continue;
                }
            }
            if (peek().kind == TokenKind::Arrow || peek().kind == TokenKind::MapArrow) {
                get();
                Type val = parse_type();
                base = make_map(base, val);
                continue;
            }
            break;
        }
        return base;
    }
    Diagnostic d{Diagnostic::Severity::Error, t.loc, "expected type", "PAR021"};
    if (diag_) { diag_->report(d); recover_to_sync(); return Type(TypeKind::INT); }
    throw DiagnosticError(d);
}

vector<StmtPtr> Parser::parse_program() {
    vector<StmtPtr> out;
    while (peek().kind != TokenKind::End) out.push_back(parse_statement());
    return out;
}

StmtPtr Parser::parse_statement() {
    if (peek().kind == TokenKind::LBrace) return parse_block();
    if (peek().kind == TokenKind::KeywordLet) return parse_vardecl();
    if (peek().kind == TokenKind::KeywordPrint) return parse_print();
    if (peek().kind == TokenKind::KeywordIf) return parse_if();
    if (peek().kind == TokenKind::KeywordWhile) return parse_while();
    if (peek().kind == TokenKind::KeywordFor) return parse_for();
    if (peek().kind == TokenKind::KeywordFn) return parse_func_decl();
    if (peek().kind == TokenKind::KeywordReturn) return parse_return();

    // expression statement (e.g., call(...); )
    if (peek().kind == TokenKind::Identifier && (idx_ + 1 < toks_.size()) && toks_[idx_+1].kind == TokenKind::LParen) {
        auto e = parse_expression();
        expect(TokenKind::Semicolon, "expected ';' after expression");
        return make_unique<ExprStmt>(move(e), e->loc);
    }

    if (peek().kind == TokenKind::Identifier) {
        Token id = get();
        if (peek().kind == TokenKind::Equal) {
            get();
            auto e = parse_expression();
            expect(TokenKind::Semicolon, "expected ';' after assignment");
            return make_unique<AssignStmt>(id.text, move(e), id.loc);
        } else {
            Diagnostic d{Diagnostic::Severity::Error, id.loc, "expected '=' after identifier in statement", "PAR003"};
            if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, id.loc); }
            throw DiagnosticError(d);
        }
    }
    Diagnostic d{Diagnostic::Severity::Error, peek().loc, "unexpected token in statement", "PAR004"};
    if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, peek().loc); }
    throw DiagnosticError(d);
}

StmtPtr Parser::parse_vardecl() {
    Token letTok = get();
    Token id = peek();
    if (id.kind != TokenKind::Identifier) {
        Diagnostic d{Diagnostic::Severity::Error, id.loc, "expected identifier after 'let'", "PAR005"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, letTok.loc); }
        throw DiagnosticError(d);
    }
    string name = id.text; get();
    optional<Type> declared;
    ExprPtr initializer = nullptr;
    if (accept(TokenKind::Colon)) {
        declared = parse_type();
    }
    if (accept(TokenKind::Equal)) initializer = parse_expression();
    if (!declared && !initializer) {
        Diagnostic d{Diagnostic::Severity::Error, letTok.loc, "let must include a type or initializer", "PAR007"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, letTok.loc); }
        throw DiagnosticError(d);
    }
    expect(TokenKind::Semicolon, "expected ';' after variable declaration");
    return make_unique<VarDeclStmt>(name, declared, move(initializer), letTok.loc);
}

StmtPtr Parser::parse_print() {
    Token t = get();
    auto e = parse_expression();
    expect(TokenKind::Semicolon, "expected ';' after print");
    return make_unique<PrintStmt>(move(e), t.loc);
}

StmtPtr Parser::parse_if() {
    Token t = get();
    auto cond = parse_expression();
    auto block = parse_block();
    return make_unique<IfStmt>(move(cond), move(block), t.loc);
}

StmtPtr Parser::parse_while() {
    Token t = get();
    auto cond = parse_expression();
    auto body = parse_block();
    return make_unique<WhileStmt>(move(cond), move(body), t.loc);
}

StmtPtr Parser::parse_for() {
    Token t = get();
    if (!accept(TokenKind::KeywordLet)) {
        Diagnostic d{Diagnostic::Severity::Error, peek().loc, "expected 'let' in for initializer", "PAR008"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, t.loc); }
        throw DiagnosticError(d);
    }
    Token id = peek();
    if (id.kind != TokenKind::Identifier) {
        Diagnostic d{Diagnostic::Severity::Error, id.loc, "expected identifier in 'for' initializer", "PAR009"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, t.loc); }
        throw DiagnosticError(d);
    }
    string name = id.text; get();
    optional<Type> declared;
    ExprPtr initializer = nullptr;
    if (accept(TokenKind::Colon)) {
        declared = parse_type();
    }
    if (accept(TokenKind::Equal)) initializer = parse_expression();
    if (!declared && !initializer) {
        Diagnostic d{Diagnostic::Severity::Error, t.loc, "for initializer must include type or initializer", "PAR011"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, t.loc); }
        throw DiagnosticError(d);
    }
    expect(TokenKind::Semicolon, "expected ';' after for initializer");
    auto cond = parse_expression();
    expect(TokenKind::Semicolon, "expected ';' after for condition");
    Token id2 = peek();
    if (id2.kind != TokenKind::Identifier) {
        Diagnostic d{Diagnostic::Severity::Error, id2.loc, "expected identifier in for update", "PAR012"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, t.loc); }
        throw DiagnosticError(d);
    }
    string upd_name = id2.text; get();
    expect(TokenKind::Equal, "expected '=' in for update");
    auto upd_expr = parse_expression();
    auto initStmt = make_unique<VarDeclStmt>(name, declared, move(initializer), t.loc);
    auto updateStmt = make_unique<AssignStmt>(upd_name, move(upd_expr), id2.loc);
    auto body = parse_block();
    return make_unique<ForStmt>(move(initStmt), move(cond), move(updateStmt), move(body), t.loc);
}

StmtPtr Parser::parse_func_decl() {
    Token fnTok = get(); // 'fn'
    Token id = peek();
    if (id.kind != TokenKind::Identifier) {
        Diagnostic d{Diagnostic::Severity::Error, id.loc, "expected function name after 'fn'", "PAR_FN001"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, fnTok.loc); }
        throw DiagnosticError(d);
    }
    string name = id.text; get();
    expect(TokenKind::LParen, "expected '(' after function name");
    vector<Param> params;
    if (!accept(TokenKind::RParen)) {
        while (true) {
            Token p = peek();
            if (p.kind != TokenKind::Identifier) {
                Diagnostic d{Diagnostic::Severity::Error, p.loc, "expected parameter name", "PAR_FN002"};
                if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, fnTok.loc); }
                throw DiagnosticError(d);
            }
            string pname = p.text; get();
            expect(TokenKind::Colon, "expected ':' after parameter name");
            Type ptype = parse_type();
            params.emplace_back(pname, ptype);
            if (accept(TokenKind::Comma)) continue;
            expect(TokenKind::RParen, "expected ')' after parameter list");
            break;
        }
    }
    optional<Type> retty;
    if (accept(TokenKind::Arrow)) {
        retty = parse_type();
    }
    StmtPtr body = parse_block();
    return make_unique<FuncDeclStmt>(name, params, retty, move(body), fnTok.loc);
}

StmtPtr Parser::parse_return() {
    Token t = get();
    if (peek().kind == TokenKind::Semicolon) {
        get();
        return make_unique<ReturnStmt>(nullptr, t.loc);
    } else {
        auto val = parse_expression();
        expect(TokenKind::Semicolon, "expected ';' after return");
        return make_unique<ReturnStmt>(move(val), t.loc);
    }
}

StmtPtr Parser::parse_block() {
    Token t = peek();
    if (!accept(TokenKind::LBrace)) {
        Diagnostic d{Diagnostic::Severity::Error, t.loc, "expected '{' to start block", "PAR013"};
        if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<BlockStmt>(vector<StmtPtr>{}, t.loc); }
        throw DiagnosticError(d);
    }
    vector<StmtPtr> stmts;
    while (peek().kind != TokenKind::RBrace && peek().kind != TokenKind::End) {
        stmts.push_back(parse_statement());
    }
    expect(TokenKind::RBrace, "expected '}' to close block");
    return make_unique<BlockStmt>(move(stmts), t.loc);
}

// Expression parsing (precedence)
ExprPtr Parser::parse_expression() { return parse_cast(); }

ExprPtr Parser::parse_cast() {
    auto left = parse_equality();
    while (peek().kind == TokenKind::KeywordAs) {
        Token asTok = get();
        Token t = peek();
        if (!is_type_name(t)) {
            Diagnostic d{Diagnostic::Severity::Error, t.loc, "expected type after 'as'", "PAR014"};
            if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<IntLiteral>(0, asTok.loc); }
            throw DiagnosticError(d);
        }
        Type target = parse_type();
        left = make_unique<CastExpr>(move(left), target, asTok.loc);
    }
    return left;
}

ExprPtr Parser::parse_equality() {
    auto left = parse_relational();
    while (peek().kind == TokenKind::EqualEqual || peek().kind == TokenKind::NotEqual) {
        Token op = get();
        auto right = parse_relational();
        BinOp bop = (op.kind == TokenKind::EqualEqual) ? BinOp::Eq : BinOp::Ne;
        left = make_unique<BinaryExpr>(bop, move(left), move(right), op.loc);
    }
    return left;
}

ExprPtr Parser::parse_relational() {
    auto left = parse_add();
    while (peek().kind == TokenKind::Less || peek().kind == TokenKind::Greater ||
           peek().kind == TokenKind::LessEqual || peek().kind == TokenKind::GreaterEqual) {
        Token op = get();
        auto right = parse_add();
        BinOp bop;
        if (op.kind == TokenKind::Less) bop = BinOp::Lt;
        else if (op.kind == TokenKind::Greater) bop = BinOp::Gt;
        else if (op.kind == TokenKind::LessEqual) bop = BinOp::Le;
        else bop = BinOp::Ge;
        left = make_unique<BinaryExpr>(bop, move(left), move(right), op.loc);
    }
    return left;
}

ExprPtr Parser::parse_add() {
    auto left = parse_mul();
    while (peek().kind == TokenKind::Plus || peek().kind == TokenKind::Minus) {
        Token op = get();
        auto right = parse_mul();
        BinOp bop = (op.kind == TokenKind::Plus) ? BinOp::Add : BinOp::Sub;
        left = make_unique<BinaryExpr>(bop, move(left), move(right), op.loc);
    }
    return left;
}

ExprPtr Parser::parse_mul() {
    auto left = parse_unary();
    while (peek().kind == TokenKind::Star || peek().kind == TokenKind::Slash) {
        Token op = get();
        auto right = parse_unary();
        BinOp bop = (op.kind == TokenKind::Star) ? BinOp::Mul : BinOp::Div;
        left = make_unique<BinaryExpr>(bop, move(left), move(right), op.loc);
    }
    return left;
}

ExprPtr Parser::parse_unary() {
    if (peek().kind == TokenKind::Minus) {
        Token op = get();
        auto inner = parse_unary();
        return make_unique<UnaryExpr>(UnOp::Neg, move(inner), op.loc);
    }
    return parse_primary();
}

ExprPtr Parser::parse_primary() {
    Token t = peek();
    if (t.kind == TokenKind::IntLiteral) { get(); return make_unique<IntLiteral>(stoll(t.text), t.loc); }
    if (t.kind == TokenKind::FloatLiteral) { get(); return make_unique<FloatLiteral>(stod(t.text), t.loc); }
    if (t.kind == TokenKind::CharLiteral) { get(); return make_unique<CharLiteral>(t.text.empty() ? 0 : t.text[0], t.loc); }
    if (t.kind == TokenKind::StringLiteral) { get(); return make_unique<StringLiteral>(t.text, t.loc); }
    if (t.kind == TokenKind::KeywordTrue) { get(); return make_unique<BoolLiteral>(true, t.loc); }
    if (t.kind == TokenKind::KeywordFalse) { get(); return make_unique<BoolLiteral>(false, t.loc); }
    if (t.kind == TokenKind::KeywordScan) { get(); return make_unique<ScanExpr>(t.loc); }
    if (t.kind == TokenKind::LParen) {
        // could be parenthesized expr or tuple literal
        get();
        ExprPtr first = parse_expression();
        if (peek().kind == TokenKind::Comma) {
            // tuple literal
            vector<ExprPtr> elems;
            elems.push_back(move(first));
            while (accept(TokenKind::Comma)) {
                elems.push_back(parse_expression());
            }
            expect(TokenKind::RParen, "expected ')'");
            return make_unique<TupleLiteralExpr>(move(elems), t.loc);
        } else {
            expect(TokenKind::RParen, "expected ')'");
            return first;
        }
    }
    if (t.kind == TokenKind::LBracket) {
        // array literal
        get();
        vector<ExprPtr> elems;
        if (peek().kind != TokenKind::RBracket) {
            while (true) {
                elems.push_back(parse_expression());
                if (accept(TokenKind::Comma)) continue;
                break;
            }
        }
        expect(TokenKind::RBracket, "expected ']'");
        return make_unique<ArrayLiteralExpr>(move(elems), t.loc);
    }
    if (t.kind == TokenKind::LBrace) {
        // map literal: accept string literal keys for simplicity: {"a": expr, ...}
        get();
        vector<pair<ExprPtr,ExprPtr>> entries;
        if (peek().kind != TokenKind::RBrace) {
            while (true) {
                Token keytok = peek();
                ExprPtr keyexpr;
                if (keytok.kind == TokenKind::StringLiteral) { get(); keyexpr = make_unique<StringLiteral>(keytok.text, keytok.loc); }
                else { // allow general expression keys too
                    keyexpr = parse_expression();
                }
                expect(TokenKind::Colon, "expected ':' in map literal");
                ExprPtr val = parse_expression();
                entries.emplace_back(move(keyexpr), move(val));
                if (accept(TokenKind::Comma)) continue;
                break;
            }
        }
        expect(TokenKind::RBrace, "expected '}'");
        return make_unique<MapLiteralExpr>(move(entries), t.loc);
    }
    if (t.kind == TokenKind::Identifier) {
        get();
        ExprPtr base = make_unique<VarExpr>(t.text, t.loc);
        while (true) {
            if (peek().kind == TokenKind::LParen) {
                get();
                vector<ExprPtr> args;
                if (peek().kind != TokenKind::RParen) {
                    while (true) {
                        args.push_back(parse_expression());
                        if (accept(TokenKind::Comma)) continue;
                        break;
                    }
                }
                expect(TokenKind::RParen, "expected ')' in call");
                base = make_unique<CallExpr>(move(base), move(args), t.loc);
                continue;
            }
            if (peek().kind == TokenKind::LBracket) {
                get();
                auto idx = parse_expression();
                expect(TokenKind::RBracket, "expected ']'");
                base = make_unique<IndexExpr>(move(base), move(idx), t.loc);
                continue;
            }
            if (peek().kind == TokenKind::Dot) {
                get();
                Token nameTok = peek();
                if (nameTok.kind != TokenKind::Identifier) {
                    Diagnostic d{Diagnostic::Severity::Error, nameTok.loc, "expected identifier after '.'", "PAR016"};
                    if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<IntLiteral>(0, t.loc); }
                    throw DiagnosticError(d);
                }
                get();
                base = make_unique<MemberExpr>(move(base), nameTok.text, nameTok.loc);
                continue;
            }
            break;
        }
        return base;
    }
    Diagnostic d{Diagnostic::Severity::Error, t.loc, "unexpected token in expression", "PAR015"};
    if (diag_) { diag_->report(d); recover_to_sync(); return make_unique<IntLiteral>(0, t.loc); }
    throw DiagnosticError(d);
}

void Parser::recover_to_sync() {
    while (peek().kind != TokenKind::Semicolon && peek().kind != TokenKind::RBrace && peek().kind != TokenKind::End) get();
    if (peek().kind == TokenKind::Semicolon) get();
}
