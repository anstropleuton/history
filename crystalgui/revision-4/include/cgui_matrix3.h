// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// 3x3 matrix utilities for 2D transformations
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_MATRIX3_H_
#define CGUI_MATRIX3_H_

#include "cgui_api.h"
#include "raylib.h"
#include <math.h>

// Defines a 3x3 transformation matrix for 2D homogeneous transformations; column-major storage (OpenGL-style); row-major indexing operations
typedef struct CguiMatrix3 {
    float m0, m3, m6;
    float m1, m4, m7;
    float m2, m5, m8;
} CguiMatrix3;

// Defines an arbitrary quadrilateral shape.
typedef struct CguiQuad {
    // Vertices:
    // 0 -> top-left
    // 1 -> top-right
    // 2 -> bottom-left
    // 3 -> bottom-right
    Vector2 vertices[4];
} CguiQuad;

// Returns the identity matrix
CGSAPI CguiMatrix3 CguiMatrix3Identity(void) {
    CguiMatrix3 r = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    return r;
}

// Returns the element-wise sum of two matrices
CGSAPI CguiMatrix3 CguiMatrix3AddE(CguiMatrix3 matA, CguiMatrix3 matB) {
    CguiMatrix3 r;
    r.m0 = matA.m0 + matB.m0;
    r.m1 = matA.m1 + matB.m1;
    r.m2 = matA.m2 + matB.m2;
    r.m3 = matA.m3 + matB.m3;
    r.m4 = matA.m4 + matB.m4;
    r.m5 = matA.m5 + matB.m5;
    r.m6 = matA.m6 + matB.m6;
    r.m7 = matA.m7 + matB.m7;
    r.m8 = matA.m8 + matB.m8;
    return r;
}

// Returns the matrix product of two matrices
CGSAPI CguiMatrix3 CguiMatrix3Multiply(CguiMatrix3 matA, CguiMatrix3 matB) {
    CguiMatrix3 r;
    r.m0 = matA.m0 * matB.m0 + matA.m1 * matB.m3 + matA.m2 * matB.m6;
    r.m1 = matA.m0 * matB.m1 + matA.m1 * matB.m4 + matA.m2 * matB.m7;
    r.m2 = matA.m0 * matB.m2 + matA.m1 * matB.m5 + matA.m2 * matB.m8;

    r.m3 = matA.m3 * matB.m0 + matA.m4 * matB.m3 + matA.m5 * matB.m6;
    r.m4 = matA.m3 * matB.m1 + matA.m4 * matB.m4 + matA.m5 * matB.m7;
    r.m5 = matA.m3 * matB.m2 + matA.m4 * matB.m5 + matA.m5 * matB.m8;

    r.m6 = matA.m6 * matB.m0 + matA.m7 * matB.m3 + matA.m8 * matB.m6;
    r.m7 = matA.m6 * matB.m1 + matA.m7 * matB.m4 + matA.m8 * matB.m7;
    r.m8 = matA.m6 * matB.m2 + matA.m7 * matB.m5 + matA.m8 * matB.m8;
    return r;
}

// Returns the element-wise product of two matrices
CGSAPI CguiMatrix3 CguiMatrix3MultiplyE(CguiMatrix3 matA, CguiMatrix3 matB) {
    CguiMatrix3 r;
    r.m0 = matA.m0 * matB.m0;
    r.m1 = matA.m1 * matB.m1;
    r.m2 = matA.m2 * matB.m2;
    r.m3 = matA.m3 * matB.m3;
    r.m4 = matA.m4 * matB.m4;
    r.m5 = matA.m5 * matB.m5;
    r.m6 = matA.m6 * matB.m6;
    r.m7 = matA.m7 * matB.m7;
    r.m8 = matA.m8 * matB.m8;
    return r;
}

// Returns the product of a matrix and a scalar
CGSAPI CguiMatrix3 CguiMatrix3MultiplyF(CguiMatrix3 mat, float scalar) {
    CguiMatrix3 r;
    r.m0 = mat.m0 * scalar;
    r.m1 = mat.m1 * scalar;
    r.m2 = mat.m2 * scalar;
    r.m3 = mat.m3 * scalar;
    r.m4 = mat.m4 * scalar;
    r.m5 = mat.m5 * scalar;
    r.m6 = mat.m6 * scalar;
    r.m7 = mat.m7 * scalar;
    r.m8 = mat.m8 * scalar;
    return r;
}

// Returns the product of a matrix and a vector (x, y, 1)
CGSAPI Vector2 CguiMatrix3MultiplyV(CguiMatrix3 mat, float x, float y) {
    float   rx = mat.m0 * x + mat.m1 * y + mat.m2 * 1.0f;
    float   ry = mat.m3 * x + mat.m4 * y + mat.m5 * 1.0f;
    float   rw = mat.m6 * x + mat.m7 * y + mat.m8 * 1.0f;
    Vector2 out;
    out.x = rx / rw;
    out.y = ry / rw;
    return out;
}

// Returns the determinant of a matrix
CGSAPI float CguiMatrix3Det(CguiMatrix3 mat) {
    float det = mat.m0 * (mat.m4 * mat.m8 - mat.m5 * mat.m7) -
                mat.m1 * (mat.m3 * mat.m8 - mat.m5 * mat.m6) +
                mat.m2 * (mat.m3 * mat.m7 - mat.m4 * mat.m6);
    return det;
}

// Returns the inverse of a matrix
// Note: Non-invertibility is not checked
CGSAPI CguiMatrix3 CguiMatrix3Inv(CguiMatrix3 mat) {
    float det = mat.m0 * (mat.m4 * mat.m8 - mat.m5 * mat.m7) -
                mat.m1 * (mat.m3 * mat.m8 - mat.m5 * mat.m6) +
                mat.m2 * (mat.m3 * mat.m7 - mat.m4 * mat.m6);
    float invdet = 1.0f / det;

    CguiMatrix3 r;
    r.m0 = (mat.m4 * mat.m8 - mat.m5 * mat.m7) * invdet;
    r.m1 = -(mat.m1 * mat.m8 - mat.m2 * mat.m7) * invdet;
    r.m2 = (mat.m1 * mat.m5 - mat.m2 * mat.m4) * invdet;

    r.m3 = -(mat.m3 * mat.m8 - mat.m5 * mat.m6) * invdet;
    r.m4 = (mat.m0 * mat.m8 - mat.m2 * mat.m6) * invdet;
    r.m5 = -(mat.m0 * mat.m5 - mat.m2 * mat.m3) * invdet;

    r.m6 = (mat.m3 * mat.m7 - mat.m4 * mat.m6) * invdet;
    r.m7 = -(mat.m0 * mat.m7 - mat.m1 * mat.m6) * invdet;
    r.m8 = (mat.m0 * mat.m4 - mat.m1 * mat.m3) * invdet;
    return r;
}

// Returns the transpose of a matrix
CGSAPI CguiMatrix3 CguiMatrix3Trans(CguiMatrix3 mat) {
    CguiMatrix3 r;
    r.m0 = mat.m0;
    r.m1 = mat.m3;
    r.m2 = mat.m6;
    r.m3 = mat.m1;
    r.m4 = mat.m4;
    r.m5 = mat.m7;
    r.m6 = mat.m2;
    r.m7 = mat.m5;
    r.m8 = mat.m8;
    return r;
}

// Returns the symmetric of a matrix
CGSAPI CguiMatrix3 CguiMatrix3Sym(CguiMatrix3 mat) {
    CguiMatrix3 r;
    r.m0 = mat.m0;
    r.m1 = 0.5f * (mat.m1 + mat.m3);
    r.m2 = 0.5f * (mat.m2 + mat.m6);
    r.m3 = 0.5f * (mat.m3 + mat.m1);
    r.m4 = mat.m4;
    r.m5 = 0.5f * (mat.m5 + mat.m7);
    r.m6 = 0.5f * (mat.m6 + mat.m2);
    r.m7 = 0.5f * (mat.m7 + mat.m5);
    r.m8 = mat.m8;
    return r;
}

// Returns the skew-symmetric of a matrix
CGSAPI CguiMatrix3 CguiMatrix3Skew(CguiMatrix3 mat) {
    CguiMatrix3 r;
    r.m0 = 0.0f;
    r.m1 = 0.5f * (mat.m1 - mat.m3);
    r.m2 = 0.5f * (mat.m2 - mat.m6);
    r.m3 = 0.5f * (mat.m3 - mat.m1);
    r.m4 = 0.0f;
    r.m5 = 0.5f * (mat.m5 - mat.m7);
    r.m6 = 0.5f * (mat.m6 - mat.m2);
    r.m7 = 0.5f * (mat.m7 - mat.m5);
    r.m8 = 0.0f;
    return r;
}

// Returns the product of a matrix and a translation matrix
CGSAPI CguiMatrix3 CguiMatrix3Translate(CguiMatrix3 mat, float tx, float ty) {
    CguiMatrix3 r = mat;
    r.m2          = mat.m0 * tx + mat.m1 * ty + mat.m2;
    r.m5          = mat.m3 * tx + mat.m4 * ty + mat.m5;
    r.m8          = mat.m6 * tx + mat.m7 * ty + mat.m8;
    return r;
}

// Returns the product of a matrix and a rotation matrix
CGSAPI CguiMatrix3 CguiMatrix3Rotate(CguiMatrix3 mat, float angle) {
    float       c = cosf(angle);
    float       s = sinf(angle);
    CguiMatrix3 r;
    r.m0 = mat.m0 * c + mat.m1 * s;
    r.m3 = mat.m3 * c + mat.m4 * s;
    r.m6 = mat.m6 * c + mat.m7 * s;

    r.m1 = mat.m0 * (-s) + mat.m1 * c;
    r.m4 = mat.m3 * (-s) + mat.m4 * c;
    r.m7 = mat.m6 * (-s) + mat.m7 * c;

    r.m2 = mat.m2;
    r.m5 = mat.m5;
    r.m8 = mat.m8;
    return r;
}

// Returns the product of a matrix and a scaling matrix
CGSAPI CguiMatrix3 CguiMatrix3Scale(CguiMatrix3 mat, float sx, float sy) {
    CguiMatrix3 r;
    r.m0 = mat.m0 * sx;
    r.m1 = mat.m1 * sy;
    r.m2 = mat.m2;
    r.m3 = mat.m3 * sx;
    r.m4 = mat.m4 * sy;
    r.m5 = mat.m5;
    r.m6 = mat.m6 * sx;
    r.m7 = mat.m7 * sy;
    r.m8 = mat.m8;
    return r;
}

// Returns the product of a matrix and a shearing matrix
CGSAPI CguiMatrix3 CguiMatrix3Shear(CguiMatrix3 mat, float shx, float shy) {
    CguiMatrix3 r;
    r.m0 = mat.m0 + shy * mat.m1;
    r.m3 = mat.m3 + shy * mat.m4;
    r.m6 = mat.m6 + shy * mat.m7;

    r.m1 = shx * mat.m0 + mat.m1;
    r.m4 = shx * mat.m3 + mat.m4;
    r.m7 = shx * mat.m6 + mat.m7;

    r.m2 = mat.m2;
    r.m5 = mat.m5;
    r.m8 = mat.m8;
    return r;
}

// Returns the product of a matrix and a projection matrix
CGSAPI CguiMatrix3 CguiMatrix3Project(CguiMatrix3 mat, float px, float py) {
    CguiMatrix3 r;
    r.m0 = mat.m0 + px * mat.m2;
    r.m3 = mat.m3 + px * mat.m5;
    r.m6 = mat.m6 + px * mat.m8;

    r.m1 = mat.m1 + py * mat.m2;
    r.m4 = mat.m4 + py * mat.m5;
    r.m7 = mat.m7 + py * mat.m8;

    r.m2 = mat.m2;
    r.m5 = mat.m5;
    r.m8 = mat.m8;
    return r;
}

// Returns the product of a matrix and a reflection matrix
// Note: Input must be unit normal vector
CGSAPI CguiMatrix3 CguiMatrix3Reflect(CguiMatrix3 mat, float nx, float ny) {
    float a00 = 1.0f - 2.0f * nx * nx;
    float a01 = -2.0f * nx * ny;
    float a10 = -2.0f * nx * ny;
    float a11 = 1.0f - 2.0f * ny * ny;

    CguiMatrix3 r;
    r.m0 = a00 * mat.m0 + a10 * mat.m1;
    r.m3 = a00 * mat.m3 + a10 * mat.m4;
    r.m6 = a00 * mat.m6 + a10 * mat.m7;

    r.m1 = a01 * mat.m0 + a11 * mat.m1;
    r.m4 = a01 * mat.m3 + a11 * mat.m4;
    r.m7 = a01 * mat.m6 + a11 * mat.m7;

    r.m2 = mat.m2;
    r.m5 = mat.m5;
    r.m8 = mat.m8;
    return r;
}

// Returns the quadrilateral resulting from transforming a quadrilateral by a matrix
CGSAPI CguiQuad CguiMatrix3TransformQuad(CguiMatrix3 mat, CguiQuad quad) {
    CguiQuad r;
    r.vertices[0] = CguiMatrix3MultiplyV(mat, quad.vertices[0].x, quad.vertices[0].y);
    r.vertices[1] = CguiMatrix3MultiplyV(mat, quad.vertices[1].x, quad.vertices[1].y);
    r.vertices[2] = CguiMatrix3MultiplyV(mat, quad.vertices[2].x, quad.vertices[2].y);
    r.vertices[3] = CguiMatrix3MultiplyV(mat, quad.vertices[3].x, quad.vertices[3].y);
    return r;
}

// Returns the quadrilateral resulting from transforming a rectangle by a matrix
CGSAPI CguiQuad CguiMatrix3TransformRec(CguiMatrix3 mat, Rectangle rec) {
    CguiQuad r;
    r.vertices[0] = CguiMatrix3MultiplyV(mat, rec.x, rec.y);
    r.vertices[1] = CguiMatrix3MultiplyV(mat, rec.x + rec.width, rec.y);
    r.vertices[2] = CguiMatrix3MultiplyV(mat, rec.x, rec.y + rec.height);
    r.vertices[3] = CguiMatrix3MultiplyV(mat, rec.x + rec.width, rec.y + rec.height);
    return r;
}

#endif // CGUI_MATRIX3_H_
