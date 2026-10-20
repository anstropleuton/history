// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Raylib-style extra utilities and helper functions for Crystal GUI
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_EXTRA_H_
#define CGUI_EXTRA_H_

#include "cgui_api.h"
#include "raylib.h"

// Note: some things are duplicated even though they exist in raylib
// Can't be bothered to calculate exact difference :(

// Hotkey as an exclusive modifier
typedef enum CguiHotkey {
    CGUI_HOTKEY_SHIFT   = 1,
    CGUI_HOTKEY_CONTROL = 2,
    CGUI_HOTKEY_ALT     = 4,
    CGUI_HOTKEY_SUPER   = 8,
} CguiHotkey;

CGAPI bool CguiIsKeyRepeated(int key);                       // Check if a key is pressed or repeated
CGAPI bool CguiIsHotkeyDown(int hotkey);                     // Easily check if a single or multiple hotkeys are pressed exclusively
CGAPI bool CguiIsKeyComboUp(int hotkey, int key);            // Easily check if a hotkey is down && key is up
CGAPI bool CguiIsKeyComboDown(int hotkey, int key);          // Easily check if a hotkey is down && key is down
CGAPI bool CguiIsKeyComboPressed(int hotkey, int key);       // Easily check if a hotkey is down && key is pressed once
CGAPI bool CguiIsKeyComboReleased(int hotkey, int key);      // Easily check if a hotkey is down && key is released
CGAPI bool CguiIsKeyComboPressedRepeat(int hotkey, int key); // Easily check if a hotkey is down && key is pressed again
CGAPI bool CguiIsKeyComboRepeated(int hotkey, int key);      // Easily check if a hotkey is down && key is pressed or repeated

// Textures

CGAPI void CguiDrawTextureDest(Texture texture, Rectangle dest, Color tint);                          // Draw texture with destination rectangle parameter
CGAPI void CguiDrawTextureFullscreen(Texture texture, Color tint);                                    // Draw texture stretched to the size of the screen
CGAPI void CguiDrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint);                   // Draw texture stretched to the size of the screen with extended parameters
CGAPI void CguiDrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint);        // Draw render texture with destination rectangle parameter
CGAPI void CguiDrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint);                  // Draw render texture stretched to the size of the screen (NOTE: Flips the render texture's y axis)
CGAPI void CguiDrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint); // Draw render texture stretched to the size of the screen with extended parameters (NOTE: Flips the render texture's y axis)

CGAPI Vector2   CguiGetTextureSizeV(Texture texture);                     // Return texture's width and height as Vector2
CGAPI Rectangle CguiGetTextureSizeRec(Texture texture);                   // Return texture's width and height as Rectangle (positioned zero)
CGAPI Vector2   CguiGetRenderTextureSizeV(RenderTexture renderTexture);   // Return render texture's width and height as Vector2
CGAPI Rectangle CguiGetRenderTextureSizeRec(RenderTexture renderTexture); // Return render texture's width and height as Rectangle (positioned zero)

// Display sizes

CGAPI Vector2   CguiGetScreenSizeV(void);           // Get screen size in Vector2 form
CGAPI Rectangle CguiGetScreenSizeRec(void);         // Get screen size in Rectangle form (positioned zero)
CGAPI Vector2   CguiGetMonitorSizeV(int monitor);   // Get monitor size in Vector2 form
CGAPI Rectangle CguiGetMonitorSizeRec(int monitor); // Get monitor size in Rectangle form

// Math

CGAPI int       CguiMinI(int a, int b);                                      // Return the minimum of two integers
CGAPI int       CguiMaxI(int a, int b);                                      // Return the minimum of two integers
CGAPI int       CguiWrapModI(int a, int b);                                  // Wrapping modulo (returns value in the range of [0..b))
CGAPI float     CguiWrapModF(float a, float b);                              // Wrapping modulo (returns value in the range of [0..b))
CGAPI int       CguiMirrorModI(int a, int b);                                // Mirroring modulus (each odd modulus returns mirrored value in the range of [0..b))
CGAPI float     CguiMirrorModF(float a, float b);                            // Mirroring modulus (each odd modulus returns mirrored value in the range of [0..b))
CGAPI int       CguiClampI(int a, int mn, int mx);                           // Clamp the value between mn and mx (flips mn and mx if mn > mx)
CGAPI float     CguiClampF(float a, float mn, float mx);                     // Clamp the value between mn and mx (flips mn and mx if mn > mx)
CGAPI Rectangle CguiUnionRec(Rectangle a, Rectangle b);                      // Get the union of two rectangles
CGAPI Rectangle CguiIntersectionRec(Rectangle a, Rectangle b);               // Get the intersection of two rectangles, or rectangle containing zero if no overlap
CGAPI Rectangle CguiContainRec(Rectangle a, Rectangle b);                    // Get the rectangle obtained by moving the rectangle 'a' inside 'b' and clamping to max of 'b'.
CGAPI Vector2   CguiRotatePoint(Vector2 point, Vector2 origin, float angle); // Rotate point along origin with angle (in degrees)
CGAPI Rectangle CguiFlipRectangleX(Rectangle rec, float axis);               // Flip rectangle in X-axis
CGAPI Rectangle CguiFlipRectangleY(Rectangle rec, float axis);               // Flip rectangle in Y-axis
CGAPI Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis);            // Flip rectangle in X- and Y-axis
CGAPI Rectangle CguiChangeAspectFit(Rectangle rec, float aspect);            // Change the rectangle's aspect ratio (fitting mode) while keeping it centered
CGAPI Rectangle CguiChangeAspectFill(Rectangle rec, float aspect);           // Change the rectangle's aspect ratio (filling mode) while keeping it centered

// Misc

CGAPI bool    CguiIsRectangleEqual(Rectangle a, Rectangle b);                               // Check whether the rectangles are equal
CGAPI bool    CguiIsColorEqual(Color a, Color b);                                           // Check whether the colors are equal
CGAPI Texture CguiLoadTextureFromRenderTexture(RenderTexture renderTexture);                // Load texture from render texture (NOTE: Flips the render texture's y axis)
CGAPI Color   CguiColorMultiply(Color color, Vector4 values);                               // Multiply color with RGBA values
CGAPI Color   CugiColorAlphaTint(Color color, Color tint);                                  // Apply alpha-based tint over the color (raylib's ColorTint only multiplies the colors)
CGAPI Color   CguiColorFromHSL(float hue, float saturation, float lightness);               // Get color from HSL color model
CGAPI Color   CguiColorFromHSLA(float hue, float saturation, float lightness, float alpha); // Get color from HSL color model with alpha
CGAPI Color   CguiColorFromHSLV(Vector3 hsl);                                               // Get color from HSL color model (vector version)
CGAPI Color   CguiColorFromHSLAV(Vector4 hsla);                                             // Get color from HSL color model with alpha (vector version)
CGAPI Vector3 CguiColorToHSL(Color color);                                                  // Get HSL color model from color
CGAPI Vector4 CguiColorToHSLA(Color color);                                                 // Get HSL color model with alpha from color
CGAPI Color   CguiColorFromHSV(float hue, float saturation, float value);                   // Get color from HSV color model
CGAPI Color   CguiColorFromHSVA(float hue, float saturation, float value, float alpha);     // Get color from HSV color model with alpha
CGAPI Color   CguiColorFromHSVV(Vector3 hsl);                                               // Get color from HSV color model (vector version)
CGAPI Color   CguiColorFromHSVAV(Vector4 hsla);                                             // Get color from HSV color model with alpha (vector version)
CGAPI Vector3 CguiColorToHSV(Color color);                                                  // Get HSV color model from color
CGAPI Vector4 CguiColorToHSVA(Color color);                                                 // Get HSV color model with alpha from color
CGAPI Vector3 CguiColorNormal(Color color);                                                 // Get normalized color from color
CGAPI Vector4 CguiColorNormalA(Color color);                                                // Get normalized color with alpha from color
CGAPI Color   CguiColorDenormal(Vector3 color);                                             // Get normalized color from color
CGAPI Color   CguiColorDenormalA(Vector4 color);                                            // Get normalized color with alpha from color
CGAPI void    CguiBeginScissorModeRec(Rectangle area);                                      // Begin scissor mode (define a screen area for following drawing) using rectangle

// Rectangle points

CGAPI Vector2   CguiGetRecCenter(Rectangle rec);                  // Get center point of the rectangle
CGAPI Rectangle CguiSetRecCenter(Rectangle rec, Vector2 v);       // Set center point of the rectangle (moves rectangle)
CGAPI Vector2   CguiGetRecTopLeft(Rectangle rec);                 // Get top-left point of the rectangle
CGAPI Rectangle CguiSetRecTopLeft(Rectangle rec, Vector2 v);      // Set top-left point of the rectangle (resizes rectangle)
CGAPI Vector2   CguiGetRecTopRight(Rectangle rec);                // Get top-right point of the rectangle
CGAPI Rectangle CguiSetRecTopRight(Rectangle rec, Vector2 v);     // Set top-right point of the rectangle (resizes rectangle)
CGAPI Vector2   CguiGetRecBottomLeft(Rectangle rec);              // Get bottom-left point of the rectangle
CGAPI Rectangle CguiSetRecBottomLeft(Rectangle rec, Vector2 v);   // Set bottom-left point of the rectangle (resizes rectangle)
CGAPI Vector2   CguiGetRecBottomRight(Rectangle rec);             // Get bottom-right point of the rectangle
CGAPI Rectangle CguiSetRecBottomRight(Rectangle rec, Vector2 v);  // Set bottom-right point of the rectangle (resizes rectangle)
CGAPI Vector2   CguiGetRecTopCenter(Rectangle rec);               // Get top-center point of the rectangle
CGAPI Rectangle CguiSetRecTopCenter(Rectangle rec, Vector2 v);    // Set top-center point of the rectangle (moves and resizes rectangle)
CGAPI Vector2   CguiGetRecBottomCenter(Rectangle rec);            // Get bottom-center point of the rectangle
CGAPI Rectangle CguiSetRecBottomCenter(Rectangle rec, Vector2 v); // Set bottom-center point of the rectangle (moves and resizes rectangle)
CGAPI Vector2   CguiGetRecLeftCenter(Rectangle rec);              // Get left-center point of the rectangle
CGAPI Rectangle CguiSetRecLeftCenter(Rectangle rec, Vector2 v);   // Set left-center point of the rectangle (moves and resizes rectangle)
CGAPI Vector2   CguiGetRecRightCenter(Rectangle rec);             // Get right-center point of the rectangle
CGAPI Rectangle CguiSetRecRightCenter(Rectangle rec, Vector2 v);  // Set right-center point of the rectangle (moves and resizes rectangle)
CGAPI Vector2   CguiGetRecSize(Rectangle rec);                    // Get width and height of rectangle
CGAPI Rectangle CguiSetRecSize(Rectangle rec, Vector2 v);         // Set width and height of rectangle (resizes rectangle)

// Drawing

CGAPI void CguiDrawTextCentered(Font font, const char *text, Rectangle rec, float fontSize, float spacing, Color tint); // Draw a text centered in the rectangle

#endif // CGUI_EXTRA_H_
