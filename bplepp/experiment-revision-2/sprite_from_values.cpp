#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "raylib.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace stdr = std::ranges;
namespace stdv = stdr::views;

auto trim(std::string_view str) -> std::string
{
    std::string chars(128, '\0');
    stdr::generate_n(chars.begin(), 128, [n { char { 0 } }] mutable { return n++; });
    std::string spaces {};
    stdr::copy_if(chars, std::back_inserter(spaces), [](auto &&c) { return std::isspace(c); });

    auto first { str.find_first_not_of(spaces) };
    auto last { str.find_last_not_of(spaces) };
    return first != std::string::npos && last != std::string::npos && first >= last ? "" : std::string(str.substr(first, last - first + 1));
}

int main()
{
    constexpr auto linesOfDataCount { 8zu };
    std::array<std::string, linesOfDataCount> linesOfData {};

    constexpr auto linesOfComponentCount { 2zu };
    std::array<std::string, linesOfComponentCount> linesOfComponent {};

    std::println("Dump lines of sprite data's from selection to uv rectangle and then lines of game object sprite component's scale and pivot:");
    for (auto i : stdv::iota(0zu, linesOfDataCount)) std::getline(std::cin, linesOfData[i]);
    for (auto i : stdv::iota(0zu, linesOfComponentCount)) std::getline(std::cin, linesOfComponent[i]);

    auto &&transformer { [](auto &&line) {
        auto first { line.find('(') };
        auto last { line.find(')') };
        auto trimmed { line.substr(first + 1, last - first - 1) };
        return stdv::split(trimmed, ", "s) | stdv::transform([](auto &&subrange) { return stdr::to<std::string>(subrange); }) | stdr::to<std::vector>();
    } };

    auto transformedData { stdv::transform(linesOfData, transformer) | stdr::to<std::vector>() };
    auto transformedComponent { stdv::transform(linesOfComponent, transformer) | stdr::to<std::vector>() };

    auto &&td { transformedData };
    auto &&tc { transformedComponent };

    // Rovio stuff

    struct SpriteData {
        int selectionX;
        int selectionY;
        int selectionWidth;
        int selectionHeight;
        int pivotX;
        int pivotY;
        int UVx;
        int UVy;
        int width;
        int height;
        Rectangle uv;
    };

    SpriteData data {
        std::stoi(td[0][0]),
        std::stoi(td[0][1]),
        std::stoi(td[0][2]),
        std::stoi(td[0][3]),
        std::stoi(td[1][0]),
        std::stoi(td[1][1]),
        std::stoi(td[2][0]),
        std::stoi(td[2][1]),
        std::stoi(td[3][0]),
        std::stoi(td[3][1]),
        { std::stof(td[7][0]),
          std::stof(td[7][1]),
          std::stof(td[7][2]),
          std::stof(td[7][3]) }
    };

    float m_scaleX { std::stof(tc[0][0]) };
    float m_scaleY { std::stof(tc[0][1]) };
    float m_pivotX { std::stof(tc[1][0]) };
    float m_pivotY { std::stof(tc[1][1]) };

    int num = (int)(m_scaleX * (float)data.width);
    int num2 = (int)(m_scaleY * (float)data.height);
    int num3 = data.selectionX + data.selectionWidth / 2;
    int num4 = data.selectionY + data.selectionHeight / 2;
    int num5 = data.UVx + data.width / 2;
    int num6 = data.UVy + data.height / 2;
    int num7 = num3 - num5;
    int num8 = num4 - num6;
    int num9 = (int)(m_scaleX * (float)(num7 + data.pivotX + m_pivotX));
    int num10 = (int)(m_scaleY * (float)(num8 + data.pivotY + m_pivotY));

    float num11 = (float)num * 10.0f / 768.0f;
    float num12 = (float)num2 * 10.0f / 768.0f;
    float num13 = -2.0f * (float)num9 * 10.0f / 768.0f;
    float num14 = -2.0f * (float)num10 * 10.0f / 768.0f;

    Vector3 vertices[] = {
        Vector3 { num13 - num11, num14 - num12, 0.0f },
        Vector3 { num13 - num11, num14 + num12, 0.0f },
        Vector3 { num13 + num11, num14 + num12, 0.0f },
        Vector3 { num13 + num11, num14 - num12, 0.0f }
    };

    Rectangle dest = {
        vertices[0].x, -vertices[2].y,
        vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y
    };

    data.uv.x *= 2048;
    data.uv.y *= 2048;
    data.uv.width *= 2048;
    data.uv.height *= 2048;
    data.uv.y = data.height - data.uv.y - data.uv.height;

    std::println("Rectangle /* src */ {{ {}.0f, {}.0f, {}.0f, {}.0f }},", std::round(data.uv.x), std::round(data.uv.y), std::round(data.uv.width), std::round(data.uv.height));
    std::println("Rectangle /* dest */ {{ 0.0f, 0.0f, {:.6f}f, {:.6f}f }},", dest.width, dest.height);
    std::println("Vector2 /* origin */ {{ {:.6f}f, {:.6f}f }}", -dest.x, -dest.y);
}
