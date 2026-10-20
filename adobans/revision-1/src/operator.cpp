#include "operator.hpp"
#include <memory>
#include <optional>
#include <ranges>
#include <variant>

using namespace adobans;

void adobans::operator_config::register_highest(const infix_operator op) {
    precedences.emplace(precedences.end(), std::vector{op});
}

void adobans::operator_config::register_highest(const prefix_operator op) {
    precedences.emplace(precedences.end(), std::vector{op});
}

void adobans::operator_config::register_highest(const postfix_operator op) {
    precedences.emplace(precedences.end(), std::vector{op});
}

void adobans::operator_config::register_lowest(const infix_operator op) {
    precedences.emplace(precedences.begin(), std::vector{op});
}

void adobans::operator_config::register_lowest(const prefix_operator op) {
    precedences.emplace(precedences.begin(), std::vector{op});
}

void adobans::operator_config::register_lowest(const postfix_operator op) {
    precedences.emplace(precedences.begin(), std::vector{op});
}

void adobans::operator_config::register_higher_level(const infix_operator op, std::string_view relative) {
}

void adobans::operator_config::register_higher_level(const prefix_operator op, std::string_view relative) {
}

void adobans::operator_config::register_higher_level(const postfix_operator op, std::string_view relative) {
}

void adobans::operator_config::register_lower_level(const infix_operator op, std::string_view relative) {}
void adobans::operator_config::register_lower_level(const prefix_operator op, std::string_view relative) {}
void adobans::operator_config::register_lower_level(const postfix_operator op, std::string_view relative) {}
void adobans::operator_config::register_equal_level(const infix_operator op, std::string_view relative) {}
void adobans::operator_config::register_equal_level(const prefix_operator op, std::string_view relative) {}
void adobans::operator_config::register_equal_level(const postfix_operator op, std::string_view relative) {}

std::optional<infix_operator> adobans::operator_config::get_infix_info(std::string_view op_name) {
    for (std::size_t i = 0; i < precedences.size(); i++) {
        const auto &precedence = precedences[i];
    }
}

std::optional<prefix_operator> adobans::operator_config::get_prefix_info(std::string_view op_name) {}

std::optional<postfix_operator> adobans::operator_config::get_postfix_info(std::string_view op_name) {}

std::unique_ptr<operator_config> adobans::default_op_config() {
    operator_config op_config;

    op_config.register_first(operator_type("addition", "+", [](int value) -> int { return value; }), infix_operator(operator_assoc::left));
    op_config.register_equal_level(operator_type("subtraction", "-", [](int value) -> int { return value; }), "addition");

    op_config.register_higher_level(operator_type("multiplication", "*", [](int value) -> int { return value; }), infix_operator(operator_assoc::left), "addition");
    op_config.register_equal_level(operator_type("division", "/", [](int value) -> int { return value; }), "multiplication");

    op_config.register_higher_level(operator_type("exponentiation", "^", [](int value) -> int { return value; }), infix_operator(operator_assoc::right), "multiplication");

    op_config.register_higher_level(operator_type("negation", "-", [](int value) -> int { return value; }), prefix_operator(), "exponentiation");
    op_config.register_higher_level(operator_type("factorial", "!", [](int value) -> int { return value; }), postfix_operator(), "negation");

    return std::make_unique<operator_config>(op_config);
}
