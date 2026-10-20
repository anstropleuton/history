#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace adobans {
    enum class operator_assoc {
        left,
        right
    };

    struct infix_operator {
        std::string                  name;
        std::string                  op_symbol;
        operator_assoc               assoc = operator_assoc::left;
        std::function<int(int, int)> operate;
        infix_operator(const std::string &name, const std::string &op_symbol, const std::function<int(int, int)> &operate, operator_assoc assoc) : name(name), op_symbol(op_symbol), operate(operate), assoc(assoc) {}
    };

    struct prefix_operator {
        std::string             name;
        std::string             op_symbol;
        std::function<int(int)> operate;
        prefix_operator(const std::string &name, const std::string &op_symbol, const std::function<int(int)> &operate) : name(name), op_symbol(op_symbol), operate(operate) {}
    };

    struct postfix_operator {
        std::string             name;
        std::string             op_symbol;
        std::function<int(int)> operate;
        postfix_operator(const std::string &name, const std::string &op_symbol, const std::function<int(int)> &operate) : name(name), op_symbol(op_symbol), operate(operate) {}
    };

    /// This is Rust addiction: You force your types to reflect correctness instead of enforcing constraints in the runtime
    /// In this case, a precedence MUST have all operators be of same kind
    using precedence = std::variant<
        std::vector<infix_operator>,
        std::vector<prefix_operator>,
        std::vector<postfix_operator>>;

    struct operator_config {
        /// Precedence in order of index (higher index == higher precedence)
        std::vector<precedence> precedences;

        void register_highest(const infix_operator op);
        void register_highest(const prefix_operator op);
        void register_highest(const postfix_operator op);
        void register_lowest(const infix_operator op);
        void register_lowest(const prefix_operator op);
        void register_lowest(const postfix_operator op);
        void register_higher_level(const infix_operator op, std::string_view relative);
        void register_higher_level(const prefix_operator op, std::string_view relative);
        void register_higher_level(const postfix_operator op, std::string_view relative);
        void register_lower_level(const infix_operator op, std::string_view relative);
        void register_lower_level(const prefix_operator op, std::string_view relative);
        void register_lower_level(const postfix_operator op, std::string_view relative);
        void register_equal_level(const infix_operator op, std::string_view relative);
        void register_equal_level(const prefix_operator op, std::string_view relative);
        void register_equal_level(const postfix_operator op, std::string_view relative);

        std::optional<infix_operator>   get_infix_info(std::string_view op_name);
        std::optional<prefix_operator>  get_prefix_info(std::string_view op_name);
        std::optional<postfix_operator> get_postfix_info(std::string_view op_name);
    };

    std::unique_ptr<operator_config> default_op_config();
}
