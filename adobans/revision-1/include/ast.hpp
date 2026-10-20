#pragma once

#include "operator.hpp"
#include "token.hpp"
#include <memory>
#include <vector>

namespace adobans {
    struct ast_node {
        text_range range;
        ast_node(text_range range) : range(range) {}
        virtual ~ast_node() = default;

        virtual int evaluate() = 0;
    };

    struct prefix_op_node : ast_node {
        operator_info             op_info;
        std::unique_ptr<ast_node> right;

        prefix_op_node(operator_info op, std::unique_ptr<ast_node> right, text_range range) : ast_node(range), op_info(op), right(std::move(right)) {}

        int evaluate() override {
        }
    };

    inline std::unique_ptr<ast_node> make_prefix_op(operator_info op, std::unique_ptr<ast_node> right, text_range range) {
        return std::make_unique<prefix_op_node>(op, std::move(right), range);
    }

    struct postfix_op_node : ast_node {
        operator_info             op_info;
        std::unique_ptr<ast_node> left;

        postfix_op_node(operator_info op, std::unique_ptr<ast_node> left, text_range range) : ast_node(range), op_info(op), left(std::move(left)) {}

        int evaluate() override {
        }
    };

    inline std::unique_ptr<ast_node> make_postfix_op(operator_info op, std::unique_ptr<ast_node> right, text_range range) {
        return std::make_unique<postfix_op_node>(op, std::move(right), range);
    }

    struct infix_op_node : ast_node {
        operator_info             op_info;
        std::unique_ptr<ast_node> left;
        std::unique_ptr<ast_node> right;

        infix_op_node(operator_info op, std::unique_ptr<ast_node> left, std::unique_ptr<ast_node> right, text_range range) : ast_node(range), op_info(op), left(std::move(left)), right(std::move(right)) {}

        int evaluate() override {
        }
    };

    inline std::unique_ptr<ast_node> make_infix_op(operator_info op, std::unique_ptr<ast_node> left, std::unique_ptr<ast_node> right, text_range range) {
        return std::make_unique<infix_op_node>(op, std::move(left), std::move(right), range);
    }

    struct int_node : ast_node {
        int value;

        int_node(int value, text_range range) : ast_node(range), value(value) {}

        int evaluate() override {
        }
    };

    inline std::unique_ptr<ast_node> make_int(int value, text_range range) {
        return std::make_unique<int_node>(value, range);
    }

    struct parser_result {
        std::unique_ptr<ast_node> ast;
        std::vector<report>       reports;
    };

    parser_result parse(const std::vector<token> &tokens, const std::unique_ptr<operator_config> &op_config);
}
