// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "cgui_shape.h"
#include "cgui_extra.h"
#include "cgui_transition.h"
#include "raymath.h"

CguiIRectangle CguiRecToIRec(Rectangle rec) {
    return (CguiIRectangle){
        .x      = floorf(rec.x),
        .y      = floorf(rec.y),
        .width  = ceilf(rec.width),
        .height = ceilf(rec.height)
    };
}

CguiIVector2 CguiVec2ToIVec2(Vector2 vec) {
    return (CguiIVector2){
        .x = roundf(vec.x),
        .y = roundf(vec.y)
    };
}

Rectangle CguiIRecToRec(CguiIRectangle rec) {
    return (Rectangle){
        .x      = rec.x,
        .y      = rec.y,
        .width  = rec.width,
        .height = rec.height
    };
}

Vector2 CguiIVec2ToVec2(CguiIVector2 vec) {
    return (Vector2){
        .x = vec.x,
        .y = vec.y
    };
}

bool CguiIsIRecEqual(CguiIRectangle a, CguiIRectangle b) {
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

bool CguiIsIVec2Equal(CguiIVector2 a, CguiIVector2 b) {
    return a.x == b.x && a.y == b.y;
}

void CguiTransformGeom(CguiGeom *geom, CguiMatrix3 mat) {
    if (!geom) return;

    CGUI_ARRAY_FOR_EACH(Vector2, geom->vertices, vertex) {
        (*vertex) = CguiMatrix3MultiplyV(mat, vertex->x, vertex->y);
    }
}

Image CguiRasterizeGeom(CguiComputeGeom geom, CguiRasterizer rasterizer, void *data) {
    Image image = GenImageColor(geom.rec.width, geom.rec.height, BLANK);

    for (int x = 0; x < geom.rec.width; x++) {
        for (int y = 0; y < geom.rec.height; y++) {
            int   index = y * geom.rec.width + x;
            float dist  = geom.fields[index];

            CguiRasterParam param = {
                .posX = x,
                .posY = y,
                .dist = dist
            };

            ImageDrawPixel(&image, x, y, rasterizer(data, param));
        }
    }

    return image;
}

/*

// Old approach: inverse distance weighting with all stops influencing each other
// Reason for removal: Too expensive to compute, and too much influence of distant stops

Color CguiResolveGradientFill(CguiGradientFill *fill, Vector2 point) {
    if (!fill || fill->stopsCount == 0) return BLANK;
    if (fill->stopsCount == 1) return fill->stops[0].color;

    float weights[CGUI_MAX_GRADIENT_STOPS] = {0};

    float totalWeight = 0.0f;
    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stop) {
        weights[stopIndex] = 1.0f / (1.0f + Vector2Distance(point, stop->position));
        totalWeight += weights[stopIndex];
    }

    if (totalWeight <= 0.0f) return BLANK;

    Vector4 color = {0};

    float hueX = 0.0f;
    float hueY = 0.0f;

    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stop) {
        float weight = weights[stopIndex] / totalWeight;

        Vector4 slopColor = CguiColorNormalA(stop->color);
        color             = Vector4Add(color, Vector4Scale(slopColor, weight));
    }

    return CguiColorDenormalA(color);
}

*/

/*

// Old approach: average of all linear gradient pairs
// Reason for removal: still too expensive to compute, and still too much influence of distant stops

Color CguiResolveGradientFill(CguiGradientFill *fill, Vector2 point) {
    if (!fill || fill->stopsCount == 0) return BLANK;
    if (fill->stopsCount == 1) return fill->stops[0].color;

    Vector4 color      = {0};
    int     pairsCount = 0;

    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stopA) {
        CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stopB) {
            if (stopBIndex <= stopAIndex) continue;

            Vector2 line   = Vector2Subtract(stopB->position, stopA->position);
            Vector2 offset = Vector2Subtract(point, stopA->position);

            float factor = Clamp(Vector2DotProduct(offset, line) / Vector2LengthSqr(line), 0.0f, 1.0f);

            Vector4 colorA = CguiColorNormalA(stopA->color);
            Vector4 colorB = CguiColorNormalA(stopB->color);

            Vector4 pairColor = Vector4Lerp(colorA, colorB, factor);

            color = Vector4Add(color, pairColor);
            pairsCount++;
        }
    }

    color = Vector4Scale(color, 1.0f / pairsCount);

    return CguiColorDenormalA(color);
}

*/

/*

// Old approach: average of nearest stop's linear gradient pairs
// Reason for removal: hard edges when multiple stops are introduced

Color CguiResolveGradientFill(CguiGradientFill *fill, Vector2 point) {
    if (!fill || fill->stopsCount == 0) return BLANK;
    if (fill->stopsCount == 1) return fill->stops[0].color;

    int   closestIndex = 0;
    float closestDist  = INFINITY;

    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stop) {
        float dist = Vector2Distance(point, stop->position);

        if (dist < closestDist) {
            closestIndex = stopIndex;
            closestDist  = dist;
        }
    }

    Vector4 color       = {0};
    float   totalWeight = 0.0f;

    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stop) {
        if (stopIndex == closestIndex) continue;

        Vector2 line = Vector2Subtract(stop->position, fill->stops[closestIndex].position);
        Vector2 offset = Vector2Subtract(point, fill->stops[closestIndex].position);

        float factor = Clamp(Vector2DotProduct(offset, line) / Vector2LengthSqr(line), 0.0f, 1.0f);

        Vector4 colorA = CguiColorNormalA(fill->stops[closestIndex].color);
        Vector4 colorB = CguiColorNormalA(stop->color);

        Vector4 pairColor = Vector4Lerp(colorA, colorB, factor);

        color = Vector4Add(color, pairColor);
        totalWeight += 1.0f;
    }

    color = Vector4Scale(color, 1.0f / totalWeight);

    return CguiColorDenormalA(color);
}

*/

// Current approach: pairwise influence with inverse distance weighting

Color CguiResolveGradientFill(CguiGradientFill *fill, Vector2 point) {
    if (!fill || fill->stopsCount == 0) return BLANK;
    if (fill->stopsCount == 1) return fill->stops[0].color;

    Vector4 color       = {0};
    float   totalWeight = 0.0f;

    CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stopA) {
        float distSqr    = Vector2LengthSqr(Vector2Subtract(point, stopA->position));
        float baseWeight = 1.0f / (1.0f + distSqr * distSqr);

        CGUI_ARRAY_FOR_EACH(CguiGradientStop, fill->stops, stopB) {
            if (stopBIndex == stopAIndex) continue;

            Vector2 ab = Vector2Subtract(stopB->position, stopA->position);
            Vector2 ap = Vector2Subtract(point, stopA->position);

            float t = Vector2DotProduct(ap, ab) / Vector2LengthSqr(ab);
            t       = Clamp(t, 0.0f, 1.0f);

            Vector4 ca = CguiColorNormalA(stopA->color);
            Vector4 cb = CguiColorNormalA(stopB->color);

            Vector4 pairColor = Vector4Lerp(ca, cb, t);

            color = Vector4Add(color, Vector4Scale(pairColor, baseWeight));
            totalWeight += baseWeight;
        }
    }

    color = Vector4Scale(color, 1.0f / totalWeight);

    return CguiColorDenormalA(color);
}

Color CguiResolveImageFill(CguiImageFill *fill, Vector2 point) {
    if (!fill) return BLANK;

    Vector2 imagePoint = CguiMatrix3MultiplyV(fill->transform, point.x, point.y);

    switch (fill->wrapX) {
    case CGUI_IMAGE_WRAP_CLAMP_TO_EDGE:
        imagePoint.x = Clamp(imagePoint.x, 0.0f, fill->image.width - 1.0f);
        break;
    case CGUI_IMAGE_WRAP_REPEAT:
        imagePoint.x = CguiWrapModF(imagePoint.x, fill->image.width - 1.0f);
        break;
    case CGUI_IMAGE_WRAP_MIRRORED_REPEAT:
        imagePoint.x = CguiMirrorModF(imagePoint.x, fill->image.width - 1.0f);
        break;
    }

    switch (fill->wrapY) {
    case CGUI_IMAGE_WRAP_CLAMP_TO_EDGE:
        imagePoint.y = Clamp(imagePoint.y, 0.0f, fill->image.height - 1.0f);
        break;
    case CGUI_IMAGE_WRAP_REPEAT:
        imagePoint.y = CguiWrapModF(imagePoint.y, fill->image.height - 1.0f);
        break;
    case CGUI_IMAGE_WRAP_MIRRORED_REPEAT:
        imagePoint.y = CguiMirrorModF(imagePoint.y, fill->image.height - 1.0f);
        break;
    }

    imagePoint.x = roundf(imagePoint.x);
    imagePoint.y = roundf(imagePoint.y);

    if (imagePoint.x < 0.0f || imagePoint.x > fill->image.width - 1.0f || imagePoint.y < 0.0f || imagePoint.y > fill->image.height - 1.0f) {
        return fill->fallback;
    }

    return GetImageColor(fill->image, imagePoint.x, imagePoint.y);
}

Color CguiResolveRasterFill(CguiRasterFill *fill, Vector2 point) {
    if (!fill) return BLANK;

    switch (fill->fillType) {
    case CGUI_RASTER_COLOR:
        return fill->color;
    case CGUI_RASTER_GRADIENT:
        return CguiResolveGradientFill(&fill->gradient, point);
    case CGUI_RASTER_IMAGE:
        return CguiResolveImageFill(&fill->image, point);
    default: break;
    }

    return BLANK;
}

Color CguiDefaultRasterizer(void *data, CguiRasterParam param) {
    if (!data) return BLANK;

    Vector2 point = (Vector2){param.posX, param.posY};

    CguiRasterData *rasterData = (CguiRasterData *)data;

    Color finalColor = BLANK;

    // Base
    Color outerColor = CguiResolveRasterFill(&rasterData->outerFill, point);
    Color innerColor = CguiResolveRasterFill(&rasterData->innerFill, point);
    float baseFactor = Clamp(param.dist, -0.5f, 0.5f) + 0.5f;
    Color baseColor  = CguiLerpColor(outerColor, innerColor, baseFactor);
    finalColor       = baseColor;

    // Border
    if (rasterData->borderThickness > 0.0f) {
        float borderAlpha = Clamp(fabsf(param.dist + rasterData->borderShift) - rasterData->borderThickness, 0.0f, 1.0f);
        Color borderColor = CguiResolveRasterFill(&rasterData->borderFill, point);
        borderColor.a *= borderAlpha;
        finalColor = CugiColorAlphaTint(finalColor, borderColor);
    }

    // Shadow
    if (rasterData->shadowRadius > 0.0f) {
        float shadowAlpha = Clamp(param.dist - rasterData->shadowRadius, 0.0f, 1.0f);
        Color shadowColor = CguiResolveRasterFill(&rasterData->shadowFill, point);
        shadowColor.a *= shadowAlpha;
        finalColor = CugiColorAlphaTint(finalColor, shadowColor);
    }

    return finalColor;
}

Image CguiRasterFilled(CguiComputeGeom geom, Color color) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_COLOR, .color = color},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterFilledImg(CguiComputeGeom geom, Image image, Color tint) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_IMAGE, .image = {.image = image, .transform = CguiMatrix3Identity(), .wrapX = CGUI_IMAGE_WRAP_NO_WRAP, .wrapY = CGUI_IMAGE_WRAP_NO_WRAP, .tint = tint, .fallback = BLANK}},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterFilledGrdV(CguiComputeGeom geom, Color start, Color end) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_GRADIENT, .gradient = {.stopsCount = 2, .stops = {{.position = {0, 0}, .color = start}, {.position = {0, geom.rec.height}, .color = end}}}},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterFilledGrdH(CguiComputeGeom geom, Color start, Color end) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_GRADIENT, .gradient = {.stopsCount = 2, .stops = {{.position = {0, 0}, .color = start}, {.position = {geom.rec.width, 0}, .color = end}}}},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterFilledGrdD(CguiComputeGeom geom, Color start, Color end) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_GRADIENT, .gradient = {.stopsCount = 2, .stops = {{.position = {0, 0}, .color = start}, {.position = {geom.rec.width, geom.rec.height}, .color = end}}}},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterFilledGrdC(CguiComputeGeom geom, Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_GRADIENT, .gradient = {.stopsCount = 4, .stops = {{.position = {0, 0}, .color = topLeft}, {.position = {geom.rec.width, 0}, .color = topRight}, {.position = {0, geom.rec.height}, .color = bottomLeft}, {.position = {geom.rec.width, geom.rec.height}, .color = bottomRight}}}},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterBorder(CguiComputeGeom geom, Color border, float thickness, float shift) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = border},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderThickness = thickness,
        .borderShift     = shift,
        .shadowRadius    = 0.0f
    };

    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

Image CguiRasterShadow(CguiComputeGeom geom, Color shadow, float radius) {
    CguiRasterData rasterData = {
        .outerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .innerFill       = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .borderFill      = {.fillType = CGUI_RASTER_COLOR, .color = BLANK},
        .shadowFill      = {.fillType = CGUI_RASTER_COLOR, .color = shadow},
        .borderThickness = 0.0f,
        .borderShift     = 0.0f,
        .shadowRadius    = radius
    };
    return CguiRasterizeGeom(geom, CguiDefaultRasterizer, &rasterData);
}

CguiGeom CguiCreateLineGeom(Vector2 first, Vector2 second) {
    CguiGeom geom = {0};

    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, first);
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, second);

    return geom;
}

CguiGeom CguiCreateRecGeom(Rectangle rec) {
    CguiGeom geom = {0};

    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, ((Vector2){rec.x, rec.y}));
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, ((Vector2){rec.x + rec.width, rec.y}));
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, ((Vector2){rec.x + rec.width, rec.y + rec.height}));
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, ((Vector2){rec.x, rec.y + rec.height}));

    return geom;
}

CguiGeom CguiCreateRoundedRecGeom(Rectangle rec, Vector4 cornerRadius) {
    CguiGeom geom = {0};

    // Calculate segments of the corners
    int cornerSegments[] = {
        ceilf(cornerRadius.x * PI / 2.0f),
        ceilf(cornerRadius.y * PI / 2.0f),
        ceilf(cornerRadius.z * PI / 2.0f),
        ceilf(cornerRadius.w * PI / 2.0f)
    };

    for (int c = 0; c < 4; c++) {
        // Evil
        float radius = ((float *)&cornerRadius)[c];

        int segments = cornerSegments[c];

        Vector2 center = {0};
        switch (c) {
        case 0: center = (Vector2){rec.x + cornerRadius.x, rec.y + cornerRadius.x}; break;
        case 1: center = (Vector2){rec.x + rec.width - cornerRadius.y, rec.y + cornerRadius.y}; break;
        case 2: center = (Vector2){rec.x + rec.width - cornerRadius.z, rec.y + rec.height - cornerRadius.z}; break;
        case 3: center = (Vector2){rec.x + cornerRadius.w, rec.y + rec.height - cornerRadius.w}; break;
        }

        for (int s = 0; s < segments; s++) {
            float angle = (float)s / (segments - 1) * PI / 2.0f + c * PI / 2.0f;

            Vector2 point = {
                center.x + cosf(angle) * radius,
                center.y + sinf(angle) * radius
            };

            CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
        }
    }

    return geom;
}

CguiGeom CguiCreateCircleGeom(Vector2 center, float radius) {
    CguiGeom geom = {0};

    int segments = ceilf(radius * PI * 2.0f);

    for (int s = 0; s < segments; s++) {
        float angle = (float)s / segments * PI * 2.0f;

        Vector2 point = {
            center.x + cosf(angle) * radius,
            center.y + sinf(angle) * radius
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    return geom;
}

CguiGeom CguiCreateCircleArcGeom(Vector2 center, float radius, float startAngle, float endAngle) {
    CguiGeom geom = {0};

    float angleRange = endAngle - startAngle;
    int   segments   = ceilf(radius * angleRange);

    for (int s = 0; s < segments; s++) {
        float angle = (float)s / (segments - 1) * angleRange + startAngle;

        Vector2 point = {
            center.x + cosf(angle) * radius,
            center.y + sinf(angle) * radius
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    // A reverse arc to form a closed shape
    for (int s = segments - 1; s >= 0; s--) {
        float angle = (float)s / (segments - 1) * angleRange + startAngle;

        Vector2 point = {
            center.x + cosf(angle) * radius,
            center.y + sinf(angle) * radius
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    return geom;
}

CguiGeom CguiCreateEllipseGeom(Vector2 center, Vector2 radius) {
    CguiGeom geom = {0};

    int segments = ceilf(fmaxf(radius.x, radius.y) * PI * 2.0f);

    for (int s = 0; s < segments; s++) {
        float angle = (float)s / segments * PI * 2.0f;

        Vector2 point = {
            center.x + cosf(angle) * radius.x,
            center.y + sinf(angle) * radius.y
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    return geom;
}

CguiGeom CguiCreateEllipseArcGeom(Vector2 center, Vector2 radius, float startAngle, float endAngle) {
    CguiGeom geom = {0};

    float angleRange = endAngle - startAngle;
    int   segments   = ceilf(fmaxf(radius.x, radius.y) * angleRange);

    for (int s = 0; s < segments; s++) {
        float angle = (float)s / (segments - 1) * angleRange + startAngle;

        Vector2 point = {
            center.x + cosf(angle) * radius.x,
            center.y + sinf(angle) * radius.y
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    // A reverse arc to form a closed shape
    for (int s = segments - 1; s >= 0; s--) {
        float angle = (float)s / (segments - 1) * angleRange + startAngle;

        Vector2 point = {
            center.x + cosf(angle) * radius.x,
            center.y + sinf(angle) * radius.y
        };

        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
    }

    return geom;
}

CguiGeom CguiCreateTriangleGeom(Vector2 v1, Vector2 v2, Vector2 v3) {
    CguiGeom geom = {0};

    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, v1);
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, v2);
    CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, v3);

    return geom;
}

CguiGeom CguiCreateRoundedTriangleGeom(Vector2 v1, Vector2 v2, Vector2 v3, Vector3 cornerRadius) {
    CguiGeom geom = {0};

    Vector2 vertices[3] = {v1, v2, v3};

    for (int c = 0; c < 3; c++) {
        // More evil
        float radius = ((float *)&cornerRadius)[c];

        Vector2 current = vertices[CguiWrapModI(c + 0, 3)];
        Vector2 prev    = vertices[CguiWrapModI(c - 1, 3)];
        Vector2 next    = vertices[CguiWrapModI(c + 1, 3)];

        Vector2 dirToPrev = Vector2Normalize(Vector2Subtract(prev, current));
        Vector2 dirToNext = Vector2Normalize(Vector2Subtract(next, current));

        Vector2 bisector = Vector2Normalize(Vector2Add(dirToPrev, dirToNext));

        float cosAngle     = Vector2DotProduct(dirToPrev, dirToNext);
        float sinHalfAngle = sqrtf((1.0f - cosAngle) / 2.0f);
        float tanHalfAngle = sqrtf((1.0f - cosAngle) / (1.0f + cosAngle));

        float dist   = radius / sinHalfAngle;
        float offset = radius / tanHalfAngle;

        Vector2 center = Vector2Add(current, Vector2Scale(bisector, dist));

        Vector2 startPoint = Vector2Add(current, Vector2Scale(dirToPrev, offset));
        Vector2 endPoint   = Vector2Add(current, Vector2Scale(dirToNext, offset));

        float startAngle = atan2f(startPoint.y - center.y, startPoint.x - center.x);
        float endAngle   = atan2f(endPoint.y - center.y, endPoint.x - center.x);

        if (endAngle < startAngle) {
            endAngle += PI * 2.0f;
        }

        int segments = ceilf(radius * (endAngle - startAngle));

        for (int s = 0; s < segments; s++) {
            float angle = (float)s / (segments - 1) * (endAngle - startAngle) + startAngle;

            Vector2 point = {
                center.x + cosf(angle) * radius,
                center.y + sinf(angle) * radius
            };

            CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
        }
    }

    return geom;
}

CguiGeom CguiCreatePolygonGeom(const Vector2 *vertices, int verticesCount) {
    CguiGeom geom = {0};

    for (int i = 0; i < verticesCount; i++) {
        CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, vertices[i]);
    }

    return geom;
}

CguiGeom CguiCreateRoundedPolygonGeom(const Vector2 *vertices, const float *cornerRadii, int verticesCount) {
    CguiGeom geom = {0};

    for (int c = 0; c < verticesCount; c++) {
        float radius = cornerRadii[c];

        Vector2 current = vertices[CguiWrapModI(c + 0, verticesCount)];
        Vector2 prev    = vertices[CguiWrapModI(c - 1, verticesCount)];
        Vector2 next    = vertices[CguiWrapModI(c + 1, verticesCount)];

        Vector2 dirToPrev = Vector2Normalize(Vector2Subtract(prev, current));
        Vector2 dirToNext = Vector2Normalize(Vector2Subtract(next, current));

        Vector2 bisector = Vector2Normalize(Vector2Add(dirToPrev, dirToNext));

        float cosAngle     = Vector2DotProduct(dirToPrev, dirToNext);
        float sinHalfAngle = sqrtf((1.0f - cosAngle) / 2.0f);
        float tanHalfAngle = sqrtf((1.0f - cosAngle) / (1.0f + cosAngle));

        float dist   = radius / sinHalfAngle;
        float offset = radius / tanHalfAngle;

        Vector2 center = Vector2Add(current, Vector2Scale(bisector, dist));

        Vector2 startPoint = Vector2Add(current, Vector2Scale(dirToPrev, offset));
        Vector2 endPoint   = Vector2Add(current, Vector2Scale(dirToNext, offset));

        float startAngle = atan2f(startPoint.y - center.y, startPoint.x - center.x);
        float endAngle   = atan2f(endPoint.y - center.y, endPoint.x - center.x);

        if (endAngle < startAngle) {
            endAngle += PI * 2.0f;
        }

        int segments = ceilf(radius * (endAngle - startAngle));

        for (int s = 0; s < segments; s++) {
            float angle = (float)s / (segments - 1) * (endAngle - startAngle) + startAngle;

            Vector2 point = {
                center.x + cosf(angle) * radius,
                center.y + sinf(angle) * radius
            };

            CGUI_ARRAY_INSERT_BACK(Vector2, geom.vertices, point);
        }
    }

    return geom;
}

CguiComputeGeom CguiComputeFields(CguiGeom geom) {
    if (geom.verticesCount == 0) return (CguiComputeGeom){0};

    Vector2 min = geom.vertices[0];
    Vector2 max = geom.vertices[0];

    CGUI_ARRAY_FOR_EACH(Vector2, geom.vertices, vertex) {
        min.x = fminf(min.x, vertex->x);
        min.y = fminf(min.y, vertex->y);
        max.x = fmaxf(max.x, vertex->x);
        max.y = fmaxf(max.y, vertex->y);
    }

    min.x = floorf(min.x);
    min.y = floorf(min.y);
    max.x = ceilf(max.x);
    max.y = ceilf(max.y);

    CguiIRectangle rec    = {min.x, min.y, max.x - min.x, max.y - min.y};
    int            width  = rec.width;
    int            height = rec.height;

    CguiComputeGeom cGeom = {0};
    cGeom.rec             = rec;
    CGUI_ARRAY_RESIZE(float, cGeom.fields, width *height, INFINITY);

    // Wind counting
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2 point = {x + min.x, y + min.y};

            float minDist   = INFINITY;
            float sumAngles = 0.0f;

            CGUI_ARRAY_FOR_EACH(Vector2, geom.vertices, vertex) {
                Vector2 start  = geom.vertices[(vertexIndex + 0) % geom.verticesCount];
                Vector2 end    = geom.vertices[(vertexIndex + 1) % geom.verticesCount];
                Vector2 line   = Vector2Subtract(end, start);
                Vector2 offset = Vector2Subtract(point, start);

                float   factor      = Clamp(Vector2DotProduct(offset, line) / Vector2LengthSqr(line), 0.0f, 1.0f);
                Vector2 pointOnLine = Vector2Add(start, Vector2Scale(line, factor));
                float   dist        = Vector2Distance(point, pointOnLine);

                minDist = fminf(minDist, dist);

                Vector2 toStart = Vector2Subtract(start, point);
                Vector2 toEnd   = Vector2Subtract(end, point);
                float   angle   = atan2f(Vector2CrossProduct(toStart, toEnd), Vector2DotProduct(toStart, toEnd));
                sumAngles += angle;
            }

            bool  isInside      = (int)roundf(sumAngles / 2.0f / PI) % 2 != 0;
            float field         = minDist * (isInside ? -1.0f : 1.0f);
            int   index         = y * width + x;
            cGeom.fields[index] = field;
        }
    }

    return cGeom;
}

CguiComputeGeom CguiResizeFields(CguiComputeGeom geom, CguiIRectangle rec) {
    CguiComputeGeom cGeom = {0};
    cGeom.rec             = rec;
    CGUI_ARRAY_RESIZE(float, cGeom.fields, rec.width *rec.height, INFINITY);

    for (int xNewIndex = 0; xNewIndex < rec.width; xNewIndex++) {
        for (int yNewIndex = 0; yNewIndex < rec.height; yNewIndex++) {
            int     newIndex = yNewIndex * rec.width + xNewIndex;
            int     px       = xNewIndex + rec.x;
            int     py       = yNewIndex + rec.y;
            Vector2 p        = {px, py};

            bool isInsideOld = px >= geom.rec.x && px < geom.rec.x + geom.rec.width && py >= geom.rec.y && py < geom.rec.y + geom.rec.height;

            if (isInsideOld) {
                int xOldIndex          = px - geom.rec.x;
                int yOldIndex          = py - geom.rec.y;
                int oldIndex           = yOldIndex * geom.rec.width + xOldIndex;
                cGeom.fields[newIndex] = geom.fields[oldIndex];
                continue;
            }

            int     qx        = CguiClampI(px, geom.rec.x, geom.rec.x + geom.rec.width - 1);
            int     qy        = CguiClampI(py, geom.rec.y, geom.rec.y + geom.rec.height - 1);
            Vector2 q         = {qx, qy};
            int     xOldIndex = qx - geom.rec.x;
            int     yOldIndex = qy - geom.rec.y;
            int     oldIndex  = yOldIndex * geom.rec.width + xOldIndex;
            float   oldField  = geom.fields[oldIndex];

            Vector2 g = {0.0f, 0.0f};

            if (qx > geom.rec.x && qx < geom.rec.x + geom.rec.width - 1) {
                int leftIndex  = yOldIndex * geom.rec.width + (qx - 1 - geom.rec.x);
                int rightIndex = yOldIndex * geom.rec.width + (qx + 1 - geom.rec.x);
                g.x            = (geom.fields[rightIndex] - geom.fields[leftIndex]) / 2.0f;
            } else if (qx == geom.rec.x) {
                int rightIndex = yOldIndex * geom.rec.width + (qx + 1 - geom.rec.x);
                g.x            = geom.fields[rightIndex] - oldField;
            } else if (qx == geom.rec.x + geom.rec.width - 1) {
                int leftIndex = yOldIndex * geom.rec.width + (qx - 1 - geom.rec.x);
                g.x           = oldField - geom.fields[leftIndex];
            }

            if (qy > geom.rec.y && qy < geom.rec.y + geom.rec.height - 1) {
                int upIndex   = (qy - 1 - geom.rec.y) * geom.rec.width + xOldIndex;
                int downIndex = (qy + 1 - geom.rec.y) * geom.rec.width + xOldIndex;
                g.y           = (geom.fields[downIndex] - geom.fields[upIndex]) / 2.0f;
            } else if (qy == geom.rec.y) {
                int downIndex = (qy + 1 - geom.rec.y) * geom.rec.width + xOldIndex;
                g.y           = geom.fields[downIndex] - oldField;
            } else if (qy == geom.rec.y + geom.rec.height - 1) {
                int upIndex = (qy - 1 - geom.rec.y) * geom.rec.width + xOldIndex;
                g.y         = oldField - geom.fields[upIndex];
            }

            cGeom.fields[newIndex] = oldField + Vector2DotProduct(Vector2Normalize(g), Vector2Subtract(p, q));
        }
    }

    return cGeom;
}

void CguiDeleteGeom(CguiGeom geom) {
    CGUI_ARRAY_DELETE(Vector2, geom.vertices);
}

CguiComputeGeom CguiCombineFields(CguiComputeGeom geomA, CguiComputeGeom geomB, CguiCombineGeomFunc combine) {
    if (!combine) return (CguiComputeGeom){0};

    CguiIRectangle unionRec = CguiRecToIRec(CguiUnionRec(CguiIRecToRec(geomA.rec), CguiIRecToRec(geomB.rec)));

    CguiComputeGeom cGeom = {0};
    cGeom.rec             = unionRec;
    CGUI_ARRAY_RESIZE(float, cGeom.fields, unionRec.width *unionRec.height, INFINITY);

    for (int x = 0; x < unionRec.width; x++) {
        for (int y = 0; y < unionRec.height; y++) {
            int     index = y * unionRec.width + x;
            Vector2 point = {x + unionRec.x, y + unionRec.y};

            float fieldA = INFINITY;
            if (point.x >= geomA.rec.x && point.x < geomA.rec.x + geomA.rec.width && point.y >= geomA.rec.y && point.y < geomA.rec.y + geomA.rec.height) {
                int xIndex = point.x - geomA.rec.x;
                int yIndex = point.y - geomA.rec.y;
                fieldA     = geomA.fields[yIndex * geomA.rec.width + xIndex];
            }

            float fieldB = INFINITY;
            if (point.x >= geomB.rec.x && point.x < geomB.rec.x + geomB.rec.width && point.y >= geomB.rec.y && point.y < geomB.rec.y + geomB.rec.height) {
                int xIndex = point.x - geomB.rec.x;
                int yIndex = point.y - geomB.rec.y;
                fieldB     = geomB.fields[yIndex * geomB.rec.width + xIndex];
            }

            cGeom.fields[index] = combine(fieldA, fieldB);
        }
    }

    return cGeom;
}

CguiComputeGeom CguiCombineFieldsF(CguiComputeGeom geom, float value, CguiCombineGeomFunc combine) {
    if (!combine) return (CguiComputeGeom){0};

    CguiComputeGeom cGeom = {0};
    cGeom.rec             = geom.rec;
    CGUI_ARRAY_RESIZE(float, cGeom.fields, geom.rec.width *geom.rec.height, INFINITY);

    for (int x = 0; x < geom.rec.width; x++) {
        for (int y = 0; y < geom.rec.height; y++) {
            int   index         = y * geom.rec.width + x;
            float field         = geom.fields[index];
            cGeom.fields[index] = combine(field, value);
        }
    }

    return cGeom;
}

CguiComputeGeom CguiAlterFields(CguiComputeGeom geom, CguiAlterGeomFunc alter) {
    if (!alter) return (CguiComputeGeom){0};

    CguiComputeGeom cGeom = {0};
    cGeom.rec             = geom.rec;
    CGUI_ARRAY_RESIZE(float, cGeom.fields, geom.rec.width *geom.rec.height, INFINITY);

    for (int x = 0; x < geom.rec.width; x++) {
        for (int y = 0; y < geom.rec.height; y++) {
            int   index         = y * geom.rec.width + x;
            float field         = geom.fields[index];
            cGeom.fields[index] = alter(field);
        }
    }

    return cGeom;
}

static float Add(float a, float b) {
    return a + b;
}

CguiComputeGeom CguiAddFields(CguiComputeGeom geomA, CguiComputeGeom geomB) {
    return CguiCombineFields(geomA, geomB, Add);
}

CguiComputeGeom CguiAddFieldsF(CguiComputeGeom geom, float value) {
    return CguiCombineFieldsF(geom, value, Add);
}

static float Multiply(float a, float b) {
    return a * b;
}

CguiComputeGeom CguiMultiplyFields(CguiComputeGeom geomA, CguiComputeGeom geomB) {
    return CguiCombineFields(geomA, geomB, Multiply);
}

CguiComputeGeom CguiMultiplyFieldsF(CguiComputeGeom geom, float value) {
    return CguiCombineFieldsF(geom, value, Multiply);
}

static float Negate(float a) {
    return -a;
}

CguiComputeGeom CguiNegateFields(CguiComputeGeom geom) {
    return CguiAlterFields(geom, Negate);
}

static float Invert(float a) {
    return 1.0f / a;
}

CguiComputeGeom CguiInvertFields(CguiComputeGeom geom) {
    return CguiAlterFields(geom, Invert);
}

static float Pow(float a, float b) {
    return powf(a, b);
}

CguiComputeGeom CguiPowFields(CguiComputeGeom geomA, CguiComputeGeom geomB) {
    return CguiCombineFields(geomA, geomB, Pow);
}

CguiComputeGeom CguiPowFieldsF(CguiComputeGeom geom, float value) {
    return CguiCombineFieldsF(geom, value, Pow);
}

static float Min(float a, float b) {
    return fminf(a, b);
}

CguiComputeGeom CguiMinFields(CguiComputeGeom geomA, CguiComputeGeom geomB) {
    return CguiCombineFields(geomA, geomB, Min);
}

CguiComputeGeom CguiMinFieldsF(CguiComputeGeom geom, float value) {
    return CguiCombineFieldsF(geom, value, Min);
}

static float Max(float a, float b) {
    return fmaxf(a, b);
}

CguiComputeGeom CguiMaxFields(CguiComputeGeom geomA, CguiComputeGeom geomB) {
    return CguiCombineFields(geomA, geomB, Max);
}

CguiComputeGeom CguiMaxFieldsF(CguiComputeGeom geom, float value) {
    return CguiCombineFieldsF(geom, value, Max);
}

void CguiDeleteFields(CguiComputeGeom geom) {
    CGUI_ARRAY_DELETE(float, geom.fields);
}
