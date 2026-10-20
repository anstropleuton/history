// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#include "cgui_extra.h"
#include "cgui_transition.h"
#include "math.h"

float CguiTransitionUpdate(CguiTransition *transition, float deltaTime) {
    if (!transition) return 0.0f;

    if (!transition->easing) transition->easing = CguiEaseLinear;

    if (transition->active) {
        if (transition->reverse) {
            transition->time -= deltaTime;
        } else {
            transition->time += deltaTime;
        }
    }

    float cycle = transition->delayBefore + transition->duration + transition->delayAfter;
    float begin = transition->reverse ? 1.0f : 0.0f;
    float end   = transition->reverse ? 0.0f : 1.0f;

    if (cycle <= 0.0f) {
        float u = transition->time == 0.0f ? begin : end;
        return transition->easing(u);
    }

    if (transition->pingPong || transition->loopCount != 0) {
        if (transition->pingPong && (transition->time < 0.0f || transition->time > cycle)) {
            // Very large dt?
            bool odd = (int)floorf(transition->time / cycle) % 2 != 0;
            if (odd) transition->reverse = !transition->reverse;
        }
        transition->time = CguiWrapModF(transition->time, cycle);
        if (transition->loopCount > 0) transition->loopCount--;
    } else {
        transition->time = CguiClampF(transition->time, 0.0f, cycle);
    }

    if (transition->time < transition->delayBefore) {
        return transition->easing(begin);
    } else if (transition->time >= transition->delayBefore + transition->duration) {
        return transition->easing(end);
    } else {
        if (transition->duration == 0.0f) {
            float u = transition->time == 0.0f ? begin : end;
            return transition->easing(u);
        } else {
            float u = (transition->time - transition->delayBefore) / transition->duration;
            u       = transition->reverse ? 1.0f - u : u;
            return transition->easing(u);
        }
    }
}

float CguiEaseLinear(float t) {
    return t;
}

float CguiEaseIn(float t, float power) {
    return powf(t, power);
}

float CguiEaseOut(float t, float power) {
    return 1 - powf(1 - t, power);
}

float CguiEaseInOut(float t, float power) {
    if (t < .5) {
        return powf(2 * t, power) / 2;
    } else {
        return 1 - powf(2 * (1 - t), power) / 2;
    }
}

float CguiEaseInQuad(float t) {
    return CguiEaseIn(t, 2);
}

float CguiEaseOutQuad(float t) {
    return CguiEaseOut(t, 2);
}

float CguiEaseInOutQuad(float t) {
    return CguiEaseInOut(t, 2);
}

float CguiEaseInCubic(float t) {
    return CguiEaseIn(t, 3);
}

float CguiEaseOutCubic(float t) {
    return CguiEaseOut(t, 3);
}

float CguiEaseInOutCubic(float t) {
    return CguiEaseInOut(t, 3);
}

float CguiEaseInQuart(float t) {
    return CguiEaseIn(t, 4);
}

float CguiEaseOutQuart(float t) {
    return CguiEaseOut(t, 4);
}

float CguiEaseInOutQuart(float t) {
    return CguiEaseInOut(t, 4);
}

float CguiEaseInQuint(float t) {
    return CguiEaseIn(t, 5);
}

float CguiEaseOutQuint(float t) {
    return CguiEaseOut(t, 5);
}

float CguiEaseInOutQuint(float t) {
    return CguiEaseInOut(t, 5);
}

float CguiEaseInSextic(float t) {
    return CguiEaseIn(t, 6);
}

float CguiEaseOutSextic(float t) {
    return CguiEaseOut(t, 6);
}

float CguiEaseInOutSextic(float t) {
    return CguiEaseInOut(t, 6);
}

float CguiEaseInSeptic(float t) {
    return CguiEaseIn(t, 7);
}

float CguiEaseOutSeptic(float t) {
    return CguiEaseOut(t, 7);
}

float CguiEaseInOutSeptic(float t) {
    return CguiEaseInOut(t, 7);
}

float CguiEaseInOctic(float t) {
    return CguiEaseIn(t, 8);
}

float CguiEaseOutOctic(float t) {
    return CguiEaseOut(t, 8);
}

float CguiEaseInOutOctic(float t) {
    return CguiEaseInOut(t, 8);
}

float CguiEaseInNonic(float t) {
    return CguiEaseIn(t, 9);
}

float CguiEaseOutNonic(float t) {
    return CguiEaseOut(t, 9);
}

float CguiEaseInOutNonic(float t) {
    return CguiEaseInOut(t, 9);
}

float CguiEaseInSine(float t) {
    return 1 - cosf((t * PI) / 2);
}

float CguiEaseOutSine(float t) {
    return sinf((t * PI) / 2);
}

float CguiEaseInOutSine(float t) {
    return -(cosf(PI * t) - 1) / 2;
}

float CguiEaseInExpo(float t) {
    return CguiEaseInExpoPro(t, 2, 10);
}

float CguiEaseOutExpo(float t) {
    return CguiEaseOutExpoPro(t, 2, 10);
}

float CguiEaseInOutExpo(float t) {
    return CguiEaseInOutExpoPro(t, 2, 10);
}

float CguiEaseInExpoPro(float t, float base, float power) {
    return powf(base, power * (t - 1));
}

float CguiEaseOutExpoPro(float t, float base, float power) {
    return 1 - powf(base, -power * t);
}

float CguiEaseInOutExpoPro(float t, float base, float power) {
    if (t < .5)
        return (powf(base, power * (2 * t - 1))) / (powf(base, power));
    else
        return 1 - (powf(base, -power * (2 * t - 1))) / (powf(base, power));
}

float CguiEaseInCirc(float t) {
    return 1 - sqrtf(1 - t * t);
}

float CguiEaseOutCirc(float t) {
    return sqrtf(1 - powf(t - 1, 2));
}

float CguiEaseInOutCirc(float t) {
    if (t < .5)
        return (1 - sqrtf(1 - 4 * t * t)) / 2;
    else
        return (sqrtf(1 - powf(-2 * t + 2, 2)) + 1) / 2;
}

float CguiEaseInBack(float t) {
    return CguiEaseInBackPro(t, 1.70158);
}

float CguiEaseOutBack(float t) {
    return CguiEaseOutBackPro(t, 1.70158);
}

float CguiEaseInOutBack(float t) {
    return CguiEaseInOutBackPro(t, 1.70158 * 1.525);
}

float CguiEaseInBackPro(float t, float overshoot) {
    return t * t * ((overshoot + 1) * t - overshoot);
}

float CguiEaseOutBackPro(float t, float overshoot) {
    float inv = 1 - t;
    return 1 - inv * inv * ((overshoot + 1) * inv - overshoot);
}

float CguiEaseInOutBackPro(float t, float overshoot) {
    if (t < .5) {
        float t2 = 2 * t;
        return (t2 * t2 * ((overshoot + 1) * t2 - overshoot)) / 2;
    } else {
        float t2 = 2 * t - 2;
        return (t2 * t2 * ((overshoot + 1) * t2 + overshoot) + 2) / 2;
    }
}

float CguiEaseInElastic(float t) {
    return CguiEaseInElasticPro(t, 1, .3, 2, 10);
}

float CguiEaseOutElastic(float t) {
    return CguiEaseOutElasticPro(t, 1, .3, 2, 10);
}

float CguiEaseInOutElastic(float t) {
    return CguiEaseInOutElasticPro(t, 1, .3 * 1.5, 2, 10);
}

float CguiEaseInElasticPro(float t, float amplitude, float period, float base, float power) {
    float s;
    if (amplitude < 1) {
        amplitude = 1;
        s         = period / 4;
    } else {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    t -= 1;
    return -(amplitude * powf(base, power * t) * sinf((t - s) * (2 * PI) / period));
}

float CguiEaseOutElasticPro(float t, float amplitude, float period, float base, float power) {
    float s;
    if (amplitude < 1) {
        amplitude = 1;
        s         = period / 4;
    } else {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    return amplitude * powf(base, -power * t) * sinf((t - s) * (2 * PI) / period) + 1;
}

float CguiEaseInOutElasticPro(float t, float amplitude, float period, float base, float power) {
    float s;
    if (amplitude < 1) {
        amplitude = 1;
        s         = period / 4;
    } else {
        s = period / (2 * PI) * asinf(1 / amplitude);
    }

    if (t < .5) {
        t = 2 * t - 1;
        return -amplitude * powf(base, power * t) * sinf((t - s) * (2 * PI) / period) / 2;
    } else {
        t = 2 * t - 1;
        return amplitude * powf(base, -power * t) * sinf((t - s) * (2 * PI) / period) / 2 + 1;
    }
}

// Bounce easing helper
float EaseBounce(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4) {
    if (t < b1)
        return c1 * t * t;
    else if (t < b2)
        return c2 * (t - b3) * (t - b3) + b4;
    else if (t < b3)
        return c3 * (t - b4) * (t - b4) + b4;
    else
        return c4 * (t - 1) * (t - 1) + 1;
}

float CguiEaseInBounce(float t) {
    return CguiEaseInBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseOutBounce(float t) {
    return CguiEaseOutBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseInOutBounce(float t) {
    return CguiEaseInOutBouncePro(t, 4.f / 11, 8.f / 11, 9.f / 10, 21.f / 22, 121.f / 16, 363.f / 40, 4356.f / 361, 54.f / 5);
}

float CguiEaseInBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4) {
    return 1 - EaseBounce(1 - t, b1, b2, b3, b4, c1, c2, c3, c4);
}

float CguiEaseOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4) {
    return EaseBounce(t, b1, b2, b3, b4, c1, c2, c3, c4);
}

float CguiEaseInOutBouncePro(float t, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4) {
    if (t < .5)
        return (1 - EaseBounce(1 - 2 * t, b1, b2, b3, b4, c1, c2, c3, c4)) / 2;
    else
        return (1 + EaseBounce(2 * t - 1, b1, b2, b3, b4, c1, c2, c3, c4)) / 2;
}

int CguiLerpInt(int from, int to, float t) {
    return from * (1.0f - t) + to * t;
}

unsigned int CguiLerpUInt(unsigned int from, unsigned int to, float t) {
    return from * (1.0f - t) + to * t;
}

float CguiLerpFloat(float from, float to, float t) {
    return from * (1.0f - t) + to * t;
}

double CguiLerpDouble(double from, double to, float t) {
    return from * (1.0f - t) + to * t;
}

Vector2 CguiLerpVector2(Vector2 from, Vector2 to, float t) {
    return (Vector2){
        from.x * (1.0f - t) + to.x * t,
        from.y * (1.0f - t) + to.y * t
    };
}

Vector3 CguiLerpVector3(Vector3 from, Vector3 to, float t) {
    return (Vector3){
        from.x * (1.0f - t) + to.x * t,
        from.y * (1.0f - t) + to.y * t,
        from.z * (1.0f - t) + to.z * t
    };
}

Vector4 CguiLerpVector4(Vector4 from, Vector4 to, float t) {
    return (Vector4){
        from.x * (1.0f - t) + to.x * t,
        from.y * (1.0f - t) + to.y * t,
        from.z * (1.0f - t) + to.z * t,
        from.w * (1.0f - t) + to.w * t
    };
}

Matrix CguiLerpMatrix(Matrix from, Matrix to, float t) {
    return (Matrix){
        from.m0 * (1.0f - t) + to.m0 * t,
        from.m1 * (1.0f - t) + to.m1 * t,
        from.m2 * (1.0f - t) + to.m2 * t,
        from.m3 * (1.0f - t) + to.m3 * t,
        from.m4 * (1.0f - t) + to.m4 * t,
        from.m5 * (1.0f - t) + to.m5 * t,
        from.m6 * (1.0f - t) + to.m6 * t,
        from.m7 * (1.0f - t) + to.m7 * t,
        from.m8 * (1.0f - t) + to.m8 * t,
        from.m9 * (1.0f - t) + to.m9 * t,
        from.m10 * (1.0f - t) + to.m10 * t,
        from.m11 * (1.0f - t) + to.m11 * t,
        from.m12 * (1.0f - t) + to.m12 * t,
        from.m13 * (1.0f - t) + to.m13 * t,
        from.m14 * (1.0f - t) + to.m14 * t,
        from.m15 * (1.0f - t) + to.m15 * t
    };
}

CguiMatrix3 CguiLerpMatrix3(CguiMatrix3 from, CguiMatrix3 to, float t) {
    return (CguiMatrix3){
        from.m0 * (1.0f - t) + to.m0 * t,
        from.m1 * (1.0f - t) + to.m1 * t,
        from.m2 * (1.0f - t) + to.m2 * t,
        from.m3 * (1.0f - t) + to.m3 * t,
        from.m4 * (1.0f - t) + to.m4 * t,
        from.m5 * (1.0f - t) + to.m5 * t,
        from.m6 * (1.0f - t) + to.m6 * t,
        from.m7 * (1.0f - t) + to.m7 * t,
        from.m8 * (1.0f - t) + to.m8 * t
    };
}

Color CguiLerpColor(Color from, Color to, float t) {
    return (Color){
        from.r * (1.0f - t) + to.r * t,
        from.g * (1.0f - t) + to.g * t,
        from.b * (1.0f - t) + to.b * t,
        from.a * (1.0f - t) + to.a * t
    };
}

Rectangle CguiLerpRectangle(Rectangle from, Rectangle to, float t) {
    return (Rectangle){
        from.x * (1.0f - t) + to.x * t,
        from.y * (1.0f - t) + to.y * t,
        from.width * (1.0f - t) + to.width * t,
        from.height * (1.0f - t) + to.height * t
    };
}
