// NOLINTBEGIN(*-redundant-expression, *-branch-clone)

#include "style.hpp"

#include "bpp/resources.hpp"

auto bppui::load_default_style(const default_style_custom custom) -> style {
    const auto &[light, accent, radius, radius_diff] = custom;

    style style;

    style.fonts.regular            = LoadFontEx(bpp::res::get_satoshi_regular(), 64, nullptr, 0);
    style.fonts.bold               = LoadFontEx(bpp::res::get_satoshi_bold(), 64, nullptr, 0);
    style.fonts.italic             = LoadFontEx(bpp::res::get_satoshi_italic(), 64, nullptr, 0);
    style.fonts.bold_italic        = LoadFontEx(bpp::res::get_satoshi_bolditalic(), 64, nullptr, 0);

    style.small_text.font_size     = 14.0F;
    style.small_text.spacing       = 1.0F;
    style.small_text.line_spacing  = 8;

    style.medium_text.font_size    = 18.0F;
    style.medium_text.spacing      = 1.0F;
    style.medium_text.line_spacing = 4;

    style.large_text.font_size     = 22.0F;
    style.large_text.spacing       = 1.0F;
    style.large_text.line_spacing  = 2;

    style.small_icon               = 14.0F;
    style.medium_icon              = 18.0F;
    style.large_icon               = 22.0F;

    style.background_color         = hsl{accent, 0.0F, light ? 0.875F : 0.05F, 1.0F};
    style.foreground_color         = hsl{accent, 1.0F, light ? 0.0F : 1.0F, 1.0F};
    style.active_color             = hsl{accent, 1.0F, 1.0F, 1.0F};
    style.accent_color             = hsl{accent, 1.0F, 0.5F, 1.0F};
    style.success_color            = hsl{120.0F, 1.0F, 0.5F, 1.0F};
    style.info_color               = hsl{240.0F, 1.0F, 0.5F, 1.0F};
    style.warning_color            = hsl{60.0F, 1.0F, 0.5F, 1.0F};
    style.error_color              = hsl{0.0F, 1.0F, 0.5F, 1.0F};

    style.base                     = style_element::zeroes();
    style.base.background_color    = hsl{accent, 0.0F, light ? 0.95F : 0.125F, 1.0F};
    style.base.corner_radius       = radius;

    style.hover_offset             = style_element::zeroes();
    style.hover_offset.background_color.set_lightness(light ? -0.1F : 0.1F);
    style.hover_offset.border_color.set_lightness(light ? -0.1F : 0.1F);

    style.hover_scale = style_element::ones();

    style.held_offset = style_element::zeroes();
    style.held_offset.background_color.set_lightness(light ? -0.1F : -0.2F);
    style.held_offset.border_color.set_lightness(light ? -0.1F : -0.2F);

    style.held_scale                      = style_element::ones();

    style.focused_offset                  = style_element::zeroes();
    style.focused_offset.border_color     = hsl{0.0F, 0.0F, light ? 0.0F : 1.0F, 1.0F};
    style.focused_offset.border_thickness = 2.0F;

    style.focused_scale                   = style_element::ones();
    style.focused_scale.border_color      = hsl{0.0F, 0.0F, 0.0F, 0.0F};
    style.focused_scale.border_thickness  = 0.0F;

    style.disabled_offset                 = style_element::zeroes();
    style.disabled_offset.background_color.set_lightness(light ? 0.2F : -0.2F);
    style.disabled_offset.border_color.set_lightness(light ? 0.2F : -0.2F);

    style.disabled_scale = style_element::ones();
    style.disabled_scale.background_color.set_saturation(0.0F);
    style.disabled_scale.border_color.set_saturation(0.0F);

    style.bordered_offset                  = style_element::zeroes();
    style.bordered_offset.border_color     = hsl{accent, 0.2F, light ? 0.7F : 0.3F, 1.0F};
    style.bordered_offset.border_thickness = 1.0F;

    style.bordered_scale                   = style_element::ones();

    style.shadowed_offset                  = style_element::zeroes();
    style.shadowed_offset.shadow_color     = hsl{accent, 0.0F, 0.0F, 0.05F};
    style.shadowed_offset.shadow_radius    = 10.0F;
    style.shadowed_offset.shadow_shrink    = 8.0F;
    style.shadowed_offset.shadow_offset    = {0.0F, 6.0F};

    style.shadowed_scale                   = style_element::ones();

    style.layered_offset                  = style_element::zeroes();
    style.layered_offset.shadow_color     = hsl{accent, 0.0F, 0.0F, 0.05F};
    style.layered_offset.shadow_radius    = 10.0F;
    style.layered_offset.shadow_shrink    = 8.0F;
    style.layered_offset.shadow_offset    = {0.0F, 6.0F};

    style.layered_scale                   = style_element::ones();

    return style;
}

auto bppui::unload_style(const style &style) -> void {
    UnloadFont(style.fonts.regular);
    UnloadFont(style.fonts.bold);
    UnloadFont(style.fonts.italic);
    UnloadFont(style.fonts.bold_italic);
}

// NOLINTEND(*-redundant-expression, *-branch-clone)
