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

#include <stddef.h>

#include "cgui_config.h"
#include "crystalgui.h"
#include "raylib.h"
#include "raymath.h"

// Allocate memory for widget
CguiWidget *CguiCreateWidget(void)
{
    static unsigned int idCounter = 0;
    CguiWidget *widget = MemAlloc(sizeof(CguiWidget));
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to allocate memory for Widget", , NULL);

    widget->enabled = true;
    widget->id = ++idCounter;

    CGUI_LOG_ID("WIDGET", widget->id, "Successfully created Widget");
    return widget;
}

// Deallocate memory for widget (NOTE: Deletes children widgets and attached elements or compounds recursively)
void CguiDeleteWidget(CguiWidget *widget)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to delete Widget due to invalid Widget", , );
    unsigned int id = widget->id;

    for (int i = 0; i < widget->childrenCount; i++) CguiDeleteWidget(widget->children[i]);

    // Delete respective element
    if (widget->element)
    {
        switch (((CguiElementBasic *)widget->element)->elementType)
        {
            case CGUI_ELEMENT_TYPE_LABEL: CguiDeleteElementLabel(widget->element); break;
            case CGUI_ELEMENT_TYPE_ICON: CguiDeleteElementIcon(widget->element); break;
            case CGUI_ELEMENT_TYPE_ANIMATED_ICON: CguiDeleteElementAnimatedIcon(widget->element); break;
            case CGUI_ELEMENT_TYPE_BUTTON: CguiDeleteElementButton(widget->element); break;
            case CGUI_ELEMENT_TYPE_TOGGLE: CguiDeleteElementToggle(widget->element); break;
        }
    }

    MemFree(widget);
    CGUI_LOG_ID("WIDGET", id, "Successfully deleted Widget");
}

// Attach widget to parent widget
void CguiAttachWidget(CguiWidget *widget, CguiWidget *parent)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to attach Widget due to invalid Widget", , );
    CGUI_CHECK_VALID_LOG("WIDGET", parent, "Failed to attach Widget due to invalid Parent", , );

    widget->parent = parent;
    widget->parent->childrenCount++;

    // Reallocate memory for the children array
    widget->parent->children = MemRealloc(widget->parent->children, sizeof(CguiWidget *) * widget->parent->childrenCount);
    CGUI_CHECK_VALID_LOG("WIDGET", widget->parent->children, "Failed to reallocate memory for Children", , );

    // Add the child to the children array
    widget->parent->children[widget->parent->childrenCount - 1] = widget;
}

// Detach widget from parent widget
void CguiDetachWidget(CguiWidget *widget)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to detach Widget due to invalid Widget", , );
    CGUI_CHECK_VALID_LOG("WIDGET", widget->parent, "Failed to detach Widget due to invalid Widget's Parent", , );

    // Find the index of the widget in its parent's children array
    int index = -1;
    for (int i = 0; i < widget->parent->childrenCount; i++)
    {
        if (widget->parent->children[i] == widget)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        // Remove the widget from its parent's children array
        for (int i = index; i < widget->parent->childrenCount - 1; i++)
        {
            widget->parent->children[i] = widget->parent->children[i + 1];
        }

        // Reallocate the memory for the children array
        widget->parent->childrenCount--;
        widget->parent->children = MemRealloc(widget->parent->children, sizeof(CguiWidget *) * widget->parent->childrenCount);
        CGUI_CHECK_VALID_LOG("WIDGET", widget->parent->children, "Failed to reallocate memory for Parent's Children", , );
        widget->parent = NULL;
    }
}

// Traverse all the widgets in parent first fashion
int CguiTraverseWidgetParentFirst(CguiWidget *widget, CguiWidgetTraverser traverser, void *data)
{
    // Don't halt the traversing due to invalid widget or invalid traverser
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to traverse Widget due to invalid Widget", , 0);
    CGUI_CHECK_VALID_LOG("WIDGET", traverser, "Failed to traverse Widget due to invalid Traverser Function", , 0);

    traverser(widget, data);
    for (int i = 0; i < widget->childrenCount; i++)
    {
        int exitCode = CguiTraverseWidgetParentFirst(widget->children[i], traverser, data);
        if (exitCode) return exitCode;
    }
    return 0;
}

// Traverse all the widgets in child first fashion (reverse of parent first)
int CguiTraverseWidgetChildFirst(CguiWidget *widget, CguiWidgetTraverser traverser, void *data)
{
    // Don't halt the traversing due to invalid widget or invalid traverser
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to traverse Widget due to invalid Widget", , 0);
    CGUI_CHECK_VALID_LOG("WIDGET", traverser, "Failed to traverse Widget due to invalid Traverser Function", , 0);

    for (int i = 0; i < widget->childrenCount; i++)
    {
        int exitCode = CguiTraverseWidgetParentFirst(widget->children[i], traverser, data);
        if (exitCode) return exitCode;
    }

    // Simply swap execution order
    traverser(widget, data);
    return 0;
}

// Set the widget and it's child as enabled recursively
void CguiSetWidgetAndChildEnabled(CguiWidget *widget, bool enabled)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to set Widget and Child enabled due to invalid Widget", , );

    widget->enabled = enabled;
    for (int i = 0; i < widget->childrenCount; i++) CguiSetWidgetAndChildEnabled(widget->children[i], enabled);
}

// Check if widget or their parents belong to a compound
bool CguiCheckWidgetIsCompound(CguiWidget *widget)
{
    CGUI_CHECK_VALID(widget, , false);
    if (widget->compound) return true;
    // Recursively check parents
    if (CguiCheckWidgetIsCompound(widget->parent)) return true;
    return false;
}

// Get the widget transform based on it's parents
Rectangle CguiGetWidgetRectangle(CguiWidget *widget)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to get Widget Rectangle due to invalid Widget", , (Rectangle) { 0 });

    // Return local transform if parent is unavailable
    CguiTransform transform = widget->localTransform;
    if (!widget->parent) return (Rectangle) { transform.position.x, transform.position.y, transform.size.x, transform.size.y };

    // Recursively get parent's rectangle
    else
    {
        Rectangle parentRectangle = CguiGetWidgetRectangle(widget->parent);
        transform.position = Vector2Add(transform.position, (Vector2) { parentRectangle.x + parentRectangle.width / 2.0f, parentRectangle.y + parentRectangle.height / 2.0f });

        // Stretch rectangle when two opposite anchors are set
        if (transform.anchorLeft && transform.anchorRight)
            transform.size.x = parentRectangle.width - widget->localTransform.position.x * 2.0f - widget->localTransform.stretchSpacing.x;
        if (transform.anchorTop && transform.anchorBottom)
            transform.size.y = parentRectangle.height - widget->localTransform.position.y * 2.0f - widget->localTransform.stretchSpacing.y;

        // Adjust position based on anchor
        if (transform.anchorLeft && !transform.anchorRight)
            transform.position.x = parentRectangle.x + widget->localTransform.position.x + widget->localTransform.size.x / 2.0f;
        if (transform.anchorRight && !transform.anchorLeft)
            transform.position.x = parentRectangle.x + widget->localTransform.position.x + widget->localTransform.size.x / 2.0f + parentRectangle.width - transform.size.x;
        if (transform.anchorTop && !transform.anchorBottom)
            transform.position.y = parentRectangle.y + widget->localTransform.position.y + widget->localTransform.size.y / 2.0f;
        if (transform.anchorBottom && !transform.anchorTop)
            transform.position.y = parentRectangle.y + widget->localTransform.position.y + widget->localTransform.size.y / 2.0f + parentRectangle.height - transform.size.y;
    }

    return (Rectangle) { transform.position.x - transform.size.x / 2.0f, transform.position.y - transform.size.y / 2.0f, transform.size.x, transform.size.y };
}

// Attach a GUI element to widget
void CguiAttachElement(CguiWidget *widget, void *element)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to attach Widget due to invalid Widget", , );
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to attach Widget due to invalid Element", , );

    widget->element = element;
    ((CguiElementBasic *)element)->widget = widget;
}

// Detach a GUI element to widget
void CguiDetachElement(CguiWidget *widget, void *element)
{
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to detach Widget due to invalid Widget", , );
    CGUI_CHECK_VALID_LOG("WIDGET", widget, "Failed to detach Widget due to invalid Element", , );

    widget->element = NULL;
    ((CguiElementBasic *)element)->widget = NULL;
}

// Create a new label
CguiElementLabel *CguiCreateElementLabel(CguiResources *res, const char *text)
{
    CGUI_CHECK_VALID_LOG("ELEMENT LABEL", res, "Failed to create Label due to invalid Resources", , NULL);

    CguiElementLabel *label = MemAlloc(sizeof(CguiElementLabel));
    CGUI_CHECK_VALID_LOG("ELEMENT LABEL", label, "Failed to allocate memory for Label", , NULL);

    label->elementType = CGUI_ELEMENT_TYPE_LABEL;
    label->text = text;
    label->font = res->fontCollection.regularFont;
    label->fontSize = 20.0f;
    label->textColor = res->colorTheme.textColor;
    label->spacing = 1.0f;

    CGUI_LOG_NOID("ELEMENT LABEL", "Successfully created Label");
    return label;
}

// Delete created label
void CguiDeleteElementLabel(CguiElementLabel *label)
{
    CGUI_CHECK_VALID_LOG("ELEMENT LABEL", label, "Failed to delete Label due to invalid Label", , );
    MemFree(label);
    CGUI_LOG_NOID("ELEMENT LABEL", "Successfully deleted Label");
}

// Update label (does nothing)
void CguiUpdateElementLabel(CguiResources *res, CguiElementLabel *label)
{
    // ... Do nothing ... and pretend to do something:
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(label, , );
    CGUI_CHECK_VALID(label->widget, , );
}

// Draw label
void CguiDrawElementLabel(CguiResources *res, CguiElementLabel *label)
{
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(label, , );
    CGUI_CHECK_VALID(label->widget, , );

    Vector2 measureText = CguiMeasureText(label->font, label->text, label->fontSize, label->spacing);
    label->widget->localTransform.size = measureText;
    Rectangle rec = CguiGetWidgetRectangle(label->widget);

    CguiBeginGradientModeColor(res->gradientShader, label->textColor, rec);
    CguiDrawText(label->font, label->text, (Vector2) { rec.x + (rec.width - measureText.x) / 2.0f, rec.y + (rec.height - measureText.y) / 2.0f }, label->fontSize, label->spacing, WHITE);
    CguiEndGradientMode(res->gradientShader);
}

// Create a new icon
CguiElementIcon *CguiCreateElementIcon(CguiResources *res, CguiIconTexture iconTexture)
{
    CGUI_CHECK_VALID_LOG("ELEMENT ICON", res, "Failed to create Icon due to invalid Resources", , NULL);

    CguiElementIcon *icon = MemAlloc(sizeof(CguiElementIcon));
    CGUI_CHECK_VALID_LOG("ELEMENT ICON", icon, "Failed to allocate memory for Icon", , NULL);

    icon->elementType = CGUI_ELEMENT_TYPE_ICON;
    icon->iconTexture = iconTexture;
    icon->iconTint = res->colorTheme.iconColor;

    CGUI_LOG_NOID("ELEMENT ICON", "Successfully created Icon");
    return icon;
}

// Delete created icon
void CguiDeleteElementIcon(CguiElementIcon *icon)
{
    CGUI_CHECK_VALID_LOG("ELEMENT ICON", icon, "Failed to delete Icon due to invalid Icon", , );
    MemFree(icon);
    CGUI_LOG_NOID("ELEMENT ICON", "Successfully deleted Icon");
}

// Update icon (does nothing)
void CguiUpdateElementIcon(CguiResources *res, CguiElementIcon *icon)
{
    // ... Do nothing ... and pretend to do something:
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(icon, , );
    CGUI_CHECK_VALID(icon->widget, , );
}

// Draw icon
void CguiDrawElementIcon(CguiResources *res, CguiElementIcon *icon)
{
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(icon, , );
    CGUI_CHECK_VALID(icon->widget, , );

    Rectangle rec = CguiGetWidgetRectangle(icon->widget);

    CguiBeginGradientModeColor(res->gradientShader, icon->iconTint, rec);
    CguiDrawIconTextureEx(icon->iconTexture, rec, WHITE);
    CguiEndGradientMode(res->gradientShader);
}

// Create a new animated icon
CguiElementAnimatedIcon *CguiCreateElementAnimatedIcon(CguiResources *res, CguiAnimatedIcon animatedIconTexture)
{
    CGUI_CHECK_VALID_LOG("ELEMENT ANIMATED ICON", res, "Failed to create Animated Icon due to invalid Resources", , NULL);

    CguiElementAnimatedIcon *animatedIcon = MemAlloc(sizeof(CguiElementAnimatedIcon));
    CGUI_CHECK_VALID_LOG("ELEMENT ANIMATED ICON", animatedIcon, "Failed to allocate memory for Animated Icon", , NULL);

    animatedIcon->elementType = CGUI_ELEMENT_TYPE_ANIMATED_ICON;
    animatedIcon->animatedIconTexture = animatedIconTexture;
    animatedIcon->animatedIconTint = res->colorTheme.iconColor;
    animatedIcon->startTime = GetTime();

    CGUI_LOG_NOID("ELEMENT ANIMATED ICON", "Successfully created Animated Icon");
}

// Delete created animated icon
void CguiDeleteElementAnimatedIcon(CguiElementAnimatedIcon *animatedIcon)
{
    CGUI_CHECK_VALID_LOG("ELEMENT ANIMATED ICON", animatedIcon, "Failed to delete Animated Icon due to invalid Animated Icon", , );
    MemFree(animatedIcon);
    CGUI_LOG_NOID("ELEMENT ANIMATED ICON", "Successfully deleted Animated Icon");
}

// Update animated icon (does nothing)
void CguiUpdateElementAnimatedIcon(CguiResources *res, CguiElementAnimatedIcon *animatedIcon)
{
    // ... Do nothing ... and pretend to do something:
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(animatedIcon, , );
    CGUI_CHECK_VALID(animatedIcon->widget, , );
}

// Draw animated icon
void CguiDrawElementAnimatedIcon(CguiResources *res, CguiElementAnimatedIcon *animatedIcon)
{
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(animatedIcon, , );
    CGUI_CHECK_VALID(animatedIcon->widget, , );

    Rectangle rec = CguiGetWidgetRectangle(animatedIcon->widget);

    CguiBeginGradientModeColor(res->gradientShader, animatedIcon->animatedIconTint, rec);
    CguiDrawAnimatedIconEx(animatedIcon->animatedIconTexture, rec, WHITE, animatedIcon->startTime);
    CguiEndGradientMode(res->gradientShader);
}

// Create a new button
CguiElementButton *CguiCreateElementButton(CguiResources *res)
{
    CGUI_CHECK_VALID_LOG("ELEMENT BUTTON", res, "Failed to create Button due to invalid Resources", , NULL);

    CguiElementButton *button = MemAlloc(sizeof(CguiElementButton));
    CGUI_CHECK_VALID_LOG("ELEMENT BUTTON", button, "Failed to allocate memory for Button", , NULL);

    button->elementType = CGUI_ELEMENT_TYPE_BUTTON;
    button->roundness = 20.0f;
    button->color = res->colorTheme.mainColor;
    button->hoverColor.color1 = ColorMultiply(button->color.color1, res->colorTheme.hoverTintColor);
    button->hoverColor.color2 = ColorMultiply(button->color.color2, res->colorTheme.hoverTintColor);
    button->heldColor.color1 = ColorMultiply(button->color.color1, res->colorTheme.heldTintColor);
    button->heldColor.color2 = ColorMultiply(button->color.color2, res->colorTheme.heldTintColor);

    CGUI_LOG_NOID("ELEMENT BUTTON", "Successfully created Button");
    return button;
}

// Delete created button
void CguiDeleteElementButton(CguiElementButton *button)
{
    CGUI_CHECK_VALID_LOG("ELEMENT BUTTON", button, "Failed to delete Button due to invalid Button", , );
    MemFree(button);
    CGUI_LOG_NOID("ELEMENT BUTTON", "Successfully deleted Button");
}

// Update button, return true on click and call onClick event
bool CguiUpdateElementButton(CguiResources *res, CguiElementButton *button)
{
    CGUI_CHECK_VALID(res, , false);
    CGUI_CHECK_VALID(button, , false);
    CGUI_CHECK_VALID(button->widget, , false);

    bool clicked = false;
    if (!button->widget->enabled) return false;

    button->action = CGUI_MOUSE_ACTION_UP;
    button->mouseButton = 0;

    Rectangle rec = CguiGetWidgetRectangle(button->widget);
    if (CheckCollisionPointRec(GetMousePosition(), rec))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked = true;

        for (int i = 0; i < 7; i++)
        {
            if (IsMouseButtonDown(i))
            {
                button->mouseButton = i;
                button->action = CGUI_MOUSE_ACTION_DOWN;
                break;
            }
            if (IsMouseButtonPressed(i))
            {
                button->mouseButton = i;
                button->action = CGUI_MOUSE_ACTION_PRESSED;
                break;
            }
            if (IsMouseButtonReleased(i))
            {
                button->mouseButton = i;
                button->action = CGUI_MOUSE_ACTION_RELEASED;
                break;
            }
        }

        if (!button->hover && button->onHover) button->onHover(button, true);
        button->hover = true;
    }
    else
    {
        if (button->hover && button->onHover) button->onHover(button, false);
        button->hover = false;
    }

    if (button->onClick && button->action != CGUI_MOUSE_ACTION_RELEASED) button->onClick(button, button->mouseButton);
    return clicked;
}

// Draw button
void CguiDrawElementButton(CguiResources *res, CguiElementButton *button)
{
    CGUI_CHECK_VALID(res, , );
    CGUI_CHECK_VALID(button, , );
    CGUI_CHECK_VALID(button->widget, , );

    Rectangle rec = CguiGetWidgetRectangle(button->widget);
    if (button->hover) switch (button->action)
        {
            case CGUI_MOUSE_ACTION_RELEASED:
            case CGUI_MOUSE_ACTION_UP:
                CguiBeginGradientModeColor(res->gradientShader, button->hoverColor, rec);
                break;
            case CGUI_MOUSE_ACTION_PRESSED:
            case CGUI_MOUSE_ACTION_DOWN:
                CguiBeginGradientModeColor(res->gradientShader, button->heldColor, rec);
                break;
        }
    else CguiBeginGradientModeColor(res->gradientShader, button->color, rec);

    CguiDrawRectangle(res->rectangles, rec, button->roundness, WHITE);
    CguiEndGradientMode(res->gradientShader);
}

// Create a new toggle
CguiElementToggle *CguiCreateElementToggle(CguiResources *res)
{
}

// Delete created toggle
void CguiDeleteElementToggle(CguiElementToggle *toggle)
{
}

// Update toggle, return true on click and call onClick event
bool CguiUpdateElementToggle(CguiResources *res, CguiElementToggle *toggle)
{
}

// Draw toggle
void CguiDrawElementToggle(CguiResources *res, CguiElementToggle *toggle)
{
}
