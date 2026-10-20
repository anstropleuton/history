#include "utils.hpp"

#include "../deps/nanosvg/src/nanosvgrast.h"
#include "nanosvg.h"

#include <algorithm>
#include <cmath>

std::vector<Rectangle> clips;

auto color_from_hsl(float hue, float sat, float lit, float alpha) -> Color {
    hue   = std::fmod(std::fmod(hue, 360.0F) + 360.0F, 360.0F);
    sat   = std::clamp(sat, 0.0F, 1.0F);
    lit   = std::clamp(lit, 0.0F, 1.0F);
    alpha = std::clamp(alpha, 0.0F, 1.0F);

    const float chroma{(1.0F - std::abs(2.0F * lit - 1.0F)) * sat};
    const float sec{hue / 60.0F};
    const float comp{chroma * (1.0F - std::abs(std::fmod(sec, 2.0F) - 1.0F))};

    float       red{};
    float       green{};
    float       blue{};

    if (sec >= 0.0F && sec < 1.0F) {
        red   = chroma;
        green = comp;
    } else if (sec >= 1.0F && sec < 2.0F) {
        red   = comp;
        green = chroma;
    } else if (sec >= 2.0F && sec < 3.0F) {
        green = chroma;
        blue  = comp;
    } else if (sec >= 3.0F && sec < 4.0F) {
        green = comp;
        blue  = chroma;
    } else if (sec >= 4.0F && sec < 5.0F) {
        red  = comp;
        blue = chroma;
    } else {
        red  = chroma;
        blue = comp;
    }

    const float offset{lit - chroma * 0.5F};

    return {
        static_cast<unsigned char>((red + offset) * 255.0F),
        static_cast<unsigned char>((green + offset) * 255.0F),
        static_cast<unsigned char>((blue + offset) * 255.0F),
        static_cast<unsigned char>(alpha * 255.0F),
    };
}

auto color_to_hsl(const Color color) -> Vector4 {
    const float red{static_cast<float>(color.r) / 255.0F};
    const float green{static_cast<float>(color.g) / 255.0F};
    const float blue{static_cast<float>(color.b) / 255.0F};
    const float alpha{static_cast<float>(color.a) / 255.0F};

    const float max{std::max({red, green, blue})};
    const float min{std::min({red, green, blue})};
    const float delta{max - min};

    float       hue{};
    float       sat{};
    const float lit{(max + min) * 0.5F};

    if (delta > 0.0F) {
        if (max == red) {
            hue = 60.0F * std::fmod((green - blue) / delta, 6.0F);
        } else if (max == green) {
            hue = 60.0F * ((blue - red) / delta + 2.0F);
        } else {
            hue = 60.0F * ((red - green) / delta + 4.0F);
        }

        if (hue < 0.0F) {
            hue += 360.0F;
        }

        sat = delta / (1.0F - std::abs(2.0F * lit - 1.0F));
    }

    return {hue, sat, lit, alpha};
}

auto intersect_rec(const Rectangle &first, const Rectangle &second) -> Rectangle {
    const float left{std::max(first.x, second.x)};
    const float top{std::max(first.y, second.y)};
    const float right{std::min(first.x + first.width, second.x + second.width)};
    const float bottom{std::min(first.y + first.height, second.y + second.height)};

    if (left >= right || top >= bottom) {
        return Rectangle{0.0F, 0.0F, 0.0F, 0.0F};
    }

    return Rectangle{left, top, right - left, bottom - top};
}

auto union_rec(const Rectangle &first, const Rectangle &second) -> Rectangle {
    const float left{std::min(first.x, second.x)};
    const float top{std::min(first.y, second.y)};
    const float right{std::max(first.x + first.width, second.x + second.width)};
    const float bottom{std::max(first.y + first.height, second.y + second.height)};

    return Rectangle{left, top, right - left, bottom - top};
}

auto draw_rec_rounded(const Rectangle rec, const float radius, const Color color) -> void {
    const float min_size{std::min(rec.width, rec.height)};
    if (min_size <= 0.0F) {
        return;
    }

    const float roundness{radius * 2.0F / min_size};

    DrawRectangleRounded(rec, roundness, 0, color);
}

auto draw_rec_rounded_lines(const Rectangle rec, const float radius, const float thickness, const Color color) -> void {
    const float min_size{std::min(rec.width, rec.height)};
    if (min_size <= 0.0F) {
        return;
    }

    const float roundness{radius * 2.0F / min_size};

    DrawRectangleRoundedLinesEx(rec, roundness, 0, thickness, color);
}

auto draw_rec_shadow(const Rectangle rec, const float radius, const float blur, const float shrink,
    const Vector2 offset, const Color color) -> void {
    if (blur <= 0.0F || color.a == 0) {
        return;
    }

    const float       layer_count_f{std::clamp((blur + Vector2Length(offset)) * 0.55F, 4.0F, 16.0F)};
    const std::size_t layer_count{static_cast<std::size_t>(layer_count_f)};
    const float       step{blur / layer_count_f};

    for (std::size_t index{}; index < layer_count; ++index) {
        const float     current{static_cast<float>(index) + 1.0F};
        const float     ratio{current / layer_count_f};
        const float     falloff{1.0F - ratio * ratio * 0.55F};
        const float     expand{step * current - shrink};

        const Rectangle layer_rec{rec.x - expand + offset.x * ratio, rec.y - expand + offset.y * ratio,
            rec.width + expand * 2.0F, rec.height + expand * 2.0F};

        Color           shadow_color{color};
        shadow_color.a = static_cast<unsigned char>(static_cast<float>(shadow_color.a) * falloff);

        draw_rec_rounded(layer_rec, radius + expand, shadow_color);
    }
}

auto load_svg(const std::string &filename, int width, int height) -> Texture {
    NSVGimage *const image{nsvgParseFromFile(filename.c_str(), "px", 96.0F)};
    if (image == nullptr) {
        return Texture{};
    }

    const int image_width{static_cast<int>(std::ceil(image->width))};
    const int image_height{static_cast<int>(std::ceil(image->height))};

    if (width == 0) width = image_width;
    if (height == 0) height = image_height;

    unsigned char *const image_data{new unsigned char[static_cast<std::size_t>(width * height * 4)]{}};

    const float          scale_width{static_cast<float>(width) / image->width};
    const float          scale_height{static_cast<float>(height) / image->height};

    const float          scale{scale_width < scale_height ? scale_width : scale_height};

    float                offset_x{};
    float                offset_y{};

    if (scale_height > scale_width) offset_y = (static_cast<float>(height) - image->height * scale) / 2.0F;
    else offset_x = (static_cast<float>(width) - image->width * scale) / 2.0F;

    NSVGrasterizer *const rasterizer{nsvgCreateRasterizer()};
    if (rasterizer == nullptr) {
        nsvgDelete(image);
        delete[] image_data;
        return Texture{};
    }

    nsvgRasterize(rasterizer, image, offset_x, offset_y, scale, image_data, width, height, width * 4);
    nsvgDelete(image);
    nsvgDeleteRasterizer(rasterizer);

    const Image   rl_image{image_data, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    const Texture texture{LoadTextureFromImage(rl_image)};
    delete[] image_data;

    return texture;
}
