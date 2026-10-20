#include <doctest/doctest.h>
#include "lexer.h"
#include "parser.h"
#include "typechecker.h"
#include "diag.h"

// small helper to run pipeline and return diagnostics
static Diagnostics run_and_collect(const std::string &src) {
    Diagnostics diag;
    Lexer lx(src, &diag);
    auto toks = lx.tokenize();
    Parser p(toks, &diag);
    auto prog = p.parse_program();
    TypeChecker tc(&diag);
    tc.type_check_program(prog);
    return diag;
}

TEST_CASE("type error collected for bad initializer") {
    auto d = run_and_collect("let x: int = 2.5; print x;");
    CHECK(d.has_errors());
}
