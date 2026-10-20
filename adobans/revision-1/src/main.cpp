#include <iostream>
#include <memory>
#include <print>

#include "adobans.hpp"
#include "ast.hpp"
#include "operator.hpp"

using namespace adobans;

void print_ast(ast_node *node, int level = 0) {
    std::string indent(level, ' ');
    if (!node) {
        std::println("{}Null", indent);
        return;
    }

    std::print("{}AST Node {}+{}: ", indent, node->range.begin, node->range.length);
    if (auto ptr = dynamic_cast<int_node *>(node)) {
        std::println("Int node: {}", ptr->value);
    } else if (auto ptr = dynamic_cast<prefix_op_node *>(node)) {
        std::println("Prefix operator node: {} ({})", ptr->op_info.op_type.op_symbol, ptr->op_info.op_type.name);
        print_ast(ptr->right.get(), level + 1);
    } else if (auto ptr = dynamic_cast<postfix_op_node *>(node)) {
        std::println("Postfix operator node: {} ({})", ptr->op_info.op_type.op_symbol, ptr->op_info.op_type.name);
        print_ast(ptr->left.get(), level + 1);
    } else if (auto ptr = dynamic_cast<infix_op_node *>(node)) {
        std::println("Infix operator node: {} ({})", ptr->op_info.op_type.op_symbol, ptr->op_info.op_type.name);
        print_ast(ptr->left.get(), level + 1);
        print_ast(ptr->right.get(), level + 1);
    } else {
        std::println("Unknown Node");
    }
}

void print_report(const report &report) {
    std::string name;
    switch (report.sev) {
    case severity::note: name = "note"; break;
    case severity::warning: name = "warning"; break;
    case severity::error: name = "error"; break;
    case severity::help: name = "help"; break;
    }

    std::println("Report {}+{}: {}: {}", report.range.begin, report.range.length, name, report.message);
}

int main() {
    while (true) {
        std::print("> ");
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        auto code = std::make_shared<source_code>(source_code{
            .name    = "untitled.adb",
            .content = input,
        });

        std::println("Tokenizing...");

        auto tokenizer = tokenize(code);

        for (auto report : tokenizer.reports) {
            print_report(report);
        }

        if (!tokenizer.reports.empty()) {
            continue;
        }

        std::println("Tokens:");
        for (auto tok : tokenizer.tokens) {
            std::string name;
            switch (tok.type) {
            case token_type::int_lit: name = "int_lit"; break;
            case token_type::float_lit: name = "float_lit"; break;
            case token_type::string_lit: name = "string_lit"; break;
            case token_type::char_lit: name = "char_lit"; break;
            case token_type::bool_lit: name = "bool_lit"; break;
            case token_type::identifier: name = "identifier"; break;
            case token_type::delimiter: name = "delimiter"; break;
            case token_type::keyword: name = "keyword"; break;
            }

            std::visit(
                [&](const auto &v) {
                    std::println("Token {}+{}: {} ({})", tok.range.begin, tok.range.length, v, name);
                },
                tok.value
            );
        }

        std::println("Parsing...");

        auto parser = parse(tokenizer.tokens, default_op_config());

        for (auto report : parser.reports) {
            print_report(report);
        }

        if (!parser.reports.empty()) {
            continue;
        }

        std::println("AST:");
        print_ast(parser.ast.get());

        std::println("Evaluating...");
    }
}
