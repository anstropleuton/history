#include <algorithm>
#include <array>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

auto trim(std::string_view str) -> std::string
{
    auto start = str.find_first_not_of(' ');
    auto end = str.find_last_not_of(' ');
    return (start >= end ? ""s : std::string { str.substr(start, end - start) });
}

int main()
{
    constexpr std::size_t lines_count { 4 };
    std::println("Enter unity recalculated mesh vertices ({} lines):", lines_count);
    auto lines = std::array<std::string, lines_count> {};
    for (std::size_t i : stdv::iota(0zu, lines_count))
    {
        std::getline(std::cin, lines[i]);
    }

    stdr::for_each(lines, [](auto &&line) {
        auto open { line.find('(') + 1 };
        auto close { line.find(')') };
        auto data { line.substr(open, close - open) };
        auto values { stdv::split(data, ',') | stdv::transform([](auto &&subrange) {
                          std::string value { trim(stdr::to<std::string>(subrange)) };
                          return std::stof(value);
                      }) |
                      stdr::to<std::vector>() };
        std::println("{{ {:.6f}f, {:.6f}f }}", values[0], values[1]);
    });
}