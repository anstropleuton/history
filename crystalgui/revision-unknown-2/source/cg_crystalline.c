/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for Crystalline theme.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern int cguiNameCounter;

static const float lightness0 = (32.0f + 4.0f * 0.0f) / 256.0f;
static const float lightness1 = (32.0f + 4.0f * 1.0f) / 256.0f;
static const float lightness2 = (32.0f + 4.0f * 2.0f) / 256.0f;
static const float lightness3 = (32.0f + 4.0f * 3.0f) / 256.0f;
static const float lightness4 = (32.0f + 4.0f * 4.0f) / 256.0f;

static const float hoveredLightnessDiff = +8.0f / 256.0f;
static const float heldLightnessDiff    = -8.0f / 256.0f;

static Color CguiColorFromSLAaH(Vector3 sva, float hue) // Saturation, Value, Alpha and added Hue
{
    return CguiColorFromHSLA(hue, sva.x, sva.y, sva.z);
}

static Color CguiColorFromSLAaHsS(Vector3 sva, float hue, float saturation) // Saturation, Value, Alpha, added Hue and substituted Saturation
{
    return CguiColorFromHSLA(hue, saturation, sva.y, sva.z);
}

static Color CguiColorFromSLAaHsL(Vector3 sva, float hue, float lightness) // Saturation, Value, Alpha, added Hue and substituted Lightness
{
    return CguiColorFromHSLA(hue, sva.x, lightness, sva.z);
}

static CguiCrystallineThemeData CguiCreateCrystallineThemeBaseData(void)
{
    CguiCrystallineThemeData data    = { 0 };
    data.backgroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.backlayerDisabledSLA        = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.midlayerDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.frontlayerDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.foregroundDisabledSLA       = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.inactiveDisabledSLA         = (Vector3) { 0.0f, 0.500f, 1.0f };
    data.flatDisabledSLA             = (Vector3) { 1.0f, 0.500f, 0.250f };
    data.accentHue                   = 270.0f;
    data.dangerousHue                = 000.0f;
    data.warningHue                  = 030.0f;
    data.successHue                  = 120.0f;
    data.infoHue                     = 200.0f;
    data.headingFontSize             = 24.0f;
    data.bodyFontSize                = 18.0f;
    data.captionFontSize             = 14.0f;
    data.headingLineHeight           = 1.25f;
    data.bodyLineHeight              = 1.375f;
    data.captionLineHeight           = 1.5f;
    data.textFont                    = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Regular.ttf", 18, NULL, 0);
    data.textFontItalic              = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Italic.ttf", 18, NULL, 0);
    data.textFontBold                = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBold.ttf", 24, NULL, 0);
    data.textFontBoldItalic          = LoadFontEx("resource/fonts/Inter/static/Inter_24pt-SemiBoldItalic.ttf", 24, NULL, 0);
    data.textFontLight               = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-Light.ttf", 14, NULL, 0);
    data.textFontLightItalic         = LoadFontEx("resource/fonts/Inter/static/Inter_18pt-LightItalic.ttf", 14, NULL, 0);
    data.backlayerRadii              = (Vector4) { 20.0f, 20.0f, 20.0f, 20.0f };
    data.midlayerRadii               = (Vector4) { 15.0f, 15.0f, 15.0f, 15.0f };
    data.frontlayerRadii             = (Vector4) { 10.0f, 10.0f, 10.0f, 10.0f };
    data.layerShadowDistance         = 10.0f;
    data.layerShadowOffset           = (Vector2) { 0.0f, 10.0f };
    data.layerShadowShrink           = 5.0f;
    data.layerBorderThickness        = 2.0f;
    data.componentRadii              = (Vector4) { 5.0f, 5.0f, 5.0f, 5.0f };
    data.componentShadowDistance     = 10.0f;
    data.componentShadowOffset       = (Vector2) { 0.0f, 10.0f };
    data.componentShadowShrink       = 5.0f;
    data.componentBorderThickness    = 2.0f;
    data.componentTransitionDuration = 0.25f;
    data.componentTransitionEasing   = CguiEaseInOutQuad;
    data.switchTrackSize             = (Vector2) { 50.0f, 30.0f };
    data.switchTrackRoundness        = 25.0f;
    data.switchThumbPadding          = 5.0f;
    data.switchThumbRoundness        = 15.5f;
    data.checkBoxSize                = (Vector2) { 30.0f, 30.0f };
    data.checkBoxRoundness           = 5.0f;
    data.checkMarkTexture            = LoadTexture("resource/textures/check_mark.png");
    data.checkMarkMargin             = 3.0f;

    return data;
}

CguiTheme *CguiCreateCrystallineThemeDark(void)
{
    CguiCrystallineThemeData data = CguiCreateCrystallineThemeBaseData();
    data.inactiveSLA              = (Vector3) { 0.0f, lightness0, 1.0f };
    data.inactiveHoveredSLA       = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA          = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.backgroundSLA            = (Vector3) { 0.0f, lightness1, 1.0f };
    data.backgroundHoveredSLA     = (Vector3) { 0.0f, lightness1 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA        = (Vector3) { 0.0f, lightness1 + heldLightnessDiff, 1.0f };
    data.backlayerSLA             = (Vector3) { 0.0f, lightness2, 1.0f };
    data.backlayerHoveredSLA      = (Vector3) { 0.0f, lightness2 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA         = (Vector3) { 0.0f, lightness2 + heldLightnessDiff, 1.0f };
    data.midlayerSLA              = (Vector3) { 0.0f, lightness3, 1.0f };
    data.midlayerHoveredSLA       = (Vector3) { 0.0f, lightness3 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA          = (Vector3) { 0.0f, lightness3 + heldLightnessDiff, 1.0f };
    data.frontlayerSLA            = (Vector3) { 0.0f, lightness4, 1.0f };
    data.frontlayerHoveredSLA     = (Vector3) { 0.0f, lightness4 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA        = (Vector3) { 0.0f, lightness4 + heldLightnessDiff, 1.0f };
    data.foregroundSLA            = (Vector3) { 0.0f, 0.950f, 1.0f };
    data.foregroundHoveredSLA     = (Vector3) { 0.0f, 0.950f + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA        = (Vector3) { 0.0f, 0.950f + heldLightnessDiff, 1.0f };
    data.activeSLA                = (Vector3) { 0.8f, 0.400f, 1.0f };
    data.activeHoveredSLA         = (Vector3) { 0.8f, 0.400f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA            = (Vector3) { 0.8f, 0.400f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA        = (Vector3) { 0.5f, 0.250f, 1.0f };
    data.flatSLA                  = (Vector3) { 1.0f, lightness4, 0.000f };
    data.flatHoveredSLA           = (Vector3) { 1.0f, lightness4 + hoveredLightnessDiff, 0.250f };
    data.flatHeldSLA              = (Vector3) { 1.0f, lightness4 + heldLightnessDiff, 0.250f };
    data.textSLA                  = (Vector3) { 1.0f, 0.950f, 1.0f };
    data.disabledTextSLA          = (Vector3) { 1.0f, 0.750f, 1.0f };
    data.layerShadowSLA           = (Vector3) { 0.0f, 0.0f, 0.000f };
    data.layerBorderSLA           = (Vector3) { 0.0f, 1.0f, 0.050f };
    data.componentShadowSLA       = (Vector3) { 0.0f, 0.0f, 0.000f };
    data.componentBorderSLA       = (Vector3) { 0.0f, 1.0f, 0.050f };

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeLight(void)
{
    CguiCrystallineThemeData data = CguiCreateCrystallineThemeBaseData();
    data.inactiveSLA              = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.inactiveHoveredSLA       = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA          = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.backgroundSLA            = (Vector3) { 0.0f, 1.0f - lightness3, 1.0f };
    data.backgroundHoveredSLA     = (Vector3) { 0.0f, 1.0f - lightness3 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA        = (Vector3) { 0.0f, 1.0f - lightness3 + heldLightnessDiff, 1.0f };
    data.backlayerSLA             = (Vector3) { 0.0f, 1.0f - lightness2, 1.0f };
    data.backlayerHoveredSLA      = (Vector3) { 0.0f, 1.0f - lightness2 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA         = (Vector3) { 0.0f, 1.0f - lightness2 + heldLightnessDiff, 1.0f };
    data.midlayerSLA              = (Vector3) { 0.0f, 1.0f - lightness1, 1.0f };
    data.midlayerHoveredSLA       = (Vector3) { 0.0f, 1.0f - lightness1 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA          = (Vector3) { 0.0f, 1.0f - lightness1 + heldLightnessDiff, 1.0f };
    data.frontlayerSLA            = (Vector3) { 0.0f, 1.0f - lightness0, 1.0f };
    data.frontlayerHoveredSLA     = (Vector3) { 0.0f, 1.0f - lightness0 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA        = (Vector3) { 0.0f, 1.0f - lightness0 + heldLightnessDiff, 1.0f };
    data.foregroundSLA            = (Vector3) { 1.0f, 0.950f, 1.0f };
    data.foregroundHoveredSLA     = (Vector3) { 1.0f, 0.950f + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA        = (Vector3) { 1.0f, 0.950f + heldLightnessDiff, 1.0f };
    data.activeSLA                = (Vector3) { 0.8f, 0.600f, 1.0f };
    data.activeHoveredSLA         = (Vector3) { 0.8f, 0.600f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA            = (Vector3) { 0.8f, 0.600f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA        = (Vector3) { 0.5f, 0.750f, 1.0f };
    data.flatSLA                  = (Vector3) { 1.0f, 1.0f - lightness0, 0.000f };
    data.flatHoveredSLA           = (Vector3) { 1.0f, 1.0f - lightness0 + hoveredLightnessDiff, 0.250f };
    data.flatHeldSLA              = (Vector3) { 1.0f, 1.0f - lightness0 + heldLightnessDiff, 0.250f };
    data.textSLA                  = (Vector3) { 1.0f, 0.050f, 1.0f };
    data.disabledTextSLA          = (Vector3) { 1.0f, 0.250f, 1.0f };
    data.layerShadowSLA           = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.layerBorderSLA           = (Vector3) { 0.0f, 0.0f, 0.000f };
    data.componentShadowSLA       = (Vector3) { 0.0f, 0.0f, 0.050f };
    data.componentBorderSLA       = (Vector3) { 0.0f, 0.0f, 0.000f };

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeDarkHighContrast(void)
{
    CguiCrystallineThemeData data = CguiCreateCrystallineThemeBaseData();
    data.backgroundSLA            = (Vector3) { 0.0f, lightness0, 1.0f };
    data.backgroundHoveredSLA     = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA        = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.backlayerSLA             = (Vector3) { 0.0f, lightness0, 1.0f };
    data.backlayerHoveredSLA      = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA         = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.midlayerSLA              = (Vector3) { 0.0f, lightness0, 1.0f };
    data.midlayerHoveredSLA       = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA          = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.frontlayerSLA            = (Vector3) { 0.0f, lightness0, 1.0f };
    data.frontlayerHoveredSLA     = (Vector3) { 0.0f, lightness0 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA        = (Vector3) { 0.0f, lightness0 + heldLightnessDiff, 1.0f };
    data.foregroundSLA            = (Vector3) { 1.0f, 1.0f, 1.0f };
    data.foregroundHoveredSLA     = (Vector3) { 1.0f, 1.0f + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA        = (Vector3) { 1.0f, 1.0f + heldLightnessDiff, 1.0f };
    data.inactiveSLA              = (Vector3) { 0.0f, lightness2, 1.0f };
    data.inactiveHoveredSLA       = (Vector3) { 0.0f, lightness2 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA          = (Vector3) { 0.0f, lightness2 + heldLightnessDiff, 1.0f };
    data.activeSLA                = (Vector3) { 1.0f, 0.400f, 1.0f };
    data.activeHoveredSLA         = (Vector3) { 1.0f, 0.400f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA            = (Vector3) { 1.0f, 0.400f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA        = (Vector3) { 0.5f, 0.250f, 1.0f };
    data.flatSLA                  = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.flatHoveredSLA           = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.flatHeldSLA              = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.textSLA                  = (Vector3) { 1.0f, 1.000f, 1.0f };
    data.disabledTextSLA          = (Vector3) { 1.0f, 0.750f, 1.0f };
    data.layerShadowSLA           = (Vector3) { 0.0f, 0.0f, 0.125f };
    data.layerBorderSLA           = (Vector3) { 0.0f, 1.0f, 0.125f };
    data.componentShadowSLA       = (Vector3) { 0.0f, 0.0f, 0.125f };
    data.componentBorderSLA       = (Vector3) { 0.0f, 1.0f, 0.125f };

    return CguiCreateCrystallineThemeFromData(data);
}

CguiTheme *CguiCreateCrystallineThemeLightHighContrast(void)
{
    CguiCrystallineThemeData data = CguiCreateCrystallineThemeBaseData();
    data.backgroundSLA            = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.backgroundHoveredSLA     = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.backgroundHeldSLA        = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.backlayerSLA             = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.backlayerHoveredSLA      = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.backlayerHeldSLA         = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.midlayerSLA              = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.midlayerHoveredSLA       = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.midlayerHeldSLA          = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.frontlayerSLA            = (Vector3) { 0.0f, 1.0f - lightness4, 1.0f };
    data.frontlayerHoveredSLA     = (Vector3) { 0.0f, 1.0f - lightness4 + hoveredLightnessDiff, 1.0f };
    data.frontlayerHeldSLA        = (Vector3) { 0.0f, 1.0f - lightness4 + heldLightnessDiff, 1.0f };
    data.foregroundSLA            = (Vector3) { 1.0f, 1.0f, 1.0f };
    data.foregroundHoveredSLA     = (Vector3) { 1.0f, 1.0f + hoveredLightnessDiff, 1.0f };
    data.foregroundHeldSLA        = (Vector3) { 1.0f, 1.0f + heldLightnessDiff, 1.0f };
    data.inactiveSLA              = (Vector3) { 0.0f, 1.0f - lightness2, 1.0f };
    data.inactiveHoveredSLA       = (Vector3) { 0.0f, 1.0f - lightness2 + hoveredLightnessDiff, 1.0f };
    data.inactiveHeldSLA          = (Vector3) { 0.0f, 1.0f - lightness2 + heldLightnessDiff, 1.0f };
    data.activeSLA                = (Vector3) { 1.0f, 0.600f, 1.0f };
    data.activeHoveredSLA         = (Vector3) { 1.0f, 0.600f + hoveredLightnessDiff, 1.0f };
    data.activeHeldSLA            = (Vector3) { 1.0f, 0.600f + heldLightnessDiff, 1.0f };
    data.activeDisabledSLA        = (Vector3) { 0.5f, 0.750f, 1.0f };
    data.flatSLA                  = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.flatHoveredSLA           = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.flatHeldSLA              = (Vector3) { 1.0f, 0.500f, 0.005f };
    data.textSLA                  = (Vector3) { 1.0f, 0.000f, 1.0f };
    data.disabledTextSLA          = (Vector3) { 1.0f, 0.250f, 1.0f };
    data.layerShadowSLA           = (Vector3) { 0.0f, 0.0f, 0.125f };
    data.layerBorderSLA           = (Vector3) { 0.0f, 1.0f, 0.125f };
    data.componentShadowSLA       = (Vector3) { 0.0f, 0.0f, 0.125f };
    data.componentBorderSLA       = (Vector3) { 0.0f, 0.0f, 0.125f };

    return CguiCreateCrystallineThemeFromData(data);
}

static void CguiCreateRootTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    CguiNode *rootTemplate = CguiCreateNodeProMax(CguiTZeroSize(), TextFormat("CguiRoot #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_ROOT, NULL, sizeof(CguiRootData), NULL, sizeof(CguiRootInstanceData));
    if (!rootTemplate)
    {
        return;
    }

    CguiRootData *rootData = rootTemplate->data;

    if (!CguiSetComponentTemplate(theme, rootTemplate))
    {
        CguiDeleteNode(rootTemplate);
        return;
    }

    rootData->backgroundColor    = CguiColorFromSLAaHsS(data.backgroundSLA, 0.0f, 0.0f);
    rootData->transitionDuration = data.componentTransitionDuration;
    rootData->transitionEasing   = data.componentTransitionEasing;
}

static void CguiCreateLayerTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    // clang-format off
    CguiNode *layerTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiLayer #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_LAYER, NULL, sizeof(CguiLayerData), NULL, sizeof(CguiLayerInstanceData),
        CG_BOX(CguiTFillParent(), 0.0f, BLANK, NULL)
    );
    // clang-format on

    if (!layerTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, layerTemplate))
    {
        CguiDeleteNode(layerTemplate);
        return;
    }

    CguiLayerData *layerData = layerTemplate->data;

    layerData->transitionDuration = data.componentTransitionDuration;
    layerData->transitionEasing   = data.componentTransitionEasing;

    static const int boxPath[] = { 0 };
    layerData->boxPath         = boxPath;
    layerData->boxPathLength   = sizeof(boxPath) / sizeof(boxPath[0]);

    CguiBoxElementData boxData;

    boxData.shadowDistance  = data.layerShadowDistance;
    boxData.shadowOffset    = data.layerShadowOffset;
    boxData.shadowShrink    = data.layerShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.layerShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.layerBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.layerBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *backlayer  = &layerData->boxDatas[CGUI_LAYER_TYPE_BACKLAYER];
    CguiBoxElementData *midlayer   = &layerData->boxDatas[CGUI_LAYER_TYPE_MIDLAYER];
    CguiBoxElementData *frontlayer = &layerData->boxDatas[CGUI_LAYER_TYPE_FRONTLAYER];

    *backlayer  = boxData;
    *midlayer   = boxData;
    *frontlayer = boxData;

    backlayer->radii  = data.backlayerRadii;
    backlayer->color  = CguiColorFromSLAaHsS(data.backlayerSLA, 0.0f, 0.0f);
    midlayer->radii   = data.midlayerRadii;
    midlayer->color   = CguiColorFromSLAaHsS(data.midlayerSLA, 0.0f, 0.0f);
    frontlayer->radii = data.frontlayerRadii;
    frontlayer->color = CguiColorFromSLAaHsS(data.frontlayerSLA, 0.0f, 0.0f);
}

static void CguiCreateLabelTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    // clang-format off
    CguiNode *labelTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiLabel #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_LABEL, NULL, sizeof(CguiLabelData), NULL, sizeof(CguiLabelInstanceData),
        CG_TEXT(CguiTFillParent(), NULL, BLANK, NULL)
    );
    // clang-format on

    if (!labelTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, labelTemplate))
    {
        CguiDeleteNode(labelTemplate);
        return;
    }

    CguiLabelData *labelData = labelTemplate->data;

    labelData->transitionDuration = data.componentTransitionDuration;
    labelData->transitionEasing   = data.componentTransitionEasing;

    static const int textPath[] = { 0 };
    labelData->textPath         = textPath;
    labelData->textPathLength   = sizeof(textPath) / sizeof(textPath[0]);

    CguiTextElementData textData;

    textData.spacing = 1.0f;
    textData.color   = CguiColorFromSLAaHsS(data.textSLA, 0.0f, 0.0f);

    CguiTextElementData *heading         = &labelData->textDatas[CGUI_LABEL_TYPE_HEADING];
    CguiTextElementData *body            = &labelData->textDatas[CGUI_LABEL_TYPE_BODY];
    CguiTextElementData *caption         = &labelData->textDatas[CGUI_LABEL_TYPE_CAPTION];
    CguiTextElementData *disabledHeading = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_HEADING];
    CguiTextElementData *disabledBody    = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_BODY];
    CguiTextElementData *disabledCaption = &labelData->disabledTextDatas[CGUI_LABEL_TYPE_CAPTION];

    *heading         = textData;
    *body            = textData;
    *caption         = textData;
    *disabledHeading = textData;
    *disabledBody    = textData;
    *disabledCaption = textData;

    heading->font        = data.textFontBold;
    heading->fontSize    = data.headingFontSize;
    heading->lineSpacing = data.headingLineHeight;

    body->font        = data.textFont;
    body->fontSize    = data.bodyFontSize;
    body->lineSpacing = data.bodyLineHeight;

    caption->font        = data.textFontLight;
    caption->fontSize    = data.captionFontSize;
    caption->lineSpacing = data.captionLineHeight;

    disabledHeading->font        = data.textFontBold;
    disabledHeading->fontSize    = data.headingFontSize;
    disabledHeading->lineSpacing = data.headingLineHeight;
    disabledHeading->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);

    disabledBody->font        = data.textFont;
    disabledBody->fontSize    = data.bodyFontSize;
    disabledBody->lineSpacing = data.bodyLineHeight;
    disabledBody->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);

    disabledCaption->font        = data.textFontLight;
    disabledCaption->fontSize    = data.captionFontSize;
    disabledCaption->lineSpacing = data.captionLineHeight;
    disabledCaption->color       = CguiColorFromSLAaHsS(data.disabledTextSLA, 0.0f, 0.0f);
}

static void CguiCreateButtonTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    // clang-format off
    CguiNode *buttonTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiButton #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_BUTTON, NULL, sizeof(CguiButtonData), NULL, sizeof(CguiButtonInstanceData),
        CG_BOX(CguiTFillParent(), 0.0f, BLANK, NULL)
    );
    // clang-format on

    if (!buttonTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, buttonTemplate))
    {
        CguiDeleteNode(buttonTemplate);
        return;
    }

    CguiButtonData *buttonData = buttonTemplate->data;

    buttonData->transitionDuration = data.componentTransitionDuration;
    buttonData->transitionEasing   = data.componentTransitionEasing;

    static const int boxPath[] = { 0 };
    buttonData->boxPath        = boxPath;
    buttonData->boxPathLength  = sizeof(boxPath) / sizeof(boxPath[0]);

    CguiBoxElementData boxData;

    boxData.radii           = data.componentRadii;
    boxData.shadowDistance  = data.componentShadowDistance;
    boxData.shadowOffset    = data.componentShadowOffset;
    boxData.shadowShrink    = data.componentShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.componentBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *normal            = &buttonData->boxDatas[CGUI_BUTTON_TYPE_NORMAL][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *flat              = &buttonData->boxDatas[CGUI_BUTTON_TYPE_FLAT][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *accent            = &buttonData->boxDatas[CGUI_BUTTON_TYPE_ACCENT][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *dangerous         = &buttonData->boxDatas[CGUI_BUTTON_TYPE_DANGEROUS][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *warning           = &buttonData->boxDatas[CGUI_BUTTON_TYPE_WARNING][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *success           = &buttonData->boxDatas[CGUI_BUTTON_TYPE_SUCCESS][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *info              = &buttonData->boxDatas[CGUI_BUTTON_TYPE_INFO][CGUI_BUTTON_STATE_NORMAL];
    CguiBoxElementData *hoveredNormal     = &buttonData->boxDatas[CGUI_BUTTON_TYPE_NORMAL][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredFlat       = &buttonData->boxDatas[CGUI_BUTTON_TYPE_FLAT][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredAccent     = &buttonData->boxDatas[CGUI_BUTTON_TYPE_ACCENT][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredDangerous  = &buttonData->boxDatas[CGUI_BUTTON_TYPE_DANGEROUS][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredWarning    = &buttonData->boxDatas[CGUI_BUTTON_TYPE_WARNING][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredSuccess    = &buttonData->boxDatas[CGUI_BUTTON_TYPE_SUCCESS][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *hoveredInfo       = &buttonData->boxDatas[CGUI_BUTTON_TYPE_INFO][CGUI_BUTTON_STATE_HOVERED];
    CguiBoxElementData *heldNormal        = &buttonData->boxDatas[CGUI_BUTTON_TYPE_NORMAL][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldFlat          = &buttonData->boxDatas[CGUI_BUTTON_TYPE_FLAT][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldAccent        = &buttonData->boxDatas[CGUI_BUTTON_TYPE_ACCENT][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldDangerous     = &buttonData->boxDatas[CGUI_BUTTON_TYPE_DANGEROUS][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldWarning       = &buttonData->boxDatas[CGUI_BUTTON_TYPE_WARNING][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldSuccess       = &buttonData->boxDatas[CGUI_BUTTON_TYPE_SUCCESS][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *heldInfo          = &buttonData->boxDatas[CGUI_BUTTON_TYPE_INFO][CGUI_BUTTON_STATE_HELD];
    CguiBoxElementData *disabledNormal    = &buttonData->boxDatas[CGUI_BUTTON_TYPE_NORMAL][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledFlat      = &buttonData->boxDatas[CGUI_BUTTON_TYPE_FLAT][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledAccent    = &buttonData->boxDatas[CGUI_BUTTON_TYPE_ACCENT][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledDangerous = &buttonData->boxDatas[CGUI_BUTTON_TYPE_DANGEROUS][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledWarning   = &buttonData->boxDatas[CGUI_BUTTON_TYPE_WARNING][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledSuccess   = &buttonData->boxDatas[CGUI_BUTTON_TYPE_SUCCESS][CGUI_BUTTON_STATE_DISABLED];
    CguiBoxElementData *disabledInfo      = &buttonData->boxDatas[CGUI_BUTTON_TYPE_INFO][CGUI_BUTTON_STATE_DISABLED];

    *normal            = boxData;
    *flat              = boxData;
    *accent            = boxData;
    *dangerous         = boxData;
    *warning           = boxData;
    *success           = boxData;
    *info              = boxData;
    *hoveredNormal     = boxData;
    *hoveredFlat       = boxData;
    *hoveredAccent     = boxData;
    *hoveredDangerous  = boxData;
    *hoveredWarning    = boxData;
    *hoveredSuccess    = boxData;
    *hoveredInfo       = boxData;
    *heldNormal        = boxData;
    *heldFlat          = boxData;
    *heldAccent        = boxData;
    *heldDangerous     = boxData;
    *heldWarning       = boxData;
    *heldSuccess       = boxData;
    *heldInfo          = boxData;
    *disabledNormal    = boxData;
    *disabledFlat      = boxData;
    *disabledAccent    = boxData;
    *disabledDangerous = boxData;
    *disabledWarning   = boxData;
    *disabledSuccess   = boxData;
    *disabledInfo      = boxData;

    normal->color            = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    flat->color              = CguiColorFromSLAaHsS(data.flatSLA, 0.0f, 0.0f);
    accent->color            = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    dangerous->color         = CguiColorFromSLAaH(data.activeSLA, data.dangerousHue);
    warning->color           = CguiColorFromSLAaH(data.activeSLA, data.warningHue);
    success->color           = CguiColorFromSLAaH(data.activeSLA, data.successHue);
    info->color              = CguiColorFromSLAaH(data.activeSLA, data.infoHue);
    hoveredNormal->color     = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    hoveredFlat->color       = CguiColorFromSLAaHsS(data.flatHoveredSLA, 0.0f, 0.0f);
    hoveredAccent->color     = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    hoveredDangerous->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.dangerousHue);
    hoveredWarning->color    = CguiColorFromSLAaH(data.activeHoveredSLA, data.warningHue);
    hoveredSuccess->color    = CguiColorFromSLAaH(data.activeHoveredSLA, data.successHue);
    hoveredInfo->color       = CguiColorFromSLAaH(data.activeHoveredSLA, data.infoHue);
    heldNormal->color        = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    heldFlat->color          = CguiColorFromSLAaHsS(data.flatHeldSLA, 0.0f, 0.0f);
    heldAccent->color        = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    heldDangerous->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.dangerousHue);
    heldWarning->color       = CguiColorFromSLAaH(data.activeHeldSLA, data.warningHue);
    heldSuccess->color       = CguiColorFromSLAaH(data.activeHeldSLA, data.successHue);
    heldInfo->color          = CguiColorFromSLAaH(data.activeHeldSLA, data.infoHue);
    disabledNormal->color    = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    disabledFlat->color      = CguiColorFromSLAaHsS(data.flatDisabledSLA, 0.0f, 0.0f);
    disabledAccent->color    = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);
    disabledDangerous->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.dangerousHue);
    disabledWarning->color   = CguiColorFromSLAaH(data.activeDisabledSLA, data.warningHue);
    disabledSuccess->color   = CguiColorFromSLAaH(data.activeDisabledSLA, data.successHue);
    disabledInfo->color      = CguiColorFromSLAaH(data.activeDisabledSLA, data.infoHue);

    // Remove shadow and border from flat
    flat->shadowColor         = BLANK;
    hoveredFlat->shadowColor  = BLANK;
    heldFlat->shadowColor     = BLANK;
    disabledFlat->shadowColor = BLANK;

    flat->borderColor         = BLANK;
    hoveredFlat->borderColor  = BLANK;
    heldFlat->borderColor     = BLANK;
    disabledFlat->borderColor = BLANK;
}

static void CguiCreateToggleTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    // clang-format off
    CguiNode *toggleTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiToggle #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_TOGGLE, NULL, sizeof(CguiToggleData), NULL, sizeof(CguiToggleInstanceData),
        CG_BOX(CguiTFillParent(), 0.0f, BLANK, NULL)
    );
    // clang-format on

    if (!toggleTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, toggleTemplate))
    {
        CguiDeleteNode(toggleTemplate);
        return;
    }

    CguiToggleData *toggleData = toggleTemplate->data;

    toggleData->transitionDuration = data.componentTransitionDuration;
    toggleData->transitionEasing   = data.componentTransitionEasing;

    static const int boxPath[] = { 0 };
    toggleData->boxPath        = boxPath;
    toggleData->boxPathLength  = sizeof(boxPath) / sizeof(boxPath[0]);

    CguiBoxElementData boxData;

    boxData.radii           = data.componentRadii;
    boxData.shadowDistance  = data.componentShadowDistance;
    boxData.shadowOffset    = data.componentShadowOffset;
    boxData.shadowShrink    = data.componentShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.componentBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *inactive       = &toggleData->boxDatas[CGUI_TOGGLE_STATE_NORMAL];
    CguiBoxElementData *hovered        = &toggleData->boxDatas[CGUI_TOGGLE_STATE_HOVERED];
    CguiBoxElementData *held           = &toggleData->boxDatas[CGUI_TOGGLE_STATE_HELD];
    CguiBoxElementData *disabled       = &toggleData->boxDatas[CGUI_TOGGLE_STATE_DISABLED];
    CguiBoxElementData *active         = &toggleData->boxDatas[CGUI_TOGGLE_STATE_ACTIVE];
    CguiBoxElementData *activeHovered  = &toggleData->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_HOVERED];
    CguiBoxElementData *activeHeld     = &toggleData->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_HELD];
    CguiBoxElementData *activeDisabled = &toggleData->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_DISABLED];

    *inactive       = boxData;
    *hovered        = boxData;
    *held           = boxData;
    *disabled       = boxData;
    *active         = boxData;
    *activeHovered  = boxData;
    *activeHeld     = boxData;
    *activeDisabled = boxData;

    inactive->color       = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    hovered->color        = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    held->color           = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    disabled->color       = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    active->color         = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    activeHovered->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    activeHeld->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    activeDisabled->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);
}

static void CguiCreateSwitchTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    Vector2 thumbSize = { data.switchTrackSize.y - data.switchThumbPadding * 2.0f, data.switchTrackSize.y - data.switchThumbPadding * 2.0f };

    // clang-format off
    CguiNode *switchTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiSwitch #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_SWITCH, NULL, sizeof(CguiSwitchData), NULL, sizeof(CguiSwitchInstanceData),
        CG_BOX(CguiTCenter(data.switchTrackSize), 0.0f, BLANK,
            CG_EMPTY(CguiTMargin(data.switchThumbPadding),
                CG_BOX(CguiTLeft(thumbSize), 0.0f, BLANK, NULL)
            )
        )
    );
    // clang-format on

    if (!switchTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, switchTemplate))
    {
        CguiDeleteNode(switchTemplate);
        return;
    }

    CguiSwitchData *switchData = switchTemplate->data;

    switchData->transitionDuration = data.componentTransitionDuration;
    switchData->transitionEasing   = data.componentTransitionEasing;

    static const int trackPath[] = { 0 };
    switchData->trackPath        = trackPath;
    switchData->trackPathLength  = sizeof(trackPath) / sizeof(trackPath[0]);

    static const int thumbPath[] = { 0, 0, 0 };
    switchData->thumbPath        = thumbPath;
    switchData->thumbPathLength  = sizeof(thumbPath) / sizeof(thumbPath[0]);

    switchData->thumbInactiveTransformation = CguiTLeft(thumbSize);
    switchData->thumbActiveTransformation   = CguiTRight(thumbSize);

    CguiBoxElementData trackData;

    trackData.radii           = (Vector4) { data.switchTrackRoundness, data.switchTrackRoundness, data.switchTrackRoundness, data.switchTrackRoundness };
    trackData.shadowDistance  = data.componentShadowDistance;
    trackData.shadowOffset    = data.componentShadowOffset;
    trackData.shadowShrink    = data.componentShadowShrink;
    trackData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    trackData.borderThickness = data.componentBorderThickness;
    trackData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *trackInactive       = &switchData->trackDatas[CGUI_SWITCH_STATE_NORMAL];
    CguiBoxElementData *trackHovered        = &switchData->trackDatas[CGUI_SWITCH_STATE_HOVERED];
    CguiBoxElementData *trackHeld           = &switchData->trackDatas[CGUI_SWITCH_STATE_HELD];
    CguiBoxElementData *trackDisabled       = &switchData->trackDatas[CGUI_SWITCH_STATE_DISABLED];
    CguiBoxElementData *trackActive         = &switchData->trackDatas[CGUI_SWITCH_STATE_ACTIVE];
    CguiBoxElementData *trackActiveHovered  = &switchData->trackDatas[CGUI_SWITCH_STATE_ACTIVE_HOVERED];
    CguiBoxElementData *trackActiveHeld     = &switchData->trackDatas[CGUI_SWITCH_STATE_ACTIVE_HELD];
    CguiBoxElementData *trackActiveDisabled = &switchData->trackDatas[CGUI_SWITCH_STATE_ACTIVE_DISABLED];

    *trackInactive       = trackData;
    *trackHovered        = trackData;
    *trackHeld           = trackData;
    *trackDisabled       = trackData;
    *trackActive         = trackData;
    *trackActiveHovered  = trackData;
    *trackActiveHeld     = trackData;
    *trackActiveDisabled = trackData;

    trackInactive->color       = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    trackHovered->color        = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    trackHeld->color           = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    trackDisabled->color       = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    trackActive->color         = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    trackActiveHovered->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    trackActiveHeld->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    trackActiveDisabled->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);

    CguiBoxElementData thumbData;

    thumbData.radii           = (Vector4) { data.switchThumbRoundness, data.switchThumbRoundness, data.switchThumbRoundness, data.switchThumbRoundness };
    thumbData.shadowDistance  = data.componentShadowDistance;
    thumbData.shadowOffset    = data.componentShadowOffset;
    thumbData.shadowShrink    = data.componentShadowShrink;
    thumbData.shadowColor     = BLANK;
    thumbData.borderThickness = data.componentBorderThickness;
    thumbData.borderColor     = BLANK;

    CguiBoxElementData *thumbInactive       = &switchData->thumbDatas[CGUI_SWITCH_STATE_NORMAL];
    CguiBoxElementData *thumbHovered        = &switchData->thumbDatas[CGUI_SWITCH_STATE_HOVERED];
    CguiBoxElementData *thumbHeld           = &switchData->thumbDatas[CGUI_SWITCH_STATE_HELD];
    CguiBoxElementData *thumbDisabled       = &switchData->thumbDatas[CGUI_SWITCH_STATE_DISABLED];
    CguiBoxElementData *thumbActive         = &switchData->thumbDatas[CGUI_SWITCH_STATE_ACTIVE];
    CguiBoxElementData *thumbActiveHovered  = &switchData->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_HOVERED];
    CguiBoxElementData *thumbActiveHeld     = &switchData->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_HELD];
    CguiBoxElementData *thumbActiveDisabled = &switchData->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_DISABLED];

    *thumbInactive       = thumbData;
    *thumbHovered        = thumbData;
    *thumbHeld           = thumbData;
    *thumbDisabled       = thumbData;
    *thumbActive         = thumbData;
    *thumbActiveHovered  = thumbData;
    *thumbActiveHeld     = thumbData;
    *thumbActiveDisabled = thumbData;

    thumbInactive->color       = CguiColorFromSLAaHsS(data.foregroundSLA, 0.0f, 0.0f);
    thumbHovered->color        = CguiColorFromSLAaHsS(data.foregroundHoveredSLA, 0.0f, 0.0f);
    thumbHeld->color           = CguiColorFromSLAaHsS(data.foregroundHeldSLA, 0.0f, 0.0f);
    thumbDisabled->color       = CguiColorFromSLAaHsS(data.foregroundSLA, 0.0f, 0.0f);
    thumbActive->color         = CguiColorFromSLAaHsS(data.foregroundSLA, 0.0f, 0.0f);
    thumbActiveHovered->color  = CguiColorFromSLAaHsS(data.foregroundHoveredSLA, 0.0f, 0.0f);
    thumbActiveHeld->color     = CguiColorFromSLAaHsS(data.foregroundHeldSLA, 0.0f, 0.0f);
    thumbActiveDisabled->color = CguiColorFromSLAaHsS(data.foregroundSLA, 0.0f, 0.0f);
}

static void CguiCreateCheckTheme(CguiTheme *theme, CguiCrystallineThemeData data)
{
    // clang-format off
    CguiNode *checkTemplate = CG_EMPTY_PRO_MAX(CguiTZeroSize(), TextFormat("CguiCheck #%d", ++cguiNameCounter), CGUI_COMPONENT_NODE_TYPE_CHECK, NULL, sizeof(CguiCheckData), NULL, sizeof(CguiCheckInstanceData),
        CG_BOX(CguiTCenter(data.checkBoxSize), 0.0f, BLANK,
            CG_TEXTURE(CguiTMargin(data.checkMarkMargin), ((Texture) { 0 }), NULL)
        )
    );
    // clang-format on

    if (!checkTemplate)
    {
        return;
    }

    if (!CguiSetComponentTemplate(theme, checkTemplate))
    {
        CguiDeleteNode(checkTemplate);
        return;
    }

    CguiCheckData *checkData = checkTemplate->data;

    checkData->transitionDuration = data.componentTransitionDuration;
    checkData->transitionEasing   = data.componentTransitionEasing;

    static const int boxPath[] = { 0 };
    checkData->boxPath         = boxPath;
    checkData->boxPathLength   = sizeof(boxPath) / sizeof(boxPath[0]);

    static const int markPath[] = { 0, 0 };
    checkData->markPath         = markPath;
    checkData->markPathLength   = sizeof(markPath) / sizeof(markPath[0]);

    CguiBoxElementData boxData;

    boxData.radii           = (Vector4) { data.checkBoxRoundness, data.checkBoxRoundness, data.checkBoxRoundness, data.checkBoxRoundness };
    boxData.shadowDistance  = data.componentShadowDistance;
    boxData.shadowOffset    = data.componentShadowOffset;
    boxData.shadowShrink    = data.componentShadowShrink;
    boxData.shadowColor     = CguiColorFromSLAaHsS(data.componentShadowSLA, 0.0f, 0.0f);
    boxData.borderThickness = data.componentBorderThickness;
    boxData.borderColor     = CguiColorFromSLAaHsS(data.componentBorderSLA, 0.0f, 0.0f);

    CguiBoxElementData *boxInactive       = &checkData->boxDatas[CGUI_CHECK_STATE_NORMAL];
    CguiBoxElementData *boxHovered        = &checkData->boxDatas[CGUI_CHECK_STATE_HOVERED];
    CguiBoxElementData *boxHeld           = &checkData->boxDatas[CGUI_CHECK_STATE_HELD];
    CguiBoxElementData *boxDisabled       = &checkData->boxDatas[CGUI_CHECK_STATE_DISABLED];
    CguiBoxElementData *boxActive         = &checkData->boxDatas[CGUI_CHECK_STATE_ACTIVE];
    CguiBoxElementData *boxActiveHovered  = &checkData->boxDatas[CGUI_CHECK_STATE_ACTIVE_HOVERED];
    CguiBoxElementData *boxActiveHeld     = &checkData->boxDatas[CGUI_CHECK_STATE_ACTIVE_HELD];
    CguiBoxElementData *boxActiveDisabled = &checkData->boxDatas[CGUI_CHECK_STATE_ACTIVE_DISABLED];

    *boxInactive       = boxData;
    *boxHovered        = boxData;
    *boxHeld           = boxData;
    *boxDisabled       = boxData;
    *boxActive         = boxData;
    *boxActiveHovered  = boxData;
    *boxActiveHeld     = boxData;
    *boxActiveDisabled = boxData;

    boxInactive->color       = CguiColorFromSLAaHsS(data.inactiveSLA, 0.0f, 0.0f);
    boxHovered->color        = CguiColorFromSLAaHsS(data.inactiveHoveredSLA, 0.0f, 0.0f);
    boxHeld->color           = CguiColorFromSLAaHsS(data.inactiveHeldSLA, 0.0f, 0.0f);
    boxDisabled->color       = CguiColorFromSLAaHsS(data.inactiveDisabledSLA, 0.0f, 0.0f);
    boxActive->color         = CguiColorFromSLAaH(data.activeSLA, data.accentHue);
    boxActiveHovered->color  = CguiColorFromSLAaH(data.activeHoveredSLA, data.accentHue);
    boxActiveHeld->color     = CguiColorFromSLAaH(data.activeHeldSLA, data.accentHue);
    boxActiveDisabled->color = CguiColorFromSLAaH(data.activeDisabledSLA, data.accentHue);

    CguiTextureElementData markData;

    markData.texture = data.checkMarkTexture;
    markData.source  = CguiGetTextureSizeRec(markData.texture);

    CguiTextureElementData *markInactive       = &checkData->markDatas[CGUI_CHECK_STATE_NORMAL];
    CguiTextureElementData *markHovered        = &checkData->markDatas[CGUI_CHECK_STATE_HOVERED];
    CguiTextureElementData *markHeld           = &checkData->markDatas[CGUI_CHECK_STATE_HELD];
    CguiTextureElementData *markDisabled       = &checkData->markDatas[CGUI_CHECK_STATE_DISABLED];
    CguiTextureElementData *markActive         = &checkData->markDatas[CGUI_CHECK_STATE_ACTIVE];
    CguiTextureElementData *markActiveHovered  = &checkData->markDatas[CGUI_CHECK_STATE_ACTIVE_HOVERED];
    CguiTextureElementData *markActiveHeld     = &checkData->markDatas[CGUI_CHECK_STATE_ACTIVE_HELD];
    CguiTextureElementData *markActiveDisabled = &checkData->markDatas[CGUI_CHECK_STATE_ACTIVE_DISABLED];

    *markInactive       = markData;
    *markHovered        = markData;
    *markHeld           = markData;
    *markDisabled       = markData;
    *markActive         = markData;
    *markActiveHovered  = markData;
    *markActiveHeld     = markData;
    *markActiveDisabled = markData;

    markInactive->tint       = BLANK;
    markHovered->tint        = BLANK;
    markHeld->tint           = BLANK;
    markDisabled->tint       = BLANK;
    markActive->tint         = CguiColorFromSLAaH(data.foregroundSLA, 0.0f);
    markActiveHovered->tint  = CguiColorFromSLAaH(data.foregroundHoveredSLA, 0.0f);
    markActiveHeld->tint     = CguiColorFromSLAaH(data.foregroundHeldSLA, 0.0f);
    markActiveDisabled->tint = CguiColorFromSLAaH(data.foregroundSLA, 0.0f);
}

CguiTheme *CguiCreateCrystallineThemeFromData(CguiCrystallineThemeData data)
{
    CguiTheme *theme = CguiCreateTheme("CguiCrystallineLightHighContrast");
    if (!theme)
    {
        return NULL;
    }

    theme->themeData = CG_MALLOC(sizeof(CguiCrystallineThemeData));
    if (!theme->themeData)
    {
        CguiDeleteTheme(theme);
        return NULL;
    }

    theme->themeDataSize   = sizeof(CguiCrystallineThemeData);
    theme->deleteThemeData = CguiDeleteCrystallineTheme;
    memcpy(theme->themeData, &data, theme->themeDataSize);

    // Component themes
    CguiCreateRootTheme(theme, data);
    CguiCreateLayerTheme(theme, data);
    CguiCreateLabelTheme(theme, data);
    CguiCreateButtonTheme(theme, data);
    CguiCreateToggleTheme(theme, data);
    CguiCreateSwitchTheme(theme, data);
    CguiCreateCheckTheme(theme, data);

    return theme;
}

void CguiDeleteCrystallineTheme(CguiTheme *theme)
{
    if (!theme)
    {
        return;
    }

    CguiCrystallineThemeData *data = theme->themeData;
    if (!data)
    {
        return;
    }

    UnloadFont(data->textFont);
    UnloadFont(data->textFontItalic);
    UnloadFont(data->textFontBold);
    UnloadFont(data->textFontBoldItalic);
    UnloadFont(data->textFontLight);
    UnloadFont(data->textFontLightItalic);
}
