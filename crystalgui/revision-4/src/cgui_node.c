// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "cgui_node.h"
#include "cgui_extra.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

CGUI_ARRAY_COMPARE_DEFAULT(CguiNodePtr);

static const CguiContextVTable basicContextVTable = {
    .createContext = CguiCreateBasicContext,
    .deleteContext = CguiDeleteBasicContext,
    .syncContext   = CguiSyncBasicContext,
};

static const CguiNodeVTable basicNodeVTable = {
    .createNode     = CguiCreateBasicNode,
    .deleteNode     = CguiDeleteBasicNode,
    .syncNode       = CguiSyncBasicNode,
    .modelNode      = CguiModelBasicNode,
    .debugDrawNode  = CguiDebugDrawBasicNode,
    .debugPrintNode = CguiDebugPrintBasicNode
};

static CguiNodePtr *syncNodes         = NULL;
static int          syncNodesCount    = 0;
static int          syncNodesCapacity = 0;

static CguiNodePtr *modelNodes         = NULL;
static int          modelNodesCount    = 0;
static int          modelNodesCapacity = 0;

static CguiNodePtr *debugPrintNodes         = NULL;
static int          debugPrintNodesCount    = 0;
static int          debugPrintNodesCapacity = 0;

CguiModel CguiModelDefault(void) {
    return (CguiModel){0};
}

CguiModel CguiEmpty(void) {
    return CguiModelDefault();
}

CguiModel CguiFilled(void) {
    CguiModel m = CguiModelDefault();
    m.scale     = (Vector2){1.0f, 1.0f};
    return m;
}

CguiModel CguiMargin(float margin) {
    CguiModel m = CguiModelDefault();
    m.scale     = (Vector2){1.0f, 1.0f};
    m.extent    = (Vector2){-margin, -margin};
    return m;
}

CguiModel CguiMarginEx(float top, float bottom, float left, float right) {
    float     mx = top + bottom;
    float     my = left + right;
    float     cx = (top - bottom) / 2.0f;
    float     cy = (left - right) / 2.0f;
    CguiModel m  = CguiModelDefault();
    m.scale      = (Vector2){1.0f, 1.0f};
    m.offset     = (Vector2){cx, cy};
    m.extent     = (Vector2){-mx, -my};
    return m;
}

CguiModel CguiScaled(Vector2 scale) {
    CguiModel m = CguiModelDefault();
    m.scale     = scale;
    return m;
}

CguiModel CguiCentered(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.extent    = extent;
    return m;
}

CguiModel CguiOffset(Vector2 offset, Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.offset    = offset;
    m.extent    = extent;
    return m;
}

CguiModel CguiTopLeft(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){-1.0f, -1.0f};
    m.floating  = (Vector2){0.5f, 0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiTopRight(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){1.0f, -1.0f};
    m.floating  = (Vector2){-0.5f, 0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiBottomLeft(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){-1.0f, 1.0f};
    m.floating  = (Vector2){0.5f, -0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiBottomRight(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){1.0f, 1.0f};
    m.floating  = (Vector2){-0.5f, -0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiTopAligned(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){0.0f, -1.0f};
    m.floating  = (Vector2){0.0f, 0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiBottomAligned(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){0.0f, 1.0f};
    m.floating  = (Vector2){0.0f, -0.5f};
    m.extent    = extent;
    return m;
}

CguiModel CguiLeftAligned(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){-1.0f, 0.0f};
    m.floating  = (Vector2){0.5f, 0.0f};
    m.extent    = extent;
    return m;
}

CguiModel CguiRightAligned(Vector2 extent) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){1.0f, 0.0f};
    m.floating  = (Vector2){-0.5f, 0.0f};
    m.extent    = extent;
    return m;
}

CguiModel CguiTopDocked(float height) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){0.0f, -1.0f};
    m.floating  = (Vector2){0.0f, 0.5f};
    m.extent    = (Vector2){0.0f, height};
    m.scale     = (Vector2){1.0f, 0.0f};
    return m;
}

CguiModel CguiBottomDocked(float height) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){0.0f, 1.0f};
    m.floating  = (Vector2){0.0f, -0.5f};
    m.extent    = (Vector2){0.0f, height};
    m.scale     = (Vector2){1.0f, 0.0f};
    return m;
}

CguiModel CguiLeftDocked(float width) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){-1.0f, 0.0f};
    m.floating  = (Vector2){0.5f, 0.0f};
    m.extent    = (Vector2){width, 0.0f};
    m.scale     = (Vector2){0.0f, 1.0f};
    return m;
}

CguiModel CguiRightDocked(float width) {
    CguiModel m = CguiModelDefault();
    m.anchor    = (Vector2){1.0f, 0.0f};
    m.floating  = (Vector2){-0.5f, 0.0f};
    m.extent    = (Vector2){width, 0.0f};
    m.scale     = (Vector2){0.0f, 1.0f};
    return m;
}

Rectangle CguiCalculateModel(CguiModel model, Rectangle parentRec) {
    Rectangle screenRec = CguiGetScreenSizeRec();

    Vector2 screenCenter = CguiGetRecCenter(screenRec);
    Vector2 screenSize   = CguiGetRecSize(screenRec);
    Vector2 parentCenter = CguiGetRecCenter(parentRec);
    Vector2 parentSize   = CguiGetRecSize(parentRec);

    Vector2 refPos  = CguiLerpVector2(parentCenter, screenCenter, model.posRef);
    Vector2 refSize = CguiLerpVector2(parentSize, screenSize, model.sizeRef);

    // finalSize = model.extent + model.scale * refSize;
    // finalPos  = model.offset + refPos + model.anchor * refSize + model.floating * finalSize;
    Vector2 finalSize = Vector2Add(model.extent, Vector2Multiply(model.scale, refSize));
    Vector2 finalPos  = Vector2Add(model.offset, Vector2Add(refPos, Vector2Add(Vector2Multiply(model.anchor, refSize), Vector2Multiply(model.floating, finalSize))));

    return (Rectangle){finalPos.x - finalSize.x / 2.0f, finalPos.y - finalSize.y / 2.0f, finalSize.x, finalSize.y};
}

CguiModel CguiLerpModel(CguiModel from, CguiModel to, float t) {
    return (CguiModel){
        CguiLerpFloat(from.posRef, to.posRef, t),
        CguiLerpFloat(from.sizeRef, to.sizeRef, t),
        CguiLerpVector2(from.offset, to.offset, t),
        CguiLerpVector2(from.anchor, to.anchor, t),
        CguiLerpVector2(from.floating, to.floating, t),
        CguiLerpVector2(from.extent, to.extent, t),
        CguiLerpVector2(from.scale, to.scale, t),
    };
}

CguiContextPtr CguiCreateBasicContext(void) {
    CguiContextPtr context = malloc(sizeof(CguiContext));
    if (!context) return NULL;

    memset(context, 0, sizeof(CguiContext));

    CguiCreateBasicContextInto(context);

    return context;
}

void CguiDeleteBasicContext(CguiContextPtr context) {
    if (!context) return;

    free(context);
}

void CguiSyncBasicContext(CguiContextPtr context, CguiContextPtr proto) {
    if (!context || !proto) return;

    CGUI_PROTO_SET_PROTO(context->enabled, CGUI_PROTO_GET(proto->enabled));
}

#define DEBUG_CONTEXT_LEVEL_1_FMT  "ptr:%016zx ty:%08x"
#define DEBUG_CONTEXT_LEVEL_1_ARGS (size_t)context, context->type
#define DEBUG_CONTEXT_LEVEL_2_FMT  "en:%d"
#define DEBUG_CONTEXT_LEVEL_2_ARGS CGUI_PROTO_GET(context->enabled)

void CguiDebugPrintBasicContext(CguiContextPtr context, int indentLevel) {
    if (!context) return;

    // 256 spaces
    static const char spaces[] = "                                                                                                                                                                                                                                                                ";

    if (indentLevel < 0) indentLevel = 0;
    if (indentLevel > 255) indentLevel = 255;

    TraceLog(LOG_DEBUG, "%.*sCguiContext " DEBUG_CONTEXT_LEVEL_1_FMT, indentLevel, spaces, DEBUG_CONTEXT_LEVEL_1_ARGS);
    TraceLog(LOG_DEBUG, "%.*s            " DEBUG_CONTEXT_LEVEL_2_FMT, indentLevel, spaces, DEBUG_CONTEXT_LEVEL_2_ARGS);
}

void CguiCreateBasicContextInto(CguiContextPtr context) {
    if (!context) return;

    CGUI_PROTO_SET_PROTO(context->enabled, true);
    context->vTable = &basicContextVTable;
}

CguiNodePtr CguiBuild(CguiNodePtr node, ...) {
    if (!node) return NULL;

    CguiNodePtr child = NULL;

    va_list args;
    va_start(args, node);

    while ((child = va_arg(args, CguiNodePtr)) != NULL) {
        CguiInsertChildBack(node, child);
    }

    va_end(args);

    return node;
}

CguiNodePtr CguiAttach(CguiNodePtr node, ...) {
    if (!node) return NULL;

    CguiContextPtr context = NULL;

    va_list args;
    va_start(args, node);

    while ((context = va_arg(args, CguiContextPtr)) != NULL) {
        CGUI_ARRAY_INSERT_BACK(CguiContextPtr, node->contexts, context);
    }

    va_end(args);

    return node;
}

CguiNodePtr CguiCreateBasicNode(void) {
    CguiNodePtr node = malloc(sizeof(CguiNode));
    if (!node) return NULL;

    memset(node, 0, sizeof(CguiNode));

    CguiCreateBasicNodeInto(node);

    return node;
}

void CguiDeleteBasicNode(CguiNodePtr node) {
    if (!node) return;

    free(node);
}

void CguiSyncBasicNode(CguiNodePtr node, CguiNodePtr proto) {
    if (!node || !proto) return;

    CGUI_PROTO_SET_PROTO(node->enabled, CGUI_PROTO_GET(proto->enabled));
    CGUI_PROTO_SET_PROTO(node->canHandleEvent, CGUI_PROTO_GET(proto->canHandleEvent));
    CGUI_PROTO_SET_PROTO(node->canHandleSinkEvent, CGUI_PROTO_GET(proto->canHandleSinkEvent));
    CGUI_PROTO_SET_PROTO(node->canHandleFloatEvent, CGUI_PROTO_GET(proto->canHandleFloatEvent));
}

void CguiModelBasicNode(CguiNodePtr node) {
    if (!node || (!node->modelDirty && !node->transformDirty && !node->quadDirty)) return;

    if (node->modelDirty) {
        Rectangle parentRec = node->parent ? node->parent->modelRec : CguiGetScreenSizeRec();
        node->modelRec      = CguiCalculateModel(CGUI_PROTO_GET(node->model), parentRec);
        node->modelDirty    = false;

        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            (*child)->modelDirty = true;
        }
    }

    if (node->transformDirty) {
        CguiMatrix3 parentMat = node->parent ? node->parent->worldTransform : CguiMatrix3Identity();
        node->worldTransform  = CguiMatrix3Multiply(CGUI_PROTO_GET(node->transform), parentMat);
        node->transformDirty  = false;

        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            (*child)->transformDirty = true;
        }
    }

    node->worldQuad = CguiMatrix3TransformRec(node->worldTransform, node->modelRec);
    node->quadDirty = false;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        (*child)->quadDirty = true;
    }
}

#define DEBUG_NODE_LEVEL_1_FMT  "ptr:%016zx ty:%08x"
#define DEBUG_NODE_LEVEL_1_ARGS (size_t)node, node->type

#define DEBUG_NODE_LEVEL_2_FMT  "pr:%.1f sr:%.1f of:%.1f+%.1f ac:%.1f+%.1f fl:%.1f+%.1f ex:%.1f+%.1f sc:%.1f+%.1f"
#define DEBUG_NODE_LEVEL_2_ARGS CGUI_PROTO_GET(node->model).posRef, CGUI_PROTO_GET(node->model).sizeRef, CGUI_PROTO_GET(node->model).offset.x, CGUI_PROTO_GET(node->model).offset.y, CGUI_PROTO_GET(node->model).anchor.x, CGUI_PROTO_GET(node->model).anchor.y, CGUI_PROTO_GET(node->model).floating.x, CGUI_PROTO_GET(node->model).floating.y, CGUI_PROTO_GET(node->model).extent.x, CGUI_PROTO_GET(node->model).extent.y, CGUI_PROTO_GET(node->model).scale.x, CGUI_PROTO_GET(node->model).scale.y

#define DEBUG_NODE_LEVEL_3_FMT  "pn:%s ch:%d pt:%s cl:%d ctx:%d"
#define DEBUG_NODE_LEVEL_3_ARGS node->parent ? "T" : "F", node->childrenCount, node->proto ? "T" : "F", node->clonesCount, node->contextsCount

static Vector4 GetHSLAFromPointer(void *p) {
    uint64_t u = (uint64_t)(uintptr_t)p;

    // MurmurHash3 finalizer (fmix64)
    u ^= u >> 33;
    u *= 0xff51afd7ed558ccdULL;
    u ^= u >> 33;
    u *= 0xc4ceb9fe1a85ec53ULL;
    u ^= u >> 33;

    unsigned bits = sizeof(ptrdiff_t) * 8;

    unsigned light_bits = bits / 3;
    unsigned sat_bits   = bits / 3;
    unsigned hue_bits   = bits - light_bits - sat_bits;

    uint64_t light_mask = (1ULL << light_bits) - 1ULL;
    uint64_t sat_mask   = (1ULL << sat_bits) - 1ULL;
    uint64_t hue_mask   = (1ULL << hue_bits) - 1ULL;

    uint64_t light_raw = u & light_mask;
    uint64_t sat_raw   = (u >> light_bits) & sat_mask;
    uint64_t hue_raw   = (u >> (light_bits + sat_bits)) & hue_mask;

    float lightness  = (float)light_raw / (float)light_mask;
    float saturation = (float)sat_raw / (float)sat_mask;
    float hue        = ((float)hue_raw / (float)hue_mask) * 360.0f;

    saturation = 0.75f + saturation * 0.25f;
    lightness  = 0.25f + lightness * 0.5f;

    Vector4 out = {0};
    out.x       = hue;
    out.y       = saturation;
    out.z       = lightness;
    out.w       = 1.0f;
    return out;
}

void CguiDebugDrawBasicNode(CguiNodePtr node, int debugLevel) {
    if (!node) return;

    int levels = 3;

    if (node->debugLevel > debugLevel) debugLevel = node->debugLevel;

    if (debugLevel < 1) debugLevel = 1;
    if (debugLevel > levels) debugLevel = levels;

    Vector4 hsla = GetHSLAFromPointer(node);

    DrawRectangleLines(node->modelRec.x, node->modelRec.y, node->modelRec.width, node->modelRec.height, CguiColorFromHSLAV(hsla));

    hsla.w = 0.125f;

    DrawRectangle(node->modelRec.x, node->modelRec.y, node->modelRec.width, node->modelRec.height, CguiColorFromHSLAV(hsla));

    char debugLines[3][256] = {0};
    int  debugLinesCount    = 0;

    switch (levels - debugLevel) {
    case 0: snprintf(debugLines[debugLinesCount++], 256, DEBUG_NODE_LEVEL_3_FMT, DEBUG_NODE_LEVEL_3_ARGS);
    case 1: snprintf(debugLines[debugLinesCount++], 256, DEBUG_NODE_LEVEL_2_FMT, DEBUG_NODE_LEVEL_2_ARGS);
    case 2: snprintf(debugLines[debugLinesCount++], 256, DEBUG_NODE_LEVEL_1_FMT, DEBUG_NODE_LEVEL_1_ARGS);
    }

    const char *debugLinesPtr[3] = {debugLines[0], debugLines[1], debugLines[2]};
    CguiDebugDrawLines(node->modelRec, CGUI_DEBUG_TOP_LEFT, CGUI_DEBUG_BOTTOM_RIGHT, debugLinesPtr, debugLinesCount);
}

void CguiDebugPrintBasicNode(CguiNodePtr node, int indentLevel) {
    if (!node) return;

    // 256 spaces
    static const char spaces[] = "                                                                                                                                                                                                                                                                ";

    if (indentLevel < 0) indentLevel = 0;
    if (indentLevel > 255) indentLevel = 255;

    TraceLog(LOG_DEBUG, "%.*sCguiNode " DEBUG_NODE_LEVEL_1_FMT, indentLevel, spaces, DEBUG_NODE_LEVEL_1_ARGS);
    TraceLog(LOG_DEBUG, "%.*s         " DEBUG_NODE_LEVEL_2_FMT, indentLevel, spaces, DEBUG_NODE_LEVEL_2_ARGS);
    TraceLog(LOG_DEBUG, "%.*s         " DEBUG_NODE_LEVEL_3_FMT, indentLevel, spaces, DEBUG_NODE_LEVEL_3_ARGS);
}

void CguiCreateBasicNodeInto(CguiNodePtr node) {
    if (!node) return;

    node->vTable = &basicNodeVTable;

    CGUI_PROTO_SET_PROTO(node->enabled, 0xFFFFFFFF);
    CGUI_PROTO_SET_PROTO(node->model, CguiFilled());
    CGUI_PROTO_SET_PROTO(node->transform, CguiMatrix3Identity());

    node->modelDirty     = true;
    node->transformDirty = true;
}

CguiNodePtr CguiCreateBasicNodeEx(CguiModel model) {
    CguiNodePtr node = CguiCreateBasicNode();
    if (!node) return NULL;

    CGUI_PROTO_SET_PROTO(node->model, model);

    return node;
}

void CguiDeleteNode(CguiNodePtr node) {
    if (!node) return;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        (*child)->parent = NULL; // Optimization: set parent to null to prevent additional traversal by CguiDeleteNode
        CguiDeleteNode(*child);
    }

    CGUI_ARRAY_DELETE(CguiNodePtr, node->children);

    if (node->parent) {
        int index = -1;
        CGUI_ARRAY_FIND(CguiNodePtr, node->parent->children, node, index);
        if (index != -1) CGUI_ARRAY_REMOVE(CguiNodePtr, node->parent->children, index);
    }

    CGUI_ARRAY_FOR(CguiNodePtr, node->clones, clone) {
        (*clone)->proto = NULL;
    }

    CGUI_ARRAY_DELETE(CguiNodePtr, node->clones);

    if (node->proto) {
        int index = -1;
        CGUI_ARRAY_FIND(CguiNodePtr, node->proto->clones, node, index);
        if (index != -1) CGUI_ARRAY_REMOVE(CguiNodePtr, node->proto->clones, index);
    }

    CGUI_ARRAY_FOR(CguiContextPtr, node->contexts, context) {
        (*context)->vTable->deleteContext(*context);
    }

    CGUI_ARRAY_DELETE(CguiContextPtr, node->contexts);

    node->vTable->deleteNode(node);
}

CguiNodePtr CguiCopyNode(CguiNodePtr node) {
    if (!node) return NULL;

    CguiNodePtr newNode = node->vTable->createNode();
    node->vTable->syncNode(newNode, node);

    CGUI_ARRAY_RESERVE(CguiNodePtr, newNode->children, node->childrenCount);
    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CguiInsertChildBack(newNode, CguiCopyNode(*child));
    }

    return newNode;
}

CguiNodePtr CguiCloneNode(CguiNodePtr node) {
    if (!node) return NULL;

    CguiNodePtr newNode = node->vTable->createNode();
    node->vTable->syncNode(newNode, node);
    newNode->proto = node;
    CGUI_ARRAY_INSERT_BACK(CguiNodePtr, node->clones, newNode);

    CGUI_ARRAY_RESERVE(CguiNodePtr, newNode->children, node->childrenCount);
    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CguiInsertChildBack(newNode, CguiCloneNode(*child));
    }

    return newNode;
}

void CguiSyncNode(CguiNodePtr node) {
    if (!node) return;

    if (node->proto) node->proto->vTable->syncNode(node, node->proto);

    CGUI_ARRAY_FOR(CguiContextPtr, node->contexts, context) {
        if ((*context)->proto) (*context)->proto->vTable->syncContext(*context, (*context)->proto);
    }

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CguiSyncNode(*child);
    }

    CGUI_ARRAY_FOR(CguiNodePtr, node->clones, clone) {
        CguiSyncNode(*clone);
    }
}

void CguiModelNode(CguiNodePtr node) {
    if (!node) return;

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_MODEL && node->vTable->modelNode) node->vTable->modelNode(node);

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_MODEL_CHILDREN) {
        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            CguiModelNode(*child);
        }
    }
}

void CguiUpdateNode(CguiNodePtr node) {
    if (!node) return;

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_UPDATE && node->vTable->updateNode) node->vTable->updateNode(node);

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_UPDATE_CHILDREN) {
        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            CguiUpdateNode(*child);
        }
    }
}

void CguiDrawNode(CguiNodePtr node) {
    if (!node) return;

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DRAW && node->vTable->drawNode) node->vTable->drawNode(node);

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DRAW_CHILDREN) {
        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            CguiDrawNode(*child);
        }
    }
}

void CguiDebugDrawNode(CguiNodePtr node, int debugLevel) {
    if (!node) return;

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DEBUG_DRAW && node->vTable->debugDrawNode) node->vTable->debugDrawNode(node, debugLevel);

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DEBUG_DRAW_CHILDREN) {
        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            CguiDebugDrawNode(*child, debugLevel);
        }
    }
}

void CguiDebugPrintNode(CguiNodePtr node, int indentLevel) {
    if (!node) return;

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DEBUG_PRINT && node->vTable->debugPrintNode) node->vTable->debugPrintNode(node, indentLevel);

    if (CGUI_PROTO_GET(node->enabled) & CGUI_ENABLE_DEBUG_PRINT_CHILDREN) {
        CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
            CguiDebugPrintNode(*child, indentLevel + 1);
        }
    }
}

void CguiDebugDrawLines(Rectangle rec, int corner, int section, const char **lines, int linesCount) {
    if (!lines || linesCount <= 0) return;

    Vector2 cornerPos = {0};
    switch (corner) {
    case CGUI_DEBUG_TOP_LEFT: cornerPos = CguiGetRecTopLeft(rec); break;
    case CGUI_DEBUG_TOP_RIGHT: cornerPos = CguiGetRecTopRight(rec); break;
    case CGUI_DEBUG_BOTTOM_LEFT: cornerPos = CguiGetRecBottomLeft(rec); break;
    case CGUI_DEBUG_BOTTOM_RIGHT: cornerPos = CguiGetRecBottomRight(rec); break;
    }

    // Padding from the corner to the text
    switch (section) {
    case CGUI_DEBUG_TOP_LEFT: cornerPos = Vector2Add(cornerPos, Vector2Scale((Vector2){-1.0f, -1.0f}, 2.0f)); break;
    case CGUI_DEBUG_TOP_RIGHT: cornerPos = Vector2Add(cornerPos, Vector2Scale((Vector2){1.0f, -1.0f}, 2.0f)); break;
    case CGUI_DEBUG_BOTTOM_LEFT: cornerPos = Vector2Add(cornerPos, Vector2Scale((Vector2){-1.0f, 1.0f}, 2.0f)); break;
    case CGUI_DEBUG_BOTTOM_RIGHT: cornerPos = Vector2Add(cornerPos, Vector2Scale((Vector2){1.0f, 1.0f}, 2.0f)); break;
    }

    float hue = (corner * 4.0f + section) * 360.0f / 16.0f;

    float linePos = cornerPos.y;

    for (int i = 0; i < linesCount; i++) {
        const char *line     = lines[i];
        Vector2     lineSize = MeasureTextEx(GetFontDefault(), line, 10.0f, 1.0f);

        Vector2 textPos = {0.0f, linePos};
        switch (section) {
        case CGUI_DEBUG_TOP_LEFT:
        case CGUI_DEBUG_BOTTOM_LEFT: textPos.x = cornerPos.x - lineSize.x; break;
        case CGUI_DEBUG_TOP_RIGHT:
        case CGUI_DEBUG_BOTTOM_RIGHT: textPos.x = cornerPos.x; break;
        }

        DrawText(line, textPos.x, textPos.y, 10, CguiColorFromHSL(hue + i * 16.0f, 0.5f, 0.5f));

        switch (section) {
        case CGUI_DEBUG_TOP_LEFT:
        case CGUI_DEBUG_TOP_RIGHT: linePos -= lineSize.y + 2.0f; break;
        case CGUI_DEBUG_BOTTOM_LEFT:
        case CGUI_DEBUG_BOTTOM_RIGHT: linePos += lineSize.y + 2.0f; break;
        }
    }
}

void CguiClearChildren(CguiNodePtr node) {
    if (!node) return;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CguiDeleteNode(*child);
    }

    CGUI_ARRAY_CLEAR(CguiNodePtr, node->children);
}

void CguiFillCopies(CguiNodePtr node, CguiNodePtr copy) {
    if (!node || !copy) return;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        (*child)->parent = NULL; // Optimization: set parent to null to prevent additional traversal by CguiDeleteNode
        CguiDeleteNode(*child);
        *child           = CguiCopyNode(copy);
        (*child)->parent = node;
    }
}

void CguiCopyChildren(CguiNodePtr to, CguiNodePtr from) {
    if (!to || !from) return;

    CGUI_ARRAY_RESERVE(CguiNodePtr, to->children, from->childrenCount);
    CGUI_ARRAY_FOR(CguiNodePtr, from->children, child) {
        CguiInsertChildBack(to, CguiCopyNode(*child));
    }
}

void CguiCloneChildren(CguiNodePtr to, CguiNodePtr from) {
    if (!to || !from) return;

    CGUI_ARRAY_RESERVE(CguiNodePtr, to->children, from->childrenCount);
    CGUI_ARRAY_FOR(CguiNodePtr, from->children, child) {
        CguiInsertChildBack(to, CguiCloneNode(*child));
    }
}

void CguiMoveChildren(CguiNodePtr to, CguiNodePtr from) {
    if (!to || !from) return;

    CGUI_ARRAY_RESERVE(CguiNodePtr, to->children, from->childrenCount);
    CGUI_ARRAY_FOR(CguiNodePtr, from->children, child) {
        (*child)->parent = NULL; // Optimization: set parent to null to prevent additional traversal by CguiInsertChildBack
        CguiInsertChildBack(to, *child);
    }
    CGUI_ARRAY_CLEAR(CguiNodePtr, from->children);
}

void CguiSliceCopyChildren(CguiNodePtr to, CguiNodePtr from, int start, int count) {
    if (!to || !from || start < 0 || start >= from->childrenCount || count < 0) return;

    // Clamp upper bound
    count = (start + count > from->childrenCount) ? from->childrenCount - start : count;

    for (int i = start; i < start + count; i++) {
        CguiInsertChildBack(to, CguiCopyNode(from->children[i]));
    }
}

void CguiSliceCloneChildren(CguiNodePtr to, CguiNodePtr from, int start, int count) {
    if (!to || !from || start < 0 || start >= from->childrenCount || count < 0) return;

    // Clamp upper bound
    count = (start + count > from->childrenCount) ? from->childrenCount - start : count;

    for (int i = start; i < start + count; i++) {
        CguiInsertChildBack(to, CguiCloneNode(from->children[i]));
    }
}

void CguiInsertChild(CguiNodePtr node, CguiNodePtr child, int index) {
    if (!node || !child || index < 0 || index > node->childrenCount) return;

    if (child->parent) {
        CguiPopChild(child);
    }

    child->parent = node;
    CGUI_ARRAY_INSERT(CguiNodePtr, node->children, index, child);
}

void CguiInsertCopies(CguiNodePtr node, CguiNodePtr copy, int children) {
    if (!node || !copy || children < 0) return;

    for (int i = 0; i < children; i++) {
        CguiInsertChildBack(node, CguiCopyNode(copy));
    }
}

void CguiRemoveChild(CguiNodePtr child) {
    if (!child) return;

    CguiDeleteNode(child); // Automatically handles relation
}

void CguiRemoveChildAt(CguiNodePtr node, int index) {
    if (!node || index < 0 || index >= node->childrenCount) return;

    CguiDeleteNode(node->children[index]); // Automatically handles relation
}

CguiNodePtr CguiPopChild(CguiNodePtr child) {
    if (!child || !child->parent) return NULL;

    int index = -1;
    CGUI_ARRAY_FIND(CguiNodePtr, child->parent->children, child, index);

    if (index != -1) {
        CGUI_ARRAY_REMOVE(CguiNodePtr, child->parent->children, index);
        child->parent = NULL;
    }

    return child;
}

CguiNodePtr CguiPopChildAt(CguiNodePtr node, int index) {
    if (!node || index < 0 || index >= node->childrenCount) return NULL;

    CguiNodePtr child = node->children[index];
    child->parent     = NULL;

    CGUI_ARRAY_REMOVE(CguiNodePtr, node->children, index);

    return child;
}

void CguiInsertChildFront(CguiNodePtr node, CguiNodePtr child) {
    if (!node || !child) return;

    CguiInsertChild(node, child, 0);
}

void CguiInsertChildBack(CguiNodePtr node, CguiNodePtr child) {
    if (!node || !child) return;

    CguiInsertChild(node, child, node->childrenCount);
}

void CguiRemoveChildFront(CguiNodePtr node) {
    if (!node) return;

    CguiRemoveChildAt(node, 0);
}

void CguiRemoveChildBack(CguiNodePtr node) {
    if (!node || node->childrenCount == 0) return;

    CguiRemoveChildAt(node, node->childrenCount - 1);
}

CguiNodePtr CguiPopChildFront(CguiNodePtr node) {
    if (!node) return NULL;

    return CguiPopChildAt(node, 0);
}

CguiNodePtr CguiPopChildBack(CguiNodePtr node) {
    if (!node || node->childrenCount == 0) return NULL;

    return CguiPopChildAt(node, node->childrenCount - 1);
}

void CguiFilterChildren(CguiNodePtr node, bool (*predicate)(CguiNodePtr node)) {
    if (!node || !predicate) return;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        if (predicate(*child)) {
            (*child)->parent = NULL; // Optimization: set parent to null to prevent additional traversal by CguiDeleteNode
            CguiDeleteNode(*child);
            CGUI_ARRAY_REMOVE(CguiNodePtr, node->children, childIndex);
            childIndex--;
        }
    }
}

void CguiExtractChildren(CguiNodePtr from, CguiNodePtr to, bool (*predicate)(CguiNodePtr node)) {
    if (!from || !to || !predicate) return;

    CGUI_ARRAY_FOR(CguiNodePtr, from->children, child) {
        if (predicate(*child)) {
            (*child)->parent = NULL; // Optimization: set parent to null to prevent additional traversal by CguiDeleteNode
            CguiInsertChildBack(to, *child);
            CGUI_ARRAY_REMOVE(CguiNodePtr, from->children, childIndex);
            childIndex--;
        }
    }
}

CguiNodePtr CguiFindIfNodeInAncestors(CguiNodePtr node, bool (*predicate)(CguiNodePtr node)) {
    if (!node || !predicate) return NULL;

    if (!node->parent) return NULL;

    if (predicate(node->parent)) {
        return node->parent;
    }

    CguiNodePtr found = CguiFindIfNodeInAncestors(node->parent, predicate);
    if (found) {
        return found;
    }

    return NULL;
}

CguiNodePtr CguiFindIfNodeInDescendants(CguiNodePtr node, bool (*predicate)(CguiNodePtr node)) {
    if (!node || !predicate) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        if (predicate(*child)) {
            return *child;
        }

        CguiNodePtr found = CguiFindIfNodeInDescendants(*child, predicate);
        if (found) {
            return found;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindIfNodeInSiblings(CguiNodePtr node, bool (*predicate)(CguiNodePtr node)) {
    if (!node || !predicate) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->parent->children, child) {
        if (node == *child) continue;

        if (predicate(*child)) {
            return *child;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindIfContextInAncestors(CguiNodePtr node, bool (*predicate)(CguiContextPtr context)) {
    if (!node || !predicate) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiContextPtr, node->parent->contexts, context) {
        if (predicate(*context)) {
            return node->parent;
        }
    }

    CguiNodePtr found = CguiFindIfContextInAncestors(node->parent, predicate);
    if (found) {
        return found;
    }

    return NULL;
}

CguiNodePtr CguiFindIfContextInDescendants(CguiNodePtr node, bool (*predicate)(CguiContextPtr context)) {
    if (!node || !predicate) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CGUI_ARRAY_FOR(CguiContextPtr, (*child)->contexts, context) {
            if (predicate(*context)) {
                return *child;
            }
        }

        CguiNodePtr found = CguiFindIfContextInDescendants(*child, predicate);
        if (found) {
            return found;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindIfContextInSiblings(CguiNodePtr node, bool (*predicate)(CguiContextPtr context)) {
    if (!node || !predicate) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->parent->children, child) {
        if (node == *child) continue;

        CGUI_ARRAY_FOR(CguiContextPtr, (*child)->contexts, context) {
            if (predicate(*context)) {
                return *child;
            }
        }
    }

    return NULL;
}

CguiNodePtr CguiFindNodeInAncestors(CguiNodePtr node, CguiNodePtr find) {
    if (!node || !find) return NULL;

    if (!node->parent) return NULL;

    if (find == node->parent) {
        return node->parent;
    }

    CguiNodePtr found = CguiFindNodeInAncestors(node->parent, find);
    if (found) {
        return found;
    }

    return NULL;
}

CguiNodePtr CguiFindNodeInDescendants(CguiNodePtr node, CguiNodePtr find) {
    if (!node || !find) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        if (find == *child) {
            return *child;
        }

        CguiNodePtr found = CguiFindNodeInDescendants(*child, find);
        if (found) {
            return found;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindNodeInSiblings(CguiNodePtr node, CguiNodePtr find) {
    if (!node || !find) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->parent->children, child) {
        if (node == *child) continue;

        if (find == *child) {
            return *child;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindContextInAncestors(CguiNodePtr node, CguiContextPtr find) {
    if (!node || !find) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiContextPtr, node->parent->contexts, context) {
        if (find == *context) {
            return node->parent;
        }
    }

    CguiNodePtr found = CguiFindContextInAncestors(node->parent, find);
    if (found) {
        return found;
    }

    return NULL;
}

CguiNodePtr CguiFindContextInDescendants(CguiNodePtr node, CguiContextPtr find) {
    if (!node || !find) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        CGUI_ARRAY_FOR(CguiContextPtr, (*child)->contexts, context) {
            if (find == *context) {
                return *child;
            }
        }

        CguiNodePtr found = CguiFindContextInDescendants(*child, find);
        if (found) {
            return found;
        }
    }

    return NULL;
}

CguiNodePtr CguiFindContextInSiblings(CguiNodePtr node, CguiContextPtr find) {
    if (!node || !find) return NULL;

    if (!node->parent) return NULL;

    CGUI_ARRAY_FOR(CguiNodePtr, node->parent->children, child) {
        if (node == *child) continue;

        CGUI_ARRAY_FOR(CguiContextPtr, (*child)->contexts, context) {
            if (find == *context) {
                return *child;
            }
        }
    }

    return NULL;
}

bool CguiTraverseAncestors(CguiNodePtr node, bool (*action)(CguiNodePtr node)) {
    if (!node || !action) return false;

    if (!node->parent) return true; // Reached end

    if (!action(node->parent)) return false;

    if (!CguiTraverseAncestors(node->parent, action)) return false;

    return true; // Reached end
}

bool CguiTraverseDescendants(CguiNodePtr node, bool (*action)(CguiNodePtr node)) {
    if (!node || !action) return false;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        if (!action(*child)) return false;

        if (!CguiTraverseDescendants(*child, action)) return false;
    }

    return true; // Reached end
}

void CguiRegisterSync(CguiNodePtr node) {
    if (!node) return;

    CGUI_ARRAY_INSERT_BACK(CguiNodePtr, syncNodes, node);
}

void CguiRegisterModel(CguiNodePtr node) {
    if (!node) return;

    CGUI_ARRAY_INSERT_BACK(CguiNodePtr, modelNodes, node);
}

void CguiRegisterDebugPrint(CguiNodePtr node) {
    if (!node) return;

    CGUI_ARRAY_INSERT_BACK(CguiNodePtr, debugPrintNodes, node);
}

void CguiProcessRegisteredNodes(void) {
    CGUI_ARRAY_FOR(CguiNodePtr, syncNodes, node) {
        CguiSyncNode(*node);
    }

    CGUI_ARRAY_FOR(CguiNodePtr, modelNodes, node) {
        CguiModelNode(*node);
    }

    CGUI_ARRAY_FOR(CguiNodePtr, debugPrintNodes, node) {
        CguiDebugPrintNode(*node, 0);
    }

    CGUI_ARRAY_CLEAR(CguiNodePtr, syncNodes);
    CGUI_ARRAY_CLEAR(CguiNodePtr, modelNodes);
    CGUI_ARRAY_CLEAR(CguiNodePtr, debugPrintNodes);
}
