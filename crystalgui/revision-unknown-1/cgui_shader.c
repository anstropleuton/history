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
#include <string.h>

#include "cgui_config.h"
#include "crystalgui.h"
#include "raylib.h"
#include "raymath.h"

// Get blur vertex shader source code for compilation
const char *CguiGetBlurVertexShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        in vec3 vertexNormal;
        in vec4 vertexColor;

        // Input uniform values
        uniform mat4 mvp;

        // Output vertex attributes (to fragment shader)
        out vec2 fragTexCoord;
        out vec4 fragColor;

        void main() {
            // Send vertex attributes to fragment shader
            fragTexCoord = vertexTexCoord;
            fragColor = vertexColor;

            // Calculate final vertex position
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        });
}

// Get blur fragment shader source code for compilation
const char *CguiGetBlurFragmentShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes (from vertex shader)
        in vec2 fragTexCoord;
        in vec4 fragColor;

        // Input uniform values
        uniform sampler2D texture0;
        uniform vec4 colDiffuse;

        // Output fragment color
        out vec4 finalColor;

        // Blur parameters
        uniform float blurRadius;
        uniform int kernelSize;

        void main() {
            vec2 texelSize = vec2(textureSize(texture0, 0));

            float sum = 0.0;
            vec4 blurColor = vec4(0.0);

            // Blur pass (2D)
            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++)
            {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++)
                {
                    float weight = exp(-(float(i * i + j * j)) / (2.0 * blurRadius * blurRadius));
                    blurColor += texture(texture0, fragTexCoord + vec2(float(i), float(j)) / texelSize) * weight;
                    sum += weight;
                }
            }

            blurColor /= sum;
            finalColor = blurColor * colDiffuse;
        });
}

// Load blur shader
CguiBlurShader *CguiLoadBlurShader(void)
{
    return CguiLoadBlurShaderEx(CguiGetBlurVertexShaderSource(), CguiGetBlurFragmentShaderSource());
}

// Load blur shader with shader source code
CguiBlurShader *CguiLoadBlurShaderEx(const char *vsCode, const char *fsCode)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("BLUR SHADER", vsCode, "Failed to load Blur Shader due to invalid Vertex Shader Code", , NULL);
    CGUI_CHECK_VALID_LOG("BLUR SHADER", fsCode, "Failed to load Blur Shader due to invalid Fragment Shader Code", , NULL);

    CguiBlurShader *shader = MemAlloc(sizeof(CguiBlurShader));
    CGUI_CHECK_VALID_LOG("BLUR SHADER", shader, "Failed to allocate memory for Blur Shader", , NULL);

    shader->shader = LoadShaderFromMemory(vsCode, fsCode);
    CGUI_CHECK_VALID_LOG("BLUR SHADER", shader->shader.id, "Failed to load Blur Shader", MemFree(shader), NULL);

    // Get all uniform locations
    shader->locs[CGUI_BLUR_SHADER_LOC_BLUR_RADIUS] = GetShaderLocation(shader->shader, "blurRadius");
    shader->locs[CGUI_BLUR_SHADER_LOC_KERNEL_SIZE] = GetShaderLocation(shader->shader, "kernelSize");

    CguiSetBlurShaderValues(shader);
    shader->id = ++idCounter;
    CGUI_LOG_ID("BLUR SHADER", shader->id, "Successfully loaded Blur Shader");
    return shader;
}

// Unload blur shader
void CguiUnloadBlurShader(CguiBlurShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    unsigned int id = shader->id;
    UnloadShader(shader->shader);
    MemFree(shader);
    CGUI_LOG_ID("BLUR SHADER", id, "Successfully unloaded Blur Shader");
}

// Set blur shader uniform values
void CguiSetBlurShaderValues(CguiBlurShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    SetShaderValue(shader->shader, shader->locs[CGUI_BLUR_SHADER_LOC_BLUR_RADIUS], &shader->values.blurRadius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader->shader, shader->locs[CGUI_BLUR_SHADER_LOC_KERNEL_SIZE], &shader->values.kernelSize, SHADER_UNIFORM_INT);
}

// Generate a new blurred texture (WARNING: Changes texture wrap)
CguiBlurTexture CguiGenerateBlurTexture(CguiBlurShader *shader, Texture texture)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("BLUR SHADER", shader, "Failed to generate Blur Texture due to invalid Blur Shader", , (CguiBlurTexture) { 0 });
    CGUI_CHECK_VALID_LOG("BLUR SHADER", texture.id, "Failed to generate Blur Texture due to invalid Regular Texture", , (CguiBlurTexture) { 0 });

    RenderTexture renderTexture = LoadRenderTexture(texture.width, texture.height);
    CGUI_CHECK_VALID_LOG("BLUR SHADER", renderTexture.id, "Failed to load Blur Render Texture", , (CguiBlurTexture) { 0 });

    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);

    // Draw texture from shader so that it will be blurred
    BeginTextureMode(renderTexture);
    ClearBackground(BLANK);
    BeginShaderMode(shader->shader);
    DrawTexture(texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();

    CguiBlurTexture blurTexture = { 0 };
    blurTexture.blurredTexture = LoadTextureFromRenderTexture(renderTexture);
    blurTexture.originalTexture = texture;
    blurTexture.id = ++idCounter;
    UnloadRenderTexture(renderTexture);

    CGUI_CHECK_VALID_LOG("BLUR SHADER", blurTexture.id, "Failed to generate Blur Texture", , (CguiBlurTexture) { 0 });
    CGUI_LOG_ID("BLUR SHADER", blurTexture.id, "Successfully generated Blur Texture");
    return blurTexture;
}

// Apply uniforms and generate a new blurred texture (WARNING: Changes texture wrap)
CguiBlurTexture CguiGenerateBlurTextureEx(CguiBlurShader *shader, Texture texture, float blurRadius, int kernelSize)
{
    CGUI_CHECK_VALID_LOG("BLUR SHADER", shader, "Failed to generate Blur Texture due to invalid Blur Shader", , (CguiBlurTexture) { 0 });

    shader->values.blurRadius = blurRadius;
    shader->values.kernelSize = kernelSize;
    CguiSetBlurShaderValues(shader);

    CguiBlurTexture blurTexture = CguiGenerateBlurTexture(shader, texture);
    blurTexture.blurRadius = blurRadius;
    blurTexture.kernelSize = kernelSize;
    return blurTexture;
}

// Dispose a generated blur texture
void CguiDisposeBlurTexture(CguiBlurTexture texture)
{
    CGUI_CHECK_VALID(texture.id, , );
    unsigned int id = texture.id;
    UnloadTexture(texture.blurredTexture);
    CGUI_LOG_ID("BLUR SHADER", id, "Successfully disposed Blur Texture");
}

// Get rectangle vertex shader source code for compilation
const char *CguiGetRectangleVertexShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        in vec3 vertexNormal;
        in vec4 vertexColor;

        // Input uniform values
        uniform mat4 mvp;

        // Output vertex attributes (to fragment shader)
        out vec2 fragTexCoord;
        out vec3 fragPosition;
        out vec4 fragColor;

        void main() {
            // Send vertex attributes to fragment shader
            fragTexCoord = vertexTexCoord;
            fragPosition = vertexPosition;
            fragColor = vertexColor;

            // Calculate final vertex position
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        });
}

// Get rectangle fragment shader source code for compilation
const char *CguiGetRectangleFragmentShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes (from vertex shader)
        in vec2 fragTexCoord;
        in vec3 fragPosition;
        in vec4 fragColor;

        // Input uniform values
        uniform sampler2D texture0;
        uniform vec4 colDiffuse;

        // Output fragment color
        out vec4 finalColor;

        // Parameters for rounded box shape
        uniform vec4 rectangle; // Rectangle parameters: x, y, width, height
        uniform vec4 tint;
        uniform float roundness;
        uniform float shadowRadius;
        uniform float shadowSize;
        uniform vec2 shadowOffset;
        uniform vec4 shadowColor;
        uniform float borderThickness;
        uniform vec4 borderColor;

        // The documentation of how this function works is provided by this link
        // https://iquilezles.org/articles/distfunctions
        // The function was taken from
        // https://www.shadertoy.com/view/WtdSDs
        float roundedBoxSDF(vec4 rectangle, vec2 point, float radius) {
            vec2 halfSize = rectangle.zw / 2.0;
            vec2 center = rectangle.xy + halfSize;
            vec2 p = abs(point - center) - halfSize + radius;
            return length(max(p, 0.0)) - radius;
        }

        void main() {
            vec4 texelColor = texture2D(texture0, fragTexCoord);

            // Calculate alpha based on the distance to the rounded box shape
            float dst = roundedBoxSDF(rectangle, fragPosition.xy, roundness);
            float alpha = 1.0 - clamp(dst, 0.0, 1.0);

            // Calculate alpha for the shadow based on the distance to the rounded box shape with an offset
            float sDst = roundedBoxSDF(vec4(rectangle.xy - shadowSize, rectangle.zw + shadowSize * 2.0), fragPosition.xy - shadowOffset, roundness);
            float sAlpha = clamp(1.0 - smoothstep(-shadowRadius, shadowRadius, sDst), 0.0, 1.0);

            // Calculate alpha for the border based on the distance to the rounded box shape
            float bAlpha = 1.0 - clamp(abs(dst) - borderThickness + 1.0, 0.0, 1.0);

            // Calculate final colors for texture, shadow, and border
            vec4 finalTexelColor = tint * vec4(texelColor.rgb, texelColor.a * alpha);
            vec4 finalShadowColor = vec4(shadowColor.rgb, shadowColor.a * sAlpha);
            vec4 finalBorderColor = borderColor;

            // Mix the final colors based on alpha values and diffuse color
            finalColor = colDiffuse * mix(mix(finalTexelColor, finalShadowColor, 1.0 - alpha), finalBorderColor, bAlpha * finalBorderColor.a);
        });
}

// Load rectangle shader
CguiRectangleShader *CguiLoadRectangleShader(void)
{
    return CguiLoadRectangleShaderEx(CguiGetRectangleVertexShaderSource(), CguiGetRectangleFragmentShaderSource());
}

// Load rectangle shader with shader source code
CguiRectangleShader *CguiLoadRectangleShaderEx(const char *vsCode, const char *fsCode)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("RECTANGLE SHADER", vsCode, "Failed to load Rectangle Shader due to invalid Vertex Shader Code", , NULL);
    CGUI_CHECK_VALID_LOG("RECTANGLE SHADER", fsCode, "Failed to load Rectangle Shader due to invalid Fragment Shader Code", , NULL);

    CguiRectangleShader *shader = MemAlloc(sizeof(CguiRectangleShader));
    CGUI_CHECK_VALID_LOG("RECTANGLE SHADER", shader, "Failed to allocate memory for Shader", , NULL);
    shader->shader = LoadShaderFromMemory(vsCode, fsCode);
    CGUI_CHECK_VALID_LOG("RECTANGLE SHADER", shader->shader.id, "Failed to load Rectangle Shader", MemFree(shader), NULL);

    // Get all uniform locations
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_RECTANGLE] = GetShaderLocation(shader->shader, "rectangle");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_TINT] = GetShaderLocation(shader->shader, "tint");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_ROUNDNESS] = GetShaderLocation(shader->shader, "roundness");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_RADIUS] = GetShaderLocation(shader->shader, "shadowRadius");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_SIZE] = GetShaderLocation(shader->shader, "shadowSize");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_OFFSET] = GetShaderLocation(shader->shader, "shadowOffset");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_COLOR] = GetShaderLocation(shader->shader, "shadowColor");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_BORDER_THICKNESS] = GetShaderLocation(shader->shader, "borderThickness");
    shader->locs[CGUI_RECTANGLE_SHADER_LOC_BORDER_COLOR] = GetShaderLocation(shader->shader, "borderColor");

    CguiSetRectangleShaderValues(shader);
    shader->id = ++idCounter;
    CGUI_LOG_ID("RECTANGLE SHADER", shader->id, "Successfully loaded Rectangle Shader");
    return shader;
}

// Unload rectangle shader
void CguiUnloadRectangleShader(CguiRectangleShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    unsigned int id = shader->id;
    UnloadShader(shader->shader);
    MemFree(shader);
    CGUI_LOG_ID("RECTANGLE SHADER", id, "Successfully unloaded Rectangle Shader");
}

// Set rectangle shader uniform values
void CguiSetRectangleShaderValues(CguiRectangleShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_RECTANGLE], shader->values.rectangle, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_TINT], shader->values.tint, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_ROUNDNESS], &shader->values.roundness, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_RADIUS], &shader->values.shadowRadius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_SIZE], &shader->values.shadowSize, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_OFFSET], shader->values.shadowOffset, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_SHADOW_COLOR], shader->values.shadowColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_BORDER_THICKNESS], &shader->values.borderThickness, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader->shader, shader->locs[CGUI_RECTANGLE_SHADER_LOC_BORDER_COLOR], shader->values.borderColor, SHADER_UNIFORM_VEC4);
}

// Draw texture-filled rounded rectangle using shader (use empty texture to fill with tint)
void CguiShaderDrawRectangle(CguiRectangleShader *shader, Texture texture, Color tint)
{
    CGUI_CHECK_VALID(shader, , );
    shader->values.tint[0] = tint.r / 255.0f;
    shader->values.tint[1] = tint.g / 255.0f;
    shader->values.tint[2] = tint.b / 255.0f;
    shader->values.tint[3] = tint.a / 255.0f;
    CguiSetRectangleShaderValues(shader);

    BeginShaderMode(shader->shader);
    if (texture.id == 0) DrawRectangleRec(GetAppResolutionRec(), WHITE);
    else DrawTexturePro(texture, GetTextureSizeRec(texture), GetAppResolutionRec(), Vector2Zero(), 0.0f, WHITE);
    EndShaderMode();
}

// Apply uniforms and draw texture-filled rounded rectangle using shader (use empty texture to fill with tint)
void CguiShaderDrawRectangleRec(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.rectangle[0] = rec.x;
    shader->values.rectangle[1] = rec.y;
    shader->values.rectangle[2] = rec.width;
    shader->values.rectangle[3] = rec.height;
    shader->values.roundness = roundness;
    CguiSetRectangleShaderValues(shader);

    CguiShaderDrawRectangle(shader, texture, tint);
}

// Apply uniforms and draw texture-filled rounded rectangle using shader with extended parameters (use empty texture to fill with tint)
void CguiShaderDrawRectangleEx(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness, Color shadowColor, float borderThickness, Color borderColor)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.shadowColor[0] = shadowColor.r / 255.0f;
    shader->values.shadowColor[1] = shadowColor.g / 255.0f;
    shader->values.shadowColor[2] = shadowColor.b / 255.0f;
    shader->values.shadowColor[3] = shadowColor.a / 255.0f;
    shader->values.borderThickness = borderThickness;
    shader->values.borderColor[0] = borderColor.r / 255.0f;
    shader->values.borderColor[1] = borderColor.g / 255.0f;
    shader->values.borderColor[2] = borderColor.b / 255.0f;
    shader->values.borderColor[3] = borderColor.a / 255.0f;
    CguiSetRectangleShaderValues(shader);

    CguiShaderDrawRectangleRec(shader, texture, tint, rec, roundness);
}

// Apply uniforms and draw texture-filled rounded rectangle using shader with 'pro' parameters (use empty texture to fill with tint)
void CguiShaderDrawRectanglePro(CguiRectangleShader *shader, Texture texture, Color tint, Rectangle rec, float roundness, float shadowRadius, float shadowSize, Vector2 shadowOffset, Color shadowColor, float borderThickness, Color borderColor)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.shadowRadius = shadowRadius;
    shader->values.shadowSize = shadowSize;
    shader->values.shadowOffset[0] = shadowOffset.x;
    shader->values.shadowOffset[1] = shadowOffset.y;
    CguiSetRectangleShaderValues(shader);

    CguiShaderDrawRectangleEx(shader, texture, tint, rec, roundness, shadowColor, borderThickness, borderColor);
}

// Get gradient vertex shader source code for compilation
const char *CguiGetGradientVertexShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        in vec3 vertexNormal;
        in vec4 vertexColor;

        // Input uniform values
        uniform mat4 mvp;

        // Output vertex attributes (to fragment shader)
        out vec2 fragTexCoord;
        out vec3 fragPosition;
        out vec4 fragColor;

        void main() {
            // Send vertex attributes to fragment shader
            fragTexCoord = vertexTexCoord;
            fragPosition = vertexPosition;
            fragColor = vertexColor;

            // Calculate final vertex position
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        });
}

// Get gradient fragment shader source code for compilation
const char *CguiGetGradientFragmentShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes (from vertex shader)
        in vec2 fragTexCoord;
        in vec3 fragPosition;
        in vec4 fragColor;

        // Input uniform values
        uniform sampler2D texture0;
        uniform vec4 colDiffuse;

        // Output fragment color
        out vec4 finalColor;

        // Gradient color and position
        uniform vec4 color1;
        uniform vec4 color2;
        uniform vec2 pos1;
        uniform vec2 pos2;

        void main() {
            // Texel color fetching from texture sampler
            vec4 texelColor = texture(texture0, fragTexCoord);

            vec2 st = (fragPosition.xy - pos1) / pos2;
            float r = clamp((st.x + st.y) / 2.0, 0.0, 1.0);

            // Blend between two colors based on the average position
            vec4 gradient = mix(color1, color2, r);

            finalColor = texelColor * gradient * colDiffuse;
        });
}

// Load gradient shader
CguiGradientShader *CguiLoadGradientShader(void)
{
    return CguiLoadGradientShaderEx(CguiGetGradientVertexShaderSource(), CguiGetGradientFragmentShaderSource());
}

// Load gradient shader with shader source code
CguiGradientShader *CguiLoadGradientShaderEx(const char *vsCode, const char *fsCode)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("GRADIENT SHADER", vsCode, "Failed to load Gradient Shader due to invalid Vertex Shader Code", , NULL);
    CGUI_CHECK_VALID_LOG("GRADIENT SHADER", fsCode, "Failed to load Gradient Shader due to invalid Fragment Shader Code", , NULL);

    CguiGradientShader *shader = MemAlloc(sizeof(CguiGradientShader));
    CGUI_CHECK_VALID_LOG("GRADIENT SHADER", shader, "Failed to allocate memory for Shader", , NULL);

    shader->shader = LoadShaderFromMemory(vsCode, fsCode);
    CGUI_CHECK_VALID_LOG("GRADIENT SHADER", shader->shader.id, "Failed to load Gradient Shader", MemFree(shader), NULL);

    // Get all uniform locations
    shader->locs[CGUI_GRADIENT_SHADER_LOC_COLOR_1] = GetShaderLocation(shader->shader, "color1");
    shader->locs[CGUI_GRADIENT_SHADER_LOC_COLOR_2] = GetShaderLocation(shader->shader, "color2");
    shader->locs[CGUI_GRADIENT_SHADER_LOC_POS_1] = GetShaderLocation(shader->shader, "pos1");
    shader->locs[CGUI_GRADIENT_SHADER_LOC_POS_2] = GetShaderLocation(shader->shader, "pos2");

    CguiSetGradientShaderValues(shader);
    shader->id = ++idCounter;
    CGUI_LOG_ID("GRADIENT SHADER", shader->id, "Successfully loaded Gradient Shader");
    return shader;
}

// Unload gradient shader
void CguiUnloadGradientShader(CguiGradientShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    unsigned int id = shader->id;
    UnloadShader(shader->shader);
    MemFree(shader);
    CGUI_LOG_ID("GRADIENT SHADER", id, "Successfully unloaded Gradient Shader");
}

// Set gradient shader uniform values
void CguiSetGradientShaderValues(CguiGradientShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    SetShaderValue(shader->shader, shader->locs[CGUI_GRADIENT_SHADER_LOC_COLOR_1], shader->values.color1, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_GRADIENT_SHADER_LOC_COLOR_2], shader->values.color2, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_GRADIENT_SHADER_LOC_POS_1], shader->values.pos1, SHADER_UNIFORM_VEC2);
    SetShaderValue(shader->shader, shader->locs[CGUI_GRADIENT_SHADER_LOC_POS_2], shader->values.pos2, SHADER_UNIFORM_VEC2);
}

// Begin gradient mode (WARNING: Uses BeginShaderMode() so you can't use shader in this mode)
void CguiBeginGradientMode(CguiGradientShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    BeginShaderMode(shader->shader);
}

// Apply uniforms and begin gradient mode with extended parameters (WARNING: Uses BeginShaderMode() so you can't use shader in this mode)
void CguiBeginGradientModeEx(CguiGradientShader *shader, Color color1, Color color2, Vector2 pos1, Vector2 pos2)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.color1[0] = color1.r / 255.0f;
    shader->values.color1[1] = color1.g / 255.0f;
    shader->values.color1[2] = color1.b / 255.0f;
    shader->values.color1[3] = color1.a / 255.0f;
    shader->values.color2[0] = color2.r / 255.0f;
    shader->values.color2[1] = color2.g / 255.0f;
    shader->values.color2[2] = color2.b / 255.0f;
    shader->values.color2[3] = color2.a / 255.0f;
    shader->values.pos1[0] = pos1.x;
    shader->values.pos1[1] = pos1.y;
    shader->values.pos2[0] = pos2.x;
    shader->values.pos2[1] = pos2.y;
    CguiSetGradientShaderValues(shader);

    CguiBeginGradientMode(shader);
}

// End gradient mode (WARNING: Uses EndShaderMode())
void CguiEndGradientMode(CguiGradientShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    EndShaderMode();
}

// Get combination vertex shader source code for compilation
const char *CguiGetCombinationVertexShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes
        in vec3 vertexPosition;
        in vec2 vertexTexCoord;
        in vec3 vertexNormal;
        in vec4 vertexColor;

        // Input uniform values
        uniform mat4 mvp;

        // Output vertex attributes (to fragment shader)
        out vec2 fragTexCoord;
        out vec3 fragPosition;
        out vec4 fragColor;

        void main() {
            // Send vertex attributes to fragment shader
            fragTexCoord = vertexTexCoord;
            fragPosition = vertexPosition;
            fragColor = vertexColor;

            // Calculate final vertex position
            gl_Position = mvp * vec4(vertexPosition, 1.0);
        });
}

// Get combination fragment shader source code for compilation
const char *CguiGetCombinationFragmentShaderSource(void)
{
    return "#version 330\n" CGUI_RAW_STRING(
        // Input vertex attributes (from vertex shader)
        in vec2 fragTexCoord;
        in vec3 fragPosition;
        in vec4 fragColor;

        // Input uniform values
        uniform sampler2D texture0;
        uniform vec4 colDiffuse;

        // Output fragment color
        out vec4 finalColor;

        // Textures to combine
        uniform sampler2D texture1;
        uniform vec4 src1;
        uniform vec4 dest1;
        uniform vec4 tint1;

        uniform sampler2D texture2;
        uniform vec4 src2;
        uniform vec4 dest2;
        uniform vec4 tint2;

        // Operation to perform on textures
        uniform int operation;
        uniform float ratio;

        void main() {
            // Texel color fetching from texture sampler
            vec4 texelColor = vec4(0.0);

            vec4 texelColor1 = vec4(0.0);
            vec2 texelSize1 = vec2(textureSize(texture1, 0));
            vec2 texCoord1 = (fragPosition.xy - dest1.xy) / dest1.zw * src1.zw + src1.xy / texelSize1;
            if (texCoord1.x > 0.0 && texCoord1.y > 0.0 && texCoord1.x < 1.0 && texCoord1.y < 1.0) texelColor1 = texture(texture1, texCoord1);

            vec4 texelColor2 = vec4(0.0);
            vec2 texelSize2 = vec2(textureSize(texture2, 0));
            vec2 texCoord2 = (fragPosition.xy - dest2.xy) / dest2.zw * src2.zw + src2.xy / texelSize2;
            if (texCoord2.x > 0.0 && texCoord2.y > 0.0 && texCoord2.x < 1.0 && texCoord2.y < 1.0) texelColor2 = texture(texture2, texCoord2);

            // 0 = multiply, 1 = addition, 2 = difference, 3 = min, 4 = max, 5 = burn, 6 = dodge, 7 = exclusion
            if (operation == 0) texelColor = texelColor1 * texelColor2;
            else if (operation == 1) texelColor = texelColor1 * (1.0 - ratio) + texelColor2 * ratio;
            else if (operation == 2) texelColor = abs(texelColor1 - texelColor2);
            else if (operation == 3) texelColor = min(texelColor1, texelColor2);
            else if (operation == 4) texelColor = max(texelColor1, texelColor2);
            else if (operation == 5) texelColor = 1.0 - (1.0 - texelColor1) / clamp(texelColor2, 0.001, 1.0);
            else if (operation == 6) texelColor = 1.0 - texelColor1 / clamp((1.0 - texelColor2), 0.001, 1.0);
            else if (operation == 7) texelColor = texelColor1 + texelColor2 - ratio * texelColor1 * texelColor2;

            finalColor = texelColor * colDiffuse;
        });
}

// Load combination shader
CguiCombinationShader *CguiLoadCombinationShader(void)
{
    return CguiLoadCombinationShaderEx(CguiGetCombinationVertexShaderSource(), CguiGetCombinationFragmentShaderSource());
}

// Load combination shader with shader source code
CguiCombinationShader *CguiLoadCombinationShaderEx(const char *vsCode, const char *fsCode)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", vsCode, "Failed to load Combination Shader due to invalid Vertex Shader Code", , NULL);
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", fsCode, "Failed to load Combination Shader due to invalid Fragment Shader Code", , NULL);

    CguiCombinationShader *shader = MemAlloc(sizeof(CguiCombinationShader));
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", shader, "Failed to allocate memory for Shader", , NULL);

    shader->shader = LoadShaderFromMemory(vsCode, fsCode);
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", shader->shader.id, "Failed to load Combination Shader", MemFree(shader), NULL);

    // Get all uniform locations
    shader->locs[CGUI_COMBINATION_SHADER_LOC_TEXTURE_1] = GetShaderLocation(shader->shader, "texture1");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_TEXTURE_2] = GetShaderLocation(shader->shader, "texture2");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_SRC_1] = GetShaderLocation(shader->shader, "src1");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_DEST_1] = GetShaderLocation(shader->shader, "dest1");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_TINT_1] = GetShaderLocation(shader->shader, "tint1");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_SRC_2] = GetShaderLocation(shader->shader, "src2");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_DEST_2] = GetShaderLocation(shader->shader, "dest2");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_TINT_2] = GetShaderLocation(shader->shader, "tint2");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_OPERATION] = GetShaderLocation(shader->shader, "operation");
    shader->locs[CGUI_COMBINATION_SHADER_LOC_RATIO] = GetShaderLocation(shader->shader, "ratio");

    CguiSetCombinationShaderValues(shader);
    shader->id = ++idCounter;
    CGUI_LOG_ID("COMBINATION SHADER", shader->id, "Successfully loaded Combination Shader");
    return shader;
}

// Unload combination shader
void CguiUnloadCombinationShader(CguiCombinationShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    unsigned int id = shader->id;
    UnloadShader(shader->shader);
    MemFree(shader);
    CGUI_LOG_ID("COMBINATION SHADER", id, "Successfully unloaded Combination Shader");
}

// Set combination shader uniform values
void CguiSetCombinationShaderValues(CguiCombinationShader *shader)
{
    CGUI_CHECK_VALID(shader, , );
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_SRC_1], shader->values.src1, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_DEST_1], shader->values.dest1, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_TINT_1], shader->values.tint1, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_SRC_2], shader->values.src2, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_DEST_2], shader->values.dest2, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_TINT_2], shader->values.tint2, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_OPERATION], &shader->values.operation, SHADER_UNIFORM_INT);
    SetShaderValue(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_RATIO], &shader->values.ratio, SHADER_UNIFORM_FLOAT);
}

// Combine textures and draw
void CguiDrawTextureCombined(CguiCombinationShader *shader, Texture texture1, Texture texture2, Rectangle dest)
{
    CGUI_CHECK_VALID(shader, , );
    CGUI_CHECK_VALID(texture1.id, , );
    CGUI_CHECK_VALID(texture2.id, , );
    SetShaderValueTexture(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_TEXTURE_1], texture1);
    SetShaderValueTexture(shader->shader, shader->locs[CGUI_COMBINATION_SHADER_LOC_TEXTURE_2], texture2);

    BeginShaderMode(shader->shader);
    DrawRectangleRec(dest, WHITE);
    EndShaderMode();
}

// Combine textures and draw with extended parameters
void CguiDrawTextureCombinedEx(CguiCombinationShader *shader, Texture texture1, Color tint1, Texture texture2, Color tint2, Rectangle dest)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.tint1[0] = tint1.r / 255.0f;
    shader->values.tint1[1] = tint1.g / 255.0f;
    shader->values.tint1[2] = tint1.b / 255.0f;
    shader->values.tint1[3] = tint1.a / 255.0f;
    shader->values.tint2[0] = tint2.r / 255.0f;
    shader->values.tint2[1] = tint2.g / 255.0f;
    shader->values.tint2[2] = tint2.b / 255.0f;
    shader->values.tint2[3] = tint2.a / 255.0f;
    CguiSetCombinationShaderValues(shader);

    CguiDrawTextureCombined(shader, texture1, texture2, dest);
}

// Combine textures by a part of a texture defined by a rectangle and draw
void CguiDrawTextureCombinedRec(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Color tint1, Texture texture2, Rectangle src2, Color tint2, Rectangle dest)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.src1[0] = src1.x;
    shader->values.src1[1] = src1.y;
    shader->values.src1[2] = src1.width;
    shader->values.src1[3] = src1.height;
    shader->values.src2[0] = src2.x;
    shader->values.src2[1] = src2.y;
    shader->values.src2[2] = src2.width;
    shader->values.src2[3] = src2.height;
    CguiSetCombinationShaderValues(shader);

    CguiDrawTextureCombinedEx(shader, texture1, tint1, texture2, tint2, dest);
}

// Combine textures by a part of a texture defined by a rectangle and draw with 'pro' parameters
void CguiDrawTextureCombinedPro(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Rectangle dest1, Color tint1, Texture texture2, Rectangle src2, Rectangle dest2, Color tint2, Rectangle dest)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , );
    shader->values.dest1[0] = dest1.x;
    shader->values.dest1[1] = dest1.y;
    shader->values.dest1[2] = dest1.width;
    shader->values.dest1[3] = dest1.height;
    shader->values.dest2[0] = dest2.x;
    shader->values.dest2[1] = dest2.y;
    shader->values.dest2[2] = dest2.width;
    shader->values.dest2[3] = dest2.height;
    CguiSetCombinationShaderValues(shader);

    CguiDrawTextureCombinedRec(shader, texture1, src1, tint1, texture2, src2, tint2, dest);
}

// Load a new combined texture
CguiCombinedTexture CguiGenerateCombinedTexture(CguiCombinationShader *shader, Texture texture1, Texture texture2, Vector2 textureSize)
{
    static unsigned int idCounter = 0;
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", shader, "Failed to generate Combined Texture due to invalid Shader", , (CguiCombinedTexture) { 0 });
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", texture1.id, "Failed to generate Combined Texture due to invalid First texture", , (CguiCombinedTexture) { 0 });
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", texture2.id, "Failed to generate Combined Texture due to invalid Second texture", , (CguiCombinedTexture) { 0 });
    Rectangle textureSizeRec = { 0.0f, 0.0f, textureSize.x, textureSize.y };

    RenderTexture combinedRenderTexture = LoadRenderTexture(textureSize.x, textureSize.y);
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", combinedRenderTexture.id, "Failed to generate Combined Texture", , (CguiCombinedTexture) { 0 });

    BeginTextureMode(combinedRenderTexture);
    ClearBackground(BLANK);
    CguiDrawTextureCombined(shader, texture1, texture2, textureSizeRec);
    EndTextureMode();

    CguiCombinedTexture combinedTexture = { 0 };
    combinedTexture.texture = LoadTextureFromRenderTexture(combinedRenderTexture);
    UnloadRenderTexture(combinedRenderTexture);

    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", combinedTexture.texture.id, "Failed to generate Combined Texture", , (CguiCombinedTexture) { 0 });
    combinedTexture.id = ++idCounter;
    CGUI_LOG_ID("COMBINATION SHADER", combinedTexture.id, "Successfully generated Combined Texture");
    return combinedTexture;
}

// Load a new combined texture with extended parameters
CguiCombinedTexture CguiGenerateCombinedTextureEx(CguiCombinationShader *shader, Texture texture1, Color tint1, Texture texture2, Color tint2, Vector2 textureSize)
{
    // Apply new uniforms
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", shader, "Failed to generate Combined Texture due to invalid Shader", , (CguiCombinedTexture) { 0 });
    shader->values.tint1[0] = tint1.r / 255.0f;
    shader->values.tint1[1] = tint1.g / 255.0f;
    shader->values.tint1[2] = tint1.b / 255.0f;
    shader->values.tint1[3] = tint1.a / 255.0f;
    shader->values.tint2[0] = tint2.r / 255.0f;
    shader->values.tint2[1] = tint2.g / 255.0f;
    shader->values.tint2[2] = tint2.b / 255.0f;
    shader->values.tint2[3] = tint2.a / 255.0f;
    CguiSetCombinationShaderValues(shader);

    CguiCombinedTexture texture = CguiGenerateCombinedTexture(shader, texture1, texture2, textureSize);
    return texture;
}

// Load a new combined texture by a part of a texture defined by a rectangle
CguiCombinedTexture CguiGenerateCombinedTextureRec(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Color tint1, Texture texture2, Rectangle src2, Color tint2, Vector2 textureSize)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , (CguiCombinedTexture) { 0 });
    shader->values.src1[0] = src1.x;
    shader->values.src1[1] = src1.y;
    shader->values.src1[2] = src1.width;
    shader->values.src1[3] = src1.height;
    shader->values.src2[0] = src2.x;
    shader->values.src2[1] = src2.y;
    shader->values.src2[2] = src2.width;
    shader->values.src2[3] = src2.height;
    CguiSetCombinationShaderValues(shader);

    CguiCombinedTexture texture = CguiGenerateCombinedTextureEx(shader, texture1, tint1, texture2, tint2, textureSize);
    return texture;
}

// Load a new combined texture by a part of a texture defined by a rectangle with 'pro' parameters
CguiCombinedTexture CguiGenerateCombinedTexturePro(CguiCombinationShader *shader, Texture texture1, Rectangle src1, Rectangle dest1, Color tint1, Texture texture2, Rectangle src2, Rectangle dest2, Color tint2, Vector2 textureSize)
{
    // Apply new uniforms
    CGUI_CHECK_VALID(shader, , (CguiCombinedTexture) { 0 });
    shader->values.dest1[0] = dest1.x;
    shader->values.dest1[1] = dest1.y;
    shader->values.dest1[2] = dest1.width;
    shader->values.dest1[3] = dest1.height;
    shader->values.dest2[0] = dest2.x;
    shader->values.dest2[1] = dest2.y;
    shader->values.dest2[2] = dest2.width;
    shader->values.dest2[3] = dest2.height;
    CguiSetCombinationShaderValues(shader);

    CguiCombinedTexture texture = CguiGenerateCombinedTextureRec(shader, texture1, src1, tint1, texture2, src2, tint2, textureSize);
    return texture;
}

// Unload a combined texture
void CguiDisposeCombinedTexture(CguiCombinedTexture texture)
{
    CGUI_CHECK_VALID_LOG("COMBINATION SHADER", texture.id, "Failed to dispose Combined Texture", , );
    unsigned int id = texture.id;
    UnloadTexture(texture.texture);
    CGUI_LOG_ID("COMBINATION SHADER", id, "Successfully disposed Combined Texture");
}
