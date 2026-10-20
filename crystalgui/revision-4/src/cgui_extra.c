// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "cgui_extra.h"
#include "math.h"
#include "raymath.h"
#include <ctype.h>
#include <stdlib.h>

bool CguiIsKeyRepeated(int key) {
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

bool CguiIsHotkeyDown(int hotkey) {
    bool result = true;
    result &= (hotkey & CGUI_HOTKEY_SHIFT) ? IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) : IsKeyUp(KEY_LEFT_SHIFT) && IsKeyUp(KEY_RIGHT_SHIFT);
    result &= (hotkey & CGUI_HOTKEY_CONTROL) ? IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) : IsKeyUp(KEY_LEFT_CONTROL) && IsKeyUp(KEY_RIGHT_CONTROL);
    result &= (hotkey & CGUI_HOTKEY_ALT) ? IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT) : IsKeyUp(KEY_LEFT_ALT) && IsKeyUp(KEY_RIGHT_ALT);
    result &= (hotkey & CGUI_HOTKEY_SUPER) ? IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER) : IsKeyUp(KEY_LEFT_SUPER) && IsKeyUp(KEY_RIGHT_SUPER);
    return result;
}

bool CguiIsKeyComboUp(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && IsKeyUp(key);
}

bool CguiIsKeyComboDown(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && IsKeyDown(key);
}

bool CguiIsKeyComboPressed(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && IsKeyPressed(key);
}

bool CguiIsKeyComboReleased(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && IsKeyReleased(key);
}

bool CguiIsKeyComboPressedRepeat(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && IsKeyPressedRepeat(key);
}

bool CguiIsKeyComboRepeated(int hotkey, int key) {
    return CguiIsHotkeyDown(hotkey) && CguiIsKeyRepeated(key);
}

void CguiDrawTextureDest(Texture texture, Rectangle dest, Color tint) {
    DrawTexturePro(texture, CguiGetTextureSizeRec(texture), dest, (Vector2){0, 0}, 0.0f, tint);
}

void CguiDrawTextureFullscreen(Texture texture, Color tint) {
    CguiDrawTextureDest(texture, CguiGetScreenSizeRec(), tint);
}

void CguiDrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint) {
    DrawTexturePro(texture, src, CguiGetScreenSizeRec(), (Vector2){0, 0}, 0.0f, tint);
}

void CguiDrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint) {
    dest = (Rectangle){dest.x, dest.y, dest.width, -dest.height};
    DrawTexturePro(renderTexture.texture, CguiGetRenderTextureSizeRec(renderTexture), dest, (Vector2){0, 0}, 0.0f, tint);
}

void CguiDrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint) {
    CguiDrawRenderTextureDest(renderTexture, CguiGetScreenSizeRec(), tint);
}

void CguiDrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint) {
    Rectangle dest = CguiGetScreenSizeRec();
    dest           = (Rectangle){dest.x, dest.y, dest.width, -dest.height};
    DrawTexturePro(renderTexture.texture, src, dest, (Vector2){0, 0}, 0.0f, tint);
}

Vector2 CguiGetTextureSizeV(Texture texture) {
    return (Vector2){(float)texture.width, (float)texture.height};
}

Rectangle CguiGetTextureSizeRec(Texture texture) {
    return (Rectangle){0, 0, (float)texture.width, (float)texture.height};
}

Vector2 CguiGetRenderTextureSizeV(RenderTexture renderTexture) {
    return (Vector2){(float)renderTexture.texture.width, (float)renderTexture.texture.height};
}

Rectangle CguiGetRenderTextureSizeRec(RenderTexture renderTexture) {
    return (Rectangle){0, 0, (float)renderTexture.texture.width, (float)renderTexture.texture.height};
}

Vector2 CguiGetScreenSizeV(void) {
    return (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()};
}

Rectangle CguiGetScreenSizeRec(void) {
    return (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
}

Vector2 CguiGetMonitorSizeV(int monitor) {
    return (Vector2){(float)GetMonitorWidth(monitor), (float)GetMonitorHeight(monitor)};
}

Rectangle CguiGetMonitorSizeRec(int monitor) {
    return (Rectangle){0, 0, (float)GetMonitorWidth(monitor), (float)GetMonitorHeight(monitor)};
}

int CguiMinI(int a, int b) {
    return a < b ? a : b;
}

int CguiMaxI(int a, int b) {
    return a > b ? a : b;
}

int CguiWrapModI(int a, int b) {
    return (a % b + b) % b;
}

float CguiWrapModF(float a, float b) {
    return fmodf(fmodf(a, b) + b, b);
}

int CguiMirrorModI(int a, int b) {
    if (b <= 0) return 0;

    int period = b * 2;
    int m      = CguiWrapModI(a, period);

    if (m >= b) m = period - 1 - m;
    return m;
}

float CguiMirrorModF(float a, float b) {
    if (b <= 0.0f) return 0.0f;

    float period = b * 2.0f;
    float m      = CguiWrapModF(a, period);

    if (m >= b) m = period - m;
    return m;
}

int CguiClampI(int a, int mn, int mx) {
    if (mn > mx) {
        return CguiMinI(CguiMaxI(a, mx), mn);
    } else {
        return CguiMinI(CguiMaxI(a, mn), mx);
    }
}

float CguiClampF(float a, float mn, float mx) {
    if (mn > mx) {
        return fminf(fmaxf(a, mx), mn);
    } else {
        return fminf(fmaxf(a, mn), mx);
    }
}

Rectangle CguiUnionRec(Rectangle a, Rectangle b) {
    Rectangle r = {0};

    float x1 = fminf(a.x, b.x);
    float y1 = fminf(a.y, b.y);
    float x2 = fmaxf(a.x + a.width, b.x + b.width);
    float y2 = fmaxf(a.y + a.height, b.y + b.height);

    r.x      = x1;
    r.y      = y1;
    r.width  = x2 - x1;
    r.height = y2 - y1;

    return r;
}

Rectangle CguiIntersectionRec(Rectangle a, Rectangle b) {
    Rectangle r = {0};

    float x1 = fmaxf(a.x, b.x);
    float y1 = fmaxf(a.y, b.y);
    float x2 = fminf(a.x + a.width, b.x + b.width);
    float y2 = fminf(a.y + a.height, b.y + b.height);

    if (x2 < x1 || y2 < y1) {
        r.x = r.y = r.width = r.height = 0;
        return r;
    }

    r.x      = x1;
    r.y      = y1;
    r.width  = x2 - x1;
    r.height = y2 - y1;

    return r;
}

Rectangle CguiContainRec(Rectangle a, Rectangle b) {
    Rectangle r = a;

    r.width  = fminf(a.width, b.width);
    r.height = fminf(a.height, b.height);

    r.x = CguiClampF(a.x, b.x, b.x + b.width - r.width);
    r.y = CguiClampF(a.y, b.y, b.y + b.height - r.height);

    return r;
}

Vector2 CguiRotatePoint(Vector2 point, Vector2 origin, float angle) {
    Vector2 po = Vector2Subtract(point, origin);
    Vector2 r  = Vector2Rotate(po, angle);
    Vector2 ro = Vector2Add(r, origin);
    return ro;
}

bool CguiIsRectangleEqual(Rectangle a, Rectangle b) {
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

bool CguiIsColorEqual(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

Texture CguiLoadTextureFromRenderTexture(RenderTexture renderTexture) {
    Image image = LoadImageFromTexture(renderTexture.texture);
    ImageFlipVertical(&image);
    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

Color CguiColorMultiply(Color color, Vector4 values) {
    return (Color){(unsigned char)(color.r * values.x), (unsigned char)(color.g * values.y), (unsigned char)(color.b * values.z), (unsigned char)(color.a * values.w)};
}

Color CugiColorAlphaTint(Color color, Color tint) {
    // Stolen from raylib :)
    color.r = (unsigned char)(((unsigned int)color.r * ((unsigned int)tint.r + 1)) >> 8);
    color.g = (unsigned char)(((unsigned int)color.g * ((unsigned int)tint.g + 1)) >> 8);
    color.b = (unsigned char)(((unsigned int)color.b * ((unsigned int)tint.b + 1)) >> 8);
    color.a = (unsigned char)(((unsigned int)color.a * ((unsigned int)tint.a + 1)) >> 8);
    return color;
}

Color CguiColorFromHSL(float hue, float saturation, float lightness) {
    return CguiColorFromHSLA(hue, saturation, lightness, 1.0f);
}

Color CguiColorFromHSLA(float hue, float saturation, float lightness, float alpha) {
    float r, g, b;

    hue = fmodf(hue, 360.0f);
    if (hue < 0) hue += 360.0f;

    float c  = (1.0f - fabsf(2.0f * lightness - 1.0f)) * saturation;
    float hp = hue / 60.0f;
    float x  = c * (1.0f - fabsf(fmodf(hp, 2.0f) - 1.0f));

    float r1 = 0, g1 = 0, b1 = 0;

    if (hp < 1) {
        r1 = c;
        g1 = x;
        b1 = 0;
    } else if (hp < 2) {
        r1 = x;
        g1 = c;
        b1 = 0;
    } else if (hp < 3) {
        r1 = 0;
        g1 = c;
        b1 = x;
    } else if (hp < 4) {
        r1 = 0;
        g1 = x;
        b1 = c;
    } else if (hp < 5) {
        r1 = x;
        g1 = 0;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    float m = lightness - c * 0.5f;

    r = r1 + m;
    g = g1 + m;
    b = b1 + m;

    Color col = {0};
    col.r = (unsigned char)(r * 255.0f);
    col.g = (unsigned char)(g * 255.0f);
    col.b = (unsigned char)(b * 255.0f);
    col.a = (unsigned char)(alpha * 255.0f);

    return col;
}

Color CguiColorFromHSLV(Vector3 hsl) {
    return CguiColorFromHSL(hsl.x, hsl.y, hsl.z);
}

Color CguiColorFromHSLAV(Vector4 hsla) {
    return CguiColorFromHSLA(hsla.x, hsla.y, hsla.z, hsla.w);
}

Vector3 CguiColorToHSL(Color color) {
    Vector4 hsl = CguiColorToHSLA(color);
    return (Vector3){hsl.x, hsl.y, hsl.z};
}

Vector4 CguiColorToHSLA(Color color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    float a = color.a / 255.0f;

    float max = r;
    if (g > max) max = g;
    if (b > max) max = b;

    float min = r;
    if (g < min) min = g;
    if (b < min) min = b;

    float h, s, l;
    float d = max - min;

    l = (max + min) * 0.5f;

    if (d == 0) {
        h = 0;
        s = 0;
    } else {
        s = d / (1.0f - fabsf(2.0f * l - 1.0f));

        if (max == r)
            h = 60.0f * fmodf(((g - b) / d), 6.0f);
        else if (max == g)
            h = 60.0f * (((b - r) / d) + 2.0f);
        else
            h = 60.0f * (((r - g) / d) + 4.0f);

        if (h < 0) h += 360.0f;
    }

    Vector4 v = {0};
    v.x = h;
    v.y = s;
    v.z = l;
    v.w = a;

    return v;
}

Color CguiColorFromHSV(float hue, float saturation, float value) {
    return CguiColorFromHSVA(hue, saturation, value, 1.0f);
}

Color CguiColorFromHSVA(float hue, float saturation, float value, float alpha) {
    float r, g, b;

    hue = fmodf(hue, 360.0f);
    if (hue < 0) hue += 360.0f;

    float c  = value * saturation;
    float hp = hue / 60.0f;
    float x  = c * (1.0f - fabsf(fmodf(hp, 2.0f) - 1.0f));

    float r1 = 0, g1 = 0, b1 = 0;

    if (hp < 1) {
        r1 = c;
        g1 = x;
        b1 = 0;
    } else if (hp < 2) {
        r1 = x;
        g1 = c;
        b1 = 0;
    } else if (hp < 3) {
        r1 = 0;
        g1 = c;
        b1 = x;
    } else if (hp < 4) {
        r1 = 0;
        g1 = x;
        b1 = c;
    } else if (hp < 5) {
        r1 = x;
        g1 = 0;
        b1 = c;
    } else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    float m = value - c;

    r = r1 + m;
    g = g1 + m;
    b = b1 + m;

    Color col = {0};
    col.r = (unsigned char)(r * 255.0f);
    col.g = (unsigned char)(g * 255.0f);
    col.b = (unsigned char)(b * 255.0f);
    col.a = (unsigned char)(alpha * 255.0f);

    return col;
}

Color CguiColorFromHSVV(Vector3 hsv) {
    return CguiColorFromHSV(hsv.x, hsv.y, hsv.z);
}

Color CguiColorFromHSVAV(Vector4 hsva) {
    return CguiColorFromHSVA(hsva.x, hsva.y, hsva.z, hsva.w);
}

Vector3 CguiColorToHSV(Color color) {
    Vector4 hsv = CguiColorToHSVA(color);
    return (Vector3){hsv.x, hsv.y, hsv.z};
}

Vector4 CguiColorToHSVA(Color color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    float a = color.a / 255.0f;

    float max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    float min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    float d   = max - min;

    float h = 0, s = 0, v = max;

    if (d != 0) {
        s = d / max;

        if (max == r)
            h = 60.0f * fmodf(((g - b) / d), 6.0f);
        else if (max == g)
            h = 60.0f * (((b - r) / d) + 2.0f);
        else
            h = 60.0f * (((r - g) / d) + 4.0f);

        if (h < 0) h += 360.0f;
    }

    Vector4 vOut = {0};
    vOut.x = h;
    vOut.y = s;
    vOut.z = v;
    vOut.w = a;

    return vOut;
}

Vector3 CguiColorNormal(Color color) {
    Vector3 norm = {0};
    norm.x       = color.r / 255.0f;
    norm.y       = color.g / 255.0f;
    norm.z       = color.b / 255.0f;
    return norm;
}

Vector4 CguiColorNormalA(Color color) {
    Vector4 norm = {0};
    norm.x       = color.r / 255.0f;
    norm.y       = color.g / 255.0f;
    norm.z       = color.b / 255.0f;
    norm.w       = color.a / 255.0f;
    return norm;
}

Color CguiColorDenormal(Vector3 color) {
    Color col = {0};
    col.r     = (unsigned char)(color.x * 255.0f);
    col.g     = (unsigned char)(color.y * 255.0f);
    col.b     = (unsigned char)(color.z * 255.0f);
    col.a     = 255;
    return col;
}

Color CguiColorDenormalA(Vector4 color) {
    Color col = {0};
    col.r     = (unsigned char)(color.x * 255.0f);
    col.g     = (unsigned char)(color.y * 255.0f);
    col.b     = (unsigned char)(color.z * 255.0f);
    col.a     = (unsigned char)(color.w * 255.0f);
    return col;
}

void CguiBeginScissorModeRec(Rectangle area) {
    BeginScissorMode(area.x, area.y, area.width, area.height);
}

Rectangle CguiFlipRectangleX(Rectangle rec, float axis) {
    rec.x = 2 * axis - (rec.x + rec.width);
    return rec;
}

Rectangle CguiFlipRectangleY(Rectangle rec, float axis) {
    rec.y = 2 * axis - (rec.y + rec.height);
    return rec;
}

Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis) {
    rec.x = 2 * axis.x - (rec.x + rec.width);
    rec.y = 2 * axis.y - (rec.y + rec.height);
    return rec;
}

Rectangle CguiChangeAspectFit(Rectangle rec, float aspect) {
    Vector2 center = CguiGetRecCenter(rec);

    float w = rec.width;
    float h = rec.height;

    float current = w / h;

    if (current > aspect) {
        w = h * aspect;
    } else {
        h = w / aspect;
    }

    Rectangle r = {0, 0, w, h};
    return CguiSetRecCenter(r, center);
}

Rectangle CguiChangeAspectFill(Rectangle rec, float aspect) {
    Vector2 center = CguiGetRecCenter(rec);

    float w = rec.width;
    float h = rec.height;

    float current = w / h;

    if (current > aspect) {
        h = w / aspect;
    } else {
        w = h * aspect;
    }

    Rectangle r = {0, 0, w, h};
    return CguiSetRecCenter(r, center);
}

// TODO

Vector2 CguiGetRecCenter(Rectangle rec) {
    return (Vector2){rec.x + rec.width / 2.0f, rec.y + rec.height / 2.0f};
}

Rectangle CguiSetRecCenter(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecTopLeft(Rectangle rec) {
    return (Vector2){rec.x, rec.y};
}

Rectangle CguiSetRecTopLeft(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecTopRight(Rectangle rec) {
    return (Vector2){rec.x + rec.width, rec.y};
}

Rectangle CguiSetRecTopRight(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecBottomLeft(Rectangle rec) {
    return (Vector2){rec.x, rec.y + rec.height};
}

Rectangle CguiSetRecBottomLeft(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecBottomRight(Rectangle rec) {
    return (Vector2){rec.x + rec.width, rec.y + rec.height};
}

Rectangle CguiSetRecBottomRight(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecTopCenter(Rectangle rec) {
    return (Vector2){rec.x + rec.width / 2.0f, rec.y};
}

Rectangle CguiSetRecTopCenter(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecBottomCenter(Rectangle rec) {
    return (Vector2){rec.x + rec.width / 2.0f, rec.y + rec.height};
}

Rectangle CguiSetRecBottomCenter(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecLeftCenter(Rectangle rec) {
    return (Vector2){rec.x, rec.y + rec.height / 2.0f};
}

Rectangle CguiSetRecLeftCenter(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecRightCenter(Rectangle rec) {
    return (Vector2){rec.x + rec.width, rec.y + rec.height / 2.0f};
}

Rectangle CguiSetRecRightCenter(Rectangle rec, Vector2 v) {
}

Vector2 CguiGetRecSize(Rectangle rec) {
    return (Vector2){rec.width, rec.height};
}

Rectangle CguiSetRecSize(Rectangle rec, Vector2 v) {
}

void CguiDrawTextCentered(Font font, const char *text, Rectangle rec, float fontSize, float spacing, Color tint) {
    Vector2 measure = MeasureTextEx(font, text, fontSize, spacing);

    DrawTextEx(font, text, (Vector2){rec.x + (rec.width - measure.x) / 2.0f, rec.y + (rec.height - measure.y) / 2.0f}, fontSize, spacing, tint);
}
