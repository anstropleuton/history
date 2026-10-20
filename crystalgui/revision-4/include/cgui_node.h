// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Node system for hierarchical GUI elements
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_NODE_H_
#define CGUI_NODE_H_

#include "cgui_array.h"
#include "cgui_event.h"
#include "cgui_matrix3.h"
#include "cgui_proto.h"
#include "cgui_transition.h"
#include "raylib.h"

// Defines the Node's visual properties such as position, size, etc.
typedef struct CguiModel {
    float posRef;  // Position reference (0.0 -> relative to parent, 1.0 -> relative to screen)
    float sizeRef; // Size reference (0.0 -> relative to parent, 1.0 -> relative to screen)

    Vector2 offset;   // Offset of the node from reference position
    Vector2 anchor;   // Additional offset from reference size
    Vector2 floating; // Additional offset from current size

    Vector2 extent; // Absolute extent of the node
    Vector2 scale;  // Additional extent from reference size
} CguiModel;

CGAPI CguiModel CguiModelDefault(void);                                         // Returns a default node model
CGAPI CguiModel CguiEmpty(void);                                                // Returns a node model with zero extent and scale
CGAPI CguiModel CguiFilled(void);                                               // Fills the node to cover its parent
CGAPI CguiModel CguiMargin(float margin);                                       // Adds a margin around the node, reducing its size relative to its parent
CGAPI CguiModel CguiMarginEx(float top, float bottom, float left, float right); // Adds a margin around the node with different values for each side
CGAPI CguiModel CguiScaled(Vector2 scale);                                      // Scales the node relative to its parent

// Negative extent also sets the scale to 1.0 for below helpers

CGAPI CguiModel CguiCentered(Vector2 extent);               // Centers the node within its parent
CGAPI CguiModel CguiOffset(Vector2 offset, Vector2 extent); // Offsets the node from its parent
CGAPI CguiModel CguiTopLeft(Vector2 extent);                // Positions the node at the top-left corner of its parent
CGAPI CguiModel CguiTopRight(Vector2 extent);               // Positions the node at the top-right corner of its parent
CGAPI CguiModel CguiBottomLeft(Vector2 extent);             // Positions the node at the bottom-left corner of its parent
CGAPI CguiModel CguiBottomRight(Vector2 extent);            // Positions the node at the bottom-right corner of its parent
CGAPI CguiModel CguiTopAligned(Vector2 extent);             // Positions the node at the top-center edge of its parent
CGAPI CguiModel CguiBottomAligned(Vector2 extent);          // Positions the node at the bottom-center edge of its parent
CGAPI CguiModel CguiLeftAligned(Vector2 extent);            // Positions the node at the left-center edge of its parent
CGAPI CguiModel CguiRightAligned(Vector2 extent);           // Positions the node at the right-center edge of its parent
CGAPI CguiModel CguiTopDocked(float height);                // Positions the node at the top-center edge of its parent, covering the full width
CGAPI CguiModel CguiBottomDocked(float height);             // Positions the node at the bottom-center edge of its parent, covering the full width
CGAPI CguiModel CguiLeftDocked(float width);                // Positions the node at the left-center edge of its parent, covering the full height
CGAPI CguiModel CguiRightDocked(float width);               // Positions the node at the right-center edge of its parent, covering the full height

CGAPI Rectangle CguiCalculateModel(CguiModel model, Rectangle parentRec); // Calculates the model rectangle of a node based on its model parameters and the parent's rectangle
CGAPI CguiModel CguiLerpModel(CguiModel from, CguiModel to, float t);     // Linear interpolation between two node models based on a parameter t (0.0 to 1.0) and an easing function

// Context VTable for abstraction

struct CguiContext;
typedef struct CguiContext CguiContext;
typedef CguiContext       *CguiContextPtr;

typedef CguiContextPtr (*CguiContextCreateFunc)(void);
typedef void (*CguiContextDeleteFunc)(CguiContextPtr context);
typedef void (*CguiContextSyncFunc)(CguiContextPtr context, CguiContextPtr proto);
typedef void (*CguiContextDebugPrintFunc)(CguiContextPtr context, int indentLevel);

typedef struct CguiContextVTable {
    // Note: These must be overridden

    CguiContextCreateFunc     createContext;     // [Virtual] Function pointer for creating the context
    CguiContextDeleteFunc     deleteContext;     // [Virtual] Function pointer for deleting the context
    CguiContextSyncFunc       syncContext;       // [Virtual] Function pointer for syncing the clone with the prototype
    CguiContextDebugPrintFunc debugPrintContext; // [Virtual] Function pointer for debug printing the context
} CguiContextVTable;

CGUI_ARRAY_TYPE(CguiContextPtr);

// Defines a context structure that can hold additional parameters for a node
// This is inheritable
struct CguiContext {
    const CguiContextVTable *vTable; // Pointer to function pointers
    unsigned int             type;   // Type identifier for the context; Note: Types in the range 0xFFFF7FFF to 0xFFFFFFFF are reserved for internal use

    CGUI_PROTO(bool, enabled); // Whether the context is effective

    CguiContextPtr proto;               // Reference to the prototype context
    CGUI_ARRAY(CguiContextPtr, clones); // Array of pointers to the clone contexts (Not owned by this context)
};

CGAPI CguiContextPtr CguiCreateBasicContext(void);                                        // [Override] Create a basic context
CGAPI void           CguiDeleteBasicContext(CguiContextPtr context);                      // [Override] Delete a basic context; deletes the pointer
CGAPI void           CguiSyncBasicContext(CguiContextPtr context, CguiContextPtr proto);  // [Override] Sync a basic context with its prototype; copies all parameters from the prototype to the clone
CGAPI void           CguiDebugPrintBasicContext(CguiContextPtr context, int indentLevel); // [Override] Debug print a basic context; prints the context's details to the trace log for debugging purposes

CGAPI void CguiCreateBasicContextInto(CguiContextPtr context); // Creates a basic context into the context pointer (must be allocated)

// Node VTable for abstraction

struct CguiNode;
typedef struct CguiNode CguiNode;
typedef CguiNode       *CguiNodePtr;
typedef CguiNodePtr (*CguiNodeCreateFunc)(void);

typedef void (*CguiNodeDeleteFunc)(CguiNodePtr node);
typedef void (*CguiNodeSyncFunc)(CguiNodePtr node, CguiNodePtr proto);
typedef void (*CguiModelFunc)(CguiNodePtr node);
typedef void (*CguiNodeUpdateFunc)(CguiNodePtr node);
typedef void (*CguiNodeDrawFunc)(CguiNodePtr node);
typedef void (*CguiNodeDebugDrawFunc)(CguiNodePtr node, int debugLevel);
typedef void (*CguiNodeDebugPrintFunc)(CguiNodePtr node, int indentLevel);
typedef bool (*CguiNodeEventFunc)(CguiNodePtr node, CguiEvent *event);
typedef bool (*CguiNodeEventSinkFunc)(CguiNodePtr node, CguiEvent *event);
typedef bool (*CguiNodeEventFloatFunc)(CguiNodePtr node, CguiEvent *event);

typedef struct CguiNodeVTable {
    // Note: These must be overridden

    CguiNodeCreateFunc createNode; // [Virtual] Function pointer for creating the node
    CguiNodeDeleteFunc deleteNode; // [Virtual] Function pointer for deleting the node
    CguiNodeSyncFunc   syncNode;   // [Virtual] Function pointer for syncing the clone with the prototype

    // These may be left as NULL if the node doesn't need it
    // Recommendation: Keep update, draw and debug draw lightweight

    CguiModelFunc          modelNode;      // [Virtual] Function pointer for modeling the node
    CguiNodeUpdateFunc     updateNode;     // [Virtual] Function pointer for updating the node
    CguiNodeDrawFunc       drawNode;       // [Virtual] Function pointer for drawing the node
    CguiNodeDebugDrawFunc  debugDrawNode;  // [Virtual] Function pointer for debug drawing the node
    CguiNodeDebugPrintFunc debugPrintNode; // [Virtual] Function pointer for debug printing the node

    CguiNodeEventFunc      handleEvent;      // [Virtual] Function pointer for handling an event; returns true if the event was handled
    CguiNodeEventSinkFunc  handleSinkEvent;  // [Virtual] Function pointer for handling an event during sinking; returns true if the event was intercepted
    CguiNodeEventFloatFunc handleFloatEvent; // [Virtual] Function pointer for handling an event during floating; returns true if the event was intercepted
} CguiNodeVTable;

typedef enum CguiNodeEnableFlags {
    CGUI_ENABLE_MODEL                = 1 << 0,
    CGUI_ENABLE_MODEL_CHILDREN       = 1 << 1,
    CGUI_ENABLE_UPDATE               = 1 << 2,
    CGUI_ENABLE_UPDATE_CHILDREN      = 1 << 3,
    CGUI_ENABLE_DRAW                 = 1 << 4,
    CGUI_ENABLE_DRAW_CHILDREN        = 1 << 5,
    CGUI_ENABLE_DEBUG_DRAW           = 1 << 6,
    CGUI_ENABLE_DEBUG_DRAW_CHILDREN  = 1 << 7,
    CGUI_ENABLE_DEBUG_PRINT          = 1 << 8,
    CGUI_ENABLE_DEBUG_PRINT_CHILDREN = 1 << 9,
    CGUI_ENABLE_EVENT                = 1 << 10,
    CGUI_ENABLE_EVENT_INTERCEPT      = 1 << 11,
} CguiNodeEnableFlags;

CGUI_ARRAY_TYPE(CguiNodePtr);

// Defines a GUI node that can have child nodes, forming a hierarchical structure
// This is inheritable
typedef struct CguiNode {
    const CguiNodeVTable *vTable; // Pointer to function pointers
    unsigned int          type;   // Type identifier for the node; Note: Types in the range 0xFFFF7FFF to 0xFFFFFFFF are reserved for internal use

    int debugLevel; // 0 to not draw/print debug

    CGUI_PROTO(unsigned int, enabled);             // Bitfield of enabled functionality of the node (e.g., CGUI_ENABLE_MODEL, CGUI_ENABLE_DEBUG_DRAW_CHILDREN, etc.; all enabled by default)
    CGUI_PROTO(unsigned int, canHandleEvent);      // Bitfield of event types that the node can handle (e.g., CGUI_EVENT_MOUSE_PRESS, CGUI_EVENT_KEY_PRESS, etc.)
    CGUI_PROTO(unsigned int, canHandleSinkEvent);  // Node can intersecpt event propagation during event sinking (when propagating down to find the target node that can handle the event)
    CGUI_PROTO(unsigned int, canHandleFloatEvent); // Node can intersecpt event propagation during event floating (when propagating back up after the event is left unhandled by the target node)

    CguiNodePtr parent;                // Reference to the parent node
    CGUI_ARRAY(CguiNodePtr, children); // Array of pointers to the child nodes (Owned by this node)

    CguiNodePtr proto;               // Reference to the prototype node
    CGUI_ARRAY(CguiNodePtr, clones); // Array of pointers to the clone nodes (Not owned by this node)

    CGUI_ARRAY(CguiContextPtr, contexts); // Array of pointers to the node's contexts (Owned by this node)

    CGUI_PROTO(CguiModel, model);       // Node's visual properties; remember to set modelDirty to true!
    CGUI_PROTO(CguiMatrix3, transform); // Node's transformation matrix; remember to set transformDirty to true!

    CGUI_TRANSITION(CguiModel, model);       // Transition for the node's model
    CGUI_TRANSITION(CguiMatrix3, transform); // Transition for the node's transformation

    // Cached values

    bool        modelDirty;     // Whether the model rectangle and world quadrilateral need to be recalculated
    bool        transformDirty; // Whether the world transformation and world quadrilateral need to be recalculated
    bool        quadDirty;      // Whether the cached model rectangle or cached world transformation itself is modified externally
    Rectangle   modelRec;       // Cached rectangle representing the node's model
    CguiMatrix3 worldTransform; // Cached world transformation matrix
    CguiQuad    worldQuad;      // Cached quadrilateral representing the node's transformed shape
} CguiNode;

// Tree builder macros are used to build the GUI node graph without suffering the pain of creating local variables

#define CG_BUILD(NODE, ...)     CguiBuild((NODE), __VA_ARGS__, NULL)
#define CG_ATTACH(NODE, ...)    CguiAttach((NODE), __VA_ARGS__, NULL)
#define CG_BASIC(...)           CG_BUILD(CguiCreateBasicNode(), __VA_ARGS__, NULL)
#define CG_BASIC_EX(MODEL, ...) CG_BUILD(CguiCreateBasicNodeEx((MODEL)), __VA_ARGS__, NULL)

CGAPI CguiNodePtr CguiBuild(CguiNodePtr node, ...);  // Build a node by attaching the variadic children to the node, returns the same node or null on failure; Note: all variadic arguments must be of type CguiNodePtr  and the last variadic argument must be null pointer
CGAPI CguiNodePtr CguiAttach(CguiNodePtr node, ...); // Attach multiple contexts to the node, returns the same node or null on failure; Note: all variadic arguments must be of type CguiContextPtr  and the last variadic argument must be null pointer

CGAPI CguiNodePtr CguiCreateBasicNode(void);                                  // [Override] Create a basic node; contains default values for all parameters and does not have any children
CGAPI void        CguiDeleteBasicNode(CguiNodePtr node);                      // [Override] Delete a basic node; deletes the pointer
CGAPI void        CguiSyncBasicNode(CguiNodePtr node, CguiNodePtr proto);     // [Override] Sync a basic node, it's children, clones and contexts with their prototype; copies all parameters from the prototype to the clone
CGAPI void        CguiModelBasicNode(CguiNodePtr node);                       // [Override] Model a basic node; updates the cached modeling values based on the node's modeling parameters
CGAPI void        CguiDebugDrawBasicNode(CguiNodePtr node, int debugLevel);   // [Override] Debug draw a basic node; renders some node's data
CGAPI void        CguiDebugPrintBasicNode(CguiNodePtr node, int indentLevel); // [Override] Debug print a basic node; prints the node's details to the trace log for debugging purposes

CGAPI CguiNodePtr CguiCreateBasicNodeEx(CguiModel model);                // Creates a basic node with extended parameters
CGAPI void        CguiCreateBasicNodeInto(CguiNodePtr node);             // Creates a basic node into the node pointer (must be allocated)
CGAPI void        CguiDeleteNode(CguiNodePtr node);                      // Deletes a node
CGAPI CguiNodePtr CguiCopyNode(CguiNodePtr node);                        // Clones a node and its children without automatic syncing
CGAPI CguiNodePtr CguiCloneNode(CguiNodePtr node);                       // Clones a node and its children
CGAPI void        CguiSyncNode(CguiNodePtr node);                        // Syncs a node with its prototype
CGAPI void        CguiModelNode(CguiNodePtr node);                       // Models a node and its children if enableModel is true
CGAPI void        CguiUpdateNode(CguiNodePtr node);                      // Updates a node and its children if enableUpdate is true
CGAPI void        CguiDrawNode(CguiNodePtr node);                        // Draws a node and its children if enableDraw is true
CGAPI void        CguiDebugDrawNode(CguiNodePtr node, int debugLevel);   // Debug draws a node and its children if enableDebugDraw is true
CGAPI void        CguiDebugPrintNode(CguiNodePtr node, int indentLevel); // Debug prints a node and its children if enableDebugPrint is true

// Draw text at a rectangle's corners with alignment
// Visual diagram to help understand layout of text:
//
//      Debug text 3 TLTL   Debug text 3 TLTR   Debug text 3 TRTL   Debug text 3 TRTR
//      Debug text 2 TLTL   Debug text 2 TLTR   Debug text 2 TRTL   Debug text 2 TRTR
//      Debug text 1 TLTL   Debug text 1 TLTR   Debug text 1 TRTL   Debug text 1 TRTR
//
//                        +---------------------------------------+
//                        |                                       |
//      Debug text 1 TLBL | Debug text 1 TLBR   Debug text 1 TRBL | Debug text 1 TRBR
//      Debug text 2 TLBL | Debug text 2 TLBR   Debug text 2 TRBL | Debug text 2 TRBR
//      Debug text 3 TLBL | Debug text 3 TLBR   Debug text 3 TRBL | Debug text 3 TRBR
//                        |                                       |
//                        |                                       |
//                        |                                       |
//      Debug text 3 BLTL | Debug text 3 BLTR   Debug text 3 BRTL | Debug text 3 BRTR
//      Debug text 2 BLTL | Debug text 2 BLTR   Debug text 2 BRTL | Debug text 2 BRTR
//      Debug text 1 BLTL | Debug text 1 BLTR   Debug text 1 BRTL | Debug text 1 BRTR
//                        |                                       |
//                        +---------------------------------------+
//
//      Debug text 1 BLBL   Debug text 1 BLBR   Debug text 1 BRBL   Debug text 1 BRBR
//      Debug text 2 BLBL   Debug text 2 BLBR   Debug text 2 BRBL   Debug text 2 BRBR
//      Debug text 3 BLBL   Debug text 3 BLBR   Debug text 3 BRBL   Debug text 3 BRBR
//
// The node's default debug text rendering occurs at the TLBR region, where the corner is the TOP_LEFT and section is the BOTTOM_RIGHT
// The colors are chosen automatically, font size is 10, and default font is used
// Note: The inner regions are reserved; lines are rendered bottom-up on the top sections
typedef enum CguiDebugTextAlign {
    CGUI_DEBUG_TOP_LEFT,
    CGUI_DEBUG_TOP_RIGHT,
    CGUI_DEBUG_BOTTOM_LEFT,
    CGUI_DEBUG_BOTTOM_RIGHT,
} CguiDebugTextAlign;

CGAPI void CguiDebugDrawLines(Rectangle rec, int corner, int section, const char **lines, int linesCount); // Draws debug text at rectangle's region specified by corner and section

// Prefer using these below functions over ones from cgui_array.h for maintaining parent-child relationship
// For the rest of the functions that does not modify this relationship (e.g., CGUI_ARRAY_SHUFFLE, CGUI_ARRAY_SORT, CGUI_ARRAY_FIND_IF), feel free to use them directly

CGAPI void        CguiClearChildren(CguiNodePtr node);                                                        // Removes and deletes all children of a node
CGAPI void        CguiFillCopies(CguiNodePtr node, CguiNodePtr copy);                                         // Replaces all the children of the node with copies
CGAPI void        CguiCopyChildren(CguiNodePtr to, CguiNodePtr from);                                         // Copies all the children from one node to another node
CGAPI void        CguiCloneChildren(CguiNodePtr to, CguiNodePtr from);                                        // Clones all the children from one node to another node
CGAPI void        CguiMoveChildren(CguiNodePtr to, CguiNodePtr from);                                         // Moves all the children from one node to another node
CGAPI void        CguiSliceCopyChildren(CguiNodePtr to, CguiNodePtr from, int start, int count);              // Copies a slice of children from one node to another node
CGAPI void        CguiSliceCloneChildren(CguiNodePtr to, CguiNodePtr from, int start, int count);             // Clones a slice of children from one node to another node
CGAPI void        CguiInsertChild(CguiNodePtr node, CguiNodePtr child, int index);                            // Inserts a child to the node
CGAPI void        CguiInsertCopies(CguiNodePtr node, CguiNodePtr copy, int children);                         // Inserts `children` copies to the node's children
CGAPI void        CguiRemoveChild(CguiNodePtr child);                                                         // Removes and deletes a child from the node
CGAPI void        CguiRemoveChildAt(CguiNodePtr node, int index);                                             // Removes and deletes a child at index from the node
CGAPI CguiNodePtr CguiPopChild(CguiNodePtr child);                                                            // Removes and returns a child from the node
CGAPI CguiNodePtr CguiPopChildAt(CguiNodePtr node, int index);                                                // Removes and returns a child at index from the node
CGAPI void        CguiInsertChildFront(CguiNodePtr node, CguiNodePtr child);                                  // Inserts a child to the node at the beginning
CGAPI void        CguiInsertChildBack(CguiNodePtr node, CguiNodePtr child);                                   // Inserts a child to the node at the ending
CGAPI void        CguiRemoveChildFront(CguiNodePtr node);                                                     // Removes and deletes a child at the beginning
CGAPI void        CguiRemoveChildBack(CguiNodePtr node);                                                      // Removes and deletes a child at the ending
CGAPI CguiNodePtr CguiPopChildFront(CguiNodePtr node);                                                        // Removes and returns a child at the beginning
CGAPI CguiNodePtr CguiPopChildBack(CguiNodePtr node);                                                         // Removes and returns a child at the ending
CGAPI void        CguiFilterChildren(CguiNodePtr node, bool (*predicate)(CguiNodePtr node));                  // Filters out the children (remove and delete) children that satisfies the given predicate function
CGAPI void        CguiExtractChildren(CguiNodePtr from, CguiNodePtr to, bool (*predicate)(CguiNodePtr node)); // Filters out the children into another node children that satisfies the given predicate function

CGAPI CguiNodePtr CguiFindIfNodeInAncestors(CguiNodePtr node, bool (*predicate)(CguiNodePtr node));            // Finds the first ancestor node (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindIfNodeInDescendants(CguiNodePtr node, bool (*predicate)(CguiNodePtr node));          // Finds the first descendant node (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindIfNodeInSiblings(CguiNodePtr node, bool (*predicate)(CguiNodePtr node));             // Finds the first sibling node (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindIfContextInAncestors(CguiNodePtr node, bool (*predicate)(CguiContextPtr context));   // Finds the first context in the ancestor nodes (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindIfContextInDescendants(CguiNodePtr node, bool (*predicate)(CguiContextPtr context)); // Finds the first context in the descendant nodes (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindIfContextInSiblings(CguiNodePtr node, bool (*predicate)(CguiContextPtr context));    // Finds the first context in the sibling nodes (excluding itself) that satisfies the given predicate function
CGAPI CguiNodePtr CguiFindNodeInAncestors(CguiNodePtr node, CguiNodePtr find);                                 // Finds the first ancestor node (excluding itself) that matches the pointer
CGAPI CguiNodePtr CguiFindNodeInDescendants(CguiNodePtr node, CguiNodePtr find);                               // Finds the first descendant node (excluding itself) that matches the pointer
CGAPI CguiNodePtr CguiFindNodeInSiblings(CguiNodePtr node, CguiNodePtr find);                                  // Finds the first sibling node (excluding itself) that matches the pointer
CGAPI CguiNodePtr CguiFindContextInAncestors(CguiNodePtr node, CguiContextPtr find);                           // Finds the first context in the ancestor nodes (excluding itself) that matches the pointer
CGAPI CguiNodePtr CguiFindContextInDescendants(CguiNodePtr node, CguiContextPtr find);                         // Finds the first context in the descendant nodes (excluding itself) that matches the pointer
CGAPI CguiNodePtr CguiFindContextInSiblings(CguiNodePtr node, CguiContextPtr find);                            // Finds the first context in the sibling nodes (excluding itself) that matches the pointer

// Traversal performs the given action function on each node until the action function returns false
// These return true if traversal reached ends without action returning false

CGAPI bool CguiTraverseAncestors(CguiNodePtr node, bool (*action)(CguiNodePtr node));   // Traverses the ancestor nodes (excluding itself)
CGAPI bool CguiTraverseDescendants(CguiNodePtr node, bool (*action)(CguiNodePtr node)); // Traverses the descendant nodes (excluding itself)

// Registration

CGAPI void CguiRegisterSync(CguiNodePtr node);       // Register the node for resyncing (e.g., when changing a parameter)
CGAPI void CguiRegisterModel(CguiNodePtr node);      // Register the node for remodeling (e.g., when changing the model or transform parameter and setting the dirty flag)
CGAPI void CguiRegisterDebugPrint(CguiNodePtr node); // Register the node for debug printing
CGAPI void CguiProcessRegisteredNodes(void);         // Process registered nodes

#endif // CGUI_NODE_H_
