// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "cgui_layout.h"
#include "cgui_extra.h"

CGUI_ARRAY_COMPARE_DEFAULT(CguiContextPtr);

const CguiNodeVTable clampNodeVTable = {
    .createNode     = CguiCreateClamp,
    .deleteNode     = CguiDeleteClamp,
    .syncNode       = CguiSyncClamp,
    .modelNode      = CguiModelClamp,
    .debugDrawNode  = CguiDebugDrawClamp,
    .debugPrintNode = CguiDebugPrintClamp,
};

const CguiContextVTable clampItemContextVTable = {
    .createContext     = CguiCreateClampItem,
    .deleteContext     = CguiDeleteClampItem,
    .syncContext       = CguiSyncClampItem,
    .debugPrintContext = CguiDebugPrintClampItem,
};

const CguiNodeVTable flexNodeVTable = {
    .createNode     = CguiCreateFlex,
    .deleteNode     = CguiDeleteFlex,
    .syncNode       = CguiSyncFlex,
    .modelNode      = CguiModelFlex,
    .debugDrawNode  = CguiDebugDrawFlex,
    .debugPrintNode = CguiDebugPrintFlex,
};

const CguiContextVTable flexItemContextVTable = {
    .createContext     = CguiCreateFlexItem,
    .deleteContext     = CguiDeleteFlexItem,
    .syncContext       = CguiSyncFlexItem,
    .debugPrintContext = CguiDebugPrintFlexItem,
};

const CguiNodeVTable gridNodeVTable = {
    .createNode     = CguiCreateGrid,
    .deleteNode     = CguiDeleteGrid,
    .syncNode       = CguiSyncGrid,
    .modelNode      = CguiModelGrid,
    .debugDrawNode  = CguiDebugDrawGrid,
    .debugPrintNode = CguiDebugPrintGrid,
};

const CguiContextVTable gridItemContextVTable = {
    .createContext     = CguiCreateGridItem,
    .deleteContext     = CguiDeleteGridItem,
    .syncContext       = CguiSyncGridItem,
    .debugPrintContext = CguiDebugPrintGridItem,
};

const CguiNodeVTable matrixNodeVTable = {
    .createNode     = CguiCreateMatrix,
    .deleteNode     = CguiDeleteMatrix,
    .syncNode       = CguiSyncMatrix,
    .modelNode      = CguiModelMatrix,
    .debugDrawNode  = CguiDebugDrawMatrix,
    .debugPrintNode = CguiDebugPrintMatrix,
};

const CguiContextVTable matrixItemContextVTable = {
    .createContext     = CguiCreateMatrixItem,
    .deleteContext     = CguiDeleteMatrixItem,
    .syncContext       = CguiSyncMatrixItem,
    .debugPrintContext = CguiDebugPrintMatrixItem,
};

CguiNodePtr CguiCreateClamp(void) {
    CguiClamp *node = malloc(sizeof(CguiClamp));
    if (!node) return NULL;

    memset(node, 0, sizeof(CguiClamp));

    CguiCreateBasicNodeInto((CguiNodePtr)node);

    node->base.type   = CGUI_LAYOUT_TYPE_CLAMP;
    node->base.vTable = &clampNodeVTable;

    CGUI_PROTO_SET_PROTO(node->reposition, true);

    return (CguiNodePtr)node;
}

void CguiDeleteClamp(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    free(node);
}

void CguiSyncClamp(CguiNodePtr node, CguiNodePtr proto) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_CLAMP || !proto || proto->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiSyncBasicNode(node, proto);

    CguiClamp *clampNode = (CguiClamp *)node;
    CguiClamp *protoNode = (CguiClamp *)proto;

    CGUI_PROTO_SET_PROTO(clampNode->minModel, CGUI_PROTO_GET(protoNode->minModel));
    CGUI_PROTO_SET_PROTO(clampNode->maxModel, CGUI_PROTO_GET(protoNode->maxModel));
    CGUI_PROTO_SET_PROTO(clampNode->useMin, CGUI_PROTO_GET(protoNode->useMin));
    CGUI_PROTO_SET_PROTO(clampNode->useMax, CGUI_PROTO_GET(protoNode->useMax));
    CGUI_PROTO_SET_PROTO(clampNode->reposition, CGUI_PROTO_GET(protoNode->reposition));
}

void CguiModelClamp(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiModelBasicNode(node);

    CguiClamp *clampNode = (CguiClamp *)node;
    Rectangle  min       = CguiCalculateModel(CGUI_PROTO_GET(clampNode->minModel), node->parent ? node->parent->modelRec : CguiGetScreenSizeRec());
    Rectangle  max       = CguiCalculateModel(CGUI_PROTO_GET(clampNode->maxModel), node->parent ? node->parent->modelRec : CguiGetScreenSizeRec());

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        int            contextIndex = -1;
        CguiClampItem *context      = NULL;
        CGUI_ARRAY_FIND_IF(CguiContextPtr, (*child)->contexts, context, (*context)->type == CGUI_LAYOUT_TYPE_CLAMP, contextIndex);
        if (contextIndex != -1) {
            context = (CguiClampItem *)(*child)->contexts[contextIndex];
        } else {
            context = (CguiClampItem *)CguiCreateClampItem();
            CGUI_ARRAY_INSERT_BACK(CguiContextPtr, (*child)->contexts, (CguiContextPtr)context);
        }

        float aspect     = 0.0f;
        int   clampStyle = CGUI_CLAMP_STYLE_FIT;
        if (context) {
            aspect     = CGUI_PROTO_GET(context->aspect);
            clampStyle = CGUI_PROTO_GET(context->clampStyle);
        }

        Rectangle rec = CguiCalculateModel(CGUI_PROTO_GET((*child)->model), node->modelRec);

        if (CGUI_PROTO_GET(clampNode->useMin)) {
            rec = CguiUnionRec(rec, min);
        }

        if (CGUI_PROTO_GET(clampNode->useMax)) {
            if (CGUI_PROTO_GET(clampNode->reposition)) {
                rec = CguiContainRec(rec, max);
            } else {
                rec = CguiIntersectionRec(rec, max);
            }
        }

        if (aspect != 0.0f) {
            if (isinf(aspect)) {
                aspect = node->modelRec.width / node->modelRec.height;
            }

            if (clampStyle == CGUI_CLAMP_STYLE_FIT) {
                rec = CguiChangeAspectFit(rec, aspect);
            } else {
                rec = CguiChangeAspectFill(rec, aspect);
            }
        }

        (*child)->modelDirty = false; // Prevent CguiModelBasicNode from overriding the modelRec we just calculated
        (*child)->modelRec   = rec;
        (*child)->quadDirty  = true;
    }
}

void CguiDebugDrawClamp(CguiNodePtr node, int debugLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiDebugDrawBasicNode(node, debugLevel);

    CguiClamp *clampNode = (CguiClamp *)node;

    // ...
}

void CguiDebugPrintClamp(CguiNodePtr node, int indentLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiDebugPrintBasicNode(node, indentLevel);

    CguiClamp *clampNode = (CguiClamp *)node;

    // ...
}

CguiNodePtr CguiCreateClampMn(CguiModel minModel) {
    CguiNodePtr node = CguiCreateClamp();
    if (!node) return NULL;

    CguiClamp *clampNode = (CguiClamp *)node;

    CGUI_PROTO_SET_PROTO(clampNode->minModel, minModel);
    CGUI_PROTO_SET_PROTO(clampNode->useMin, true);

    return node;
}

CguiNodePtr CguiCreateClampMx(CguiModel maxModel) {
    CguiNodePtr node = CguiCreateClamp();
    if (!node) return NULL;

    CguiClamp *clampNode = (CguiClamp *)node;

    CGUI_PROTO_SET_PROTO(clampNode->maxModel, maxModel);
    CGUI_PROTO_SET_PROTO(clampNode->useMax, true);

    return node;
}

CguiNodePtr CguiCreateClampMm(CguiModel minModel, CguiModel maxModel) {
    CguiNodePtr node = CguiCreateClamp();
    if (!node) return NULL;

    CguiClamp *clampNode = (CguiClamp *)node;

    CGUI_PROTO_SET_PROTO(clampNode->minModel, minModel);
    CGUI_PROTO_SET_PROTO(clampNode->maxModel, maxModel);
    CGUI_PROTO_SET_PROTO(clampNode->useMin, true);
    CGUI_PROTO_SET_PROTO(clampNode->useMax, true);

    return node;
}

CguiContextPtr CguiCreateClampItem(void) {
    CguiClampItem *context = malloc(sizeof(CguiClampItem));
    if (!context) return NULL;

    memset(context, 0, sizeof(CguiClampItem));

    context->base.type   = CGUI_LAYOUT_TYPE_CLAMP;
    context->base.vTable = &clampItemContextVTable;

    return (CguiContextPtr)context;
}

void CguiDeleteClampItem(CguiContextPtr context) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    free(context);
}

void CguiSyncClampItem(CguiContextPtr context, CguiContextPtr proto) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiClampItem *clampContext = (CguiClampItem *)context;
    CguiClampItem *protoContext = (CguiGridItem *)proto;

    CGUI_PROTO_SET_PROTO(clampContext->aspect, CGUI_PROTO_GET(protoContext->aspect));
    CGUI_PROTO_SET_PROTO(clampContext->clampStyle, CGUI_PROTO_GET(protoContext->clampStyle));
}

void CguiDebugPrintClampItem(CguiContextPtr context, int indentLevel) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_CLAMP) return;

    CguiDebugPrintBasicContext(context, indentLevel);

    CguiClampItem *clampContext = (CguiClamp *)context;

    // ...
}

CguiContextPtr CguiCreateClampItemEx(float aspect) {
    CguiContextPtr context = CguiCreateClampItem();
    if (!context) return NULL;

    CguiClampItem *clampContext = (CguiClampItem *)context;

    CGUI_PROTO_SET_PROTO(clampContext->aspect, aspect);

    return context;
}

CguiNodePtr CguiCreateFlex(void) {
    CguiFlex *node = malloc(sizeof(CguiFlex));
    if (!node) return NULL;

    memset(node, 0, sizeof(CguiFlex));

    CguiCreateBasicNodeInto((CguiNodePtr)node);

    node->base.type   = CGUI_LAYOUT_TYPE_FLEX;
    node->base.vTable = &flexNodeVTable;

    return (CguiNodePtr)node;
}

void CguiDeleteFlex(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_FLEX) return;

    free(node);
}

void CguiSyncFlex(CguiNodePtr node, CguiNodePtr proto) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_FLEX || !proto || proto->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiSyncBasicNode(node, proto);

    CguiFlex *flexNode  = node;
    CguiFlex *protoNode = (CguiFlex *)proto;

    CGUI_PROTO_SET_PROTO(flexNode->dir, CGUI_PROTO_GET(protoNode->dir));
    CGUI_PROTO_SET_PROTO(flexNode->align, CGUI_PROTO_GET(protoNode->align));
    CGUI_PROTO_SET_PROTO(flexNode->gap, CGUI_PROTO_GET(protoNode->gap));
    CGUI_PROTO_SET_PROTO(flexNode->padBefore, CGUI_PROTO_GET(protoNode->padBefore));
    CGUI_PROTO_SET_PROTO(flexNode->padAfter, CGUI_PROTO_GET(protoNode->padAfter));
}

void CguiModelFlex(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiModelBasicNode(node);

    CguiFlex *flexNode = (CguiFlex *)node;

    CguiDir dir      = CGUI_PROTO_GET(flexNode->dir);
    float   flexSize = dir == CGUI_DIR_TOP || dir == CGUI_DIR_BOTTOM ? node->modelRec.height : node->modelRec.width;

    CGUI_ARRAY_FOR(CguiNodePtr, node->children, child) {
        int           contextIndex = -1;
        CguiFlexItem *context      = NULL;
        CGUI_ARRAY_FIND_IF(CguiContextPtr, (*child)->contexts, context, (*context)->type == CGUI_LAYOUT_TYPE_FLEX, contextIndex);
        if (contextIndex != -1) {
            context = (CguiFlexItem *)(*child)->contexts[contextIndex];
        }
    }
}

void CguiDebugDrawFlex(CguiNodePtr node, int debugLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiDebugDrawBasicNode(node, debugLevel);

    CguiFlex *flexNode = (CguiFlex *)node;

    // ...
}

void CguiDebugPrintFlex(CguiNodePtr node, int indentLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiDebugPrintBasicNode(node, indentLevel);

    CguiFlex *flexNode = (CguiFlex *)node;

    // ...
}

CguiNodePtr CguiCreateFlexT(CguiAlign align) {
    CguiNodePtr node = CguiCreateFlex();
    if (!node) return NULL;

    CguiFlex *flexNode = (CguiFlex *)node;

    CGUI_PROTO_SET_PROTO(flexNode->dir, CGUI_DIR_TOP);
    CGUI_PROTO_SET_PROTO(flexNode->align, align);

    return node;
}

CguiNodePtr CguiCreateFlexB(CguiAlign align) {
    CguiNodePtr node = CguiCreateFlex();
    if (!node) return NULL;

    CguiFlex *flexNode = (CguiFlex *)node;

    CGUI_PROTO_SET_PROTO(flexNode->dir, CGUI_DIR_BOTTOM);
    CGUI_PROTO_SET_PROTO(flexNode->align, align);

    return node;
}

CguiNodePtr CguiCreateFlexL(CguiAlign align) {
    CguiNodePtr node = CguiCreateFlex();
    if (!node) return NULL;

    CguiFlex *flexNode = (CguiFlex *)node;

    CGUI_PROTO_SET_PROTO(flexNode->dir, CGUI_DIR_LEFT);
    CGUI_PROTO_SET_PROTO(flexNode->align, align);

    return node;
}

CguiNodePtr CguiCreateFlexR(CguiAlign align) {
    CguiNodePtr node = CguiCreateFlex();
    if (!node) return NULL;

    CguiFlex *flexNode = (CguiFlex *)node;

    CGUI_PROTO_SET_PROTO(flexNode->dir, CGUI_DIR_RIGHT);
    CGUI_PROTO_SET_PROTO(flexNode->align, align);

    return node;
}

CguiContextPtr CguiCreateFlexItem(void) {
    CguiClampItem *context = malloc(sizeof(CguiClampItem));
    if (!context) return NULL;

    memset(context, 0, sizeof(CguiClampItem));

    context->base.type = CGUI_LAYOUT_TYPE_FLEX;

    return (CguiContextPtr)context;
}

void CguiDeleteFlexItem(CguiContextPtr context) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_FLEX) return;

    free(context);
}

void CguiSyncFlexItem(CguiContextPtr context, CguiContextPtr proto) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiFlexItem *flexContext  = (CguiFlexItem *)context;
    CguiFlexItem *protoContext = (CguiGridItem *)proto;

    CGUI_PROTO_SET_PROTO(flexContext->weight, CGUI_PROTO_GET(protoContext->weight));
}

void CguiDebugPrintFlexItem(CguiContextPtr context, int indentLevel) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_FLEX) return;

    CguiDebugPrintBasicContext(context, indentLevel);

    CguiFlexItem *flexContext = (CguiFlex *)context;

    // ...
}

CguiContextPtr CguiCreateFlexItemEx(float weight) {
    CguiContextPtr context = CguiCreateFlexItem();
    if (!context) return NULL;

    CguiFlexItem *flexContext = (CguiFlexItem *)context;

    CGUI_PROTO_SET_PROTO(flexContext->weight, weight);

    return context;
}

CguiNodePtr CguiCreateGrid(void) {
    CguiGrid *node = malloc(sizeof(CguiGrid));
    if (!node) return NULL;

    memset(node, 0, sizeof(CguiGrid));

    CguiCreateBasicNodeInto((CguiNodePtr)node);

    node->base.type   = CGUI_LAYOUT_TYPE_GRID;
    node->base.vTable = &gridNodeVTable;

    return (CguiNodePtr)node;
}

void CguiDeleteGrid(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_GRID) return;

    free(node);
}

void CguiSyncGrid(CguiNodePtr node, CguiNodePtr proto) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_GRID || !proto || proto->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiSyncBasicNode(node, proto);

    CguiGrid *gridNode  = node;
    CguiGrid *protoNode = (CguiGrid *)proto;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_PROTO_GET(protoNode->dir));
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_PROTO_GET(protoNode->wrapDir));
    CGUI_PROTO_SET_PROTO(gridNode->align, CGUI_PROTO_GET(protoNode->align));
    CGUI_PROTO_SET_PROTO(gridNode->wrapAlign, CGUI_PROTO_GET(protoNode->wrapAlign));
}

void CguiModelGrid(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiModelBasicNode(node);

    CguiGrid *gridNode = (CguiGrid *)node;

    // ...
}

void CguiDebugDrawGrid(CguiNodePtr node, int debugLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiDebugDrawBasicNode(node, debugLevel);

    CguiGrid *gridNode = (CguiGrid *)node;

    // ...
}

void CguiDebugPrintGrid(CguiNodePtr node, int indentLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiDebugPrintBasicNode(node, indentLevel);

    CguiGrid *gridNode = (CguiGrid *)node;

    // ...
}

CguiNodePtr CguiCreateGridTR(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_TOP);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_RIGHT);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridTL(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_TOP);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_LEFT);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridBR(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_BOTTOM);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_RIGHT);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridBL(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_BOTTOM);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_LEFT);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridLB(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_LEFT);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_BOTTOM);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridLT(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_LEFT);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_TOP);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridRB(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_RIGHT);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_BOTTOM);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiNodePtr CguiCreateGridRT(CguiAlign align, CguiAlign wrapAlign) {
    CguiNodePtr node = CguiCreateGrid();
    if (!node) return NULL;

    CguiGrid *gridNode = (CguiGrid *)node;

    CGUI_PROTO_SET_PROTO(gridNode->dir, CGUI_DIR_RIGHT);
    CGUI_PROTO_SET_PROTO(gridNode->wrapDir, CGUI_DIR_TOP);
    CGUI_PROTO_SET_PROTO(gridNode->align, align);

    return node;
}

CguiContextPtr CguiCreateGridItem(void) {
    CguiClampItem *context = malloc(sizeof(CguiClampItem));
    if (!context) return NULL;

    memset(context, 0, sizeof(CguiClampItem));

    context->base.type = CGUI_LAYOUT_TYPE_GRID;

    return (CguiContextPtr)context;
}

void CguiDeleteGridItem(CguiContextPtr context) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_GRID) return;

    free(context);
}

void CguiSyncGridItem(CguiContextPtr context, CguiContextPtr proto) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiGridItem *gridContext  = (CguiGridItem *)context;
    CguiGridItem *protoContext = (CguiGridItem *)proto;

    CGUI_PROTO_SET_PROTO(gridContext->weight, CGUI_PROTO_GET(protoContext->weight));
    CGUI_PROTO_SET_PROTO(gridContext->wrapWeight, CGUI_PROTO_GET(protoContext->wrapWeight));
}

void CguiDebugPrintGridItem(CguiContextPtr context, int indentLevel) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_GRID) return;

    CguiDebugPrintBasicContext(context, indentLevel);

    CguiGridItem *gridContext = (CguiGridItem *)context;

    // ...
}

CguiContextPtr CguiCreateGridItemEx(float weight, float wrapWeight) {
    CguiContextPtr context = CguiCreateGridItem();
    if (!context) return NULL;

    CguiGridItem *gridContext = (CguiGridItem *)context;

    CGUI_PROTO_SET_PROTO(gridContext->weight, weight);
    CGUI_PROTO_SET_PROTO(gridContext->wrapWeight, wrapWeight);

    return gridContext;
}

CguiNodePtr CguiCreateMatrix(void) {
    CguiMatrix *node = malloc(sizeof(CguiMatrix));
    if (!node) return NULL;

    memset(node, 0, sizeof(CguiMatrix));

    CguiCreateBasicNodeInto((CguiNodePtr)node);

    node->base.type   = CGUI_LAYOUT_TYPE_MATRIX;
    node->base.vTable = &matrixNodeVTable;

    return (CguiNodePtr)node;
}

void CguiDeleteMatrix(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    free(node);
}

void CguiSyncMatrix(CguiNodePtr node, CguiNodePtr proto) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_MATRIX || !proto || proto->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiSyncBasicNode(node, proto);

    CguiMatrix *matrixNode = (CguiMatrix *)node;
    CguiMatrix *protoNode  = proto;

    CGUI_PROTO_SET_PROTO(matrixNode->xSlots, CGUI_PROTO_GET(protoNode->xSlots));
    CGUI_PROTO_SET_PROTO(matrixNode->ySlots, CGUI_PROTO_GET(protoNode->ySlots));
}

void CguiModelMatrix(CguiNodePtr node) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiModelBasicNode(node);

    CguiMatrix *matrixNode = (CguiMatrix *)node;

    // ...
}

void CguiDebugDrawMatrix(CguiNodePtr node, int debugLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiDebugDrawBasicNode(node, debugLevel);

    CguiMatrix *matrixNode = (CguiMatrix *)node;

    // ...
}

void CguiDebugPrintMatrix(CguiNodePtr node, int indentLevel) {
    if (!node || node->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiDebugPrintBasicNode(node, indentLevel);

    CguiMatrix *matrixNode = (CguiMatrix *)node;

    // ...
}

CguiNodePtr CguiCreateMatrixEx(int xSlots, int ySlots) {
    CguiNodePtr node = CguiCreateMatrix();
    if (!node) return NULL;

    CguiMatrix *matrixNode = (CguiMatrix *)node;

    CGUI_PROTO_SET_PROTO(matrixNode->xSlots, xSlots);
    CGUI_PROTO_SET_PROTO(matrixNode->ySlots, ySlots);

    return node;
}

CguiContextPtr CguiCreateMatrixItem(void) {
    CguiClampItem *context = malloc(sizeof(CguiClampItem));
    if (!context) return NULL;

    memset(context, 0, sizeof(CguiClampItem));

    context->base.type = CGUI_LAYOUT_TYPE_MATRIX;

    return (CguiContextPtr)context;
}

void CguiDeleteMatrixItem(CguiContextPtr context) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    free(context);
}

void CguiSyncMatrixItem(CguiContextPtr context, CguiContextPtr proto) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiMatrixItem *matrixContext = (CguiMatrixItem *)context;
    CguiMatrixItem *protoContext  = (CguiGridItem *)proto;

    CGUI_PROTO_SET_PROTO(matrixContext->xSlot, CGUI_PROTO_GET(protoContext->xSlot));
    CGUI_PROTO_SET_PROTO(matrixContext->ySlot, CGUI_PROTO_GET(protoContext->ySlot));
    CGUI_PROTO_SET_PROTO(matrixContext->xSpan, CGUI_PROTO_GET(protoContext->xSpan));
    CGUI_PROTO_SET_PROTO(matrixContext->ySpan, CGUI_PROTO_GET(protoContext->ySpan));
    CGUI_PROTO_SET_PROTO(matrixContext->xWeight, CGUI_PROTO_GET(protoContext->xWeight));
    CGUI_PROTO_SET_PROTO(matrixContext->yWeight, CGUI_PROTO_GET(protoContext->yWeight));
}

void CguiDebugPrintMatrixItem(CguiContextPtr context, int indentLevel) {
    if (!context || context->type != CGUI_LAYOUT_TYPE_MATRIX) return;

    CguiDebugPrintBasicContext(context, indentLevel);

    CguiMatrixItem *matrixContext = (CguiMatrixItem *)context;

    // ...
}

CguiContextPtr CguiCreateMatrixItemEx(int xSlot, int ySlot, int xSpan, int ySpan) {
    CguiContextPtr context = CguiCreateMatrixItem();
    if (!context) return NULL;

    CguiMatrixItem *matrixContext = (CguiMatrixItem *)context;

    CGUI_PROTO_SET_PROTO(matrixContext->xSlot, xSlot);
    CGUI_PROTO_SET_PROTO(matrixContext->ySlot, ySlot);
    CGUI_PROTO_SET_PROTO(matrixContext->xSpan, xSpan);
    CGUI_PROTO_SET_PROTO(matrixContext->ySpan, ySpan);

    return context;
}

CguiContextPtr CguiCreateMatrixItemW(int xSlot, int ySlot, int xSpan, int ySpan, float xWeight, float yWeight) {
    CguiContextPtr context = CguiCreateMatrixItem();
    if (!context) return NULL;

    CguiMatrixItem *matrixContext = (CguiMatrixItem *)context;

    CGUI_PROTO_SET_PROTO(matrixContext->xSlot, xSlot);
    CGUI_PROTO_SET_PROTO(matrixContext->ySlot, ySlot);
    CGUI_PROTO_SET_PROTO(matrixContext->xSpan, xSpan);
    CGUI_PROTO_SET_PROTO(matrixContext->ySpan, ySpan);
    CGUI_PROTO_SET_PROTO(matrixContext->xWeight, xWeight);
    CGUI_PROTO_SET_PROTO(matrixContext->yWeight, yWeight);

    return context;
}
