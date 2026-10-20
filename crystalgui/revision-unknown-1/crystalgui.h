#ifndef CRYSTALGUI_H
#define CRYSTALGUI_H

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HOTKEY_SHIFT   = 0x0001,
    HOTKEY_CONTROL = 0x0002,
    HOTKEY_ALT     = 0x0004,
    HOTKEY_SUPER   = 0x0008,
} Hotkey;

typedef struct Vector2Int {
    int x;
    int y;
} Vector2Int;

typedef struct Vector3Int {
    int x;
    int y;
    int z;
} Vector3Int;

typedef struct Vector4Int {
    int x;
    int y;
    int z;
    int w;
} Vector4Int;

RLAPI bool IsHotkeyPressed(int hotkey);

RLAPI void DrawEllipseLinesEx(int centerX, int centerY, float radiusH, float radiusV, float lineThick, Color color);
RLAPI void DrawTriangleLinesEx(Vector2 v1, Vector2 v2, Vector2 v3, float lineThick, Color color);
RLAPI void DrawRectangleRoundedPro(Rectangle rec, float roundness, int segments, float rotation, Color color);
RLAPI void DrawRectangleRoundedLinesPro(Rectangle rec, float roundness, int segments, float rotation, float lineThick, Color color);
RLAPI void DrawRectangleLinesPro(Rectangle rec, float lineThick, float rotation, Color color);
RLAPI bool CheckCollisionPointRoundedRec(Vector2 point, Rectangle rec, float roundness);
RLAPI bool CheckCollisionPointRoundedRecEx(Vector2 point, Rectangle rec, float roundness, Vector2 origin, float rotation);

RLAPI void DrawTextureDest(Texture texture, Rectangle dest, Color tint);
RLAPI void DrawRenderTextureDest(RenderTexture renderTexture, Rectangle dest, Color tint);
RLAPI void DrawTextureFullscreen(Texture texture, Color tint);
RLAPI void DrawTextureFullscreenEx(Texture texture, Rectangle src, Color tint);
RLAPI void DrawRenderTextureFullscreen(RenderTexture renderTexture, Color tint);
RLAPI void DrawRenderTextureFullscreenEx(RenderTexture renderTexture, Rectangle src, Color tint);

RLAPI Vector2 GetTextureSizeV(Texture texture);
RLAPI Rectangle GetTextureSizeRec(Texture texture);
RLAPI Vector2 GetRenderTextureSizeV(RenderTexture renderTexture);
RLAPI Rectangle GetRenderTextureSizeRec(RenderTexture renderTexture);
RLAPI Vector2 RotatePoint(Vector2 point, Vector2 origin, float angle);
RLAPI Vector2 GetScreenResolutionV(void);
RLAPI Rectangle GetScreenResolutionRec(void);
RLAPI Vector2 GetMonitorResolutionV(int monitor);
RLAPI Rectangle GetMonitorResolutionRec(int monitor);
RLAPI int GetAppWidth(void);
RLAPI int GetAppHeight(void);
RLAPI Vector2 GetAppResolutionV(void);
RLAPI Rectangle GetAppResolutionRec(void);
RLAPI Texture LoadTextureFromRenderTexture(RenderTexture renderTexture);
RLAPI float GetMinRoundness(Rectangle rec, float roundness);
RLAPI Color ColorMultiply(Color color, Vector4 values);

#ifndef CGUI_API
#define CGUI_API RLAPI
#endif

#define CGUI_RAW_STRING(...) #__VA_ARGS__

#define CGUI_LOG(__cguiType, ...) TraceLog(__cguiType, __VA_ARGS__)

typedef enum CguiBlurShaderLocationIndex {
    CGUI_BLUR_SHADER_LOC_BLUR_RADIUS = 0,
    CGUI_BLUR_SHADER_LOC_KERNEL_SIZE,
    CGUI_BLUR_SHADER_LOC_MAX,
} CguiBlurShaderLocationIndex;

typedef struct CguiBlurShaderUniforms {
    float blurRadius;
    int kernelSize;
} CguiBlurShaderUniforms;

typedef struct CguiBlurShader {
    Shader shader;
    int locs[CGUI_BLUR_SHADER_LOC_MAX];
    CguiBlurShaderUniforms values;
    unsigned int id;
} CguiBlurShader;

CGUI_API const char *CguiGetBlurVertexShaderSource(void);
CGUI_API const char *CguiGetBlurFragmentShaderSource(void);
CGUI_API CguiBlurShader *CguiLoadBlurShader(void);
CGUI_API CguiBlurShader *CguiLoadBlurShaderEx(const char *vsCode, const char *fsCode);
CGUI_API void CguiUnloadBlurShader(CguiBlurShader *shader);
CGUI_API void CguiSetBlurShaderValues(CguiBlurShader *shader);

CGUI_API void CguiShaderDrawBlurredTexture(CguiBlurShader *shader, Texture texture);
CGUI_API void CguiShaderDrawBlurredTextureEx(CguiBlurShader *shader, Texture texture, float blurRadius, int kernelSize);

typedef enum CguiRectangleShaderLocationIndex {
    CGUI_RECTANGLE_SHADER_LOC_RECTANGLE = 0,
    CGUI_RECTANGLE_SHADER_LOC_TINT,
    CGUI_RECTANGLE_SHADER_LOC_ROUNDNESS,
    CGUI_RECTANGLE_SHADER_LOC_SHADOW_RADIUS,
    CGUI_RECTANGLE_SHADER_LOC_SHADOW_SIZE,
    CGUI_RECTANGLE_SHADER_LOC_SHADOW_OFFSET,
    CGUI_RECTANGLE_SHADER_LOC_SHADOW_COLOR,
    CGUI_RECTANGLE_SHADER_LOC_BORDER_THICKNESS,
    CGUI_RECTANGLE_SHADER_LOC_BORDER_COLOR,
    CGUI_RECTANGLE_SHADER_LOC_MAX
} CguiRectangleShaderLocationIndex;

typedef struct CguiRectangleShaderUniforms {
    float rectangle[4];
    float tint[4];
    float roundness;
    float shadowRadius;
    float shadowSize;
    float shadowOffset[2];
    float shadowColor[4];
    float borderThickness;
    float borderColor[4];
} CguiRectangleShaderUniforms;

typedef struct CguiRectangleShader {
    Shader shader;
    int locs[CGUI_RECTANGLE_SHADER_LOC_MAX];
    CguiRectangleShaderUniforms values;
    unsigned int id;
} CguiRectangleShader;

CGUI_API const char *CguiGetRectangleVertexShaderSource(void);
CGUI_API const char *CguiGetRectangleFragmentShaderSource(void);
CGUI_API CguiRectangleShader *CguiLoadRectangleShader(void);
CGUI_API CguiRectangleShader *CguiLoadRectangleShaderEx(const char *vsCode, const char *fsCode);
CGUI_API void CguiUnloadRectangleShader(CguiRectangleShader *shader);
CGUI_API void CguiSetRectangleShaderValues(CguiRectangleShader *shader);

CGUI_API void CguiShaderDrawRectangle(CguiRectangleShader *shader, Texture texture, Color tint);
CGUI_API void CguiShaderDrawRectangleRec(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness);
CGUI_API void CguiShaderDrawRectangleEx(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness, Color shadowColor, float borderThickness, Color borderColor);
CGUI_API void CguiShaderDrawRectanglePro(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness, float shadowRadius, float shadowSize, Vector2 shadowOffset, Color shadowColor, float borderThickness, Color borderColor);

typedef enum CguiGradientShaderUniformLocationIndex {
    CGUI_GRADIENT_SHADER_LOC_COLOR_1 = 0,
    CGUI_GRADIENT_SHADER_LOC_COLOR_2,
    CGUI_GRADIENT_SHADER_LOC_POS_1,
    CGUI_GRADIENT_SHADER_LOC_POS_2,
    CGUI_GRADIENT_SHADER_LOC_MAX
} CguiGradientShaderUniformLocationIndex;

typedef struct CguiGradientShaderUniforms {
    float color1[4];
    float color2[4];
    float pos1[2];
    float pos2[2];
} CguiGradientShaderUniforms;

typedef struct CguiGradientShader {
    Shader shader;
    int locs[CGUI_GRADIENT_SHADER_LOC_MAX];
    CguiGradientShaderUniforms values;
    unsigned int id;
} CguiGradientShader;

CGUI_API const char *CguiGetGradientVertexShaderSource(void);
CGUI_API const char *CguiGetGradientFragmentShaderSource(void);
CGUI_API CguiGradientShader *CguiLoadGradientShader(void);
CGUI_API CguiGradientShader *CguiLoadGradientShaderEx(const char *vsCode, const char *fsCode);
CGUI_API void CguiUnloadGradientShader(CguiGradientShader *shader);
CGUI_API void CguiSetGradientShaderValues(CguiGradientShader *shader);

CGUI_API void CguiBeginGradientMode(CguiGradientShader *shader);
CGUI_API void CguiBeginGradientModeEx(CguiGradientShader *shader, Color color1, Color color2, Vector2 pos1, Vector2 pos2);
CGUI_API void CguiEndGradientMode(CguiGradientShader *shader);
CGUI_API void CguiDrawTextureGradient(CguiGradientShader *shader, Texture texture, Rectangle dest);
CGUI_API void CguiDrawTextureGradientEx(CguiGradientShader *shader, Color color1, Color color2, Vector2 pos1, Vector2 pos2, Texture texture, Rectangle src, Rectangle dest);

typedef enum CguiCombinationShaderUniformLocationIndex {
    CGUI_COMBINATION_SHADER_LOC_TEXTURE_1 = 0,
    CGUI_COMBINATION_SHADER_LOC_TEXTURE_2,
    CGUI_COMBINATION_SHADER_LOC_SRC_1,
    CGUI_COMBINATION_SHADER_LOC_DEST_1,
    CGUI_COMBINATION_SHADER_LOC_TINT_1,
    CGUI_COMBINATION_SHADER_LOC_SRC_2,
    CGUI_COMBINATION_SHADER_LOC_DEST_2,
    CGUI_COMBINATION_SHADER_LOC_TINT_2,
    CGUI_COMBINATION_SHADER_LOC_OPERATION,
    CGUI_COMBINATION_SHADER_LOC_RATIO,
    CGUI_COMBINATION_SHADER_LOC_MAX
} CguiCombinationShaderUniformLocationIndex;

typedef struct CguiCombinationShaderUniforms {
    float src1[4];
    float dest1[4];
    float tint1[4];
    float src2[4];
    float dest2[4];
    float tint2[4];
    int operation;
    float ratio;
} CguiCombinationShaderUniforms;

typedef struct CguiCombinationShader {
    Shader shader;
    int locs[CGUI_COMBINATION_SHADER_LOC_MAX];
    CguiCombinationShaderUniforms values;
    unsigned int id;
} CguiCombinationShader;

CGUI_API const char *CguiGetCombinationVertexShaderSource(void);
CGUI_API const char *CguiGetCombinationFragmentShaderSource(void);
CGUI_API CguiCombinationShader *CguiLoadCombinationShader(void);
CGUI_API CguiCombinationShader *CguiLoadCombinationShaderEx(const char *vsCode, const char *fsCode);
CGUI_API void CguiUnloadCombinationShader(CguiCombinationShader *shader);
CGUI_API void CguiSetCombinationShaderValues(CguiCombinationShader *shader);

CGUI_API void CguiShaderDrawTextureCombined(CguiCombinationShader *shader, Texture texture1, Texture texture2, Rectangle dest);
CGUI_API void CguiShaderDrawTextureCombinedEx(CguiCombinationShader *shader, Texture texture1, Color tint1, Texture texture2, Color tint2, Rectangle dest);
CGUI_API void CguiShaderDrawTextureCombinedRec(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Color tint1, Texture texture2, Rectangle src2, Color tint2, Rectangle dest);
CGUI_API void CguiShaderDrawTextureCombinedPro(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Rectangle dest1, Color tint1, Texture texture2, Rectangle src2, Rectangle dest2, Color tint2, Rectangle dest);

typedef struct CguiCombinedTexture {
    Texture texture;
    unsigned int id;
} CguiCombinedTexture;

CGUI_API CguiCombinedTexture CguiGenerateCombinedTexture(CguiCombinationShader *shader, Texture texture1, Texture texture2, Vector2 textureSize);
CGUI_API CguiCombinedTexture CguiGenerateCombinedTextureEx(CguiCombinationShader *shader, Texture texture1, Color tint1, Texture texture2, Color tint2, Vector2 textureSize);
CGUI_API CguiCombinedTexture CguiGenerateCombinedTextureRec(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Color tint1, Texture texture2, Rectangle src2, Color tint2, Vector2 textureSize);
CGUI_API CguiCombinedTexture CguiGenerateCombinedTexturePro(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Rectangle dest1, Color tint1, Texture texture2, Rectangle src2, Rectangle dest2, Color tint2, Vector2 textureSize);
CGUI_API void CguiDisposeCombinedTexture(CguiCombinedTexture texture);

typedef struct CguiBlurTexture {
    Texture originalTexture;
    Texture blurredTexture;
    float blurRadius;
    int kernelSize;
    unsigned int id;
} CguiBlurTexture;

CGUI_API CguiBlurTexture CguiGenerateBlurTexture(CguiBlurShader *shader, Texture texture);
CGUI_API CguiBlurTexture CguiGenerateBlurTextureEx(CguiBlurShader *shader, Texture texture, float blurRadius, int kernelSize);
CGUI_API void CguiDisposeBlurTexture(CguiBlurTexture blurredTexture);

typedef struct CguiRectangleTexture {
    CguiRectangleShaderUniforms values;
    Texture texture;
    Vector2 center;
    unsigned int id;
} CguiRectangleTexture;

CGUI_API CguiRectangleTexture CguiGenerateRectangleTexture(CguiRectangleShader *shader);
CGUI_API CguiRectangleTexture CguiGenerateRectangleTextureEx(CguiRectangleShader *shader, Color color);
CGUI_API void CguiDisposeRectangleTexture(CguiRectangleTexture rectangle);

typedef struct CguiMultiRoundnessRectangleTexture {
    CguiRectangleTexture *rectangleTextures;
    int rectanglesCount;
    unsigned int id;
} CguiMultiRoundnessRectangleTexture;

CGUI_API CguiMultiRoundnessRectangleTexture CguiGenerateMultiRoundnessRectangleTexture(CguiRectangleShader *shader, float startingRoundness, float roundnessDiff, int rectanglesCount);
CGUI_API CguiMultiRoundnessRectangleTexture CguiGenerateMultiRoundnessRectangleTextureEx(CguiRectangleShader *shader, float startingRoundness, float roundnessDiff, int rectanglesCount, Color color);
CGUI_API void CguiDisposeMultiRoundnessRectangleTexture(CguiMultiRoundnessRectangleTexture rectangles);

CGUI_API void CguiDrawRectangle(CguiMultiRoundnessRectangleTexture rectangles, Rectangle dest, float roundness, Color tint);

typedef struct CguiMultiSizeFont {
    Font *fonts;
    int fontsCount;
    unsigned int id;
} CguiMultiSizeFont;

CGUI_API CguiMultiSizeFont CguiLoadMultiSizeFont(const char *fontFilename, float startingFontSize, float fontSizeDiff, int fontsCount, int *codepoints, int codepointCount);
CGUI_API void CguiUnloadMultiSizeFont(CguiMultiSizeFont multiSizeFont);
CGUI_API void CguiDrawText(CguiMultiSizeFont multiSizeFont, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
CGUI_API Vector2 CguiMeasureText(CguiMultiSizeFont multiSizeFont, const char *text, float fontSize, float spacing);
CGUI_API void CguiDrawTextShadow(CguiMultiSizeFont multiSizeFont, const char *text, Vector2 position, float fontSize, float spacing, Color tint, CguiBlurShader *blurShader, float shadowRadius, Vector2 shadowOffset, Color shadowColor);
CGUI_API Vector2 CguiMeasureTextShadow(CguiMultiSizeFont multiSizeFont, const char *text, float fontSize, float spacing, float shadowRadius, Vector2 shadowOffset);

typedef struct CguiFontCollection {
    CguiMultiSizeFont regularFont;
    CguiMultiSizeFont boldFont;
    CguiMultiSizeFont italicFont;
    CguiMultiSizeFont boldItalicFont;
    CguiMultiSizeFont monospaceFont;
} CguiFontCollection;

CGUI_API CguiFontCollection CguiLoadFontCollection(const char *regularFontFilename, const char *boldFontFilename, const char *italicFontFilename, const char *boldItalicFontFilename, const char *monospaceFontFilename, float startingFontSize, float fontSizeDiff, int fontsCount, int *codepoints, int codepointCount);
CGUI_API void CguiUnloadFontCollection(CguiFontCollection fontCollection);

typedef struct CguiTextTexture {
    Texture texture;
    const char *text;
    CguiMultiSizeFont font;
} CguiTextTexture;

typedef struct CguiMarkdownConfig {
    float h1Size;
    float h2Size;
    float h3Size;
    float h4Size;
    float h5Size;
    float h6Size;

} CguiMarkdownConfig;

CGUI_API CguiTextTexture CguiGenerateTextTexture(const char *textInHTML, const char *textInCSS);
CGUI_API CguiTextTexture CguiGenerateTextTextureFromMarkdown(CguiFontCollection fontCollection, CguiMarkdownConfig config, const char *text, float fontSize, float spacing, Color color, Vector2 bounds);
CGUI_API void CguiDisposeTextTexture(CguiTextTexture textTexture);

typedef enum CguiTextEditorCharacterType {
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_UNKNOWN = 0,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_CONTROL,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_ALPHABET,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_NUMBER,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_SYMBOL,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_SPACE,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_TAB,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_NEW_LINE,
    CGUI_TEXT_EDITOR_CHARACTER_TYPE_MAX
} CguiTextEditorCharacterType;

typedef enum CguiTextEditorActionType {
    CGUI_TEXT_EDITOR_ACTION_TYPE_NONE = 0,
    CGUI_TEXT_EDITOR_ACTION_TYPE_INSERT,
    CGUI_TEXT_EDITOR_ACTION_TYPE_REMOVE,
    CGUI_TEXT_EDITOR_ACTION_TYPE_MAX
} CguiTextEditorActionType;

typedef struct CguiTextEditorHistory {
    char *text;
    int allocatedLength;
    int *cursorPositions;
    int *cursorSelections;
    int cursorsCount;
    struct CguiTextEditorHistory *previous;
    struct CguiTextEditorHistory *next;
} CguiTextEditorHistory;

typedef struct CguiTextEditor {
    char *text;
    int allocatedLength;

    int *cursorPositions;
    int *cursorSelections;
    int cursorsCount;

    int previousActionType;

    bool multiLine;
    char *newLines;
    int linesCount;

    CguiTextEditorHistory *undoHistory;
    CguiTextEditorHistory *redoHistory;
    int undoHistoryLimit;

    unsigned int id;
} CguiTextEditor;

CGUI_API CguiTextEditor CguiLoadTextEditor(void);
CGUI_API void CguiUnloadTextEditor(CguiTextEditor textEditor);
CGUI_API void CguiUpdateTextEditor(CguiTextEditor *textEditor);

typedef enum CguiShape {
    CGUI_ICON_SHAPE_UNKNOWN = 0,
    CGUI_ICON_SHAPE_LINE,
    CGUI_ICON_SHAPE_CIRCLE,
    CGUI_ICON_SHAPE_ELLIPSE,
    CGUI_ICON_SHAPE_RECTANGLE,
    CGUI_ICON_SHAPE_TRIANGLE,
    CGUI_ICON_SHAPE_POLY,
    CGUI_ICON_SHAPE_MAX
} CguiShape;

typedef struct CguiShapeBasic {
    int iconShapeType;

} CguiShapeBasic;

typedef struct CguiShapeLine {
    int iconShapeType;
    Color color;
    Vector2 startPos;
    Vector2 endPos;
    float lineThick;
} CguiShapeLine;

typedef struct CguiShapeCircle {
    int iconShapeType;
    Color color;
    Vector2 center;
    float radius;
    float lineThick;
    float startAngle;
    float endAngle;
    int segments;
} CguiShapeCircle;

typedef struct CguiShapeEllipse {
    int iconShapeType;
    Color color;
    Vector2 center;
    float radiusH;
    float radiusV;
    float lineThick;
} CguiShapeEllipse;

typedef struct CguiShapeRectangle {
    int iconShapeType;
    Color color;
    Rectangle rec;
    float rotation;
    float lineThick;
    float roundness;
} CguiShapeRectangle;

typedef struct CguiShapeTriangle {
    int iconShapeType;
    Color color;
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;
    float lineThick;
} CguiShapeTriangle;

typedef struct CguiShapePoly {
    int iconShapeType;
    Color color;
    Vector2 center;
    int sides;
    float radius;
    float rotation;
    float lineThick;
} CguiShapePoly;

typedef struct CguiIcon {
    CguiShapeBasic **shapes;
    int shapesCount;
} CguiIcon;

typedef struct CguiIconTexture {
    Texture texture;
    unsigned int id;
} CguiIconTexture;

typedef struct CguiAnimatedIcon {
    CguiIconTexture *animatedTextures;
    int animatedTexturesCount;
    float animationDuration;
    unsigned int id;
} CguiAnimatedIcon;

CGUI_API CguiShapeBasic *CguiGenerateShape(const char *shapeFormat);
CGUI_API void CguiDisposeShape(CguiShapeBasic *shape);
CGUI_API void CguiDrawShape(CguiShapeBasic *shape, Vector2 position, float scaling);

CGUI_API CguiIcon CguiGenerateIcon(const char *iconShapesFormat);
CGUI_API void CguiDisposeIcon(CguiIcon icon);
CGUI_API void CguiDrawIcon(CguiIcon icon, Vector2 position, float scaling);

CGUI_API CguiIconTexture CguiGenerateIconTexture(CguiIcon icon, float scaling);
CGUI_API void CguiDisposeIconTexture(CguiIconTexture iconTexture);
CGUI_API void CguiDrawIconTexture(CguiIconTexture iconTexture, Vector2 position);
CGUI_API void CguiDrawIconTextureEx(CguiIconTexture iconTexture, Rectangle dest, Color tint);

CGUI_API CguiAnimatedIcon CguiGenerateAnimatedIcon(const char *animatedIconFormat, float scaling, float animationDuration);
CGUI_API void CguiDisposeAnimatedIcon(CguiAnimatedIcon animatedIcon);
CGUI_API void CguiDrawAnimatedIcon(CguiAnimatedIcon animatedIcon, Vector2 position, float startTime);
CGUI_API void CguiDrawAnimatedIconEx(CguiAnimatedIcon animatedIcon, Rectangle dest, Color tint, float startTime);

typedef struct CguiGradientColor {
    Color color1;
    Color color2;
    Vector2 pos1;
    Vector2 pos2;
} CguiGradientColor;

CGUI_API void CguiBeginGradientModeColor(CguiGradientShader *gradientShader, CguiGradientColor color, Rectangle bounds);

typedef struct CguiColorTheme {

    CguiGradientColor darkBackgroundColor;
    CguiGradientColor backgroundColor;
    CguiGradientColor lightBackgroundColor;

    CguiGradientColor darkWidgetColor;
    CguiGradientColor widgetColor;
    CguiGradientColor lightWidgetColor;

    CguiGradientColor accentColor;
    CguiGradientColor suggestColor;
    CguiGradientColor warningColor;
    CguiGradientColor errorColor;

    CguiGradientColor darkMainColor;
    CguiGradientColor mainColor;
    CguiGradientColor lightMainColor;

    CguiGradientColor textColor;
    CguiGradientColor textColorDisabled;

    CguiGradientColor iconColor;
    CguiGradientColor iconColorDisabled;

    Vector4 hoverTintColor;
    Vector4 heldTintColor;
} CguiColorTheme;

CguiColorTheme CguiGetAdwaitaColorTheme(void);
CguiColorTheme CguiGetArOsWinConceptColorTheme(void);

typedef struct CguiResources {
    CguiBlurShader *blurShader;
    CguiRectangleShader *rectangleShader;
    CguiGradientShader *gradientShader;

    CguiMultiRoundnessRectangleTexture rectangles;
    CguiMultiRoundnessRectangleTexture rectanglesShadow;
    CguiMultiRoundnessRectangleTexture rectanglesBorder;

    CguiColorTheme colorTheme;
    CguiFontCollection fontCollection;

    float transitionSpeed;

    unsigned int id;
} CguiResources;

CGUI_API CguiResources *CguiLoadResources(void);
CGUI_API void CguiUnloadResources(CguiResources *resources);

typedef struct CguiTransform {
    Vector2 position;
    Vector2 size;
    bool anchorTop;
    bool anchorBottom;
    bool anchorLeft;
    bool anchorRight;
    Vector2 stretchSpacing;
} CguiTransform;

typedef struct CguiWidget {
    struct CguiWidget *parent;
    struct CguiWidget **children;
    int childrenCount;

    unsigned int id;

    bool enabled;
    CguiTransform localTransform;

    void *element;
    void *compound;

    void (*customElementUpdate)(CguiResources *res, void *element, void *data);
    void (*customElementDraw)(CguiResources *res, void *element, void *data);
    void (*beginMode)(CguiResources *res, struct CguiWidget *widget);
    void (*endMode)(CguiResources *res, struct CguiWidget *widget);

    void *userData;
} CguiWidget;

typedef int(CguiWidgetTraverser)(CguiWidget *widget, void *data);

CGUI_API CguiWidget *CguiCreateWidget(void);
CGUI_API void CguiDeleteWidget(CguiWidget *widget, bool deleteRecursively, bool deleteAttached);
CGUI_API void CguiAttachWidget(CguiWidget *widget, CguiWidget *parent);
CGUI_API void CguiDetachWidget(CguiWidget *widget);
CGUI_API int CguiTraverseWidgetParentFirst(CguiWidget *widget, CguiWidgetTraverser traverser, void *data);
CGUI_API int CguiTraverseWidgetChildFirst(CguiWidget *widget, CguiWidgetTraverser traverser, void *data);
CGUI_API void CguiSetWidgetAndChildEnabled(CguiWidget *widget, bool enabled);
CGUI_API bool CguiCheckWidgetIsCompound(CguiWidget *widget);
CGUI_API Rectangle CguiGetWidgetRectangle(CguiWidget *widget);
CGUI_API void CguiAttachElement(CguiWidget *widget, void *element);
CGUI_API void CguiDetachElement(CguiWidget *widget, void *element);

typedef enum CguiElementType {
    CGUI_ELEMENT_TYPE_UNKNOWN = 0,
    CGUI_ELEMENT_TYPE_LABEL,
    CGUI_ELEMENT_TYPE_ICON,
    CGUI_ELEMENT_TYPE_ANIMATED_ICON,
    CGUI_ELEMENT_TYPE_BUTTON,
    CGUI_ELEMENT_TYPE_TOGGLE,
    CGUI_ELEMENT_TYPE_MAX
} CguiElementType;

typedef struct CguiElementBasic {
    int elementType;
    CguiWidget *widget;

} CguiElementBasic;

typedef struct CguiElementLabel {
    int elementType;
    CguiWidget *widget;

    const char *text;
    CguiMultiSizeFont font;
    float fontSize;
    float spacing;
    CguiGradientColor textColor;
} CguiElementLabel;

CGUI_API CguiElementLabel *CguiCreateElementLabel(CguiResources *res, const char *text);
CGUI_API void CguiDeleteElementLabel(CguiElementLabel *label);
CGUI_API void CguiUpdateElementLabel(CguiResources *res, CguiElementLabel *label);
CGUI_API void CguiDrawElementLabel(CguiResources *res, CguiElementLabel *label);

typedef struct CguiElementIcon {
    int elementType;
    CguiWidget *widget;

    CguiIconTexture iconTexture;
    CguiGradientColor iconTint;
} CguiElementIcon;

CGUI_API CguiElementIcon *CguiCreateElementIcon(CguiResources *res, CguiIconTexture iconTexture);
CGUI_API void CguiDeleteElementIcon(CguiElementIcon *icon);
CGUI_API void CguiUpdateElementIcon(CguiResources *res, CguiElementIcon *icon);
CGUI_API void CguiDrawElementIcon(CguiResources *res, CguiElementIcon *icon);

typedef struct CguiElementAnimatedIcon {
    int elementType;
    CguiWidget *widget;

    CguiAnimatedIcon animatedIconTexture;
    CguiGradientColor animatedIconTint;
    float startTime;
} CguiElementAnimatedIcon;

CGUI_API CguiElementAnimatedIcon *CguiCreateElementAnimatedIcon(CguiResources *res, CguiAnimatedIcon animatedIconTexture);
CGUI_API void CguiDeleteElementAnimatedIcon(CguiElementAnimatedIcon *animatedIcon);
CGUI_API void CguiUpdateElementAnimatedIcon(CguiResources *res, CguiElementAnimatedIcon *animatedIcon);
CGUI_API void CguiDrawElementAnimatedIcon(CguiResources *res, CguiElementAnimatedIcon *animatedIcon);

typedef enum CguiMouseAction {

    CGUI_MOUSE_ACTION_UP = 0,
    CGUI_MOUSE_ACTION_DOWN,
    CGUI_MOUSE_ACTION_PRESSED,
    CGUI_MOUSE_ACTION_RELEASED,
    CGUI_MOUSE_ACTION_MAX
} CguiMouseAction;

typedef struct CguiElementButton {
    int elementType;
    CguiWidget *widget;

    float roundness;
    CguiGradientColor color;
    CguiGradientColor hoverColor;
    CguiGradientColor heldColor;

    float transitionTimer;
    bool hover;
    int mouseButton;
    int action;

    void (*onClick)(struct CguiElementButton *button, int mouseButton);
    void (*onHover)(struct CguiElementButton *button, bool hoverEnter);
} CguiElementButton;

CGUI_API CguiElementButton *CguiCreateElementButton(CguiResources *res);
CGUI_API void CguiDeleteElementButton(CguiElementButton *button);
CGUI_API bool CguiUpdateElementButton(CguiResources *res, CguiElementButton *button);
CGUI_API void CguiDrawElementButton(CguiResources *res, CguiElementButton *button);

typedef enum CguiElementToggleType {

    CGUI_ELEMENT_TOGGLE_TYPE_REGULAR = 0,
    CGUI_ELEMENT_TOGGLE_TYPE_SLIDER,
    CGUI_ELEMENT_TOGGLE_TYPE_CHECKBOX,
    CGUI_ELEMENT_TOGGLE_TYPE_MAX
} CguiElementToggleType;

typedef struct CguiElementToggle {
    int elementType;
    CguiWidget *widget;

    float roundness;
    CguiGradientColor color;
    CguiGradientColor hoverColor;
    CguiGradientColor heldColor;
    CguiGradientColor activeColor;
    CguiGradientColor activeHoverColor;
    CguiGradientColor activeHeldColor;

    int toggleType;
    bool isActive;

    float transitionTimer;
    bool hover;
    int mouseButton;
    int action;

    void (*onClick)(struct CguiElementToggle *toggle, int mouseButton);
    void (*onHover)(struct CguiElementToggle *toggle, bool hoverEnter);
} CguiElementToggle;

CGUI_API CguiElementToggle *CguiCreateElementToggle(CguiResources *res);
CGUI_API void CguiDeleteElementToggle(CguiElementToggle *toggle);
CGUI_API bool CguiUpdateElementToggle(CguiResources *res, CguiElementToggle *toggle);
CGUI_API void CguiDrawElementToggle(CguiResources *res, CguiElementToggle *toggle);

typedef enum CguiCompoundType {
    CGUI_COMPOUND_TYPE_UNKNOWN = 0,
    CGUI_COMPOUND_TYPE_LABEL,
    CGUI_COMPOUND_TYPE_ICON,
    CGUI_COMPOUND_TYPE_ANIMATED_ICON,
    CGUI_COMPOUND_TYPE_BUTTON,
    CGUI_COMPOUND_TYPE_TOGGLE,
    CGUI_COMPOUND_TYPE_MAX
} CguiCompoundType;

typedef struct CguiCompoundBasic {
    int compoundType;
    CguiWidget *widget;

} CguiCompoundBasic;

typedef struct CguiInstance {
    CguiResources *resources;
    bool sharedRes;
    CguiWidget *grandWidget;
    unsigned int id;
} CguiInstance;

CGUI_API CguiInstance *CguiLoadInstance(void);
CGUI_API CguiInstance *CguiLoadInstanceEx(CguiResources *res);
CGUI_API void CguiUnloadInstance(CguiInstance *instance);
CGUI_API void CguiUpdateInstance(CguiInstance *instance, void *customWidgetData);
CGUI_API void CguiDrawInstance(CguiInstance *instance, void *customWidgetData);

#ifdef __cplusplus
}
#endif

#endif
