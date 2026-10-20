// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Event system for handling user interactions and custom events
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_EVENT_H_
#define CGUI_EVENT_H_

// Event types for handling user interactions
typedef enum CguiEventType {
    CGUI_EVENT_KEY_PRESS,      // When a key is pressed
    CGUI_EVENT_KEY_RELEASE,    // When a key is released
    CGUI_EVENT_KEY_TEXT_INPUT, // When text input is received
    CGUI_EVENT_MOUSE_MOVE,     // When the mouse is moved
    CGUI_EVENT_MOUSE_PRESS,    // When a mouse button is pressed
    CGUI_EVENT_MOUSE_RELEASE,  // When a mouse button is released
    CGUI_EVENT_MOUSE_ENTER,    // When the mouse enters a node
    CGUI_EVENT_MOUSE_LEAVE,    // When the mouse leaves a node
    CGUI_EVENT_MOUSE_WHEEL,    // When the mouse wheel is scrolled
    CGUI_EVENT_TOUCH_PRESS,    // When a touch is pressed
    CGUI_EVENT_TOUCH_RELEASE,  // When a touch is released
    CGUI_EVENT_TOUCH_MOVE,     // When a touch is moved
} CguiEventType;

typedef struct CguiEventKeyPress {
    int key;       // The key that was pressed (e.g., KEY_A, KEY_ENTER)
    int scancode;  // The platform-specific scancode of the key
    int modifiers; // Bitfield of modifier keys (e.g., shift, control, alt)
} CguiEventKeyPress;

typedef struct CguiEventKeyRelease {
    int key;       // The key that was released (e.g., KEY_A, KEY_ENTER)
    int scancode;  // The platform-specific scancode of the key
    int modifiers; // Bitfield of modifier keys (e.g., shift, control, alt)
} CguiEventKeyRelease;

typedef struct CguiEventKeyTextInput {
    int codepoint; // The Unicode codepoint of the text input
} CguiEventKeyTextInput;

typedef struct CguiEventMouseMove {
    float x;      // The new x-coordinate of the mouse
    float y;      // The new y-coordinate of the mouse
    float deltaX; // The change in x-coordinate since the last event
    float deltaY; // The change in y-coordinate since the last event
} CguiEventMouseMove;

typedef struct CguiEventMousePress {
    int   button; // The mouse button that was pressed (e.g., MOUSE_LEFT_BUTTON)
    float x;      // The x-coordinate of the mouse when the button was pressed
    float y;      // The y-coordinate of the mouse when the button was pressed
} CguiEventMousePress;

typedef struct CguiEventMouseRelease {
    int   button; // The mouse button that was released (e.g., MOUSE_LEFT_BUTTON)
    float x;      // The x-coordinate of the mouse when the button was released
    float y;      // The y-coordinate of the mouse when the button was released
} CguiEventMouseRelease;

typedef struct CguiEventMouseWheel {
    float deltaX; // The horizontal scroll amount
    float deltaY; // The vertical scroll amount
} CguiEventMouseWheel;

typedef struct CguiEventTouchPress {
    int   id; // The unique identifier for the touch point
    float x;  // The x-coordinate of the touch point when pressed
    float y;  // The y-coordinate of the touch point when pressed
} CguiEventTouchPress;

typedef struct CguiEventTouchRelease {
    int   id; // The unique identifier for the touch point
    float x;  // The x-coordinate of the touch point when released
    float y;  // The y-coordinate of the touch point when released
} CguiEventTouchRelease;

typedef struct CguiEventTouchMove {
    int   id;     // The unique identifier for the touch point
    float x;      // The new x-coordinate of the touch point
    float y;      // The new y-coordinate of the touch point
    float deltaX; // The change in x-coordinate since the last event
    float deltaY; // The change in y-coordinate since the last event
} CguiEventTouchMove;

typedef struct CguiEvent {
    CguiEventType type; // The type of the event
    union {
        CguiEventKeyPress     keyPress;
        CguiEventKeyRelease   keyRelease;
        CguiEventKeyTextInput keyTextInput;
        CguiEventMouseMove    mouseMove;
        CguiEventMousePress   mousePress;
        CguiEventMouseRelease mouseRelease;
        CguiEventMouseWheel   mouseWheel;
        CguiEventTouchPress   touchPress;
        CguiEventTouchRelease touchRelease;
        CguiEventTouchMove    touchMove;
    };
} CguiEvent;

#endif // CGUI_EVENT_H_
