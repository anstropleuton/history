// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Transition and easing functions for smooth animations
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_TRANSITION_H_
#define CGUI_TRANSITION_H_

#include "cgui_matrix3.h"
#include "raylib.h"

#define CGUI_TRANSITION(TYPE, NAME) \
    TYPE           NAME##From;      \
    TYPE           NAME##To;        \
    CguiTransition NAME##Transition

#define CGUI_TRANSITION_UPDATE(NAME, DELTA_TIME, LERP, RESULT)           \
    do {                                                                 \
        float t = CguiTransitionUpdate(&NAME##Transition, (DELTA_TIME)); \
        if (NAME##Transition.easing) {                                   \
            t = NAME##Transition.easing(t);                              \
        }                                                                \
        (RESULT) = (LERP)(NAME##From, NAME##To, t);                      \
    } while (0)

// Easing function type
typedef float (*CguiEasingFunc)(float t);

// Transition state
typedef struct CguiTransition {
    CguiEasingFunc easing;      // Easing of the timing
    float          time;        // Current animation progress
    float          duration;    // Duration in seconds animation should last
    float          delayBefore; // Delay before the animation begin
    float          delayAfter;  // Delay after the animation end
    int            loopCount;   // Number of loops to animate (-1 to loop forever)
    bool           active;      // Is animation active (turns off when animation ends)
    bool           reverse;     // Current animation direction (ping pong animation modifies this field)
    bool           pingPong;    // Whether to bounce the animation time back and forth (loopCount decreases each bounce)
} CguiTransition;

// Updates a transition and returns the current interpolation parameter t based on the elapsed time and transition settings
CGAPI float CguiTransitionUpdate(CguiTransition *transition, float deltaTime);

// Linear easing function
CGAPI float CguiEaseLinear(float t);

// Quadratic (2) easing functions
CGAPI float CguiEaseInQuad(float t);
CGAPI float CguiEaseOutQuad(float t);
CGAPI float CguiEaseInOutQuad(float t);

// Cubic (3) easing functions
CGAPI float CguiEaseInCubic(float t);
CGAPI float CguiEaseOutCubic(float t);
CGAPI float CguiEaseInOutCubic(float t);

// Quartic (4) easing functions
CGAPI float CguiEaseInQuart(float t);
CGAPI float CguiEaseOutQuart(float t);
CGAPI float CguiEaseInOutQuart(float t);

// Quintic (5) easing functions
CGAPI float CguiEaseInQuint(float t);
CGAPI float CguiEaseOutQuint(float t);
CGAPI float CguiEaseInOutQuint(float t);

// Sextic (6) easing functions
CGAPI float CguiEaseInSextic(float t);
CGAPI float CguiEaseOutSextic(float t);
CGAPI float CguiEaseInOutSextic(float t);

// Septic (7) easing functions
CGAPI float CguiEaseInSeptic(float t);
CGAPI float CguiEaseOutSeptic(float t);
CGAPI float CguiEaseInOutSeptic(float t);

// Octic (8) easing functions
CGAPI float CguiEaseInOctic(float t);
CGAPI float CguiEaseOutOctic(float t);
CGAPI float CguiEaseInOutOctic(float t);

// Nonic (9) easing functions
CGAPI float CguiEaseInNonic(float t);
CGAPI float CguiEaseOutNonic(float t);
CGAPI float CguiEaseInOutNonic(float t);

// Sine easing functions
CGAPI float CguiEaseInSine(float t);
CGAPI float CguiEaseOutSine(float t);
CGAPI float CguiEaseInOutSine(float t);

// Exponential easing functions
CGAPI float CguiEaseInExpo(float t);
CGAPI float CguiEaseOutExpo(float t);
CGAPI float CguiEaseInOutExpo(float t);

// Circular easing functions
CGAPI float CguiEaseInCirc(float t);
CGAPI float CguiEaseOutCirc(float t);
CGAPI float CguiEaseInOutCirc(float t);

// Back easing functions
CGAPI float CguiEaseInBack(float t);
CGAPI float CguiEaseOutBack(float t);
CGAPI float CguiEaseInOutBack(float t);

// Elastic easing functions
CGAPI float CguiEaseInElastic(float t);
CGAPI float CguiEaseOutElastic(float t);
CGAPI float CguiEaseInOutElastic(float t);

// Bounce easing functions
CGAPI float CguiEaseInBounce(float t);
CGAPI float CguiEaseOutBounce(float t);
CGAPI float CguiEaseInOutBounce(float t);

// Additional easing functions
// Note: Not compatible with CguiEasingFunc

CGAPI float CguiEaseIn(float t, float power);
CGAPI float CguiEaseOut(float t, float power);
CGAPI float CguiEaseInOut(float t, float power);

CGAPI float CguiEaseInExpoPro(float t, float base, float power);
CGAPI float CguiEaseOutExpoPro(float t, float base, float power);
CGAPI float CguiEaseInOutExpoPro(float t, float base, float power);

CGAPI float CguiEaseInBackPro(float t, float overshoot);
CGAPI float CguiEaseOutBackPro(float t, float overshoot);
CGAPI float CguiEaseInOutBackPro(float t, float overshoot);

CGAPI float CguiEaseInElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseOutElasticPro(float t, float amplitude, float period, float base, float power);
CGAPI float CguiEaseInOutElasticPro(float t, float amplitude, float period, float base, float power);

CGAPI float CguiEaseOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);
CGAPI float CguiEaseInOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4);

// Linear interpolation for types

CGAPI int          CguiLerpInt(int from, int to, float t);
CGAPI unsigned int CguiLerpUInt(unsigned int from, unsigned int to, float t);
CGAPI float        CguiLerpFloat(float from, float to, float t);
CGAPI double       CguiLerpDouble(double from, double to, float t);
CGAPI Vector2      CguiLerpVector2(Vector2 from, Vector2 to, float t);
CGAPI Vector3      CguiLerpVector3(Vector3 from, Vector3 to, float t);
CGAPI Vector4      CguiLerpVector4(Vector4 from, Vector4 to, float t);
CGAPI Matrix       CguiLerpMatrix(Matrix from, Matrix to, float t);
CGAPI CguiMatrix3  CguiLerpMatrix3(CguiMatrix3 from, CguiMatrix3 to, float t);
CGAPI Color        CguiLerpColor(Color from, Color to, float t);
CGAPI Rectangle    CguiLerpRectangle(Rectangle from, Rectangle to, float t);

#endif // CGUI_TRANSITION_H_
