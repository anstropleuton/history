// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Layout system for arranging GUI elements
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_LAYOUT_H_
#define CGUI_LAYOUT_H_

#include "cgui_node.h"

// Note: Layout nodes overrides its direct children's model
// Note: If layout item's context is missing, it will insert a new one

typedef enum CguiLayoutType {
    CGUI_LAYOUT_TYPE_CLAMP = 0xFFFF8000, // Clamp layout
    CGUI_LAYOUT_TYPE_FLEX,               // Flex layout
    CGUI_LAYOUT_TYPE_GRID,               // Grid layout
    CGUI_LAYOUT_TYPE_MATRIX,             // Matrix layout
} CguiLayoutType;

typedef enum CguiAlign {
    CGUI_ALIGN_BEGIN,         // Align at the beginning
    CGUI_ALIGN_CENTER,        // Align at the center
    CGUI_ALIGN_END,           // Align at the end
    CGUI_ALIGN_SPACE_BETWEEN, // Add space between items
    CGUI_ALIGN_SPACE_EVENLY,  // Add even space between items (including before first and after last)
    CGUI_ALIGN_SPACE_AROUND,  // Add even space around items (including half-size before first and after last)
} CguiAlign;

typedef enum CguiDir {
    CGUI_DIR_TOP,    // Layout items towards top
    CGUI_DIR_BOTTOM, // Layout items towards bottom
    CGUI_DIR_LEFT,   // Layout items towards left
    CGUI_DIR_RIGHT,  // Layout items towards right
} CguiDir;

typedef enum CguiClampStyle {
    CGUI_CLAMP_STYLE_FIT,  // Fit the content when clamping with aspect ratio
    CGUI_CLAMP_STYLE_FILL, // Fill the content when clamping with aspect ratio (may become larger than parent)
} CguiClampStyle;

// Clamp layout: Clamps items to min/max models
typedef struct CguiClamp {
    CguiNode base;                   // [Inheritance] Base class
    CGUI_PROTO(CguiModel, minModel); // Minimum model for items
    CGUI_PROTO(CguiModel, maxModel); // Maximum model for items
    CGUI_PROTO(bool, useMin);        // Clamp at minimum
    CGUI_PROTO(bool, useMax);        // Clamp at maximum
    CGUI_PROTO(bool, reposition);    // Whether to reposition the item before performing clamping at maximum (true by default)
} CguiClamp;

#define CG_CLAMP(...)              CG_BUILD(CguiCreateClamp(), __VA_ARGS__, NULL)
#define CG_CLAMP_MN(MIN, ...)      CG_BUILD(CguiCreateClampMn((MIN)), __VA_ARGS__, NULL)
#define CG_CLAMP_MX(MAX, ...)      CG_BUILD(CguiCreateClampMx((MAX)), __VA_ARGS__, NULL)
#define CG_CLAMP_MM(MIN, MAX, ...) CG_BUILD(CguiCreateClampMm((MIN), (MAX)), __VA_ARGS__, NULL)

CGAPI CguiNodePtr CguiCreateClamp(void);                                  // [Override] Create a clamp layout
CGAPI void        CguiDeleteClamp(CguiNodePtr node);                      // [Override] Delete a clamp layout
CGAPI void        CguiSyncClamp(CguiNodePtr node, CguiNodePtr proto);     // [Override] Sync a clamp layout
CGAPI void        CguiModelClamp(CguiNodePtr node);                       // [Override] Model a clamp layout
CGAPI void        CguiDebugDrawClamp(CguiNodePtr node, int debugLevel);   // [Override] Debug draw a clamp layout
CGAPI void        CguiDebugPrintClamp(CguiNodePtr node, int indentLevel); // [Override] Debug print a clamp layout

CGAPI CguiNodePtr CguiCreateClampMn(CguiModel minModel);                     // Creates a clamp layout with a minimum model
CGAPI CguiNodePtr CguiCreateClampMx(CguiModel maxModel);                     // Creates a clamp layout with a maximum model
CGAPI CguiNodePtr CguiCreateClampMm(CguiModel minModel, CguiModel maxModel); // Creates a clamp layout with min and max models

// Clamp layout item context: Additional parameters for each item in a clamp layout
typedef struct CguiClampItem {
    CguiContext base;            // [Inheritance] Base class
    CGUI_PROTO(float, aspect);   // Item aspect ratio (0.0f to not preserve aspect ratio, inf to mimic parent's aspect ratio)
    CGUI_PROTO(int, clampStyle); // Clamping style (fit vs fill) for preserving the aspect ratio
} CguiClampItem;

CGAPI CguiContextPtr CguiCreateClampItem(void);                                        // [Override] Create a clamp layout item context
CGAPI void           CguiDeleteClampItem(CguiContextPtr context);                      // [Override] Delete a clamp layout item context
CGAPI void           CguiSyncClampItem(CguiContextPtr context, CguiContextPtr proto);  // [Override] Sync a clamp layout item context
CGAPI void           CguiDebugPrintClampItem(CguiContextPtr context, int indentLevel); // [Override] Debug print a clamp layout item context

CGAPI CguiContextPtr CguiCreateClampItemEx(float aspect); // Creates a clamp layout item context with extended parameters

// Flex layout: Arranges items linearly
typedef struct CguiFlex {
    CguiNode base;                     // [Inheritance] Base class
    CGUI_PROTO(CguiDir, dir);          // Direction for items to flow
    CGUI_PROTO(CguiAlign, align);      // Alignment of items in the flow direction
    CGUI_PROTO(CguiAlign, crossAlign); // Alignment of items in the direction 90 degrees to the flow direction
    CGUI_PROTO(float, gap);            // Gap between items in the flow direction
    CGUI_PROTO(float, padBefore);      // Padding before the first item in the flow direction
    CGUI_PROTO(float, padAfter);       // Padding after the last item in the flow direction
} CguiFlex;

#define CG_FLEX(...)          CG_BUILD(CguiCreateFlex(), __VA_ARGS__, NULL)
#define CG_FLEX_T(ALIGN, ...) CG_BUILD(CguiCreateFlexT((ALIGN)), __VA_ARGS__, NULL)
#define CG_FLEX_B(ALIGN, ...) CG_BUILD(CguiCreateFlexB((ALIGN)), __VA_ARGS__, NULL)
#define CG_FLEX_L(ALIGN, ...) CG_BUILD(CguiCreateFlexL((ALIGN)), __VA_ARGS__, NULL)
#define CG_FLEX_R(ALIGN, ...) CG_BUILD(CguiCreateFlexR((ALIGN)), __VA_ARGS__, NULL)

CGAPI CguiNodePtr CguiCreateFlex(void);                                  // [Override] Create a flex layout
CGAPI void        CguiDeleteFlex(CguiNodePtr node);                      // [Override] Delete a flex layout
CGAPI void        CguiSyncFlex(CguiNodePtr node, CguiNodePtr proto);     // [Override] Sync a flex layout
CGAPI void        CguiModelFlex(CguiNodePtr node);                       // [Override] Model a flex layout
CGAPI void        CguiDebugDrawFlex(CguiNodePtr node, int debugLevel);   // [Override] Debug draw a flex layout
CGAPI void        CguiDebugPrintFlex(CguiNodePtr node, int indentLevel); // [Override] Debug print a flex layout

CGAPI CguiNodePtr CguiCreateFlexT(CguiAlign align); // Creates a flex layout with bottom-to-top flow
CGAPI CguiNodePtr CguiCreateFlexB(CguiAlign align); // Creates a flex layout with top-to-bottom flow
CGAPI CguiNodePtr CguiCreateFlexL(CguiAlign align); // Creates a flex layout with right-to-left flow
CGAPI CguiNodePtr CguiCreateFlexR(CguiAlign align); // Creates a flex layout with left-to-right flow

// Flex layout item context: Additional parameters for each item in a flex layout
typedef struct CguiFlexItem {
    CguiContext base;           // [Inheritance] Base class
    CGUI_PROTO(float, weight);  // Item weight in the flow direction (0 is treated as 1)
    CGUI_PROTO(float, minSize); // Minimum size of the item in the flow direction (0 by default)
    CGUI_PROTO(float, maxSize); // Maximum size of the item in the flow direction (inf by default)
} CguiFlexItem;

CGAPI CguiContextPtr CguiCreateFlexItem(void);                                        // [Override] Create a flex layout item context
CGAPI void           CguiDeleteFlexItem(CguiContextPtr context);                      // [Override] Delete a flex layout item context
CGAPI void           CguiSyncFlexItem(CguiContextPtr context, CguiContextPtr proto);  // [Override] Sync a flex layout item context
CGAPI void           CguiDebugPrintFlexItem(CguiContextPtr context, int indentLevel); // [Override] Debug print a flex layout item context

CGAPI CguiContextPtr CguiCreateFlexItemEx(float weight); // Creates a flex layout item context with extended parameters

// Grid layout: Arranges items in a grid
typedef struct CguiGrid {
    CguiNode base;                    // [Inheritance] Base class
    CGUI_PROTO(CguiDir, dir);         // Direction for items to flow
    CGUI_PROTO(CguiDir, wrapDir);     // Direction for items to wrap
    CGUI_PROTO(CguiAlign, align);     // Alignment of items in the flow direction
    CGUI_PROTO(CguiAlign, wrapAlign); // Alignment of items in the wrap direction
} CguiGrid;

#define CG_GRID(...)                       CG_BUILD(CguiCreateGrid(), __VA_ARGS__, NULL)
#define CG_GRID_TR(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridTR((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_TL(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridTL((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_BR(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridBR((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_BL(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridBL((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_LB(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridLB((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_LT(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridLT((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_RB(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridRB((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)
#define CG_GRID_RT(ALIGN, WRAP_ALIGN, ...) CG_BUILD(CguiCreateGridRT((ALIGN), (WRAP_ALIGN)), __VA_ARGS__, NULL)

CGAPI CguiNodePtr CguiCreateGrid(void);                                  // [Override] Create a grid layout
CGAPI void        CguiDeleteGrid(CguiNodePtr node);                      // [Override] Delete a grid layout
CGAPI void        CguiSyncGrid(CguiNodePtr node, CguiNodePtr proto);     // [Override] Sync a grid layout
CGAPI void        CguiModelGrid(CguiNodePtr node);                       // [Override] Model a grid layout
CGAPI void        CguiDebugDrawGrid(CguiNodePtr node, int debugLevel);   // [Override] Debug draw a grid layout
CGAPI void        CguiDebugPrintGrid(CguiNodePtr node, int indentLevel); // [Override] Debug print a grid layout

CGAPI CguiNodePtr CguiCreateGridTR(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with bottom-to-top flow and left-to-right wrap direction
CGAPI CguiNodePtr CguiCreateGridTL(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with bottom-to-top flow and right-to-left wrap direction
CGAPI CguiNodePtr CguiCreateGridBR(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with top-to-bottom flow and left-to-right wrap direction
CGAPI CguiNodePtr CguiCreateGridBL(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with top-to-bottom flow and right-to-left wrap direction
CGAPI CguiNodePtr CguiCreateGridLB(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with right-to-left flow and top-to-bottom wrap direction
CGAPI CguiNodePtr CguiCreateGridLT(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with right-to-left flow and bottom-to-top wrap direction
CGAPI CguiNodePtr CguiCreateGridRB(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with left-to-right flow and top-to-bottom wrap direction
CGAPI CguiNodePtr CguiCreateGridRT(CguiAlign align, CguiAlign wrapAlign); // Creates a grid layout with left-to-right flow and bottom-to-top wrap direction

// Grid layout item context: Additional parameters for each item in a grid layout
typedef struct CguiGridItem {
    CguiContext base;              // [Inheritance] Base class
    CGUI_PROTO(float, weight);     // Item weight in the flow direction (0 is treated as 1)
    CGUI_PROTO(float, wrapWeight); // Item weight in the wrap direction (0 is treated as 1)
} CguiGridItem;

CGAPI CguiContextPtr CguiCreateGridItem(void);                                        // [Override] Create a grid layout item context
CGAPI void           CguiDeleteGridItem(CguiContextPtr context);                      // [Override] Delete a grid layout item context
CGAPI void           CguiSyncGridItem(CguiContextPtr context, CguiContextPtr proto);  // [Override] Sync a grid layout item context
CGAPI void           CguiDebugPrintGridItem(CguiContextPtr context, int indentLevel); // [Override] Debug print a grid layout item context

CGAPI CguiContextPtr CguiCreateGridItemEx(float weight, float wrapWeight); // Creates a grid layout item context with extended parameters

// Matrix layout: Arranges items in a fixed grid
typedef struct CguiMatrix {
    CguiNode base;           // [Inheritance] Base class
    CGUI_PROTO(int, xSlots); // Total number of slots in x-axis
    CGUI_PROTO(int, ySlots); // Total number of slots in y-axis
} CguiMatrix;

#define CG_MATRIX(X_SLOTS, Y_SLOTS, ...) CG_BUILD(CguiCreateMatrixEx((X_SLOTS), (Y_SLOTS)), __VA_ARGS__, NULL)

CGAPI CguiNodePtr CguiCreateMatrix(void);                                  // [Override] Create a matrix layout
CGAPI void        CguiDeleteMatrix(CguiNodePtr node);                      // [Override] Delete a matrix layout
CGAPI void        CguiSyncMatrix(CguiNodePtr node, CguiNodePtr proto);     // [Override] Sync a matrix layout
CGAPI void        CguiModelMatrix(CguiNodePtr node);                       // [Override] Model a matrix layout
CGAPI void        CguiDebugDrawMatrix(CguiNodePtr node, int debugLevel);   // [Override] Debug draw a matrix layout
CGAPI void        CguiDebugPrintMatrix(CguiNodePtr node, int indentLevel); // [Override] Debug print a matrix layout

CGAPI CguiNodePtr CguiCreateMatrixEx(int xSlots, int ySlots); // Creates a matrix layout with specified number of slots in x and y axes

// Matrix layout item context: Additional parameters for each item in a matrix layout
typedef struct CguiMatrixItem {
    CguiContext base;           // [Inheritance] Base class
    CGUI_PROTO(int, xSlot);     // Starting slot in x-axis for the item
    CGUI_PROTO(int, ySlot);     // Starting slot in y-axis for the item
    CGUI_PROTO(int, xSpan);     // Number of slots in x-axis the item spans
    CGUI_PROTO(int, ySpan);     // Number of slots in y-axis the item spans
    CGUI_PROTO(float, xWeight); // Item weight in x-axis (0 is treated as 1)
    CGUI_PROTO(float, yWeight); // Item weight in y-axis (0 is treated as 1)
} CguiMatrixItem;

CGAPI CguiContextPtr CguiCreateMatrixItem(void);                                        // [Override] Create a matrix layout item context
CGAPI void           CguiDeleteMatrixItem(CguiContextPtr context);                      // [Override] Delete a matrix layout item context
CGAPI void           CguiSyncMatrixItem(CguiContextPtr context, CguiContextPtr proto);  // [Override] Sync a matrix layout item context
CGAPI void           CguiDebugPrintMatrixItem(CguiContextPtr context, int indentLevel); // [Override] Debug print a matrix layout item context

CGAPI CguiContextPtr CguiCreateMatrixItemEx(int xSlot, int ySlot, int xSpan, int ySpan);                              // Creates a matrix layout item context with extended parameters
CGAPI CguiContextPtr CguiCreateMatrixItemW(int xSlot, int ySlot, int xSpan, int ySpan, float xWeight, float yWeight); // Creates a matrix layout item context with extended parameters including weights

#endif // CGUI_LAYOUT_H_
