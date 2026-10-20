/*******************************************************************************
 *
 *  Crystal GUI - Crystal-Clear Modern-Looking GUI Library for raylib!
 *
 *  Copyright (c) 2023 Anstro Pleuton (@anstropleuton)
 *
 *  License: MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sub-license, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 ******************************************************************************/

#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#include "cgui_config.h"
#include "crystalgui.h"
#include "raylib.h"
#include "raymath.h"

// Load multi-size font, loads several fonts with varying sizes
CguiMultiSizeFont CguiLoadMultiSizeFont(const char *fontFilename, float startingFontSize, float fontSizeDiff, int fontsCount, int *codepoints, int codepointCount)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("MULTI SIZE FONT", fontFilename, "Failed to load Multi-size Font due to invalid Font Filename", , (CguiMultiSizeFont) { 0 });
    CGUI_CHECK_VALID_LOG("MULTI SIZE FONT", fontsCount, "Failed to load Multi-size Font due to invalid Fonts count", , (CguiMultiSizeFont) { 0 });

    CguiMultiSizeFont multiSizeFont = { 0 };
    multiSizeFont.fontsCount = fontsCount;
    multiSizeFont.fonts = MemAlloc(sizeof(Font) * multiSizeFont.fontsCount);
    CGUI_CHECK_VALID_LOG("MULTI SIZE FONT", multiSizeFont.fonts, "Failed to allocate memory for Fonts", , (CguiMultiSizeFont) { 0 });
    multiSizeFont.id = ++idCounter;

    // Load multiple fonts
    for (int i = 0; i < multiSizeFont.fontsCount; i++)
    {
        float fontSize = startingFontSize + fontSizeDiff * i;
        multiSizeFont.fonts[i] = LoadFontEx(fontFilename, fontSize, codepoints, codepointCount);
    }

    CGUI_LOG_ID("MULTI SIZE FONT", multiSizeFont.id, "Successfully loaded Multi-size Font");
    return multiSizeFont;
}

// Unload loaded multi-size font
void CguiUnloadMultiSizeFont(CguiMultiSizeFont multiSizeFont)
{
    CGUI_CHECK_VALID_LOG("MULTI SIZE FONT", multiSizeFont.fonts, "Failed to unload Multi-size Font due to invalid Fonts", , );
    unsigned int id = multiSizeFont.id;
    for (int i = 0; i < multiSizeFont.fontsCount; i++) UnloadFont(multiSizeFont.fonts[i]);
    MemFree(multiSizeFont.fonts);
    CGUI_LOG_ID("MULTI SIZE FONT", id, "Successfully loaded Multi-size Font");
}

// Draw text with multi-size font
void CguiDrawText(CguiMultiSizeFont multiSizeFont, const char *text, Vector2 position, float fontSize, float spacing, Color tint)
{
    CGUI_CHECK_VALID(multiSizeFont.fonts, , );
    CGUI_CHECK_VALID(text, , );

    // Get nearest font that has close font size
    int fontIndex = 0;
    float minDiff = multiSizeFont.fonts[multiSizeFont.fontsCount - 1].baseSize;
    for (int i = 0; i < multiSizeFont.fontsCount; i++)
    {
        float diff = fabsf(fontSize - multiSizeFont.fonts[i].baseSize);
        if (diff < minDiff)
        {
            minDiff = diff;
            fontIndex = i;
        }
    }

    DrawTextEx(multiSizeFont.fonts[fontIndex], text, position, fontSize, spacing, tint);
}

// Measure text size
Vector2 CguiMeasureText(CguiMultiSizeFont multiSizeFont, const char *text, float fontSize, float spacing)
{
    CGUI_CHECK_VALID(multiSizeFont.fonts, , Vector2Zero());
    CGUI_CHECK_VALID(text, , Vector2Zero());

    // Get nearest font that has close font size
    int fontIndex = 0;
    float minDiff = multiSizeFont.fonts[multiSizeFont.fontsCount - 1].baseSize;
    for (int i = 0; i < multiSizeFont.fontsCount; i++)
    {
        float diff = fabsf(fontSize - multiSizeFont.fonts[i].baseSize);
        if (diff < minDiff)
        {
            minDiff = diff;
            fontIndex = i;
        }
    }

    return MeasureTextEx(multiSizeFont.fonts[fontIndex], text, fontSize, spacing);
}

// INTERNALS: Get text editor character type
int __CguiTextEditorGetCharacterType(char c)
{
    if (c == '\0') return CGUI_TEXT_EDITOR_CHARACTER_TYPE_UNKNOWN;
    if (c == ' ') return CGUI_TEXT_EDITOR_CHARACTER_TYPE_SPACE;
    if (c == '\t') return CGUI_TEXT_EDITOR_CHARACTER_TYPE_TAB;
    if (c == '\n') return CGUI_TEXT_EDITOR_CHARACTER_TYPE_NEW_LINE;
    if (isalpha(c)) return CGUI_TEXT_EDITOR_CHARACTER_TYPE_ALPHABET;
    if (isdigit(c)) return CGUI_TEXT_EDITOR_CHARACTER_TYPE_NUMBER;
    if (iscntrl(c)) return CGUI_TEXT_EDITOR_CHARACTER_TYPE_CONTROL;
    return CGUI_TEXT_EDITOR_CHARACTER_TYPE_SYMBOL;
}

// INTERNALS: Copy text editor history state of a to b (b must be allocated)
void __CguiTextEditorHistoryCreateCopy(CguiTextEditorHistory *a, CguiTextEditorHistory *b)
{
    CGUI_CHECK_VALID(a, , );
    CGUI_CHECK_VALID(b, , );

    b->allocatedLength = a->allocatedLength;

    b->cursorsCount = a->cursorsCount;
    b->cursorPositions = MemAlloc(sizeof(int) * b->cursorsCount);
    b->cursorSelections = MemAlloc(sizeof(int) * b->cursorsCount);
    for (int i = 0; i < b->cursorsCount; i++)
    {
        b->cursorPositions[i] = a->cursorPositions[i];
        b->cursorSelections[i] = a->cursorSelections[i];
    }

    b->text = MemAlloc(b->allocatedLength);
    memcpy(b->text, a->text, b->allocatedLength);
}

// INTERNALS: Free copied history (copy pointer isn't freed)
void __CguiTextEditorHistoryDeleteCopy(CguiTextEditorHistory *copy)
{
    CGUI_CHECK_VALID(copy, , );
    if (copy->cursorPositions) MemFree(copy->cursorPositions);
    if (copy->cursorSelections) MemFree(copy->cursorSelections);
    if (copy->text) MemFree(copy->text);
}

// INTERNALS: Limit history list
void __CguiTextEditorHistoryLimitHistory(CguiTextEditorHistory *history)
{
    int size = 0;
}

// Load a text editor (allocate memory to stuff)
CguiTextEditor CguiLoadTextEditor(void)
{
    static unsigned int idCounter = 0;
    CguiTextEditor textEditor = { 0 };
    textEditor.allocatedLength = CGUI_TEXT_EDITOR_TEXT_UNIT_LENGTH;
    textEditor.text = MemAlloc(textEditor.allocatedLength);
    CGUI_CHECK_VALID_LOG("TEXT EDITOR", textEditor.text, "Failed to allocate memory for text", , (CguiTextEditor) { 0 });

    textEditor.cursorsCount = 1;
    textEditor.undoHistoryLimit = CGUI_TEXT_EDITOR_UNDO_HISTORY_LIMIT;
    textEditor.id = ++idCounter;
    return textEditor;
}

// Unload a text editor (deallocate memory from stuff)
void CguiUnloadTextEditor(CguiTextEditor textEditor)
{
    CGUI_CHECK_VALID_LOG("TEXT EDITOR", textEditor.text, "Failed to deallocate memory for text", , );
}

// Update text and other fields
void CguiUpdateTextEditor(CguiTextEditor *textEditor)
{
    CGUI_CHECK_VALID(textEditor, , );
    CGUI_CHECK_VALID(textEditor->text, , );
}

// Generate rectangle texture
CguiRectangleTexture CguiGenerateRectangleTexture(CguiRectangleShader *shader)
{
    return CguiGenerateRectangleTextureEx(shader, WHITE);
}

// Generate rectangle texture with extended parameters
CguiRectangleTexture CguiGenerateRectangleTextureEx(CguiRectangleShader *shader, Color color)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("RECTANGLE TEXTURE", shader, "Failed to generate Rectangle Texture due to invalid Shader", , (CguiRectangleTexture) { 0 });

    // Calculate texture size
    Vector2 textureSize = { (shader->values.roundness + shader->values.shadowRadius + shader->values.shadowSize) * 2.0f,
                            (shader->values.roundness + shader->values.shadowRadius + shader->values.shadowSize) * 2.0f };

    RenderTexture rectangleRenderTexture = LoadRenderTexture(textureSize.x, textureSize.y);
    CGUI_CHECK_VALID_LOG("RECTANGLE TEXTURE", rectangleRenderTexture.id, "Failed to generate Rectangle Texture", , (CguiRectangleTexture) { 0 });

    // Calculate rectangle's center
    CguiRectangleTexture rectangle = { 0 };
    rectangle.values = shader->values;
    rectangle.center = (Vector2) { shader->values.shadowRadius + shader->values.shadowSize - shader->values.shadowOffset[0] + shader->values.roundness,
                                   shader->values.shadowRadius + shader->values.shadowSize - shader->values.shadowOffset[1] + shader->values.roundness };

    // Prepare smallest rectangle that preserves the details
    shader->values.rectangle[0] = shader->values.shadowRadius + shader->values.shadowSize - shader->values.shadowOffset[0];
    shader->values.rectangle[1] = shader->values.shadowRadius + shader->values.shadowSize - shader->values.shadowOffset[1];
    shader->values.rectangle[2] = textureSize.x - shader->values.shadowRadius * 2.0f - shader->values.shadowSize * 2.0f;
    shader->values.rectangle[3] = textureSize.y - shader->values.shadowRadius * 2.0f - shader->values.shadowSize * 2.0f;
    shader->values.tint[0] = color.r / 255.0f;
    shader->values.tint[1] = color.g / 255.0f;
    shader->values.tint[2] = color.b / 255.0f;
    shader->values.tint[3] = color.a / 255.0f;
    CguiSetRectangleShaderValues(shader);

    // Draw on render texture using shader
    BeginTextureMode(rectangleRenderTexture);
    ClearBackground(BLANK);
    BeginShaderMode(shader->shader);
    DrawRectangleV(Vector2Zero(), textureSize, WHITE);
    TraceLog(LOG_INFO, "Texture Size: %f, %f", textureSize.x, textureSize.y);
    EndShaderMode();
    EndTextureMode();

    rectangle.texture = LoadTextureFromRenderTexture(rectangleRenderTexture);
    UnloadRenderTexture(rectangleRenderTexture);
    CGUI_CHECK_VALID_LOG("RECTANGLE TEXTURE", rectangle.texture.id, "Failed to generate Rectangle Texture", , (CguiRectangleTexture) { 0 });
    rectangle.id = ++idCounter;
    CGUI_LOG_ID("RECTANGLE TEXTURE", rectangle.id, "Successfully generated Rectangle Texture");
    return rectangle;
}

// Unload generated rectangle texture
void CguiDisposeRectangle(CguiRectangleTexture rectangle)
{
    int id = rectangle.id;
    UnloadTexture(rectangle.texture);
    CGUI_LOG_ID("RECTANGLE TEXTURE", id, "Successfully disposed Rectangle Texture");
}

// Generate multi-roundness rectangle texture
CguiMultiRoundnessRectangleTexture CguiGenerateMultiRoundnessRectangleTexture(CguiRectangleShader *shader, float startingRoundness, float roundnessDiff, int rectanglesCount)
{
    return CguiGenerateMultiRoundnessRectangleTextureEx(shader, startingRoundness, roundnessDiff, rectanglesCount, WHITE);
}

// Generate multi-roundness rectangle texture with extended parameters
CguiMultiRoundnessRectangleTexture CguiGenerateMultiRoundnessRectangleTextureEx(CguiRectangleShader *shader, float startingRoundness, float roundnessDiff, int rectanglesCount, Color color)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("MULTI ROUNDNESS RECTANGLE TEXTURE", shader, "Failed to generate Multi-roundness Rectangle Texture due to invalid Shader", , (CguiMultiRoundnessRectangleTexture) { 0 });
    CGUI_CHECK_VALID_LOG("MULTI ROUNDNESS RECTANGLE TEXTURE", rectanglesCount, "Failed to generate Multi-roundness Rectangle Texture due to invalid Rectangles count", , (CguiMultiRoundnessRectangleTexture) { 0 });

    CguiMultiRoundnessRectangleTexture rectangles = { 0 };
    rectangles.rectanglesCount = rectanglesCount;
    rectangles.rectangleTextures = MemAlloc(sizeof(CguiRectangleTexture) * rectangles.rectanglesCount);
    CGUI_CHECK_VALID_LOG("MULTI ROUNDNESS RECTANGLE TEXTURE", rectangles.rectangleTextures, "Failed to allocate memory for Rectangles", , (CguiMultiRoundnessRectangleTexture) { 0 });

    // Draw multiple rectangles with varying roundness
    for (int i = 0; i < rectangles.rectanglesCount; i++)
    {
        shader->values.roundness = startingRoundness + roundnessDiff * i;
        CguiSetRectangleShaderValues(shader);
        rectangles.rectangleTextures[i] = CguiGenerateRectangleTextureEx(shader, color);
    }

    rectangles.id = ++idCounter;
    CGUI_LOG_ID("MULTI ROUNDNESS RECTANGLE TEXTURE", rectangles.id, "Successfully generated Multi-roundness Rectangle Texture");
    return rectangles;
}

// Unload generated multi-roundness rectangle texture
void CguiDisposeMultiRoundnessRectangleTexture(CguiMultiRoundnessRectangleTexture rectangles)
{
    CGUI_CHECK_VALID_LOG("MULTI ROUNDNESS RECTANGLE TEXTURE", rectangles.rectangleTextures, "Failed to dispose Multi-roundness Rectangle Texture due to invalid Rectangles", , );
    for (int i = 0; i < rectangles.rectanglesCount; i++) CguiDisposeRectangle(rectangles.rectangleTextures[i]);
    unsigned int id = rectangles.id;
    MemFree(rectangles.rectangleTextures);
    CGUI_LOG_ID("MULTI ROUNDNESS RECTANGLE TEXTURE", id, "Successfully disposed Multi-roundness Rectangle Texture");
}

// Choose a rectangle from array for roundness and draw, return false when no roundness match
void CguiDrawRectangle(CguiMultiRoundnessRectangleTexture rectangles, Rectangle dest, float roundness, Color tint)
{
    CGUI_CHECK_VALID(rectangles.rectangleTextures, , );

    // Get nearest rectangle that has close roundness
    int rectangleIndex = 0;
    float minDiff = rectangles.rectangleTextures[rectangles.rectanglesCount - 1].values.roundness;
    for (int i = 0; i < rectangles.rectanglesCount; i++)
    {
        float diff = fabsf(roundness - rectangles.rectangleTextures[i].values.roundness);
        if (diff < minDiff)
        {
            minDiff = diff;
            rectangleIndex = i;
        }
    }

    // Adjust the destination
    CguiRectangleTexture rec = rectangles.rectangleTextures[rectangleIndex];
    Rectangle realDest = {
        dest.x + (rec.values.shadowRadius + rec.values.shadowSize - rec.values.shadowOffset[0]) * -1.0f,
        dest.y + (rec.values.shadowRadius + rec.values.shadowSize - rec.values.shadowOffset[1]) * -1.0f,
        dest.width + (rec.values.shadowRadius + rec.values.shadowSize) * 2.0f,
        dest.height + (rec.values.shadowRadius + rec.values.shadowSize) * 2.0f
    };

    // Corners
    DrawTexturePro(rec.texture, (Rectangle) { 0.0f, 0.0f, rec.center.x, rec.center.y }, (Rectangle) { realDest.x, realDest.y, rec.center.x, rec.center.y }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x, 0.0f, rec.texture.width - rec.center.x, rec.center.y }, (Rectangle) { realDest.x + realDest.width - rec.texture.width + rec.center.x, realDest.y, rec.texture.width - rec.center.x, rec.center.y }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { 0.0f, rec.center.y, rec.center.x, rec.texture.height - rec.center.y }, (Rectangle) { realDest.x, realDest.y + realDest.height - rec.texture.height + rec.center.y, rec.center.x, rec.texture.height - rec.center.y }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x, rec.center.y, rec.texture.width - rec.center.x, rec.texture.height - rec.center.y }, (Rectangle) { realDest.x + realDest.width - rec.texture.width + rec.center.x, realDest.y + realDest.height - rec.texture.height + rec.center.y, rec.texture.width - rec.center.x, rec.texture.height - rec.center.y }, Vector2Zero(), 0.0f, tint);

    // Edges
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x - 1.0f, 0.0f, 2.0f, rec.center.y }, (Rectangle) { realDest.x + rec.center.x, realDest.y, realDest.width - rec.texture.width, rec.center.y }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x - 1.0f, rec.center.y, 2.0f, rec.texture.height - rec.center.y }, (Rectangle) { realDest.x + rec.center.x, realDest.y + realDest.height - rec.texture.height + rec.center.y, realDest.width - rec.texture.width, rec.texture.height - rec.center.y }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x, rec.center.y - 1.0f, rec.texture.width - rec.center.x, 2.0f }, (Rectangle) { realDest.x + realDest.height - rec.texture.width + rec.center.x, realDest.y + rec.center.y, rec.texture.width - rec.center.x, realDest.height - rec.texture.height }, Vector2Zero(), 0.0f, tint);
    DrawTexturePro(rec.texture, (Rectangle) { 0.0f, rec.center.y - 1.0f, rec.center.x, 2.0f }, (Rectangle) { realDest.x, realDest.y + rec.center.y, rec.center.x, realDest.height - rec.texture.height }, Vector2Zero(), 0.0f, tint);

    // Center
    DrawTexturePro(rec.texture, (Rectangle) { rec.center.x - 1.0f, rec.center.y - 1.0f, 2.0f, 2.0f }, (Rectangle) { realDest.x + rec.center.x, realDest.y + rec.center.y, realDest.width - rec.texture.width, realDest.height - rec.texture.height }, Vector2Zero(), 0.0f, tint);
}

// Begin gradient mode with color parameters (WARNING: Uses BeginShaderMode() so you can't use shader in this mode)
void CguiBeginGradientModeColor(CguiGradientShader *gradientShader, CguiGradientColor color, Rectangle bounds)
{
    CguiBeginGradientModeEx(gradientShader, color.color1, color.color2, (Vector2) { color.pos1.x * bounds.width + bounds.x, color.pos1.y * bounds.height + bounds.y }, (Vector2) { color.pos2.x * bounds.width + bounds.x, color.pos2.y * bounds.height + bounds.y });
}

// Adwaita-inspired color theme (default)
CguiColorTheme CguiGetAdwaitaColorTheme(void)
{
    CguiColorTheme colorTheme = { 0 };
    colorTheme.darkBackgroundColor = (CguiGradientColor) { (Color) { 12, 12, 12, 255 }, (Color) { 12, 12, 12, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.backgroundColor = (CguiGradientColor) { (Color) { 24, 24, 24, 255 }, (Color) { 24, 24, 24, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.lightBackgroundColor = (CguiGradientColor) { (Color) { 36, 36, 36, 255 }, (Color) { 36, 36, 36, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.darkWidgetColor = (CguiGradientColor) { (Color) { 48, 48, 48, 255 }, (Color) { 48, 48, 48, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.widgetColor = (CguiGradientColor) { (Color) { 60, 60, 60, 255 }, (Color) { 60, 60, 60, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.lightWidgetColor = (CguiGradientColor) { (Color) { 72, 72, 72, 255 }, (Color) { 72, 72, 72, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.accentColor = (CguiGradientColor) { (Color) { 53, 132, 228, 255 }, (Color) { 53, 132, 228, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.suggestColor = (CguiGradientColor) { (Color) { 38, 162, 105, 255 }, (Color) { 38, 162, 105, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.warningColor = (CguiGradientColor) { (Color) { 230, 97, 0, 255 }, (Color) { 230, 97, 0, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.errorColor = (CguiGradientColor) { (Color) { 192, 28, 40, 255 }, (Color) { 192, 28, 40, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.darkMainColor = (CguiGradientColor) { (Color) { 13, 13, 13, 255 }, (Color) { 13, 13, 13, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.mainColor = (CguiGradientColor) { (Color) { 57, 57, 57, 255 }, (Color) { 57, 57, 57, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.lightMainColor = (CguiGradientColor) { (Color) { 70, 70, 70, 255 }, (Color) { 70, 70, 70, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.textColor = (CguiGradientColor) { (Color) { 250, 250, 250, 255 }, (Color) { 250, 250, 250, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.textColorDisabled = (CguiGradientColor) { (Color) { 255, 255, 255, 255 }, (Color) { 255, 255, 255, 255 }, (Vector2) { 0.0f, 0.0f }, (Vector2) { 1.0f, 1.0f } };
    colorTheme.hoverTintColor = (Vector4) { 1.2f, 1.2f, 1.2f, 1.0f };
    colorTheme.heldTintColor = (Vector4) { 0.8f, 0.8f, 0.8f, 1.0f };
    return colorTheme;
}

// Load all the resources that Crystal GUI needs
CguiResources *CguiLoadResources(void)
{
    // Use default values
    CguiBlurShaderUniforms blurValues = { 0 };
    blurValues.blurRadius = 25.0f;
    blurValues.kernelSize = 25;

    CguiRectangleShaderUniforms rectangleValues = { 0 };
    rectangleValues.tint[0] = 1.0f;
    rectangleValues.tint[1] = 1.0f;
    rectangleValues.tint[2] = 1.0f;
    rectangleValues.tint[3] = 1.0f;
    rectangleValues.shadowRadius = 30.0f;
    rectangleValues.shadowSize = -2.5f;
    rectangleValues.shadowOffset[0] = 5.0f;
    rectangleValues.shadowOffset[1] = 5.0f;
    rectangleValues.shadowColor[0] = 0.0f;
    rectangleValues.shadowColor[1] = 0.0f;
    rectangleValues.shadowColor[2] = 0.0f;
    rectangleValues.shadowColor[3] = 1.0f;
    rectangleValues.borderThickness = 2.5f;
    rectangleValues.borderColor[0] = 20.8f;
    rectangleValues.borderColor[1] = 51.8f;
    rectangleValues.borderColor[2] = 89.4f;
    rectangleValues.borderColor[3] = 1.0f;

    CguiGradientShaderUniforms gradientValues = { 0 };
    gradientValues.color1[0] = 16.1f;
    gradientValues.color1[1] = 24.7f;
    gradientValues.color1[2] = 67.8f;
    gradientValues.color1[3] = 1.0f;
    gradientValues.color2[0] = 77.6f;
    gradientValues.color2[1] = 36.1f;
    gradientValues.color2[2] = 48.6f;
    gradientValues.color2[3] = 1.0f;
    gradientValues.pos2[0] = 1.0f; // Causes internal division by zero
    gradientValues.pos2[1] = 1.0f; // Causes internal division by zero

    float startingRoundness = 0.0f;
    float roundnessDiff = 5.0f;
    int rectanglesCount = 10;

    static unsigned int idCounter = 0;
    CguiResources *resources = MemAlloc(sizeof(CguiResources));
    CGUI_CHECK_VALID_LOG("RESOURCES", resources, "Failed to allocate memory for Resources", , NULL);

    // Load and set shaders
    resources->blurShader = CguiLoadBlurShader();
    if (resources->blurShader)
    {
        resources->blurShader->values = blurValues;
        CguiSetBlurShaderValues(resources->blurShader);
    }

    resources->rectangleShader = CguiLoadRectangleShader();
    if (resources->rectangleShader)
    {
        resources->rectangleShader->values = rectangleValues;
        CguiSetRectangleShaderValues(resources->rectangleShader);
    }

    resources->gradientShader = CguiLoadGradientShader();
    if (resources->gradientShader)
    {
        resources->gradientShader->values = gradientValues;
        CguiSetGradientShaderValues(resources->gradientShader);
    }

    if (resources->rectangleShader)
    {
        // Get shadow and border alpha
        float borderColorA = resources->rectangleShader->values.borderColor[3];
        float shadowColorA = resources->rectangleShader->values.shadowColor[3];
        float tintR = resources->rectangleShader->values.tint[0];
        float tintG = resources->rectangleShader->values.tint[1];
        float tintB = resources->rectangleShader->values.tint[2];
        float tintA = resources->rectangleShader->values.tint[3];

        // Generate shadow-less and border-less rectangles
        resources->rectangleShader->values.shadowColor[3] = 0.0f;
        resources->rectangleShader->values.borderColor[3] = 0.0f;
        CguiSetRectangleShaderValues(resources->rectangleShader);
        resources->rectangles = CguiGenerateMultiRoundnessRectangleTexture(resources->rectangleShader, startingRoundness, roundnessDiff, rectanglesCount);

        // Disable tint
        resources->rectangleShader->values.tint[0] = 0.0f;
        resources->rectangleShader->values.tint[1] = 0.0f;
        resources->rectangleShader->values.tint[2] = 0.0f;
        resources->rectangleShader->values.tint[3] = 0.0f;
        CguiSetRectangleShaderValues(resources->rectangleShader);

        // Generate shadow-only (border-less) rectangles
        resources->rectangleShader->values.shadowColor[3] = shadowColorA;
        resources->rectangleShader->values.borderColor[3] = 0.0f;
        CguiSetRectangleShaderValues(resources->rectangleShader);
        resources->rectanglesShadow = CguiGenerateMultiRoundnessRectangleTexture(resources->rectangleShader, startingRoundness, roundnessDiff, rectanglesCount);

        // Generate border-only (shadow-less) rectangles
        resources->rectangleShader->values.shadowColor[3] = 0.0f;
        resources->rectangleShader->values.borderColor[3] = borderColorA;
        CguiSetRectangleShaderValues(resources->rectangleShader);
        resources->rectanglesBorder = CguiGenerateMultiRoundnessRectangleTexture(resources->rectangleShader, startingRoundness, roundnessDiff, rectanglesCount);

        // Reset back uniforms
        resources->rectangleShader->values.shadowColor[3] = shadowColorA;
        resources->rectangleShader->values.borderColor[3] = borderColorA;
        resources->rectangleShader->values.tint[0] = tintR;
        resources->rectangleShader->values.tint[1] = tintG;
        resources->rectangleShader->values.tint[2] = tintB;
        resources->rectangleShader->values.tint[3] = tintA;
        CguiSetRectangleShaderValues(resources->rectangleShader);
    }

    resources->colorTheme = CguiGetAdwaitaColorTheme();
    resources->transitionSpeed = 0.25f;
    resources->id = ++idCounter;
    CGUI_LOG_ID("RESOURCES", resources->id, "Successfully loaded Resources");
    return resources;
}

// Unload all the loaded resources
void CguiUnloadResources(CguiResources *resources)
{
    CGUI_CHECK_VALID_LOG("RESOURCES", resources, "Failed to unload Resources due to invalid Resources", , );
    unsigned int id = resources->id;
    CguiUnloadBlurShader(resources->blurShader);
    CguiUnloadRectangleShader(resources->rectangleShader);
    CguiUnloadGradientShader(resources->gradientShader);
    CguiDisposeMultiRoundnessRectangleTexture(resources->rectangles);
    CguiDisposeMultiRoundnessRectangleTexture(resources->rectanglesShadow);
    CguiDisposeMultiRoundnessRectangleTexture(resources->rectanglesBorder);
    CGUI_LOG_ID("RESOURCES", id, "Successfully unloaded Resources");
}

// Load the Crystal GUI
CguiInstance *CguiLoadInstance(void)
{
    CguiInstance *instance = CguiLoadInstanceEx(CguiLoadResources());
    CGUI_CHECK_VALID(instance, , NULL);

    instance->sharedRes = false;
    return instance;
}

// Load the Crystal GUI without loading resources
CguiInstance *CguiLoadInstanceEx(CguiResources *res)
{
    static unsigned int idCounter = 0;
    CguiInstance *instance = MemAlloc(sizeof(CguiInstance));
    CGUI_CHECK_VALID_LOG("INSTANCE", instance, "Failed to allocate memory for Instance", , NULL);

    instance->resources = res;
    instance->grandWidget = CguiCreateWidget();
    instance->sharedRes = true;
    instance->id = ++idCounter;

    CGUI_LOG_ID("INSTANCE", instance->id, "Successfully loaded Instance");
    return instance;
}

// Unload the Crystal GUI
void CguiUnloadInstance(CguiInstance *instance)
{
    CGUI_CHECK_VALID_LOG("INSTANCE", instance, "Failed to unload Instance due to invalid Instance", , );
    unsigned int id = instance->id;
    if (!instance->sharedRes) CguiUnloadResources(instance->resources);
    if (instance->grandWidget) CguiDeleteWidget(instance->grandWidget, true, true);
    MemFree(instance);
    CGUI_LOG_ID("INSTANCE", id, "Successfully unloaded Instance");
}

// INTERNALS: Data for widget update and draw
typedef struct __CguiWidgetData {
    CguiResources *res;
    void *customWidgetData;
} __CguiWidgetData;

// INTERNALS: Traverser to update each element
int __CguiUpdateWidget(CguiWidget *widget, void *data)
{
    // Don't halt the iteration just because of invalid widget
    CGUI_CHECK_VALID(widget, , 0);

    if (!widget->element)
    {
        if (widget->customElementUpdate) widget->customElementUpdate(((__CguiWidgetData *)data)->res, widget->element, ((__CguiWidgetData *)data)->customWidgetData);
        return 0;
    }

    if (data) switch (((CguiElementBasic *)widget->element)->elementType)
        {
            case CGUI_ELEMENT_TYPE_LABEL: CguiUpdateElementLabel(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_ICON: CguiUpdateElementIcon(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_ANIMATED_ICON: CguiUpdateElementAnimatedIcon(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_BUTTON: CguiUpdateElementButton(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_TOGGLE: CguiUpdateElementToggle(((__CguiWidgetData *)data)->res, widget->element); break;
        }
    return 0;
}

// INTERNALS: Traverser to draw each element
int __CguiDrawWidget(CguiWidget *widget, void *data)
{
    // Don't halt the iteration just because of invalid widget
    CGUI_CHECK_VALID(widget, , 0);

    if (!widget->element)
    {
        if (widget->customElementDraw) widget->customElementDraw(((__CguiWidgetData *)data)->res, widget->element, ((__CguiWidgetData *)data)->customWidgetData);
        return 0;
    }

    if (data) switch (((CguiElementBasic *)widget->element)->elementType)
        {
            case CGUI_ELEMENT_TYPE_LABEL: CguiDrawElementLabel(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_ICON: CguiDrawElementIcon(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_ANIMATED_ICON: CguiDrawElementAnimatedIcon(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_BUTTON: CguiDrawElementButton(((__CguiWidgetData *)data)->res, widget->element); break;
            case CGUI_ELEMENT_TYPE_TOGGLE: CguiDrawElementToggle(((__CguiWidgetData *)data)->res, widget->element); break;
        }

    // Rectangle rec = CguiGetWidgetRectangle(widget);
    // DrawRectangleLinesEx(rec, 1.0f, WHITE);
    return 0;
}

// Update the Crystal GUI
void CguiUpdateInstance(CguiInstance *instance, void *customWidgetData)
{
    CGUI_CHECK_VALID(instance, , );
    static __CguiWidgetData data;
    data.res = instance->resources;
    data.customWidgetData = customWidgetData;
    instance->grandWidget->localTransform.size = GetAppResolutionV();
    CguiTraverseWidgetParentFirst(instance->grandWidget, __CguiUpdateWidget, &data);
}

// Draw the Crystal GUI
void CguiDrawInstance(CguiInstance *instance, void *customWidgetData)
{
    CGUI_CHECK_VALID(instance, , );
    static __CguiWidgetData data;
    data.res = instance->resources;
    data.customWidgetData = customWidgetData;
    CguiTraverseWidgetParentFirst(instance->grandWidget, __CguiDrawWidget, &data);
}
