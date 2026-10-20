#include "lexer.h"
#include "parser.h"
#include "typechecker.h"
#include "interpreter.h"
#include "diag.h"
#include "ast_printer.h"
#include <fstream>
#include <ios>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    bool dump_ast = false;
    bool trace = false;
    std::string infile;
    for (int i=1;i<argc;++i) {
        std::string a = argv[i];
        if (a == "--dump-ast") dump_ast = true;
        else if (a == "--trace") trace = true;
        else infile = a;
    }

    std::string source;
    if (!infile.empty()) {
        std::ifstream ifs(infile);
        if (!ifs) { std::cerr<<"could not open file\n"; return 1; }
        std::stringstream ss; ss << ifs.rdbuf(); source = ss.str();
    } else {
        std::stringstream ss; ss << std::cin.rdbuf(); source = ss.str();
    }

    Diagnostics diag; // collector
    try {
        Lexer lx(source, &diag);
        auto toks = lx.tokenize();
        if (trace) {
            std::cerr << "Tokens:\n";
            for (auto &t : toks) std::cerr << t.text << " (" << (int)t.kind << ") at " << t.loc.to_string() << "\n";
        }

        Parser p(toks, &diag);
        auto prog = p.parse_program();

        TypeChecker tc(&diag);
        tc.type_check_program(prog);

        if (dump_ast) {
            ASTPrinter ap(std::cout, &tc);
            ap.print_program(prog);
        }

        if (diag.has_errors()) {
            diag.print_all(std::cerr);
            return 1;
        }

        Interpreter interp(&diag);
        interp.run_program(prog);
    } catch (const DiagnosticError &de) {
        std::cerr << de.diag.to_string() << "\n";
        return 2;
    } catch (const std::exception &ex) {
        std::cerr << "fatal: " << ex.what() << "\n";
        return 3;
    }
    return 0;
}
