#ifndef CRYSTALGUI_H
#define CRYSTALGUI_H

#include "raylib.h"

#ifndef CGAPI
#define CGAPI RLAPI
#endif

#ifndef CG_MALLOC
#define CG_MALLOC(sz) MemAlloc(sz)
#endif

#ifndef CG_MALLOC_NULL
#define CG_MALLOC_NULL(sz) CG_MALLOC(sz)
#endif

#ifndef CG_REALLOC
#define CG_REALLOC(ptr, sz) MemRealloc(ptr, sz)
#endif

#ifndef CG_FREE
#define CG_FREE(ptr) MemFree(ptr)
#endif

#ifndef CG_FREE_NULL
#define CG_FREE_NULL(ptr) \
    do                    \
    {                     \
        CG_FREE(ptr);     \
        ptr = NULL;       \
    }                     \
    while (false)
#endif

#ifndef CG_LOG
#define CG_LOG(level, ...) TraceLog(level, "Crystal GUI: " __VA_ARGS__)
#endif

#ifndef CG_LOG_TRACE
#define CG_LOG_TRACE(...) CG_LOG(LOG_TRACE, __VA_ARGS__)
#endif
#ifndef CG_LOG_DEBUG
#define CG_LOG_DEBUG(...) CG_LOG(LOG_DEBUG, __VA_ARGS__)
#endif
#ifndef CG_LOG_INFO
#define CG_LOG_INFO(...) CG_LOG(LOG_INFO, __VA_ARGS__)
#endif
#ifndef CG_LOG_WARNING
#define CG_LOG_WARNING(...) CG_LOG(LOG_WARNING, __VA_ARGS__)
#endif
#ifndef CG_LOG_ERROR
#define CG_LOG_ERROR(...) CG_LOG(LOG_ERROR, __VA_ARGS__)
#endif
#ifndef CG_LOG_FATAL
#define CG_LOG_FATAL(...) CG_LOG(LOG_FATAL, __VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CguiHotkey {
    CGUI_HOTKEY_SHIFT   = 1,
    CGUI_HOTKEY_CONTROL = 2,
    CGUI_HOTKEY_ALT     = 4,
    CGUI_HOTKEY_SUPER   = 8,
} CguiHotkey;

CGAPI bool CguiIsKeyRepeated(int key);
CGAPI bool CguiIsHotkeyDown(int hotkey);
CGAPI bool CguiIsKeyComboUp(int hotkey, int key);
CGAPI bool CguiIsKeyComboDown(int hotkey, int key);
CGAPI bool CguiIsKeyComboPressed(int hotkey, int key);
CGAPI bool CguiIsKeyComboReleased(int hotkey, int key);

typedef enum CguiTextJustify {
    CGUI_TEXT_JUSTIFY_BEGIN,
    CGUI_TEXT_JUSTIFY_CENTER,
    CGUI_TEXT_JUSTIFY_END,
    CGUI_TEXT_JUSTIFY_SPACE_BETWEEN
} CguiTextJustify;

CGAPI void CguiDrawTextPro(const char *text, Font font, Rectangle bounds, float fontSize, float spacing, float lineSpacing, Color color, int justify);

CGAPI void CguiDrawTextureDest(Texture texture, Rectangle dest, Color tint);
CGAPI void CguiDrawTextureFullscreen(Texture texture, Color tint);
CGAPI void CguiDrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint);
CGAPI void CguiDrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint);
CGAPI void CguiDrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint);
CGAPI void CguiDrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint);

CGAPI Vector2   CguiGetTextureSizeV(Texture texture);
CGAPI Rectangle CguiGetTextureSizeRec(Texture texture);
CGAPI Vector2   CguiGetRenderTextureSizeV(RenderTexture renderTexture);
CGAPI Rectangle CguiGetRenderTextureSizeRec(RenderTexture renderTexture);

CGAPI Vector2   CguiGetScreenSizeV(void);
CGAPI Rectangle CguiGetScreenSizeRec(void);
CGAPI Vector2   CguiGetMonitorSizeV(int monitor);
CGAPI Rectangle CguiGetMonitorSizeRec(int monitor);
CGAPI int       CguiGetAppWidth(void);
CGAPI int       CguiGetAppHeight(void);
CGAPI Vector2   CguiGetAppSizeV(void);
CGAPI Rectangle CguiGetAppSizeRec(void);

CGAPI Vector2   CguiRotatePoint(Vector2 point, Vector2 origin, float angle);
CGAPI Rectangle CguiRecZero(void);
CGAPI Texture   CguiLoadTextureFromRenderTexture(RenderTexture renderTexture);
CGAPI float     CguiGetMinRoundness(Rectangle rec, float roundness);
CGAPI Color     CguiColorMultiply(Color color, Vector4 values);
CGAPI Color     CguiColorFromHSL(float hue, float saturation, float lightness);
CGAPI Color     CguiColorFromHSLA(float hue, float saturation, float lightness, float alpha);
CGAPI Color     CguiColorFromVecHSL(Vector3 hsl);
CGAPI Color     CguiColorFromVecHSLA(Vector4 hsla);
CGAPI Vector3   CguiColorToHSLA(Color color);
CGAPI Vector4   CguiColorToVecHSLA(Color color);
CGAPI void      CguiBeginScissorModeRec(Rectangle area);
CGAPI Rectangle CguiFlipRectangleX(Rectangle rec, float axis);
CGAPI Rectangle CguiFlipRectangleY(Rectangle rec, float axis);
CGAPI Rectangle CguiFlipRectangleXY(Rectangle rec, Vector2 axis);

typedef float (*CguiEasingFunction)(float t);

CGAPI float CguiEaseLinear(float t);

CGAPI float CguiEaseIn(float t, float power);
CGAPI float CguiEaseOut(float t, float power);
CGAPI float CguiEaseInOut(float t, float power);

CGAPI float CguiEaseInQuad(float t);
CGAPI float CguiEaseOutQuad(float t);
CGAPI float CguiEaseInOutQuad(float t);

CGAPI float CguiEaseInCubic(float t);
CGAPI float CguiEaseOutCubic(float t);
CGAPI float CguiEaseInOutCubic(float t);

CGAPI float CguiEaseInQuart(float t);
CGAPI float CguiEaseOutQuart(float t);
CGAPI float CguiEaseInOutQuart(float t);

CGAPI float CguiEaseInQuint(float t);
CGAPI float CguiEaseOutQuint(float t);
CGAPI float CguiEaseInOutQuint(float t);

CGAPI float CguiEaseInSextic(float t);
CGAPI float CguiEaseOutSextic(float t);
CGAPI float CguiEaseInOutSextic(float t);

CGAPI float CguiEaseInSeptic(float t);
CGAPI float CguiEaseOutSeptic(float t);
CGAPI float CguiEaseInOutSeptic(float t);

CGAPI float CguiEaseInOctic(float t);
CGAPI float CguiEaseOutOctic(float t);
CGAPI float CguiEaseInOutOctic(float t);

CGAPI float CguiEaseInNonic(float t);
CGAPI float CguiEaseOutNonic(float t);
CGAPI float CguiEaseInOutNonic(float t);

CGAPI float CguiEaseInSine(float t);
CGAPI float CguiEaseOutSine(float t);
CGAPI float CguiEaseInOutSine(float t);

CGAPI float CguiEaseInExpo(float t);
CGAPI float CguiEaseOutExpo(float t);
CGAPI float CguiEaseInOutExpo(float t);

CGAPI float CguiEaseInExpoPro(float t, float base, float power);
CGAPI float CguiEaseOutExpoPro(float t, float base, float power);
CGAPI float CguiEaseInOutExpoPro(float t, float base, float power);

CGAPI float CguiEaseInCirc(float t);
CGAPI float CguiEaseOutCirc(float t);
CGAPI float CguiEaseInOutCirc(float t);

CGAPI float CguiEaseInBack(float t);
CGAPI float CguiEaseOutBack(float t);
CGAPI float CguiEaseInOutBack(float t);

CGAPI float CguiEaseInBackPro(float t, float overshoot);
CGAPI float CguiEaseOutBackPro(float t, float overshoot);
CGAPI float CguiEaseInOutBackPro(float t, float overshoot);

CGAPI float CguiEaseInElastic(float t);
CGAPI float CguiEaseOutElastic(float t);
CGAPI float CguiEaseInOutElastic(float t);

CGAPI float CguiEaseInElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseOutElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseInOutElasticPro(float t, float amplitude, float period, float base, float power);

CGAPI float CguiEaseInBounce(float t);
CGAPI float CguiEaseOutBounce(float t);
CGAPI float CguiEaseInOutBounce(float t);

CGAPI float CguiEaseOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);

typedef enum CguiEventType {
    CGUI_EVENT_TYPE_MOUSE_MOVE,
    CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS,
    CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE,
    CGUI_EVENT_TYPE_KEYBOARD_KEY_PRESS,
    CGUI_EVENT_TYPE_KEYBOARD_KEY_RELEASE,
} CguiEventType;

typedef struct CguiEvent {
    int eventType;
} CguiEvent;

typedef struct CguiMouseMoveEvent {
    int     eventType;
    Vector2 delta;
    bool    moveIn;
    bool    moveOut;
} CguiMouseMoveEvent;

typedef struct CguiMouseButtonPressEvent {
    int eventType;
    int button;
} CguiMouseButtonPressEvent;

typedef struct CguiMouseButtonReleaseEvent {
    int eventType;
    int button;
} CguiMouseButtonReleaseEvent;

typedef struct CguiKeyboardKeyPressEvent {
    int eventType;
    int key;
    int typingChar;
} CguiKeyboardKeyPressEvent;

typedef struct CguiKeyboardKeyReleaseEvent {
    int eventType;
    int key;
} CguiKeyboardKeyReleaseEvent;

typedef struct CguiTransformation {
    Vector2 position;
    Vector2 size;
    Vector2 isRelativePosition;
    Vector2 isRelativeSize;
    Vector2 anchor;
    Vector2 shrink;
} CguiTransformation;

struct CguiNode;
typedef struct CguiNode CguiNode;

typedef void (*CguiNodeFunction)(CguiNode *node);
typedef bool (*CguiTransformNodeFunction)(CguiNode *node);
typedef bool (*CguiHandleEventFunction)(CguiNode *node, CguiEvent *event);

struct CguiNode {
    bool  enabled;
    char *name;
    int   type;
    void *data;
    int   dataSize;

    CguiTransformation transformation;
    Rectangle          bounds;
    bool               rebound;

    CguiNode  *parent;
    CguiNode **children;
    int        childrenCount;
    int        childrenCapacity;

    CguiNode        *templateSource;
    CguiNode       **instances;
    int              instancesCount;
    int              instancesCapacity;
    bool             resync;
    void            *instanceData;
    int              instanceDataSize;
    CguiNodeFunction override;

    CguiTransformNodeFunction transform;
    CguiNodeFunction          updatePre;
    CguiNodeFunction          updatePost;
    CguiNodeFunction          drawPre;
    CguiNodeFunction          drawPost;
    CguiNodeFunction          debugDraw;
    CguiNodeFunction          deleteNodeData;

    bool                    canHandleMouseEvents;
    bool                    canHandleKeyboardEvents;
    bool                    includeNavKeys;
    CguiHandleEventFunction handleEvent;
};

CGAPI CguiNode *CguiCreateNode(void);
CGAPI CguiNode *CguiCreateNodeEx(const char *name);
CGAPI CguiNode *CguiCreateNodePro(const char *name, int type, const void *data, int dataSize);
CGAPI CguiNode *CguiCreateNodeProMax(const char *name, int type, const void *data, int dataSize, const void *instanceData, int instanceDataSize);
CGAPI void      CguiDeleteNode(CguiNode *node);
CGAPI void      CguiDeleteNodeSelf(CguiNode *node);
CGAPI bool      CguiRenameNode(CguiNode *node, const char *newName);

CGAPI void CguiTransformNode(CguiNode *node, bool rebound);
CGAPI bool CguiTransformNodeSelf(CguiNode *node, bool rebound);
CGAPI void CguiUpdateNode(CguiNode *node);
CGAPI void CguiUpdatePreNodeSelf(CguiNode *node);
CGAPI void CguiUpdatePostNodeSelf(CguiNode *node);
CGAPI void CguiDrawNode(CguiNode *node);
CGAPI void CguiDrawPreNodeSelf(CguiNode *node);
CGAPI void CguiDrawPostNodeSelf(CguiNode *node);
CGAPI void CguiDebugDrawNode(CguiNode *node);
CGAPI void CguiDebugDrawNodeSelf(CguiNode *node);

CGAPI CguiNode *CguiCloneNode(CguiNode *node);
CGAPI CguiNode *CguiCloneNodeSelf(CguiNode *node);

CGAPI CguiNode *CguiCreateInstance(CguiNode *templateNode);
CGAPI CguiNode *CguiCreateInstanceSelf(CguiNode *templateNode);
CGAPI void      CguiApplyTemplateResync(CguiNode *node);
CGAPI bool      CguiLinkTemplate(CguiNode *node, CguiNode *templateNode);
CGAPI bool      CguiUnlinkTemplate(CguiNode *node);
CGAPI bool      CguiSetInstancesCapacity(CguiNode *node, int newCapacity);
CGAPI int       CguiFindInstanceIndex(CguiNode *templateSource, CguiNode *instance);
CGAPI void      CguiSyncInstances(CguiNode *node, bool resync);
CGAPI bool      CguiSyncInstancesSelf(CguiNode *node, bool resync);
CGAPI void      CguiSyncHierarchy(CguiNode *node);

CGAPI CguiNode *CguiCreateChild(CguiNode *parent);
CGAPI CguiNode *CguiCreateChildAt(CguiNode *parent, int childIndex);
CGAPI bool      CguiInsertChild(CguiNode *parent, CguiNode *child);
CGAPI bool      CguiInsertChildAt(CguiNode *parent, CguiNode *child, int childIndex);
CGAPI bool      CguiRemoveChild(CguiNode *parent, CguiNode *child);
CGAPI bool      CguiRemoveChildAt(CguiNode *parent, int childIndex);
CGAPI bool      CguiDeleteChild(CguiNode *parent, CguiNode *child);
CGAPI bool      CguiDeleteChildAt(CguiNode *parent, int childIndex);
CGAPI bool      CguiRemoveAllChildren(CguiNode *parent);
CGAPI bool      CguiDeleteAllChildren(CguiNode *parent);
CGAPI bool      CguiSetChildrenCapacity(CguiNode *node, int newCapacity);
CGAPI bool      CguiReserveChildrenCapacity(CguiNode *node, int capacity);
CGAPI bool      CguiShrinkChildrenCapacity(CguiNode *node);
CGAPI bool      CguiCloneAllChildren(CguiNode *fromParent, CguiNode *toParent);
CGAPI bool      CguiCloneChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent);
CGAPI bool      CguiTransferChild(CguiNode *fromParent, CguiNode *child, CguiNode *toParent);
CGAPI bool      CguiTransferChildAt(CguiNode *fromParent, int childIndex, CguiNode *toParent);
CGAPI bool      CguiTransferAllChildren(CguiNode *fromParent, CguiNode *toParent);
CGAPI bool      CguiTransferChildrenRange(CguiNode *fromParent, int indexBegin, int indexEnd, CguiNode *toParent);

CGAPI CguiTransformation CguiTZeroSize(void);
CGAPI CguiTransformation CguiTAbsolute(Vector2 pos, Vector2 size);
CGAPI CguiTransformation CguiTOffset(Vector2 offset, Vector2 size);
CGAPI CguiTransformation CguiTScale(Vector2 pos, Vector2 scale);
CGAPI CguiTransformation CguiTOffsetScale(Vector2 offset, Vector2 scale);
CGAPI CguiTransformation CguiTCentered(Vector2 size);
CGAPI CguiTransformation CguiTPadded(float padding);
CGAPI CguiTransformation CguiTFillParent(void);
CGAPI bool               CguiIsTransformationEqual(CguiTransformation a, CguiTransformation b);
CGAPI void               CguiSetTransformation(CguiNode *node, CguiTransformation t);

CGAPI bool      CguiIsTreeStructureEqual(CguiNode *a, CguiNode *b);
CGAPI bool      CguiIsTreeTypeEqual(CguiNode *a, CguiNode *b);
CGAPI bool      CguiCopyNodeValues(CguiNode *fromNode, CguiNode *toNode);
CGAPI bool      CguiCopyNodeValuesNoTi(CguiNode *fromNode, CguiNode *toNode);
CGAPI bool      CguiCopyNode(CguiNode *fromNode, CguiNode *toNode);
CGAPI bool      CguiCopyNodeNoTi(CguiNode *fromNode, CguiNode *toNode);
CGAPI int       CguiFindChildIndex(CguiNode *parent, CguiNode *child);
CGAPI bool      CguiIsDescendantOf(CguiNode *parent, CguiNode *child);
CGAPI bool      CguiIsAncestorOf(CguiNode *child, CguiNode *parent);
CGAPI CguiNode *CguiFindTypeInChildren(CguiNode *parent, int type);
CGAPI CguiNode *CguiFindTypeInParents(CguiNode *child, int type);
CGAPI Rectangle CguiComputeNodeBounds(CguiNode *node);
CGAPI CguiNode *CguiCheckCollision(CguiNode *node, Vector2 point);

typedef struct CguiAnimatedTexture {
    Texture           *frames;
    int                framesCount;
    int                framesCapacity;
    float              delay;
    float              duration;
    CguiEasingFunction easer;
    int                repeatCount;
    bool               repeatDelay;
} CguiAnimatedTexture;

CGAPI CguiAnimatedTexture CguiLoadAnimatedTexture(const char *filename);
CGAPI void                CguiUnloadAnimatedTexture(CguiAnimatedTexture texture);
CGAPI void                CguiAnimatedTextureInsertFrame(CguiAnimatedTexture *texture, Texture frame, int index);
CGAPI void                CguiAnimatedTextureRemoveFrame(CguiAnimatedTexture *texture, int index);

CGAPI void CguiDrawAnimatedTexture(CguiAnimatedTexture texture, float startTime, float x, float y, Color tint);
CGAPI void CguiDrawAnimatedTextureEx(CguiAnimatedTexture texture, float startTime, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

typedef struct CguiTransition {
    float              delay;
    float              duration;
    CguiEasingFunction easer;
    int                repeatCount;
    bool               repeatDelay;
} CguiTransition;

CGAPI int                CguiTransitInt(int a, int b, CguiTransition transition, float startTime);
CGAPI float              CguiTransitFloat(float a, float b, CguiTransition transition, float startTime);
CGAPI Vector2            CguiTransitVector2(Vector2 a, Vector2 b, CguiTransition transition, float startTime);
CGAPI Vector3            CguiTransitVector3(Vector3 a, Vector3 b, CguiTransition transition, float startTime);
CGAPI Vector4            CguiTransitVector4(Vector4 a, Vector4 b, CguiTransition transition, float startTime);
CGAPI Color              CguiTransitColor(Color a, Color b, CguiTransition transition, float startTime);
CGAPI Rectangle          CguiTransitRectangle(Rectangle a, Rectangle b, CguiTransition transition, float startTime);
CGAPI CguiTransformation CguiTransitTransformation(CguiTransformation a, CguiTransformation b, CguiTransition transition, float startTime);

typedef enum CguiLayoutNodeType {
    CGUI_LAYOUT_NODE_TYPE_CLAMP = 0x00FF0100,
    CGUI_LAYOUT_NODE_TYPE_CLAMP_ITEM,
    CGUI_LAYOUT_NODE_TYPE_LINEAR,
    CGUI_LAYOUT_NODE_TYPE_LINEAR_ITEM,
    CGUI_LAYOUT_NODE_TYPE_GRID,
    CGUI_LAYOUT_NODE_TYPE_GRID_ITEM,
} CguiLayoutNodeType;

typedef enum CguiLayoutDirection {
    CGUI_LAYOUT_DIRECTION_X,
    CGUI_LAYOUT_DIRECTION_Y,

    CGUI_LAYOUT_DIRECTION_HORIZONTAL = CGUI_LAYOUT_DIRECTION_X,
    CGUI_LAYOUT_DIRECTION_VERTICAL   = CGUI_LAYOUT_DIRECTION_Y,
    CGUI_LAYOUT_DIRECTION_COLUMN     = CGUI_LAYOUT_DIRECTION_X,
    CGUI_LAYOUT_DIRECTION_ROW        = CGUI_LAYOUT_DIRECTION_Y
} CguiLayoutDirection;

typedef enum CguiLayoutJustify {
    CGUI_LAYOUT_JUSTIFY_BEGIN,
    CGUI_LAYOUT_JUSTIFY_CENTER,
    CGUI_LAYOUT_JUSTIFY_END,
    CGUI_LAYOUT_JUSTIFY_SPACE_BETWEEN,
} CguiLayoutJustify;

typedef struct CguiClampLayoutData {

    int empty;
} CguiClampLayoutData;

CGAPI CguiNode *CguiCreateClampLayout(void);
CGAPI bool      CguiTransformClampLayout(CguiNode *node);

typedef struct CguiClampLayoutItemData {
    bool    preserveAspectRatio;
    float   aspectRatio;
    bool    clampFill;
    Vector2 minSize;
    Vector2 maxSize;
} CguiClampLayoutItemData;

CGAPI CguiNode *CguiCreateClampLayoutItem(bool preserveAspectRatio, float aspectRatio, bool clampFill, Vector2 minSize, Vector2 maxSize);

typedef struct CguiLinearLayoutData {
    int   direction;
    int   justify;
    float spacing;
} CguiLinearLayoutData;

CGAPI CguiNode *CguiCreateLinearLayout(int direction, int justify, float spacing);
CGAPI bool      CguiTransformLinearLayout(CguiNode *node);

typedef struct CguiLinearLayoutItemData {
    float weight;
    float minSize;
    float maxSize;
} CguiLinearLayoutItemData;

CGAPI CguiNode *CguiCreateLinearLayoutItem(float weight, float minSize, float maxSize);

typedef struct CguiGridLayoutSlotData {
    float weight;
    float minSize;
    float maxSize;

    float position;
    float size;
} CguiGridLayoutSlotData;

typedef struct CguiGridLayoutData {
    CguiGridLayoutSlotData *xSlots;
    int                     xSlotsCount;
    int                     xSlotsCapacity;
    CguiGridLayoutSlotData *ySlots;
    int                     ySlotsCount;
    int                     ySlotsCapacity;
    int                     xJustify;
    int                     yJustify;
    Vector2                 spacing;
} CguiGridLayoutData;

CGAPI CguiNode *CguiCreateGridLayout(int xSlotsCount, int ySlotsCount, int xJustify, int yJustify, Vector2 spacing);
CGAPI void      CguiDeleteGridLayoutData(CguiNode *node);
CGAPI bool      CguiTransformGridLayout(CguiNode *node);

CGAPI bool CguiGridLayoutInsertSlotX(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize);
CGAPI bool CguiGridLayoutInsertSlotY(CguiGridLayoutData *layoutData, int index, float weight, float minSize, float maxSize);
CGAPI bool CguiGridLayoutRemoveSlotX(CguiGridLayoutData *layoutData, int index);
CGAPI bool CguiGridLayoutRemoveSlotY(CguiGridLayoutData *layoutData, int index);

typedef struct CguiGridLayoutItemData {
    int xSlot;
    int ySlot;
    int xSpan;
    int ySpan;
} CguiGridLayoutItemData;

CGAPI CguiNode *CguiCreateGridLayoutItem(int xSlot, int ySlot, int xSpan, int ySpan);

typedef enum CguiElementNodeType {
    CGUI_ELEMENT_NODE_TYPE_TEXT = 0x00FF0800,
    CGUI_ELEMENT_NODE_TYPE_TEXTURE,
    CGUI_ELEMENT_NODE_TYPE_BOX,
} CguiElementNodeType;

typedef struct CguiTextElementData {
    const char *text;
    Font        font;
    float       fontSize;
    float       spacing;
    float       lineSpacing;
    Color       color;
    int         justify;
} CguiTextElementData;

CGAPI CguiNode *CguiCreateTextElement(const char *text, Font font, float fontSize, float spacing, float lineSpacing, Color color, int justification);
CGAPI void      CguiDrawPreTextElement(CguiNode *node);

typedef struct CguiTextureElementData {
    Texture   texture;
    Rectangle source;
    Vector2   origin;
    float     rotation;
    Color     tint;
} CguiTextureElementData;

CGAPI CguiNode *CguiCreateTextureElement(Texture texture, Rectangle source, Vector2 origin, float rotation, Color tint);
CGAPI void      CguiDrawPreTextureElement(CguiNode *node);

typedef struct CguiBoxElementData {
    Vector4 radii;
    Color   color;
    Texture texture;

    float   shadowDist;
    Vector2 shadowOffset;
    float   shadowShrink;
    Color   shadowColor;
    Texture shadowTexture;

    float   borderThickness;
    Color   borderColor;
    Texture borderTexture;
} CguiBoxElementData;

CGAPI CguiNode *CguiCreateBoxElement(Vector4 radii, Color color, Texture texture, float shadowDist, Vector2 shadowOffset, float shadowShrink, Color shadowColor, Texture shadowTexture, float borderThickness, Color borderColor, Texture borderTexture);
CGAPI void      CguiDrawPreBoxElement(CguiNode *node);

typedef enum CguiCommonOverrideField {
    CGUI_COMMON_OVERRIDE_FIELD_ENABLED        = 1 << 0,
    CGUI_COMMON_OVERRIDE_FIELD_NAME           = 1 << 1,
    CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION = 1 << 2
} CguiCommonOverrideField;

typedef struct CguiCommonOverrides {
    bool               enabled;
    const char        *name;
    CguiTransformation transformation;
    int                fields;
} CguiCommonOverrides;

CGAPI void CguiApplyOverrides(CguiNode *node, CguiCommonOverrides overrides);
CGAPI void CguiOverrideEnabled(CguiNode *node, CguiCommonOverrides *overrides, bool enabled);
CGAPI void CguiOverrideName(CguiNode *node, CguiCommonOverrides *overrides, const char *name);
CGAPI void CguiOverrideTransformation(CguiNode *node, CguiCommonOverrides *overrides, CguiTransformation transformation);

typedef enum CguiComponentNodeType {
    CGUI_COMPONENT_NODE_TYPE_ROOT = 0x00FF0F00,
    CGUI_COMPONENT_NODE_TYPE_LABEL,
    CGUI_COMPONENT_NODE_TYPE_BUTTON,
    CGUI_COMPONENT_NODE_TYPE_MAX
} CguiComponentNodeType;

typedef enum CguiComponentID {
    CGUI_COMPONENT_ROOT,
    CGUI_COMPONENT_LABEL,
    CGUI_COMPONENT_BUTTON,
    CGUI_COMPONENT_MAX
} CguiComponentID;

typedef struct CguiRootData {
    Color backgroundColor;
} CguiRootData;

typedef struct CguiRootInstanceData {
    CguiCommonOverrides overrides;
} CguiRootInstanceData;

CGAPI CguiNode            *CguiCreateRoot(void);
CGAPI void                 CguiPreDrawRoot(CguiNode *node);
CGAPI void                 CguiOverrideRoot(CguiNode *node);
CGAPI CguiCommonOverrides *CguiGetRootOverrides(CguiNode *node);

typedef enum CguiLabelComposition {
    CGUI_LABEL_COMPOSITION_TEXT_NODE,
} CguiLabelComposition;

typedef enum CguiLabelType {
    CGUI_LABEL_TYPE_HEADING,
    CGUI_LABEL_TYPE_BODY,
    CGUI_LABEL_TYPE_CAPTION
} CguiLabelType;

typedef struct CguiLabelData {

    CguiTextElementData headingTextData;
    CguiTextElementData bodyTextData;
    CguiTextElementData captionTextData;
    CguiTextElementData disabledHeadingTextData;
    CguiTextElementData disabledBodyTextData;
    CguiTextElementData disabledCaptionTextData;
} CguiLabelData;

typedef struct CguiLabelInstanceData {
    const char *text;
    int         type;
    bool        disabled;

    CguiCommonOverrides overrides;
} CguiLabelInstanceData;

CGAPI CguiNode *CguiCreateLabel(Rectangle bounds, const char *text, int type, bool disabled);
CGAPI void      CguiPreUpdateLabel(CguiNode *node);
CGAPI void      CguiOverrideLabel(CguiNode *node);

typedef void (*CguiButtonPressCallback)(CguiNode *button);

typedef enum CguiButtonComposition {
    CGUI_BUTTON_COMPOSITION_BOX_NODE
} CguiButtonComposition;

typedef enum CguiButtonType {
    CGUI_BUTTON_TYPE_NORMAL,
    CGUI_BUTTON_TYPE_FLAT,
    CGUI_BUTTON_TYPE_SUPERFLAT,
    CGUI_BUTTON_TYPE_ACCENT,
    CGUI_BUTTON_TYPE_DANGEROUS,
    CGUI_BUTTON_TYPE_WARNING,
    CGUI_BUTTON_TYPE_SUCCESS,
    CGUI_BUTTON_TYPE_INFO
} CguiButtonType;

typedef struct CguiButtonData {
    CguiButtonPressCallback pressCallback;

    bool hovered;
    bool held;
    bool active;

    CguiBoxElementData boxData;
    CguiBoxElementData flatBoxData;
    CguiBoxElementData superflatBoxData;
    CguiBoxElementData accentBoxData;
    CguiBoxElementData dangerousBoxData;
    CguiBoxElementData warningBoxData;
    CguiBoxElementData successBoxData;
    CguiBoxElementData infoBoxData;
} CguiButtonData;

CGAPI CguiNode *CguiCreateButton(Rectangle bounds, CguiButtonPressCallback pressCallback);
CGAPI void      CguiPreUpdateButton(CguiNode *node);
CGAPI bool      CguiHandleButtonEvents(CguiNode *node, CguiEvent *event);

struct CguiTheme;
typedef struct CguiTheme CguiTheme;

typedef void (*CguiDeleteThemeData)(CguiTheme *theme);

struct CguiTheme {
    char               *themeName;
    void               *themeData;
    int                 themeDataSize;
    CguiDeleteThemeData deleteThemeData;
    CguiNode          **templates;
};

CGAPI CguiTheme *CguiCreateTheme(const char *themeName);
CGAPI void       CguiDeleteTheme(CguiTheme *theme);
CGAPI void       CguiSetActiveTheme(CguiTheme *theme);
CGAPI CguiTheme *CguiGetActiveTheme(void);
CGAPI CguiTheme *CguiGetDefaultTheme(void);
CGAPI CguiNode  *CguiGetComponentTemplate(CguiTheme *theme, int componentType);
CGAPI bool       CguiSetComponentTemplate(CguiTheme *theme, CguiNode *node);

typedef struct CguiCrystallineThemeData {

    float accentHue;
    float dangerHue;
    float warningHue;
    float successHue;
    float infoHue;

    Vector3 backgroundSLA;
    Vector3 backgroundHoverSLA;
    Vector3 backgroundHeldSLA;
    Vector3 containerSLA;
    Vector3 containerHoverSLA;
    Vector3 containerHeldSLA;
    Vector3 componentSLA;
    Vector3 componentHoverSLA;
    Vector3 componentHeldSLA;
    Vector3 foregroundSLA;
    Vector3 foregroundHoverSLA;
    Vector3 foregroundHeldSLA;

    Vector3 shadowSLA;
    Vector3 borderSLA;
    Vector3 focusBorderSLA;

    Vector3 textSLA;
    Vector3 disabledTextSLA;

    float headingFontSize;
    float bodyFontSize;
    float captionFontSize;

    float headingLineHeight;
    float bodyLineHeight;
    float captionLineHeight;

    Font textFont;
    Font textFontBold;
    Font textFontItalic;
    Font textFontBoldItalic;
    Font textFontLight;
    Font textFontLightItalic;
} CguiCrystallineThemeData;

CGAPI CguiTheme *CguiCreateCrystallineThemeDark(void);
CGAPI CguiTheme *CguiCreateCrystallineThemeLight(void);
CGAPI CguiTheme *CguiCreateCrystallineThemeDarkHighContrast(void);
CGAPI CguiTheme *CguiCreateCrystallineThemeLightHighContrast(void);
CGAPI CguiTheme *CguiCreateCrystallineThemeFromData(CguiCrystallineThemeData data);
CGAPI void       CguiDeleteCrystallineTheme(CguiTheme *theme);

CGAPI void CguiInit(void);
CGAPI void CguiClose(void);
CGAPI void CguiUpdate(CguiNode *root);
CGAPI void CguiDraw(CguiNode *root, bool debugBounds);

#ifdef __cplusplus
}
#endif

#endif
