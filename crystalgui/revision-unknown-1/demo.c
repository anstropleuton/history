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
#include <stdio.h>

#include "crystalgui.h"
#include "raylib.h"
#include "raymath.h"

#define PANEL_WIDTH 350.0f

float EaseInOut(float x, float p)
{
    float y = powf(x, p) / (powf(x, p) + powf(1 - x, p));
    if (x <= 0.0f) return 0.0f;
    if (x >= 1.0f) return 1.0f;
    return y;
}

int DrawOutline(CguiWidget *widget, void *data)
{
    Rectangle rec = CguiGetWidgetRectangle(widget);
    if (widget == data) DrawRectangleLinesEx(rec, 1, WHITE);
    else DrawRectangleLinesEx(rec, 1, ColorAlpha(WHITE, 0.25f));
    return 0;
}

typedef struct GenerateHierarchyButtonData {
    CguiWidget *mainLayout;
    CguiWidget *widgetToAttach;
    CguiResources *res;
} GenerateHierarchyButtonData;

int GenerateHierarchyButtonTemp = 0;
int GenerateHierarchyButton(CguiWidget *widget, void *data)
{
    int indent = 0;
    CguiWidget *currentWidget = widget;
    while (currentWidget)
    {
        indent++;
        currentWidget = currentWidget->parent;
    }
    indent -= 3;

    GenerateHierarchyButtonData *generateHierarchyButtonData = data;

    CguiWidget *buttonWidget = CguiCreateWidget();
    buttonWidget->localTransform.anchorTop = true;
    buttonWidget->localTransform.position = (Vector2) { indent * 20.0f, (GenerateHierarchyButtonTemp++) * 60.0f + 120.0f };
    buttonWidget->localTransform.size = (Vector2) { PANEL_WIDTH - 40.0f, 40.0f };
    CguiAttachWidget(buttonWidget, generateHierarchyButtonData->widgetToAttach);

    CguiElementButton *button = CguiCreateElementButton(generateHierarchyButtonData->res);
    CguiAttachElement(buttonWidget, button);
    return 0;
}

typedef struct SelectHierarchyButtonData {
    CguiWidget **selectedWidget;
    CguiWidget *mainLayout;
    CguiWidget *widgetsCounterWidget;
} SelectHierarchyButtonData;

int SelectHierarchyButton(CguiWidget *widget, void *data)
{
    CguiElementButton *button = widget->element;
    if (!button) return 0;
    SelectHierarchyButtonData *selectHierarchyButtonData = data;
    if (button->action == CGUI_MOUSE_ACTION_RELEASED)
    {
        if (selectHierarchyButtonData->widgetsCounterWidget->childrenCount != selectHierarchyButtonData->mainLayout->childrenCount) return 0;
        for (int i = 0; i < selectHierarchyButtonData->widgetsCounterWidget->childrenCount; i++)
        {
            if (selectHierarchyButtonData->widgetsCounterWidget->children[i] == widget)
            {
                *selectHierarchyButtonData->selectedWidget = selectHierarchyButtonData->mainLayout->children[i];
            }
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Crystal GUI - Demo");
    SetTargetFPS(60);

    // Load Crystal GUI
    CguiInstance *instance = CguiLoadInstance();
    instance->resources->fontCollection.regularFont = CguiLoadMultiSizeFont("fonts/Segoe-UI-Variable-Static-Display.ttf", 8.0f, 2.0f, 32, NULL, 0);

    // Load texture and blur it
    Texture backgroundTexture = LoadTexture("background.png");
    CguiBlurTexture backgroundTextureBlurred = CguiGenerateBlurTexture(instance->resources->blurShader, backgroundTexture);

    // Icons and stuff
    CguiIcon addIcon = CguiGenerateIcon("l255,255,255,255,0.5,0.25,0.5,0.75,0.05;l255,255,255,255,0.25,0.5,0.75,0.5,0.05");
    CguiIconTexture addIconTexture = CguiGenerateIconTexture(addIcon, 40.0f);
    CguiIcon removeIcon = CguiGenerateIcon("l255,255,255,255,0.676776695,0.676776695,0.323223305,0.323223305,0.05;l255,255,255,255,0.323223305,0.676776695,0.676776695,0.323223305,0.05");
    CguiIconTexture removeIconTexture = CguiGenerateIconTexture(removeIcon, 40.0f);

    CguiWidget *selectedWidget = NULL;

    // Prepare GUI layout
    CguiWidget *leftPanelWidget = CguiCreateWidget();
    CguiWidget *widgetHierarchyTextWidget = CguiCreateWidget();
    CguiWidget *widgetsCounterWidget = CguiCreateWidget();
    CguiWidget *widgetAddWidget = CguiCreateWidget();
    CguiWidget *widgetAddIconWidget = CguiCreateWidget();
    CguiWidget *widgetAddLabelWidget = CguiCreateWidget();
    CguiWidget *widgetRemoveWidget = CguiCreateWidget();
    CguiWidget *widgetRemoveIconWidget = CguiCreateWidget();
    CguiWidget *widgetRemoveLabelWidget = CguiCreateWidget();

    CguiWidget *rightPanel = CguiCreateWidget();
    CguiWidget *detailsText = CguiCreateWidget();
    CguiWidget *commandInput = CguiCreateWidget();

    CguiWidget *mainLayout = CguiCreateWidget(); // Child of this widget will be listed in left panel widget counter

    // Panel size and position
    leftPanelWidget->localTransform.anchorLeft = true;
    leftPanelWidget->localTransform.anchorTop = true;    // Stretch to top and bottom
    leftPanelWidget->localTransform.anchorBottom = true; // by setting both anchors
    leftPanelWidget->localTransform.size.x = PANEL_WIDTH;

    widgetHierarchyTextWidget->localTransform.anchorTop = true;
    widgetHierarchyTextWidget->localTransform.anchorLeft = true;  // Center the text by setting the
    widgetHierarchyTextWidget->localTransform.anchorRight = true; // top and bottom anchors
    widgetHierarchyTextWidget->localTransform.position = (Vector2) { 0.0f, 20.0f };

    widgetsCounterWidget->localTransform.size = (Vector2) { PANEL_WIDTH, 0.0f };
    widgetsCounterWidget->localTransform.position = (Vector2) { 0.0f, -10.0f };

    widgetAddWidget->localTransform.anchorBottom = true;
    widgetAddWidget->localTransform.anchorLeft = true;
    widgetAddWidget->localTransform.position = (Vector2) { 20.0f, -20.0f };
    widgetAddWidget->localTransform.size = (Vector2) { 140.0f, 40.0f };

    widgetAddIconWidget->localTransform.anchorLeft = true;
    widgetAddIconWidget->localTransform.position = (Vector2) { 10.0f, 0.0f };
    widgetAddIconWidget->localTransform.size = (Vector2) { 40.0f, 40.0f };

    widgetAddLabelWidget->localTransform.anchorLeft = true;
    widgetAddLabelWidget->localTransform.anchorRight = true;
    widgetAddLabelWidget->localTransform.anchorTop = true;
    widgetAddLabelWidget->localTransform.anchorBottom = true;
    widgetAddLabelWidget->localTransform.position = (Vector2) { 15.0f, 0.0f };
    widgetAddLabelWidget->localTransform.stretchSpacing = (Vector2) { 0.0f, 0.0f };

    widgetRemoveWidget->localTransform.anchorBottom = true;
    widgetRemoveWidget->localTransform.anchorRight = true;
    widgetRemoveWidget->localTransform.position = (Vector2) { -20.0f, -20.0f };
    widgetRemoveWidget->localTransform.size = (Vector2) { 140.0f, 40.0f };

    widgetRemoveIconWidget->localTransform.anchorLeft = true;
    widgetRemoveIconWidget->localTransform.position = (Vector2) { 10.0f, 0.0f };
    widgetRemoveIconWidget->localTransform.size = (Vector2) { 40.0f, 40.0f };

    widgetRemoveLabelWidget->localTransform.anchorLeft = true;
    widgetRemoveLabelWidget->localTransform.anchorRight = true;
    widgetRemoveLabelWidget->localTransform.anchorTop = true;
    widgetRemoveLabelWidget->localTransform.anchorBottom = true;
    widgetRemoveLabelWidget->localTransform.position = (Vector2) { 15.0f, 0.0f };
    widgetRemoveLabelWidget->localTransform.stretchSpacing = (Vector2) { 0.0f, 0.0f };

    rightPanel->localTransform.anchorRight = true;
    rightPanel->localTransform.anchorTop = true;    // Stretch to top and bottom
    rightPanel->localTransform.anchorBottom = true; // by setting both anchors
    rightPanel->localTransform.size.x = PANEL_WIDTH;

    detailsText->localTransform.anchorTop = true;
    detailsText->localTransform.anchorLeft = true;  // Center the text by setting the
    detailsText->localTransform.anchorRight = true; // top and bottom anchors
    detailsText->localTransform.position = (Vector2) { 0.0f, 20.0f };

    CguiAttachWidget(leftPanelWidget, instance->grandWidget);
    CguiAttachWidget(widgetHierarchyTextWidget, leftPanelWidget);
    CguiAttachWidget(widgetsCounterWidget, leftPanelWidget);
    CguiAttachWidget(widgetAddWidget, leftPanelWidget);
    CguiAttachWidget(widgetAddIconWidget, widgetAddWidget);
    CguiAttachWidget(widgetAddLabelWidget, widgetAddWidget);
    CguiAttachWidget(widgetRemoveWidget, leftPanelWidget);
    CguiAttachWidget(widgetRemoveIconWidget, widgetRemoveWidget);
    CguiAttachWidget(widgetRemoveLabelWidget, widgetRemoveWidget);

    CguiAttachWidget(rightPanel, instance->grandWidget);
    CguiAttachWidget(detailsText, rightPanel);
    CguiAttachWidget(commandInput, rightPanel);

    CguiAttachWidget(mainLayout, instance->grandWidget);

    // Add elements to layout
    CguiElementLabel *widgetsTextLabel = CguiCreateElementLabel(instance->resources, "Widget Hierarchy");
    CguiElementLabel *detailsTextLabel = CguiCreateElementLabel(instance->resources, "Crystal GUI - v1.0");
    CguiElementButton *widgetAddButton = CguiCreateElementButton(instance->resources);
    CguiElementIcon *widgetAddIcon = CguiCreateElementIcon(instance->resources, addIconTexture);
    CguiElementLabel *widgetAddLabel = CguiCreateElementLabel(instance->resources, "Add");
    CguiElementButton *widgetRemoveButton = CguiCreateElementButton(instance->resources);
    CguiElementIcon *widgetRemoveIcon = CguiCreateElementIcon(instance->resources, addIconTexture);
    CguiElementLabel *widgetRemoveLabel = CguiCreateElementLabel(instance->resources, "Remove");

    widgetsTextLabel->fontSize = 24.0f;
    detailsTextLabel->fontSize = 24.0f;
    widgetAddButton->roundness = 20.0f;
    widgetAddIcon->iconTexture = addIconTexture;
    widgetAddLabel->fontSize = 24.0f;
    widgetRemoveButton->roundness = 20.0f;
    widgetRemoveIcon->iconTexture = removeIconTexture;
    widgetRemoveLabel->fontSize = 24.0f;

    CguiAttachElement(widgetHierarchyTextWidget, widgetsTextLabel);
    CguiAttachElement(detailsText, detailsTextLabel);
    CguiAttachElement(widgetAddWidget, widgetAddButton);
    CguiAttachElement(widgetAddIconWidget, widgetAddIcon);
    CguiAttachElement(widgetAddLabelWidget, widgetAddLabel);
    CguiAttachElement(widgetRemoveWidget, widgetRemoveButton);
    CguiAttachElement(widgetRemoveIconWidget, widgetRemoveIcon);
    CguiAttachElement(widgetRemoveLabelWidget, widgetRemoveLabel);

    // Left panel animation controls
    float leftPanelPositionTime = 0.0f;
    float leftPanelPositionAnimationTime = 0.0f;
    float leftPanelPositionSpeed = 5.0f;
    bool leftPanelPositionDirection = false;

    // Right panel animation controls
    float rightPanelPositionTime = 0.0f;
    float rightPanelPositionAnimationTime = 0.0f;
    float rightPanelPositionSpeed = 5.0f;
    bool rightPanelPositionDirection = false;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update

        // Left panel
        if (IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) leftPanelPositionDirection = !leftPanelPositionDirection;
        if (leftPanelPositionDirection) leftPanelPositionAnimationTime += GetFrameTime() * leftPanelPositionSpeed;
        else leftPanelPositionAnimationTime -= GetFrameTime() * leftPanelPositionSpeed;
        leftPanelPositionAnimationTime = Clamp(leftPanelPositionAnimationTime, 0.0f, 1.0f);
        leftPanelPositionTime = EaseInOut(leftPanelPositionAnimationTime, 3.0f);
        leftPanelWidget->localTransform.position.x = (leftPanelPositionTime - 1.0f) * leftPanelWidget->localTransform.size.x;

        // Right panel
        if (IsKeyPressed(KEY_TAB) || IsKeyPressedRepeat(KEY_TAB)) rightPanelPositionDirection = !rightPanelPositionDirection;
        if (rightPanelPositionDirection) rightPanelPositionAnimationTime += GetFrameTime() * rightPanelPositionSpeed;
        else rightPanelPositionAnimationTime -= GetFrameTime() * rightPanelPositionSpeed;
        rightPanelPositionAnimationTime = Clamp(rightPanelPositionAnimationTime, 0.0f, 1.0f);
        rightPanelPositionTime = EaseInOut(rightPanelPositionAnimationTime, 3.0f);
        rightPanel->localTransform.position.x = (1.0 - rightPanelPositionTime) * rightPanel->localTransform.size.x;

        mainLayout->localTransform.size.x = GetAppWidth() - leftPanelPositionTime * leftPanelWidget->localTransform.size.x - rightPanelPositionTime * rightPanel->localTransform.size.x;
        mainLayout->localTransform.size.y = GetAppHeight();
        leftPanelWidget->localTransform.size.y = GetAppHeight();
        rightPanel->localTransform.size.y = GetAppHeight();
        widgetsCounterWidget->localTransform.size.y = GetAppHeight() - 140.0f;

        if (widgetAddButton->action == CGUI_MOUSE_ACTION_RELEASED && widgetAddButton->mouseButton == MOUSE_BUTTON_LEFT)
        {
            CguiWidget *widget = CguiCreateWidget();
            widget->localTransform.size.x = 100.0f + GetRandomValue(-80, 80);
            widget->localTransform.size.y = 100.0f + GetRandomValue(-80, 80);
            CguiAttachWidget(widget, mainLayout);
            selectedWidget = widget;

            for (int i = 0; i < widgetsCounterWidget->childrenCount; i++) CguiDeleteWidget(widgetsCounterWidget->children[i], true, true);
            widgetsCounterWidget->childrenCount = 0;
            widgetsCounterWidget->children = MemRealloc(widgetsCounterWidget->children, 0);
            GenerateHierarchyButtonData generateHierarchyButtonData = { mainLayout, widgetsCounterWidget, instance->resources };
            GenerateHierarchyButtonTemp = 0;
            for (int i = 0; i < mainLayout->childrenCount; i++) CguiTraverseWidgetParentFirst(mainLayout->children[i], GenerateHierarchyButton, &generateHierarchyButtonData);
        }

        if (widgetRemoveButton->action == CGUI_MOUSE_ACTION_RELEASED && widgetRemoveButton->mouseButton == MOUSE_BUTTON_LEFT)
        {
            for (int i = mainLayout->childrenCount - 1; i >= 0; i--)
            {
                if (mainLayout->children[i] == selectedWidget)
                {
                    CguiDetachWidget(mainLayout->children[i]);
                    CguiDeleteWidget(mainLayout->children[i], true, true);
                }
            }

            for (int i = 0; i < widgetsCounterWidget->childrenCount; i++) CguiDeleteWidget(widgetsCounterWidget->children[i], true, true);
            widgetsCounterWidget->childrenCount = 0;
            widgetsCounterWidget->children = MemRealloc(widgetsCounterWidget->children, 0);
            GenerateHierarchyButtonData generateHierarchyButtonData = { mainLayout, widgetsCounterWidget, instance->resources };
            GenerateHierarchyButtonTemp = 0;
            for (int i = 0; i < mainLayout->childrenCount; i++) CguiTraverseWidgetParentFirst(mainLayout->children[i], GenerateHierarchyButton, &generateHierarchyButtonData);
        }

        SelectHierarchyButtonData selectHierarchyButtonData = { &selectedWidget, mainLayout, widgetsCounterWidget };
        for (int i = 0; i < widgetsCounterWidget->childrenCount; i++) CguiTraverseWidgetParentFirst(widgetsCounterWidget->children[i], SelectHierarchyButton, &selectHierarchyButtonData);

        // Update everything else
        CguiUpdateInstance(instance, NULL);

        // Draw
        BeginDrawing();
        ClearBackground((Color) { 36, 36, 36, 255 });

        // Draw background
        instance->resources->rectangleShader->values.borderColor[3] = 0.0f;
        CguiSetRectangleShaderValues(instance->resources->rectangleShader);

        BeginScissorMode(0, 0, GetAppWidth() / 2, GetAppHeight());
        CguiShaderDrawRectangleRec(instance->resources->rectangleShader, backgroundTexture, WHITE, (Rectangle) { leftPanelPositionTime * leftPanelWidget->localTransform.size.x, 0.0f, GetAppWidth() - leftPanelPositionTime * leftPanelWidget->localTransform.size.x, GetAppHeight() }, leftPanelPositionTime * 20.0f);
        EndScissorMode();
        BeginScissorMode(GetAppWidth() / 2, 0, GetAppWidth() / 2, GetAppHeight());
        CguiShaderDrawRectangleRec(instance->resources->rectangleShader, backgroundTexture, WHITE, (Rectangle) { 0.0f, 0.0f, GetAppWidth() - rightPanelPositionTime * rightPanel->localTransform.size.x, GetAppHeight() }, rightPanelPositionTime * 20.0f);
        EndScissorMode();

        // Draw everything else
        CguiDrawInstance(instance, NULL);

        for (int i = 0; i < mainLayout->childrenCount; i++) CguiTraverseWidgetParentFirst(mainLayout->children[i], DrawOutline, selectedWidget);
        EndDrawing();
    }

    // De-Initialization
    // NOTE: No need to de-initialize widgets and elements
    // All the widgets and elements that are joined to grand widget will be deleted automatically
    // Detach widgets or elements accordingly, if you think they should not be deleted

    CguiDisposeIconTexture(removeIconTexture);
    CguiDisposeIcon(removeIcon);
    CguiDisposeIconTexture(addIconTexture);
    CguiDisposeIcon(addIcon);

    CguiDisposeBlurTexture(backgroundTextureBlurred);
    UnloadTexture(backgroundTexture);

    // Unload Crystal GUI
    CguiUnloadInstance(instance);
    CloseWindow();
    return 0;
}
