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

#include <math.h>

#include "cgui_config.h"
#include "crystalgui.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

// Some shape functions and utils functions are not available in raylib, implement our own

// Draw ellipse outline with extended parameters
void DrawEllipseLinesEx(int centerX, int centerY, float radiusH, float radiusV, float lineThick, Color color)
{
    float innerRadiusH = radiusH - lineThick;
    float innerRadiusV = radiusV - lineThick;

    rlBegin(RL_TRIANGLES);
    for (int i = 0; i < 360; i += 10)
    {
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlVertex2f(centerX + cosf(DEG2RAD * (i + 10)) * radiusH, centerY + sinf(DEG2RAD * (i + 10)) * radiusV);
        rlVertex2f(centerX + cosf(DEG2RAD * i) * radiusH, centerY + sinf(DEG2RAD * i) * radiusV);
        rlVertex2f(centerX + cosf(DEG2RAD * i) * innerRadiusH, centerY + sinf(DEG2RAD * i) * innerRadiusV);

        rlVertex2f(centerX + cosf(DEG2RAD * i) * innerRadiusH, centerY + sinf(DEG2RAD * i) * innerRadiusV);
        rlVertex2f(centerX + cosf(DEG2RAD * (i + 10)) * innerRadiusH, centerY + sinf(DEG2RAD * (i + 10)) * innerRadiusV);
        rlVertex2f(centerX + cosf(DEG2RAD * (i + 10)) * radiusH, centerY + sinf(DEG2RAD * (i + 10)) * radiusV);
    }
    rlEnd();
}

// Draw triangle outline with extended parameters (vertex in counter-clockwise order!)
void DrawTriangleLinesEx(Vector2 v1, Vector2 v2, Vector2 v3, float lineThick, Color color)
{
    // ERROR: Implementation pending
}

// Draw rectangle with rounded edges
void DrawRectangleRoundedEx(Rectangle rec, float roundness, int segments, float rotation, Color color)
{
    // Not a rounded rectangle
    if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1))
    {
        DrawRectanglePro((Rectangle) { rec.x + rec.width / 2.0, rec.y + rec.height / 2.0, rec.width, rec.height }, (Vector2) { rec.width / 2.0f, rec.height / 2.0f }, rotation, color);
        return;
    }

    if (roundness >= 1.0f) roundness = 1.0f;

    // Calculate corner radius
    float radius = (rec.width > rec.height) ? (rec.height * roundness) / 2 : (rec.width * roundness) / 2;
    if (radius <= 0.0f) return;

    // Calculate number of segments to use for the corners
    if (segments < 4)
    {
        // Calculate the maximum angle between segments based on the error rate (usually 0.5f)
        float th = acosf(2 * powf(1 - SMOOTH_CIRCLE_ERROR_RATE / radius, 2) - 1);
        segments = (int)(ceilf(2 * PI / th) / 4.0f);
        if (segments <= 0) segments = 4;
    }

    float stepLength = 90.0f / (float)segments;

    /*
    Quick sketch to make sense of all of this,
    there are 9 parts to draw, also mark the 12 points we'll use

          P0____________________P1
          /|                    |\
         /1|          2         |3\
     P7 /__|____________________|__\ P2
       |   |P8                P9|   |
       | 8 |          9         | 4 |
       | __|____________________|__ |
     P6 \  |P11              P10|  / P3
         \7|          6         |5/
          \|____________________|/
          P5                    P4

    NOTE: We remove the const specifier since we transform the points with rotation
    */
    // Coordinates of the 12 points that define the rounded rect
    Vector2 point[12] = {
        { (float)rec.x + radius, rec.y }, { (float)(rec.x + rec.width) - radius, rec.y }, { rec.x + rec.width, (float)rec.y + radius }, // PO, P1, P2
        { rec.x + rec.width, (float)(rec.y + rec.height) - radius },
        { (float)(rec.x + rec.width) - radius, rec.y + rec.height },                                                                    // P3, P4
        { (float)rec.x + radius, rec.y + rec.height },
        { rec.x, (float)(rec.y + rec.height) - radius },
        { rec.x, (float)rec.y + radius },                               // P5, P6, P7
        { (float)rec.x + radius, (float)rec.y + radius },
        { (float)(rec.x + rec.width) - radius, (float)rec.y + radius }, // P8, P9
        { (float)(rec.x + rec.width) - radius, (float)(rec.y + rec.height) - radius },
        { (float)rec.x + radius, (float)(rec.y + rec.height) - radius } // P10, P11
    };

    Vector2 centers[4] = { point[8], point[9], point[10], point[11] };

    // Rotate all the points with rotation
    float sinRotation = sinf(rotation * DEG2RAD);
    float cosRotation = cosf(rotation * DEG2RAD);

    const Vector2 recCenter = { rec.x + rec.width / 2, rec.y + rec.height / 2 };

    for (int i = 0; i < 16; i++)
    {
        Vector2 translatedPoint = { point[i].x - recCenter.x, point[i].y - recCenter.y };
        point[i] = (Vector2) { translatedPoint.x * cosRotation - translatedPoint.y * sinRotation + recCenter.x, translatedPoint.x * sinRotation + translatedPoint.y * cosRotation + recCenter.y };
    }
    for (int i = 0; i < 4; i++)
    {
        Vector2 translatedPoint = { centers[i].x - recCenter.x, centers[i].y - recCenter.y };
        centers[i] = (Vector2) { translatedPoint.x * cosRotation - translatedPoint.y * sinRotation + recCenter.x, translatedPoint.x * sinRotation + translatedPoint.y * cosRotation + recCenter.y };
    }

    const float angles[4] = { rotation + 180.0f, rotation + 270.0f, rotation, rotation + 90.0f };

#if defined(SUPPORT_QUADS_DRAW_MODE)
    rlSetTexture(texShapes.id);

    rlBegin(RL_QUADS);
    // Draw all the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
    for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
    {
        float angle = angles[k];
        const Vector2 center = centers[k];

        // NOTE: Every QUAD actually represents two segments
        for (int i = 0; i < segments / 2; i++)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
            rlVertex2f(center.x, center.y);

            rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
            rlVertex2f(center.x + cosf(DEG2RAD * (angle + stepLength * 2)) * radius, center.y + sinf(DEG2RAD * (angle + stepLength * 2)) * radius);

            rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
            rlVertex2f(center.x + cosf(DEG2RAD * (angle + stepLength)) * radius, center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);

            rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
            rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);

            angle += (stepLength * 2);
        }

        // NOTE: In case number of segments is odd, we add one last piece to the cake
        if (segments % 2)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
            rlVertex2f(center.x, center.y);

            rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
            rlVertex2f(center.x + cosf(DEG2RAD * (angle + stepLength)) * radius, center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);

            rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
            rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);

            rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
            rlVertex2f(center.x, center.y);
        }
    }

    // [2] Upper Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[0].x, point[0].y);
    rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[8].x, point[8].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[9].x, point[9].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[1].x, point[1].y);

    // [4] Right Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[2].x, point[2].y);
    rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[9].x, point[9].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[10].x, point[10].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[3].x, point[3].y);

    // [6] Bottom Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[11].x, point[11].y);
    rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[5].x, point[5].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[4].x, point[4].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[10].x, point[10].y);

    // [8] Left Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[7].x, point[7].y);
    rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[6].x, point[6].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[11].x, point[11].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[8].x, point[8].y);

    // [9] Middle Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[8].x, point[8].y);
    rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[11].x, point[11].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
    rlVertex2f(point[10].x, point[10].y);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
    rlVertex2f(point[9].x, point[9].y);

    rlEnd();
    rlSetTexture(0);
#else
    rlBegin(RL_TRIANGLES);

    // Draw all of the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
    for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
    {
        float angle = angles[k];
        const Vector2 center = centers[k];
        for (int i = 0; i < segments; i++)
        {
            rlColor4ub(color.r, color.g, color.b, color.a);
            rlVertex2f(center.x, center.y);
            rlVertex2f(center.x + cosf(DEG2RAD * (angle + stepLength)) * radius, center.y + sinf(DEG2RAD * (angle + stepLength)) * radius);
            rlVertex2f(center.x + cosf(DEG2RAD * angle) * radius, center.y + sinf(DEG2RAD * angle) * radius);
            angle += stepLength;
        }
    }

    // [2] Upper Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(point[0].x, point[0].y);
    rlVertex2f(point[8].x, point[8].y);
    rlVertex2f(point[9].x, point[9].y);
    rlVertex2f(point[1].x, point[1].y);
    rlVertex2f(point[0].x, point[0].y);
    rlVertex2f(point[9].x, point[9].y);

    // [4] Right Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(point[9].x, point[9].y);
    rlVertex2f(point[10].x, point[10].y);
    rlVertex2f(point[3].x, point[3].y);
    rlVertex2f(point[2].x, point[2].y);
    rlVertex2f(point[9].x, point[9].y);
    rlVertex2f(point[3].x, point[3].y);

    // [6] Bottom Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(point[11].x, point[11].y);
    rlVertex2f(point[5].x, point[5].y);
    rlVertex2f(point[4].x, point[4].y);
    rlVertex2f(point[10].x, point[10].y);
    rlVertex2f(point[11].x, point[11].y);
    rlVertex2f(point[4].x, point[4].y);

    // [8] Left Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(point[7].x, point[7].y);
    rlVertex2f(point[6].x, point[6].y);
    rlVertex2f(point[11].x, point[11].y);
    rlVertex2f(point[8].x, point[8].y);
    rlVertex2f(point[7].x, point[7].y);
    rlVertex2f(point[11].x, point[11].y);

    // [9] Middle Rectangle
    rlColor4ub(color.r, color.g, color.b, color.a);
    rlVertex2f(point[8].x, point[8].y);
    rlVertex2f(point[11].x, point[11].y);
    rlVertex2f(point[10].x, point[10].y);
    rlVertex2f(point[9].x, point[9].y);
    rlVertex2f(point[8].x, point[8].y);
    rlVertex2f(point[10].x, point[10].y);
    rlEnd();
#endif
}

// Draw rectangle with rounded edges outline
void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float rotation, float lineThick, Color color)
{
    if ((lineThick > rec.width) || (lineThick > rec.height))
    {
        if (rec.width > rec.height) lineThick = rec.height / 2;
        else if (rec.width < rec.height) lineThick = rec.width / 2;
    }

    // ERROR: Implementation pending
}

// Draw rectangle outline with pro parameters
void DrawRectangleLinesPro(Rectangle rec, float lineThick, float rotation, Color color)
{
    // ERROR: Implementation pending
}

// Check if the point lies in rounded rectangle
bool CheckCollisionPointRoundedRec(Vector2 point, Rectangle rec, float roundness)
{
    // ERROR: Implementation pending
    return false;
}

// Check if the point lies in rounded rectangle with extended parameters
bool CheckCollisionPointRoundedRecEx(Vector2 point, Rectangle rec, float roundness, Vector2 origin, float rotation)
{
    // ERROR: Implementation pending
    return false;
}

// Draw texture with destination rectangle parameter
void DrawTextureDest(Texture texture, Rectangle rec, Color tint)
{
    DrawTexturePro(texture, GetTextureSizeRec(texture), rec, Vector2Zero(), 0.0f, tint);
}

// Draw render texture with destination rectangle parameter
void DrawRenderTextureDest(RenderTexture renderTexture, Rectangle rec, Color tint)
{
    DrawTexturePro(renderTexture.texture, GetRenderTextureSizeRec(renderTexture), rec, Vector2Zero(), 0.0f, tint);
}

// Draw texture with width and height of the screen
// NOTE: Get current monitor resolution if the app is in fullscreen mode
void DrawTextureFullscreen(Texture texture, Color tint)
{
    DrawTextureDest(texture, GetAppResolutionRec(), tint);
}

// Draw texture with width and height of the screen with extended parameters
void DrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint)
{
    DrawTexturePro(texture, src, GetAppResolutionRec(), Vector2Zero(), 0.0f, tint);
}

// Draw render texture with width and height of the screen (NOTE: Flips the render texture's y axis)
void DrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint)
{
    DrawTexturePro(renderTexture.texture, GetRenderTextureSizeRec(renderTexture), GetAppResolutionRec(), Vector2Zero(), 0.0f, tint);
}

// Draw render texture with width and height of the screen with extended parameters (NOTE: Flips the render texture's y axis)
void DrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint)
{
    DrawTexturePro(renderTexture.texture, (Rectangle) { src.x, -src.y, src.width, -src.height }, GetAppResolutionRec(), Vector2Zero(), 0.0f, tint);
}

// Return texture's width and height as Vector2
Vector2 GetTextureSizeV(Texture texture)
{
    return (Vector2) { texture.width, texture.height };
}

// Return texture's width and height as Rectangle
Rectangle GetTextureSizeRec(Texture texture)
{
    return (Rectangle) { 0.0f, 0.0f, texture.width, texture.height };
}

// Return texture's width and height as Vector2
Vector2 GetRenderTextureSizeV(RenderTexture renderTexture)
{
    return (Vector2) { renderTexture.texture.width, -renderTexture.texture.height };
}

// Return texture's width and height as Rectangle
Rectangle GetRenderTextureSizeRec(RenderTexture renderTexture)
{
    return (Rectangle) { 0.0f, 0.0f, renderTexture.texture.width, -renderTexture.texture.height };
}

// Rotate point along origin with angle (degrees)
Vector2 RotatePoint(Vector2 point, Vector2 origin, float angle)
{
    Vector2 po = Vector2Subtract(point, origin);
    float sinAngle = sinf(angle * DEG2RAD);
    float cosAngle = cosf(angle * DEG2RAD);
    Vector2 rp = { po.x * cosAngle - po.y * sinAngle, po.x * sinAngle + po.y * cosAngle };
    return Vector2Add(rp, origin);
}

// Get screen width and height in Vector2 form
Vector2 GetScreenResolutionV(void)
{
    return (Vector2) { GetScreenWidth(), GetScreenHeight() };
}

// Get screen width and height in Rectangle form
Rectangle GetScreenResolutionRec(void)
{
    return (Rectangle) { 0.0f, 0.0f, GetScreenWidth(), GetScreenHeight() };
}

// Get monitor width and height in Vector2 form
Vector2 GetMonitorResolutionV(int monitor)
{
    return (Vector2) { GetMonitorWidth(monitor), GetMonitorHeight(monitor) };
}

// Get monitor width and height in Rectangle form
Rectangle GetMonitorResolutionRec(int monitor)
{
    return (Rectangle) { 0.0f, 0.0f, GetMonitorWidth(monitor), GetMonitorHeight(monitor) };
}

// Get monitor width if app is fullscreen mode, else get screen width
int GetAppWidth(void)
{
    if (IsWindowFullscreen()) return GetMonitorWidth(GetCurrentMonitor());
    else return GetScreenWidth();
}

// Get monitor height if app is fullscreen mode, else get screen height
int GetAppHeight(void)
{
    if (IsWindowFullscreen()) return GetMonitorHeight(GetCurrentMonitor());
    else return GetScreenHeight();
}

// Get monitor width and height if app is fullscreen mode, else get screen width and height in Vector2 form
Vector2 GetAppResolutionV(void)
{
    return (Vector2) { GetAppWidth(), GetAppHeight() };
}

// Get monitor width and height if app is fullscreen mode, else get screen width and height in Rectangle form
Rectangle GetAppResolutionRec(void)
{
    return (Rectangle) { 0.0f, 0.0f, GetAppWidth(), GetAppHeight() };
}

// Load texture from render texture (NOTE: Flips the render texture's y axis)
Texture LoadTextureFromRenderTexture(RenderTexture renderTexture)
{
    Image image = LoadImageFromTexture(renderTexture.texture);
    ImageFlipVertical(&image);
    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

// Get the minimum roundness acceptable by the rectangle
float GetMinRoundness(Rectangle rec, float roundness)
{
    float minRoundness = fminf(rec.width, rec.height) / 2.0f;
    if (roundness > minRoundness) return minRoundness;
    else return roundness;
}

// Multiply color with values
Color ColorMultiply(Color color, Vector4 values)
{
    Color finalColor = color;
    finalColor.r = Clamp(finalColor.r * values.x, 0, 255);
    finalColor.g = Clamp(finalColor.g * values.y, 0, 255);
    finalColor.b = Clamp(finalColor.b * values.z, 0, 255);
    finalColor.a = Clamp(finalColor.a * values.w, 0, 255);
    return finalColor;
}
