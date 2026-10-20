/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This source file contains implementations for components.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

extern CguiNode *cguiComponentTemplates[];

void CguiApplyOverrides(CguiNode *node, CguiCommonOverrides overrides)
{
    if (!node)
    {
        return;
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_ENABLED)
    {
        node->enabled = overrides.enabled;
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_NAME)
    {
        CguiRenameNode(node, overrides.name);
    }

    if (overrides.fields & CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION)
    {
        node->transformation = overrides.transformation;
        node->rebound        = true;
    }
}

void CguiOverrideEnabled(CguiNode *node, CguiCommonOverrides *overrides, bool enabled)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_ENABLED;
    overrides->enabled = enabled;

    CguiApplyOverrides(node, *overrides);
}

void CguiOverrideName(CguiNode *node, CguiCommonOverrides *overrides, const char *name)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_NAME;
    overrides->name = name;

    CguiApplyOverrides(node, *overrides);
}

void CguiOverrideTransformation(CguiNode *node, CguiCommonOverrides *overrides, CguiTransformation transformation)
{
    if (!overrides)
    {
        return;
    }

    overrides->fields |= CGUI_COMMON_OVERRIDE_FIELD_TRANSFORMATION;
    overrides->transformation = transformation;

    CguiApplyOverrides(node, *overrides);
}

CguiNode *CguiCreateRoot(void)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_ROOT]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    node->override  = CguiOverrideRoot;
    node->updatePre = CguiPreUpdateRoot;
    node->drawPre   = CguiPreDrawRoot;

    iData->targetBackgroundColor = data->backgroundColor;

    iData->backgroundColorTransitionChain = CguiTransitColor(&iData->currentBackgroundColor, &iData->targetBackgroundColor, &iData->transitioningBackgroundColor, data->transitionEasing, data->transitionDuration);
    if (!iData->backgroundColorTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiOverrideTransformation(node, &iData->overrides, CguiTFillParent());

    return node;
}

void CguiPreUpdateRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootData         *data  = node->data;
    CguiRootInstanceData *iData = node->instanceData;

    if (!CguiIsColorEqual(iData->targetBackgroundColor, data->backgroundColor))
    {
        iData->targetBackgroundColor  = data->backgroundColor;
        iData->currentBackgroundColor = iData->transitioningBackgroundColor;

        CguiTransition *transition                        = iData->backgroundColorTransitionChain->first;
        iData->backgroundColorTransitionChain->active     = transition;
        iData->backgroundColorTransitionChain->activeTime = 0.0f;
        iData->backgroundColorTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->backgroundColorTransitionChain);
}

void CguiPreDrawRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootInstanceData *iData = node->instanceData;

    DrawRectangleRec(node->bounds, iData->transitioningBackgroundColor);
}

void CguiOverrideRoot(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_ROOT || !node->data || !node->instanceData)
    {
        return;
    }

    CguiRootInstanceData *iData = node->instanceData;

    node->updatePre = CguiPreUpdateRoot;
    node->drawPre   = CguiPreDrawRoot;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateLayer(CguiTransformation transformation, int type)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_LAYER]);
    if (!node)
    {
        return NULL;
    }

    CguiLayerData         *data  = node->data;
    CguiLayerInstanceData *iData = node->instanceData;

    node->updatePre = CguiPreUpdateLayer;
    node->override  = CguiOverrideLayer;

    iData->type = type;

    if (iData->type >= 0 && iData->type < CGUI_LAYER_TYPE_MAX)
    {
        iData->targetBoxData = data->boxDatas[iData->type];
    }

    iData->boxDataTransitionChain = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!iData->boxDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateLayer(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LAYER || !node->data || !node->instanceData)
    {
        return;
    }

    CguiLayerData         *data       = node->data;
    CguiLayerInstanceData *iData      = node->instanceData;
    CguiNode              *boxNodeRef = CguiTravel(node, data->boxPath, data->boxPathLength);
    if (!boxNodeRef)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (iData->type < 0 || iData->type >= CGUI_LAYER_TYPE_MAX)
    {
        return;
    }

    CguiBoxElementData boxData = data->boxDatas[iData->type];

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition                = iData->boxDataTransitionChain->first;
        iData->boxDataTransitionChain->active     = transition;
        iData->boxDataTransitionChain->activeTime = 0.0f;
        iData->boxDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->boxDataTransitionChain);
    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideLayer(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LAYER || !node->data || !node->instanceData)
    {
        return;
    }

    CguiLayerInstanceData *iData = node->instanceData;

    node->updatePre = CguiPreUpdateLayer;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateLabel(CguiTransformation transformation, const char *text, int type, bool disabled, int xJustify, int yJustify)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_LABEL]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiLabelData         *data  = node->data;
    CguiLabelInstanceData *iData = node->instanceData;

    if (iData->type >= 0 && iData->type < CGUI_LAYER_TYPE_MAX)
    {
        iData->targetTextData = data->textDatas[iData->type];
    }

    iData->textDataTransitionChain = CguiTransitTextElementData(&iData->currentTextData, &iData->targetTextData, &iData->transitioningTextData, data->transitionEasing, data->transitionDuration);
    if (!iData->textDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    node->override  = CguiOverrideLabel;
    node->updatePre = CguiPreUpdateLabel;

    iData->text     = text;
    iData->type     = type;
    iData->disabled = disabled;
    iData->xJustify = xJustify;
    iData->yJustify = yJustify;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateLabel(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        return;
    }

    CguiLabelData         *data        = node->data;
    CguiLabelInstanceData *iData       = node->instanceData;
    CguiNode              *textNodeRef = CguiTravel(node, data->textPath, data->textPathLength);
    if (!textNodeRef)
    {
        return;
    }

    CguiTextElementData *textNodeData = textNodeRef->data;

    if (iData->type < 0 || iData->type >= CGUI_LABEL_TYPE_MAX)
    {
        return;
    }

    CguiTextElementData textData = data->textDatas[iData->type];

    if (iData->disabled)
    {
        textData = data->disabledTextDatas[iData->type];
    }

    if (!CguiIsTextElementDataEqual(iData->targetTextData, textData))
    {
        iData->targetTextData  = textData;
        iData->currentTextData = iData->transitioningTextData;

        CguiTransition *transition                 = iData->textDataTransitionChain->first;
        iData->textDataTransitionChain->active     = transition;
        iData->textDataTransitionChain->activeTime = 0.0f;
        iData->textDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->textDataTransitionChain);
    *textNodeData = iData->transitioningTextData;

    textNodeData->text     = iData->text;
    textNodeData->xJustify = iData->xJustify;
    textNodeData->yJustify = iData->yJustify;
}

void CguiOverrideLabel(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_LABEL || !node->data || !node->instanceData)
    {
        return;
    }

    CguiLabelInstanceData *iData = node->instanceData;

    node->updatePre = CguiPreUpdateLabel;

    CguiApplyOverrides(node, iData->overrides);
}

CguiNode *CguiCreateButton(CguiTransformation transformation, int type, CguiButtonPressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_BUTTON]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiButtonData         *data  = node->data;
    CguiButtonInstanceData *iData = node->instanceData;

    iData->boxDataTransitionChain = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!iData->boxDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideButton;
    node->updatePre            = CguiPreUpdateButton;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleButtonEvents;

    iData->type          = type;
    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateButton(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return;
    }

    CguiButtonData         *data       = node->data;
    CguiButtonInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = CguiTravel(node, data->boxPath, data->boxPathLength);
    if (!boxNodeRef)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    if (!iData->held) iData->active = false; // Keep active only for one frame

    if (iData->type < 0 || iData->type >= CGUI_BUTTON_TYPE_MAX)
    {
        return;
    }

    CguiBoxElementData boxData = data->boxDatas[iData->type][CGUI_BUTTON_STATE_NORMAL];

    if (iData->hovered)
    {
        boxData = data->boxDatas[iData->type][CGUI_BUTTON_STATE_HOVERED];
    }

    if (iData->held)
    {
        boxData = data->boxDatas[iData->type][CGUI_BUTTON_STATE_HELD];
    }

    if (iData->disabled)
    {
        boxData = data->boxDatas[iData->type][CGUI_BUTTON_STATE_DISABLED];
    }

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition                = iData->boxDataTransitionChain->first;
        iData->boxDataTransitionChain->active     = transition;
        iData->boxDataTransitionChain->activeTime = 0.0f;
        iData->boxDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->boxDataTransitionChain);
    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideButton(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return;
    }

    CguiButtonInstanceData *iData = node->instanceData;

    node->updatePre            = CguiPreUpdateButton;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleButtonEvents;

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleButtonEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_BUTTON || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiButtonInstanceData *iData = node->instanceData;

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (cursorMoveEvent->moveIn)
            {
                iData->hovered = true;
            }
            if (cursorMoveEvent->moveOut)
            {
                iData->hovered = false;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = false;
                if (iData->hovered)
                {
                    iData->active = true;
                    if (iData->pressCallback) iData->pressCallback(node);
                }
            }

            return true;
        }
    }

    return true;
}

CguiNode *CguiCreateToggle(CguiTransformation transformation, bool active, CguiTogglePressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_TOGGLE]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiToggleData         *data  = node->data;
    CguiToggleInstanceData *iData = node->instanceData;

    iData->boxDataTransitionChain = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!iData->boxDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideToggle;
    node->updatePre            = CguiPreUpdateToggle;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleToggleEvents;

    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;
    iData->active        = active;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateToggle(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return;
    }

    CguiToggleData         *data       = node->data;
    CguiToggleInstanceData *iData      = node->instanceData;
    CguiNode               *boxNodeRef = CguiTravel(node, data->boxPath, data->boxPathLength);
    if (!boxNodeRef)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    CguiBoxElementData boxData = data->boxDatas[CGUI_TOGGLE_STATE_NORMAL];

    if (iData->active)
    {
        boxData = data->boxDatas[CGUI_TOGGLE_STATE_ACTIVE];

        if (iData->hovered)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_HOVERED];
        }

        if (iData->held)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_HELD];
        }

        if (iData->disabled)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_ACTIVE_DISABLED];
        }
    }
    else
    {
        boxData = data->boxDatas[CGUI_TOGGLE_STATE_NORMAL];

        if (iData->hovered)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_HOVERED];
        }

        if (iData->held)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_HELD];
        }

        if (iData->disabled)
        {
            boxData = data->boxDatas[CGUI_TOGGLE_STATE_DISABLED];
        }
    }

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition                = iData->boxDataTransitionChain->first;
        iData->boxDataTransitionChain->active     = transition;
        iData->boxDataTransitionChain->activeTime = 0.0f;
        iData->boxDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->boxDataTransitionChain);
    *boxNodeData = iData->transitioningBoxData;
}

void CguiOverrideToggle(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return;
    }

    CguiToggleInstanceData *iData = node->instanceData;

    node->updatePre            = CguiPreUpdateToggle;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleToggleEvents;

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleToggleEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_TOGGLE || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiToggleInstanceData *iData = node->instanceData;

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (cursorMoveEvent->moveIn)
            {
                iData->hovered = true;
            }
            if (cursorMoveEvent->moveOut)
            {
                iData->hovered = false;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = false;
                if (iData->hovered)
                {
                    iData->active = !iData->active;
                    if (iData->pressCallback) iData->pressCallback(node, iData->active);
                }
            }

            return true;
        }
    }

    return true;
}

CguiNode *CguiCreateSwitch(CguiTransformation transformation, bool active, CguiSwitchPressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_SWITCH]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_SWITCH || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiSwitchData         *data  = node->data;
    CguiSwitchInstanceData *iData = node->instanceData;

    iData->trackDataTransitionChain = CguiTransitBoxElementData(&iData->currentTrackData, &iData->targetTrackData, &iData->transitioningTrackData, data->transitionEasing, data->transitionDuration);
    if (!iData->trackDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    iData->thumbDataTransitionChain = CguiTransitBoxElementData(&iData->currentThumbData, &iData->targetThumbData, &iData->transitioningThumbData, data->transitionEasing, data->transitionDuration);
    if (!iData->thumbDataTransitionChain)
    {
        CguiDeleteTransitionChain(iData->trackDataTransitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    iData->thumbTransformationTransitionChain = CguiTransitTransformation(&iData->currentThumbTransformation, &iData->targetThumbTransformation, &iData->transitioningThumbTransformation, data->transitionEasing, data->transitionDuration);
    if (!iData->thumbTransformationTransitionChain)
    {
        CguiDeleteTransitionChain(iData->trackDataTransitionChain);
        CguiDeleteTransitionChain(iData->thumbDataTransitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideSwitch;
    node->updatePre            = CguiPreUpdateSwitch;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleSwitchEvents;

    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;
    iData->active        = active;

    iData->currentThumbTransformation       = CguiTZeroSize();
    iData->targetThumbTransformation        = CguiTZeroSize();
    iData->transitioningThumbTransformation = CguiTZeroSize();

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateSwitch(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_SWITCH || !node->data || !node->instanceData)
    {
        return;
    }

    CguiSwitchData         *data         = node->data;
    CguiSwitchInstanceData *iData        = node->instanceData;
    CguiNode               *trackNodeRef = CguiTravel(node, data->trackPath, data->trackPathLength);
    if (!trackNodeRef)
    {
        return;
    }

    CguiBoxElementData *trackNodeData = trackNodeRef->data;

    CguiNode *thumbNodeRef = CguiTravel(node, data->thumbPath, data->thumbPathLength);
    if (!thumbNodeRef)
    {
        return;
    }

    CguiBoxElementData *thumbNodeData = thumbNodeRef->data;

    CguiBoxElementData trackData = data->trackDatas[CGUI_SWITCH_STATE_NORMAL];
    CguiBoxElementData thumbData = data->thumbDatas[CGUI_SWITCH_STATE_NORMAL];

    if (iData->active)
    {
        trackData = data->trackDatas[CGUI_SWITCH_STATE_ACTIVE];
        thumbData = data->thumbDatas[CGUI_SWITCH_STATE_ACTIVE];

        if (iData->hovered)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_ACTIVE_HOVERED];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_HOVERED];
        }

        if (iData->held)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_ACTIVE_HELD];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_HELD];
        }

        if (iData->disabled)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_ACTIVE_DISABLED];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_ACTIVE_DISABLED];
        }
    }
    else
    {
        trackData = data->trackDatas[CGUI_SWITCH_STATE_NORMAL];
        thumbData = data->thumbDatas[CGUI_SWITCH_STATE_NORMAL];

        if (iData->hovered)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_HOVERED];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_HOVERED];
        }

        if (iData->held)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_HELD];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_HELD];
        }

        if (iData->disabled)
        {
            trackData = data->trackDatas[CGUI_SWITCH_STATE_DISABLED];
            thumbData = data->thumbDatas[CGUI_SWITCH_STATE_DISABLED];
        }
    }

    CguiTransformation thumbTransformation = data->thumbInactiveTransformation;

    if (iData->active)
    {
        thumbTransformation = data->thumbActiveTransformation;
    }

    if (!CguiIsBoxElementDataEqual(iData->targetTrackData, trackData))
    {
        iData->targetTrackData  = trackData;
        iData->currentTrackData = iData->transitioningTrackData;

        CguiTransition *transition                  = iData->trackDataTransitionChain->first;
        iData->trackDataTransitionChain->active     = transition;
        iData->trackDataTransitionChain->activeTime = 0.0f;
        iData->trackDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->trackDataTransitionChain);
    *trackNodeData = iData->transitioningTrackData;

    if (!CguiIsBoxElementDataEqual(iData->targetThumbData, thumbData))
    {
        iData->targetThumbData  = thumbData;
        iData->currentThumbData = iData->transitioningThumbData;

        CguiTransition *transition                  = iData->thumbDataTransitionChain->first;
        iData->thumbDataTransitionChain->active     = transition;
        iData->thumbDataTransitionChain->activeTime = 0.0f;
        iData->thumbDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->thumbDataTransitionChain);
    *thumbNodeData = iData->transitioningThumbData;

    if (!CguiIsTransformationEqual(iData->targetThumbTransformation, thumbTransformation))
    {
        iData->targetThumbTransformation  = thumbTransformation;
        iData->currentThumbTransformation = iData->transitioningThumbTransformation;

        CguiTransition *transition                            = iData->thumbTransformationTransitionChain->first;
        iData->thumbTransformationTransitionChain->active     = transition;
        iData->thumbTransformationTransitionChain->activeTime = 0.0f;
        iData->thumbTransformationTransitionChain->finished   = false;
    }

    if (iData->dragged)
    {
        iData->targetThumbTransformation        = CguiInterpTransformation(data->thumbInactiveTransformation, data->thumbActiveTransformation, iData->dragAmount);
        iData->currentThumbTransformation       = iData->targetThumbTransformation;
        iData->transitioningThumbTransformation = iData->targetThumbTransformation;
    }
    else
    {
        CguiUpdateTransitionChain(iData->thumbTransformationTransitionChain);
    }

    CguiSetTransformation(thumbNodeRef, iData->transitioningThumbTransformation);
}

void CguiOverrideSwitch(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_SWITCH || !node->data || !node->instanceData)
    {
        return;
    }

    CguiSwitchData         *data  = node->data;
    CguiSwitchInstanceData *iData = node->instanceData;

    CguiNode *thumbNodeRef = CguiTravel(node, data->thumbPath, data->thumbPathLength);
    if (!thumbNodeRef)
    {
        return;
    }

    node->updatePre            = CguiPreUpdateSwitch;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleSwitchEvents;

    CguiSetTransformation(thumbNodeRef, iData->transitioningThumbTransformation);

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleSwitchEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_SWITCH || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiSwitchData         *data         = node->data;
    CguiSwitchInstanceData *iData        = node->instanceData;
    CguiNode               *trackNodeRef = CguiTravel(node, data->trackPath, data->trackPathLength);
    if (!trackNodeRef)
    {
        return false;
    }

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (iData->held)
            {
                if (!iData->dragged)
                {
                    iData->dragged   = true;
                    iData->dragStart = GetMousePosition();
                }

                Rectangle inactiveRec = CguiComputeTransformation(data->thumbInactiveTransformation, trackNodeRef->bounds);
                Rectangle activeRec   = CguiComputeTransformation(data->thumbActiveTransformation, trackNodeRef->bounds);

                Vector2 inactiveCenter = { inactiveRec.x + inactiveRec.width / 2.0f, inactiveRec.y + inactiveRec.height / 2.0f };
                Vector2 activeCenter   = { activeRec.x + activeRec.width / 2.0f, activeRec.y + activeRec.height / 2.0f };

                Vector2 a         = inactiveCenter;
                Vector2 b         = activeCenter;
                Vector2 c         = GetMousePosition();
                Vector2 ab        = Vector2Subtract(b, a);
                Vector2 ac        = Vector2Subtract(c, a);
                iData->dragAmount = Clamp(Vector2DotProduct(ac, ab) / Vector2LengthSqr(ab), 0.0f, 1.0f);
            }

            else
            {
                if (cursorMoveEvent->moveIn)
                {
                    iData->hovered = true;
                }
                if (cursorMoveEvent->moveOut)
                {
                    iData->hovered = false;
                }
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == MOUSE_BUTTON_LEFT)
            {
                if (iData->dragged)
                {
                    iData->active = iData->dragAmount > 0.5f;
                    if (iData->pressCallback) iData->pressCallback(node, iData->active);
                }
                else if (iData->hovered)
                {
                    iData->active = !iData->active;
                    if (iData->pressCallback) iData->pressCallback(node, iData->active);
                }

                iData->dragged = false;
                iData->held    = false;
            }

            return true;
        }
    }

    return true;
}

CguiNode *CguiCreateCheck(CguiTransformation transformation, bool active, CguiCheckPressCallback pressCallback, bool disabled)
{
    CguiNode *node = CguiCreateInstance(cguiComponentTemplates[CGUI_COMPONENT_CHECK]);
    if (!node)
    {
        return NULL;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_CHECK || !node->data || !node->instanceData)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    if (node->childrenCount == 0)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    CguiCheckData         *data  = node->data;
    CguiCheckInstanceData *iData = node->instanceData;

    iData->boxDataTransitionChain = CguiTransitBoxElementData(&iData->currentBoxData, &iData->targetBoxData, &iData->transitioningBoxData, data->transitionEasing, data->transitionDuration);
    if (!iData->boxDataTransitionChain)
    {
        CguiDeleteNode(node);
        return NULL;
    }

    iData->markDataTransitionChain = CguiTransitTextureElementData(&iData->currentMarkData, &iData->targetMarkData, &iData->transitioningMarkData, data->transitionEasing, data->transitionDuration);
    if (!iData->markDataTransitionChain)
    {
        CguiDeleteTransitionChain(iData->boxDataTransitionChain);
        CguiDeleteNode(node);
        return NULL;
    }

    node->override             = CguiOverrideCheck;
    node->updatePre            = CguiPreUpdateCheck;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleCheckEvents;

    iData->pressCallback = pressCallback;
    iData->disabled      = disabled;
    iData->active        = active;

    CguiOverrideTransformation(node, &iData->overrides, transformation);

    return node;
}

void CguiPreUpdateCheck(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_CHECK || !node->data || !node->instanceData)
    {
        return;
    }

    CguiCheckData         *data       = node->data;
    CguiCheckInstanceData *iData      = node->instanceData;
    CguiNode              *boxNodeRef = CguiTravel(node, data->boxPath, data->boxPathLength);
    if (!boxNodeRef)
    {
        return;
    }

    CguiBoxElementData *boxNodeData = boxNodeRef->data;

    CguiNode *markNodeRef = CguiTravel(node, data->markPath, data->markPathLength);
    if (!markNodeRef)
    {
        return;
    }

    CguiTextureElementData *markNodeData = markNodeRef->data;

    CguiBoxElementData     boxData  = data->boxDatas[CGUI_CHECK_STATE_NORMAL];
    CguiTextureElementData markData = data->markDatas[CGUI_CHECK_STATE_NORMAL];

    if (iData->active)
    {
        boxData  = data->boxDatas[CGUI_CHECK_STATE_ACTIVE];
        markData = data->markDatas[CGUI_CHECK_STATE_ACTIVE];

        if (iData->hovered)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_ACTIVE_HOVERED];
            markData = data->markDatas[CGUI_CHECK_STATE_ACTIVE_HOVERED];
        }

        if (iData->held)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_ACTIVE_HELD];
            markData = data->markDatas[CGUI_CHECK_STATE_ACTIVE_HELD];
        }

        if (iData->disabled)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_ACTIVE_DISABLED];
            markData = data->markDatas[CGUI_CHECK_STATE_ACTIVE_DISABLED];
        }
    }
    else
    {
        boxData  = data->boxDatas[CGUI_CHECK_STATE_NORMAL];
        markData = data->markDatas[CGUI_CHECK_STATE_NORMAL];

        if (iData->hovered)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_HOVERED];
            markData = data->markDatas[CGUI_CHECK_STATE_HOVERED];
        }

        if (iData->held)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_HELD];
            markData = data->markDatas[CGUI_CHECK_STATE_HELD];
        }

        if (iData->disabled)
        {
            boxData  = data->boxDatas[CGUI_CHECK_STATE_DISABLED];
            markData = data->markDatas[CGUI_CHECK_STATE_DISABLED];
        }
    }

    if (!CguiIsBoxElementDataEqual(iData->targetBoxData, boxData))
    {
        iData->targetBoxData  = boxData;
        iData->currentBoxData = iData->transitioningBoxData;

        CguiTransition *transition                = iData->boxDataTransitionChain->first;
        iData->boxDataTransitionChain->active     = transition;
        iData->boxDataTransitionChain->activeTime = 0.0f;
        iData->boxDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->boxDataTransitionChain);
    *boxNodeData = iData->transitioningBoxData;

    if (!CguiIsTextureElementDataEqual(iData->targetMarkData, markData))
    {
        iData->targetMarkData  = markData;
        iData->currentMarkData = iData->transitioningMarkData;

        CguiTransition *transition                 = iData->markDataTransitionChain->first;
        iData->markDataTransitionChain->active     = transition;
        iData->markDataTransitionChain->activeTime = 0.0f;
        iData->markDataTransitionChain->finished   = false;
    }

    CguiUpdateTransitionChain(iData->markDataTransitionChain);
    *markNodeData = iData->transitioningMarkData;
}

void CguiOverrideCheck(CguiNode *node)
{
    if (!node)
    {
        return;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_CHECK || !node->data || !node->instanceData)
    {
        return;
    }

    CguiCheckInstanceData *iData = node->instanceData;

    node->updatePre            = CguiPreUpdateCheck;
    node->canHandleMouseEvents = true;
    node->handleEvent          = CguiHandleCheckEvents;

    CguiApplyOverrides(node, iData->overrides);
}

bool CguiHandleCheckEvents(CguiNode *node, CguiEvent *event)
{
    if (!node || !event)
    {
        return false;
    }

    if (node->type != CGUI_COMPONENT_NODE_TYPE_CHECK || !node->data || !node->instanceData)
    {
        return false;
    }

    if (node->childrenCount == 0)
    {
        return false;
    }

    CguiCheckInstanceData *iData = node->instanceData;

    if (iData->disabled)
    {
        return false;
    }

    switch (event->eventType)
    {
        case CGUI_EVENT_TYPE_MOUSE_CURSOR_MOVE: {
            CguiMouseCursorMoveEvent *cursorMoveEvent = (CguiMouseCursorMoveEvent *) event;

            if (cursorMoveEvent->moveIn)
            {
                iData->hovered = true;
            }
            if (cursorMoveEvent->moveOut)
            {
                iData->hovered = false;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_PRESS: {
            CguiMouseButtonPressEvent *buttonPressEvent = (CguiMouseButtonPressEvent *) event;

            if (buttonPressEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = true;
            }

            return true;
        }

        case CGUI_EVENT_TYPE_MOUSE_BUTTON_RELEASE: {
            CguiMouseButtonReleaseEvent *buttonReleaseEvent = (CguiMouseButtonReleaseEvent *) event;

            if (buttonReleaseEvent->button == MOUSE_BUTTON_LEFT)
            {
                iData->held = false;
                if (iData->hovered)
                {
                    iData->active = !iData->active;
                    if (iData->pressCallback) iData->pressCallback(node, iData->active);
                }
            }

            return true;
        }
    }

    return true;
}
