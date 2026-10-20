// NOLINTBEGIN(*-redundant-expression, *-branch-clone)

#include "style.hpp"

#include "resources.hpp"

auto bppui::load_default_style(const default_style_custom custom) -> style {
    const auto &[light, accent, radius, radius_diff] = custom;

    style style;

    style.fonts.regular                  = LoadFontEx(bpp::res::fonts::satoshi_regular_ttf, 64, nullptr, 0);
    style.fonts.bold                     = LoadFontEx(bpp::res::fonts::satoshi_bold_ttf, 64, nullptr, 0);
    style.fonts.italic                   = LoadFontEx(bpp::res::fonts::satoshi_italic_ttf, 64, nullptr, 0);
    style.fonts.bold_italic              = LoadFontEx(bpp::res::fonts::satoshi_bolditalic_ttf, 64, nullptr, 0);

    style.icons[icon_type::home]         = load_svg(bpp::res::icons::house_svg, 64, 64);
    style.icons[icon_type::search]       = load_svg(bpp::res::icons::search_svg, 64, 64);
    style.icons[icon_type::menu]         = load_svg(bpp::res::icons::menu_svg, 64, 64);
    style.icons[icon_type::more]         = load_svg(bpp::res::icons::ellipse_svg, 64, 64);
    style.icons[icon_type::close]        = load_svg(bpp::res::icons::x_svg, 64, 64);
    style.icons[icon_type::check]        = load_svg(bpp::res::icons::check_svg, 64, 64);
    style.icons[icon_type::back]         = load_svg(bpp::res::icons::chevron_left_svg, 64, 64);
    style.icons[icon_type::forward]      = load_svg(bpp::res::icons::chevron_right_svg, 64, 64);
    style.icons[icon_type::up]           = load_svg(bpp::res::icons::chevron_up_svg, 64, 64);
    style.icons[icon_type::down]         = load_svg(bpp::res::icons::chevron_down_svg, 64, 64);
    style.icons[icon_type::add]          = load_svg(bpp::res::icons::plus_svg, 64, 64);
    style.icons[icon_type::remove]       = load_svg(bpp::res::icons::minus_svg, 64, 64);
    style.icons[icon_type::edit]         = load_svg(bpp::res::icons::pencil_svg, 64, 64);
    style.icons[icon_type::trash]        = load_svg(bpp::res::icons::trash_2_svg, 64, 64);
    style.icons[icon_type::save]         = load_svg(bpp::res::icons::save_svg, 64, 64);
    style.icons[icon_type::copy]         = load_svg(bpp::res::icons::copy_svg, 64, 64);
    style.icons[icon_type::paste]        = load_svg(bpp::res::icons::clipboard_svg, 64, 64);
    style.icons[icon_type::cut]          = load_svg(bpp::res::icons::scissors_svg, 64, 64);
    style.icons[icon_type::undo]         = load_svg(bpp::res::icons::undo_svg, 64, 64);
    style.icons[icon_type::redo]         = load_svg(bpp::res::icons::redo_svg, 64, 64);
    style.icons[icon_type::refresh]      = load_svg(bpp::res::icons::refresh_ccw_svg, 64, 64);
    style.icons[icon_type::download]     = load_svg(bpp::res::icons::download_svg, 64, 64);
    style.icons[icon_type::upload]       = load_svg(bpp::res::icons::upload_svg, 64, 64);
    style.icons[icon_type::share]        = load_svg(bpp::res::icons::share_2_svg, 64, 64);
    style.icons[icon_type::link]         = load_svg(bpp::res::icons::link_svg, 64, 64);
    style.icons[icon_type::info]         = load_svg(bpp::res::icons::info_svg, 64, 64);
    style.icons[icon_type::help]         = load_svg(bpp::res::icons::circle_question_mark_svg, 64, 64);
    style.icons[icon_type::warning]      = load_svg(bpp::res::icons::triangle_alert_svg, 64, 64);
    style.icons[icon_type::error]        = load_svg(bpp::res::icons::circle_x_svg, 64, 64);
    style.icons[icon_type::success]      = load_svg(bpp::res::icons::circle_check_svg, 64, 64);
    style.icons[icon_type::lock]         = load_svg(bpp::res::icons::lock_svg, 64, 64);
    style.icons[icon_type::unlock]       = load_svg(bpp::res::icons::lock_open_svg, 64, 64);
    style.icons[icon_type::user]         = load_svg(bpp::res::icons::user_svg, 64, 64);
    style.icons[icon_type::users]        = load_svg(bpp::res::icons::users_svg, 64, 64);
    style.icons[icon_type::settings]     = load_svg(bpp::res::icons::settings_svg, 64, 64);
    style.icons[icon_type::notification] = load_svg(bpp::res::icons::bell_svg, 64, 64);
    style.icons[icon_type::mail]         = load_svg(bpp::res::icons::mail_svg, 64, 64);
    style.icons[icon_type::calendar]     = load_svg(bpp::res::icons::calendar_svg, 64, 64);
    style.icons[icon_type::clock]        = load_svg(bpp::res::icons::clock_svg, 64, 64);
    style.icons[icon_type::favorite]     = load_svg(bpp::res::icons::heart_svg, 64, 64);
    style.icons[icon_type::bookmark]     = load_svg(bpp::res::icons::bookmark_svg, 64, 64);
    style.icons[icon_type::filter]       = load_svg(bpp::res::icons::funnel_svg, 64, 64);
    style.icons[icon_type::sort]         = load_svg(bpp::res::icons::arrow_up_down_svg, 64, 64);

    style.small_text.font_size           = 14.0F;
    style.small_text.spacing             = 1.0F;
    style.small_text.line_spacing        = 8;

    style.medium_text.font_size          = 18.0F;
    style.medium_text.spacing            = 1.0F;
    style.medium_text.line_spacing       = 4;

    style.large_text.font_size           = 22.0F;
    style.large_text.spacing             = 1.0F;
    style.large_text.line_spacing        = 2;

    style.small_icon                     = 14.0F;
    style.medium_icon                    = 18.0F;
    style.large_icon                     = 22.0F;

    style.background_color               = hsl{accent, 0.0F, light ? 0.875F : 0.05F, 1.0F};
    style.foreground_color               = hsl{accent, 1.0F, light ? 0.0F : 1.0F, 1.0F};
    style.active_color                   = hsl{accent, 1.0F, 1.0F, 1.0F};
    style.accent_color                   = hsl{accent, 1.0F, 0.5F, 1.0F};
    style.success_color                  = hsl{120.0F, 1.0F, 0.5F, 1.0F};
    style.info_color                     = hsl{240.0F, 1.0F, 0.5F, 1.0F};
    style.warning_color                  = hsl{60.0F, 1.0F, 0.5F, 1.0F};
    style.error_color                    = hsl{0.0F, 1.0F, 0.5F, 1.0F};

    style.base                           = style_element::zeroes();
    style.base.background_color          = hsl{accent, 0.0F, light ? 0.95F : 0.125F, 1.0F};
    style.base.corner_radius             = radius;

    style.hover_offset                   = style_element::zeroes();
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

    style.layered_offset                   = style_element::zeroes();
    style.layered_offset.shadow_color      = hsl{accent, 0.0F, 0.0F, 0.05F};
    style.layered_offset.shadow_radius     = 10.0F;
    style.layered_offset.shadow_shrink     = 8.0F;
    style.layered_offset.shadow_offset     = {0.0F, 6.0F};

    style.layered_scale                    = style_element::ones();

    return style;
}

auto bppui::unload_style(const style &style) -> void {
    UnloadFont(style.fonts.regular);
    UnloadFont(style.fonts.bold);
    UnloadFont(style.fonts.italic);
    UnloadFont(style.fonts.bold_italic);

    for (const auto &icon : style.icons.icons) {
        UnloadTexture(icon);
    }
}

// NOLINTEND(*-redundant-expression, *-branch-clone)
