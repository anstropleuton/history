/*******************************************************************************************
*
*   This is taken from raylib/examples/shaders/shaders_rounded_rectangle.c
*
*   Copyright (c) 2025 Ramon Santamaria and Anstro Pleuton.
*
*   This file licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   SDF by Iñigo Quilez. MIT License.
*
********************************************************************************************/

#ifndef RAYLIB_SHADERS_H
#define RAYLIB_SHADERS_H

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Rounded rectangle data
typedef struct {
    bool configured;

    Vector4 cornerRadius; // Individual corner radius (top-left, top-right, bottom-left, bottom-right)

    // Shadow variables
    float shadowRadius;
    Vector2 shadowOffset;
    float shadowScale;

    // Border variables
    float borderThickness; // Inner-border thickness

    // Shader locations
    int rectangleLoc;
    int radiusLoc;
    int colorLoc;
    int shadowRadiusLoc;
    int shadowOffsetLoc;
    int shadowScaleLoc;
    int shadowColorLoc;
    int borderThicknessLoc;
    int borderColorLoc;
} RoundedRectangle;

//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------

// Create a rounded rectangle and set uniform locations
RoundedRectangle CreateRoundedRectangle(Vector4 cornerRadius, float shadowRadius, Vector2 shadowOffset, float shadowScale, float borderThickness, Shader shader);

// Update rounded rectangle uniforms
void UpdateRoundedRectangle(RoundedRectangle rec, Shader shader);

#ifdef RAYLIB_SHADERS_IMPLEMENTATION

//------------------------------------------------------------------------------------
// Module Functions Definitions
//------------------------------------------------------------------------------------

// Create a rounded rectangle and set uniform locations
RoundedRectangle CreateRoundedRectangle(Vector4 cornerRadius, float shadowRadius, Vector2 shadowOffset, float shadowScale, float borderThickness, Shader shader)
{
    RoundedRectangle rec;
    rec.configured = true;

    rec.cornerRadius = cornerRadius;
    rec.shadowRadius = shadowRadius;
    rec.shadowOffset = shadowOffset;
    rec.shadowScale = shadowScale;
    rec.borderThickness = borderThickness;

    // Get shader uniform locations
    rec.rectangleLoc = GetShaderLocation(shader, "rectangle");
    rec.radiusLoc = GetShaderLocation(shader, "radius");
    rec.colorLoc = GetShaderLocation(shader, "color");
    rec.shadowRadiusLoc = GetShaderLocation(shader, "shadowRadius");
    rec.shadowOffsetLoc = GetShaderLocation(shader, "shadowOffset");
    rec.shadowScaleLoc = GetShaderLocation(shader, "shadowScale");
    rec.shadowColorLoc = GetShaderLocation(shader, "shadowColor");
    rec.borderThicknessLoc = GetShaderLocation(shader, "borderThickness");
    rec.borderColorLoc = GetShaderLocation(shader, "borderColor");

    UpdateRoundedRectangle(rec, shader);

    return rec;
}

// Update rounded rectangle uniforms
void UpdateRoundedRectangle(RoundedRectangle rec, Shader shader)
{
    SetShaderValue(shader, rec.radiusLoc, (float[]){ rec.cornerRadius.x, rec.cornerRadius.y, rec.cornerRadius.z, rec.cornerRadius.w }, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, rec.shadowRadiusLoc, &rec.shadowRadius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, rec.shadowOffsetLoc, (float[]){ rec.shadowOffset.x, rec.shadowOffset.y }, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader, rec.shadowScaleLoc, &rec.shadowScale, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, rec.borderThicknessLoc, &rec.borderThickness, SHADER_UNIFORM_FLOAT);
}

#endif

#ifdef __cplusplus
}
#endif

#endif
