#pragma once

#include <sstream>
#include <iostream>
#include <stdexcept>

#include "lexer.h"
#include "parser.h"
#include "typechecker.h"
#include "interpreter.h"
#include "diag.h"

// Helper RAII to restore rdbufs even if exceptions occur
struct StreamRedirect {
    std::streambuf *oldCin;
    std::streambuf *oldCout;
    std::stringstream in;
    std::stringstream out;
    StreamRedirect() : oldCin(nullptr), oldCout(nullptr) {}
    void start(const std::string &input = "") {
        in.str(input);
        oldCin = std::cin.rdbuf(in.rdbuf());
        oldCout = std::cout.rdbuf(out.rdbuf());
    }
    void stop() {
        if (oldCin) std::cin.rdbuf(oldCin);
        if (oldCout) std::cout.rdbuf(oldCout);
    }
    std::string get_output() const { return out.str(); }
};

// Runs the language pipeline on the source. If a DiagnosticError is thrown it will propagate.
// (Tests can use CHECK_THROWS_AS to assert errors.)
inline std::string run_program_capture(const std::string &source, const std::string &stdin_input = "") {
    StreamRedirect rd;
    rd.start(stdin_input);
    try {
        Lexer lx(source);
        auto toks = lx.tokenize();
        Parser p(toks);
        auto prog = p.parse_program();

        TypeChecker tc;
        tc.type_check_program(prog);

        Interpreter interp;
        interp.run_program(prog);

        std::string out = rd.get_output();
        rd.stop();
        return out;
    } catch (...) {
        rd.stop();
        throw;
    }
}
