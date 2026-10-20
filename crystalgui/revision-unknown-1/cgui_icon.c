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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "cgui_config.h"
#include "crystalgui.h"
#include "raylib.h"

// INTERNALS: Internal structure for split string
struct __CguiSplitStringT {
    char *str;
    struct __CguiSplitStringT *child;
    struct __CguiSplitStringT *next;
};

// Recursively split a string using multiple delimiters
struct __CguiSplitStringT *__CguiSplitString(const char *str, const char *delims)
{
    if (!str || !*str || !delims || !*delims) return NULL;

    // Create a copy of the input string for strtok
    int strLength = TextLength(str) + 1;
    char *strCopy = MemAlloc(strLength);
    if (!strCopy) return NULL;
    memcpy(strCopy, str, strLength);
    char *singleDelim = (char[]) { *delims, '\0' };

    struct __CguiSplitStringT *splitString = MemAlloc(sizeof(struct __CguiSplitStringT));
    if (!splitString) return NULL;

    splitString->str = NULL;
    splitString->child = NULL;
    splitString->next = NULL;

    struct __CguiSplitStringT *splitStringCurrent = splitString;

    // Use strtok to split the string
    const char *token = strtok(strCopy, singleDelim);
    int totalTokenLength = 0;

    while (token)
    {
        // Recursively call __CguiSplitString on the token with the remaining delimiters
        splitStringCurrent->child = __CguiSplitString(token, delims + 1);
        int tokenLength = strlen(token) + 1;
        splitStringCurrent->str = MemAlloc(tokenLength);
        if (!splitStringCurrent->str) return NULL;
        memcpy(splitStringCurrent->str, token, tokenLength);

        // Check if there are more tokens
        totalTokenLength += TextLength(token) + 1;
        if (strCopy[totalTokenLength]) token = strtok(strCopy + totalTokenLength, singleDelim);
        else token = NULL;

        // Create a new node and move to the next
        if (token)
        {
            splitStringCurrent->next = MemAlloc(sizeof(struct __CguiSplitStringT));
            if (!splitStringCurrent->next) return NULL;

            splitStringCurrent = splitStringCurrent->next;
            splitStringCurrent->str = NULL;
            splitStringCurrent->child = NULL;
            splitStringCurrent->next = NULL;
        }
    }

    MemFree(strCopy);
    return splitString;
}

// Free the memory allocated for the split string structure
void __CguiSplitFree(struct __CguiSplitStringT *splitString)
{
    while (splitString)
    {
        // Recursively free memory for child nodes
        if (splitString->child) __CguiSplitFree(splitString->child);
        if (splitString->str) MemFree(splitString->str);

        struct __CguiSplitStringT *splitStringCurrent = splitString;
        splitString = splitString->next;
        MemFree(splitStringCurrent);
    }
}

int __CguiSplitCount(struct __CguiSplitStringT *splitString)
{
    int count = 0;
    while (splitString)
    {
        count++;
        splitString = splitString->next;
    }
    return count;
}

// Generate shape with formatted string, see single shape format above
CguiShapeBasic *CguiGenerateShape(const char *shapeFormat)
{
    CGUI_CHECK_VALID_LOG("SHAPE", shapeFormat, "Failed to generate Shape due to invalid Shape Format", , NULL);
    CguiShapeBasic *shape = NULL;

    // Split string and parse parameters
    struct __CguiSplitStringT *splitString = __CguiSplitString(shapeFormat + 1, ",");
    int paramCount = __CguiSplitCount(splitString);
    CGUI_CHECK_VALID_LOG("SHAPE", paramCount, "Failed to generate Shape due to invalid Shape Format", , NULL);

    float *param = MemAlloc(sizeof(float) * paramCount);
    CGUI_CHECK_VALID_LOG("SHAPE", param, "Failed to allocate memory for Params", __CguiSplitFree(splitString), NULL);

    struct __CguiSplitStringT *splitStringCurrent = splitString;
    for (int i = 0; splitStringCurrent; i++)
    {
        param[i] = atof(splitStringCurrent->str);
        splitStringCurrent = splitStringCurrent->next;
    }

    char shapeChar = *shapeFormat;
    switch (shapeChar)
    {
        // Line shape
        case 'l':
        {
            CGUI_LOG_CONDITION(paramCount < 8, break, LOG_ERROR, "CGUI: SHAPE: Parameters for line shape is less than expected (8), shape string format: %s", shapeFormat)
            CguiShapeLine *lineShape = MemAlloc(sizeof(CguiShapeLine));
            lineShape->iconShapeType = CGUI_ICON_SHAPE_LINE;
            lineShape->color.r = param[0];
            lineShape->color.g = param[1];
            lineShape->color.b = param[2];
            lineShape->color.a = param[3];
            lineShape->startPos.x = param[4];
            lineShape->startPos.y = param[5];
            lineShape->endPos.x = param[6];
            lineShape->endPos.y = param[7];
            if (paramCount >= 9) lineShape->lineThick = param[8];
            else lineShape->lineThick = 1.0f; // Default value is non-zero
            shape = (CguiShapeBasic *)lineShape;
        }
        break;

        // Circle shape
        case 'c':
        {
            CGUI_LOG_CONDITION(paramCount < 7, break, LOG_ERROR, "CGUI: SHAPE: Parameters for circle shape is less than expected (7), shape string format: %s", shapeFormat)
            CguiShapeCircle *circleShape = MemAlloc(sizeof(CguiShapeCircle));
            circleShape->iconShapeType = CGUI_ICON_SHAPE_CIRCLE;
            circleShape->color.r = param[0];
            circleShape->color.g = param[1];
            circleShape->color.b = param[2];
            circleShape->color.a = param[3];
            circleShape->center.x = param[4];
            circleShape->center.y = param[5];
            circleShape->radius = param[6];
            if (paramCount >= 8) circleShape->lineThick = param[7];
            else circleShape->lineThick = 0.0f;
            if (paramCount >= 9) circleShape->startAngle = param[8];
            else circleShape->startAngle = 0.0f;
            if (paramCount >= 10) circleShape->endAngle = param[9];
            else circleShape->endAngle = 0.0f;
            if (paramCount >= 11) circleShape->segments = param[10];
            else circleShape->segments = 36.0f;
            shape = (CguiShapeBasic *)circleShape;
        }
        break;

        // Ellipse shape
        case 'e':
        {
            CGUI_LOG_CONDITION(paramCount < 8, break, LOG_ERROR, "CGUI: SHAPE: Parameters for ellipse shape is less than expected (8), shape string format: %s", shapeFormat)
            CguiShapeEllipse *ellipseShape = MemAlloc(sizeof(CguiShapeEllipse));
            ellipseShape->iconShapeType = CGUI_ICON_SHAPE_ELLIPSE;
            ellipseShape->color.r = param[0];
            ellipseShape->color.g = param[1];
            ellipseShape->color.b = param[2];
            ellipseShape->color.a = param[3];
            ellipseShape->center.x = param[4];
            ellipseShape->center.y = param[5];
            ellipseShape->radiusH = param[6];
            ellipseShape->radiusV = param[7];
            if (paramCount >= 9) ellipseShape->lineThick = param[8];
            else ellipseShape->lineThick = 0.0f;
            shape = (CguiShapeBasic *)ellipseShape;
        }
        break;

        // Rectangle shape
        case 'r':
        {
            CGUI_LOG_CONDITION(paramCount < 8, break, LOG_ERROR, "CGUI: SHAPE: Parameters for rectangle shape is less than expected (8), shape string format: %s", shapeFormat)
            CguiShapeRectangle *rectangleShape = MemAlloc(sizeof(CguiShapeRectangle));
            rectangleShape->iconShapeType = CGUI_ICON_SHAPE_RECTANGLE;
            rectangleShape->color.r = param[0];
            rectangleShape->color.g = param[1];
            rectangleShape->color.b = param[2];
            rectangleShape->color.a = param[3];
            rectangleShape->rec.x = param[4];
            rectangleShape->rec.y = param[5];
            rectangleShape->rec.width = param[6];
            rectangleShape->rec.height = param[7];
            if (paramCount >= 9) rectangleShape->rotation = param[8];
            else rectangleShape->rotation = 0.0f;
            if (paramCount >= 10) rectangleShape->lineThick = param[9];
            else rectangleShape->lineThick = 0.0f;
            if (paramCount >= 11) rectangleShape->roundness = param[10];
            else rectangleShape->roundness = 0.0f;
            shape = (CguiShapeBasic *)rectangleShape;
        }
        break;

        // Triangle shape
        case 't':
        {
            CGUI_LOG_CONDITION(paramCount < 10, break, LOG_ERROR, "CGUI: SHAPE: Parameters for triangle shape is less than expected (10), shape string format: %s", shapeFormat)
            CguiShapeTriangle *triangleShape = MemAlloc(sizeof(CguiShapeTriangle));
            triangleShape->iconShapeType = CGUI_ICON_SHAPE_TRIANGLE;
            triangleShape->color.r = param[0];
            triangleShape->color.g = param[1];
            triangleShape->color.b = param[2];
            triangleShape->color.a = param[3];
            triangleShape->v1.x = param[4];
            triangleShape->v1.y = param[5];
            triangleShape->v2.x = param[6];
            triangleShape->v2.y = param[7];
            triangleShape->v3.x = param[8];
            triangleShape->v3.y = param[9];
            if (paramCount >= 11) triangleShape->lineThick = param[10];
            else triangleShape->lineThick = 0.0f;
            shape = (CguiShapeBasic *)triangleShape;
        }
        break;

        // Regular polygon shape
        case 'p':
        {
            CGUI_LOG_CONDITION(paramCount < 8, break, LOG_ERROR, "CGUI: SHAPE: Parameters for regular polygon shape is less than expected (8), shape string format: %s", shapeFormat)
            CguiShapePoly *polyShape = MemAlloc(sizeof(CguiShapePoly));
            polyShape->iconShapeType = CGUI_ICON_SHAPE_POLY;
            polyShape->color.r = param[0];
            polyShape->color.g = param[1];
            polyShape->color.b = param[2];
            polyShape->color.a = param[3];
            polyShape->center.x = param[4];
            polyShape->center.y = param[5];
            polyShape->sides = param[6];
            polyShape->radius = param[7];
            if (paramCount >= 9) polyShape->rotation = param[8];
            else polyShape->rotation = 0.0f;
            if (paramCount >= 10) polyShape->lineThick = param[9];
            else polyShape->lineThick = 0.0f;
            shape = (CguiShapeBasic *)polyShape;
        }
        break;

        default:
        {
            CGUI_LOG(LOG_WARNING, "CGUI: SHAPE: Unknown shape specifier: %c", shapeChar);
        }
        break;
    }

    MemFree(param);
    __CguiSplitFree(splitString);
    CGUI_LOG_NOID("SHAPE", "Successfully generated Shape");
    return shape;
}

// Dispose the generate shape
void CguiDisposeShape(CguiShapeBasic *shape)
{
    CGUI_CHECK_VALID(shape, , );

    // Useless to check shape type... (?)
    switch (shape->iconShapeType)
    {
        case CGUI_ICON_SHAPE_LINE: MemFree((CguiShapeLine *)shape); break;
        case CGUI_ICON_SHAPE_CIRCLE: MemFree((CguiShapeCircle *)shape); break;
        case CGUI_ICON_SHAPE_ELLIPSE: MemFree((CguiShapeEllipse *)shape); break;
        case CGUI_ICON_SHAPE_RECTANGLE: MemFree((CguiShapeRectangle *)shape); break;
        case CGUI_ICON_SHAPE_TRIANGLE: MemFree((CguiShapeTriangle *)shape); break;
        case CGUI_ICON_SHAPE_POLY: MemFree((CguiShapePoly *)shape); break;
        default: break;
    }

    CGUI_LOG_NOID("SHAPE", "Successfully disposed Shape");
}

// Draw the shape at position with scaling
void CguiDrawShape(CguiShapeBasic *shape, Vector2 position, float scaling)
{
    CGUI_CHECK_VALID(shape, , );

    switch (shape->iconShapeType)
    {
        case CGUI_ICON_SHAPE_LINE:
        {
            CguiShapeLine *lineShape = (CguiShapeLine *)shape;
            DrawLineEx((Vector2) { position.x + lineShape->startPos.x * scaling, position.y + lineShape->startPos.y * scaling }, (Vector2) { position.x + lineShape->endPos.x * scaling, position.y + lineShape->endPos.y * scaling }, lineShape->lineThick * scaling, lineShape->color);
        }
        break;
        case CGUI_ICON_SHAPE_CIRCLE:
        {
            CguiShapeCircle *circleShape = (CguiShapeCircle *)shape;
            if (circleShape->lineThick == 0.0f) DrawCircleV((Vector2) { position.x + circleShape->center.x * scaling, position.y + circleShape->center.y * scaling }, circleShape->radius * scaling, circleShape->color);
            else DrawRing((Vector2) { position.x + circleShape->center.x * scaling, position.y + circleShape->center.y * scaling }, (circleShape->radius - circleShape->lineThick / 2.0f) * scaling, (circleShape->radius + circleShape->lineThick / 2.0f) * scaling, circleShape->startAngle, circleShape->endAngle, circleShape->segments, circleShape->color);
        }
        break;
        case CGUI_ICON_SHAPE_ELLIPSE:
        {
            CguiShapeEllipse *ellipseShape = (CguiShapeEllipse *)shape;
            if (ellipseShape->lineThick == 0.0f) DrawEllipse(position.x + ellipseShape->center.x * scaling, position.y + ellipseShape->center.y * scaling, ellipseShape->radiusH * scaling, ellipseShape->radiusV * scaling, ellipseShape->color);
            else DrawEllipseLinesEx(position.x + ellipseShape->center.x * scaling, position.y + ellipseShape->center.y * scaling, ellipseShape->radiusH * scaling, ellipseShape->radiusV * scaling, ellipseShape->lineThick * scaling, ellipseShape->color);
        }
        break;
        case CGUI_ICON_SHAPE_RECTANGLE:
        {
            // Currently a bit broken (rotation)
            CguiShapeRectangle *rectangleShape = (CguiShapeRectangle *)shape;
            float computedRoundness = rectangleShape->roundness / fminf(rectangleShape->rec.width, rectangleShape->rec.height);
            if (rectangleShape->lineThick == 0.0f) DrawRectangleRoundedEx((Rectangle) { position.x + rectangleShape->rec.x * scaling, position.y + rectangleShape->rec.y * scaling, rectangleShape->rec.width * scaling, rectangleShape->rec.height * scaling }, computedRoundness, 9, rectangleShape->rotation, rectangleShape->color);
            else DrawRectangleRoundedLinesEx((Rectangle) { position.x + rectangleShape->rec.x * scaling, position.y + rectangleShape->rec.y * scaling, rectangleShape->rec.width * scaling, rectangleShape->rec.height * scaling }, computedRoundness, 9, rectangleShape->rotation, rectangleShape->lineThick * scaling, rectangleShape->color);
        }
        break;
        case CGUI_ICON_SHAPE_TRIANGLE:
        {
            // Currently a bit broken (lines)
            CguiShapeTriangle *triangleShape = (CguiShapeTriangle *)shape;
            if (triangleShape->lineThick == 0.0f) DrawTriangle((Vector2) { position.x + triangleShape->v1.x * scaling, position.y + triangleShape->v1.y * scaling }, (Vector2) { position.x + triangleShape->v2.x * scaling, position.y + triangleShape->v2.y * scaling }, (Vector2) { position.x + triangleShape->v3.x * scaling, position.y + triangleShape->v3.y * scaling }, triangleShape->color);
            else DrawTriangleLinesEx((Vector2) { position.x + triangleShape->v1.x * scaling, position.y + triangleShape->v1.y * scaling }, (Vector2) { position.x + triangleShape->v2.x * scaling, position.y + triangleShape->v2.y * scaling }, (Vector2) { position.x + triangleShape->v3.x * scaling, position.y + triangleShape->v3.y * scaling }, triangleShape->lineThick * scaling, triangleShape->color);
        }
        break;
        case CGUI_ICON_SHAPE_POLY:
        {
            CguiShapePoly *polyShape = (CguiShapePoly *)shape;
            if (polyShape->lineThick == 0.0f) DrawPoly((Vector2) { position.x + polyShape->center.x * scaling, position.y + polyShape->center.y * scaling }, polyShape->sides, polyShape->radius * scaling, polyShape->rotation, polyShape->color);
            else DrawPolyLinesEx((Vector2) { position.x + polyShape->center.x * scaling, position.y + polyShape->center.y * scaling }, polyShape->sides, polyShape->radius * scaling, polyShape->rotation, polyShape->lineThick * scaling, polyShape->color);
        }
        break;
        default: break;
    }
}

// Generate icon with formatted string, see format above
CguiIcon CguiGenerateIcon(const char *iconShapesFormat)
{
    CGUI_CHECK_VALID_LOG("ICON", iconShapesFormat, "Failed to generate Icon due to invalid Icon Shapes Format", , (CguiIcon) { 0 });
    CguiIcon icon = { 0 };

    struct __CguiSplitStringT *splitString = __CguiSplitString(iconShapesFormat, ";");
    struct __CguiSplitStringT *splitStringCurrent = splitString;
    icon.shapesCount = __CguiSplitCount(splitString);
    ;
    icon.shapes = MemAlloc(sizeof(CguiShapeBasic *) * icon.shapesCount);
    CGUI_CHECK_VALID_LOG("ICON", icon.shapes, "Failed to allocate memory for Shapes", __CguiSplitFree(splitString), (CguiIcon) { 0 });

    int i = 0;
    while (splitStringCurrent)
    {
        CGUI_LOG(LOG_INFO, "CGUI: ICON: Shape Format String: %s", splitStringCurrent->str);
        icon.shapes[i++] = CguiGenerateShape(splitStringCurrent->str);
        splitStringCurrent = splitStringCurrent->next;
    }

    __CguiSplitFree(splitString);
    CGUI_LOG_NOID("ICON", "Successfully generated Icon");
    return icon;
}

// Dispose the generated icon
void CguiDisposeIcon(CguiIcon icon)
{
    CGUI_CHECK_VALID_LOG("ICON", icon.shapes, "Failed to dispose Icon due to invalid Icon", , );
    for (int i = 0; i < icon.shapesCount; i++) CguiDisposeShape(icon.shapes[i]);
    MemFree(icon.shapes);
    CGUI_LOG_NOID("ICON", "Successfully disposed Icon");
}

// Draw the icon at position and scaling
void CguiDrawIcon(CguiIcon icon, Vector2 position, float scaling)
{
    CGUI_CHECK_VALID(icon.shapes, , );

    // Draw all the shapes accordingly
    for (int i = 0; i < icon.shapesCount; i++) CguiDrawShape(icon.shapes[i], position, scaling);
}

// Generate an icon texture with scaling
CguiIconTexture CguiGenerateIconTexture(CguiIcon icon, float scaling)
{
    static unsigned int idCounter = 0;
    RenderTexture renderTexture = LoadRenderTexture(scaling, scaling);
    CGUI_CHECK_VALID_LOG("ICON TEXTURE", renderTexture.id, "Failed to load Icon Render Texture", , (CguiIconTexture) { 0 });

    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);
    CguiDrawIcon(icon, (Vector2) { 0.0f, 0.0f }, scaling);
    EndTextureMode();

    CguiIconTexture iconTexture = { 0 };
    iconTexture.texture = LoadTextureFromRenderTexture(renderTexture);
    UnloadRenderTexture(renderTexture);

    CGUI_CHECK_VALID_LOG("ICON TEXTURE", iconTexture.texture.id, "Failed to load Icon Texture", , (CguiIconTexture) { 0 });
    iconTexture.id = ++idCounter;
    CGUI_LOG_ID("ICON TEXTURE", iconTexture.id, "Successfully generated Icon Texture");
    return iconTexture;
}

// Dispose the generated icon texture
void CguiDisposeIconTexture(CguiIconTexture iconTexture)
{
    CGUI_CHECK_VALID_LOG("ICON TEXTURE", iconTexture.texture.id, "Failed to dispose Icon Texture due to invalid Texture", , );
    int id = iconTexture.id;
    UnloadTexture(iconTexture.texture);
    CGUI_LOG_ID("ICON TEXTURE", id, "Successfully disposed Icon Texture");
}

// Draw the generated icon texture at position
void CguiDrawIconTexture(CguiIconTexture iconTexture, Vector2 position)
{
    DrawTextureEx(iconTexture.texture, position, 0.0f, 1.0f, WHITE);
}

// Draw the generated icon texture with extended parameter
void CguiDrawIconTextureEx(CguiIconTexture iconTexture, Rectangle dest, Color tint)
{
    DrawTextureDest(iconTexture.texture, dest, tint);
}

// Generate multiple icons with formatted string, see format above
CguiAnimatedIcon CguiGenerateAnimatedIcon(const char *animatedIconFormat, float scaling, float animationDuration)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("ANIMATED ICON", animatedIconFormat, "Failed to generate Animated Icon due to invalid Animated Icon Shape Format", , (CguiAnimatedIcon) { 0 });
    CguiAnimatedIcon animatedIcon = { 0 };

    struct __CguiSplitStringT *splitString = __CguiSplitString(animatedIconFormat, "&");
    struct __CguiSplitStringT *splitStringCurrent = splitString;
    animatedIcon.animatedTexturesCount = __CguiSplitCount(splitString);
    animatedIcon.animationDuration = animationDuration;
    animatedIcon.animatedTextures = MemAlloc(sizeof(Texture) * animatedIcon.animatedTexturesCount);
    CGUI_CHECK_VALID_LOG("ANIMATED ICON", animatedIcon.animatedTextures, "Failed to allocate memory for Animated Icon Textures", , (CguiAnimatedIcon) { 0 });

    int i = 0;
    while (splitStringCurrent)
    {
        CGUI_LOG(LOG_INFO, "CGUI: ANIMATED ICON: Icon Format String: %s", splitStringCurrent->str);
        CguiIcon icon = CguiGenerateIcon(splitStringCurrent->str);
        animatedIcon.animatedTextures[i++] = CguiGenerateIconTexture(icon, scaling);
        CguiDisposeIcon(icon);
        splitStringCurrent = splitStringCurrent->next;
    }

    __CguiSplitFree(splitString);
    animatedIcon.id = ++idCounter;
    CGUI_LOG_ID("ANIMATED ICON", animatedIcon.id, "Successfully generated Animated Icon");
    return animatedIcon;
}

// Dispose the generated animated icon
void CguiDisposeAnimatedIcon(CguiAnimatedIcon animatedIcon)
{
    CGUI_CHECK_VALID_LOG("ANIMATED ICON", animatedIcon.animatedTextures, "Failed to dispose Animated Icon due to invalid Animated Icon", , );
    unsigned int id = animatedIcon.id;
    for (int i = 0; i < animatedIcon.animatedTexturesCount; i++) CguiDisposeIconTexture(animatedIcon.animatedTextures[i]);
    MemFree(animatedIcon.animatedTextures);
    CGUI_LOG_ID("ANIMATED ICON", id, "Successfully disposed Animated Icon");
}

// Draw animated icon
void CguiDrawAnimatedIcon(CguiAnimatedIcon animatedIcon, Vector2 position, float startTime)
{
    CGUI_CHECK_VALID(animatedIcon.animatedTextures, , );
    float animationTime = fmodf(GetTime() - startTime, animatedIcon.animationDuration);
    int index = (int)(roundf(animationTime / animatedIcon.animationDuration) * animatedIcon.animatedTexturesCount);
    CguiDrawIconTexture(animatedIcon.animatedTextures[index], position);
}

// Draw animated icon with extended parameter
void CguiDrawAnimatedIconEx(CguiAnimatedIcon animatedIcon, Rectangle dest, Color tint, float startTime)
{
    CGUI_CHECK_VALID(animatedIcon.animatedTextures, , );
    float animationTime = fmodf(GetTime() - startTime, animatedIcon.animationDuration);
    int index = (int)(roundf(animationTime / animatedIcon.animationDuration) * animatedIcon.animatedTexturesCount);
    CguiDrawIconTextureEx(animatedIcon.animatedTextures[index], dest, tint);
}
