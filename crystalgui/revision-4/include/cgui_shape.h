// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Shape-based image creation
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_SHAPE_H_
#define CGUI_SHAPE_H_

#include "cgui_api.h"
#include "cgui_array.h"
#include "cgui_matrix3.h"

#ifndef CGUI_MAX_GRADIENT_STOPS
    #define CGUI_MAX_GRADIENT_STOPS 12 // Why 12 specifically? /shrug
#endif

// Rectangle of integers
typedef struct CguiIRectangle {
    int x;      // Coordinates in x-axis of the top-left corner of the rectangle
    int y;      // Coordinates in y-axis of the top-left corner of the rectangle
    int width;  // Dimension of the rectangle in x-axis
    int height; // Dimension of the rectangle in y-axis
} CguiIRectangle;

// 2-dimensional Vector of integers
typedef struct CguiIVector2 {
    int x; // X component of vector
    int y; // Y component of vector
} CguiIVector2;

// Conversion

CGAPI CguiIRectangle CguiRecToIRec(Rectangle rec);                        // Convert a Rectangle to a CguiIRectangle by rounding the values (x and y are rounded down, width and height are rounded up)
CGAPI CguiIVector2   CguiVec2ToIVec2(Vector2 vec);                        // Convert a Vector2 to a CguiIVector2 by rounding the values
CGAPI Rectangle      CguiIRecToRec(CguiIRectangle rec);                   // Convert a CguiIRectangle to a Rectangle by converting the values to float
CGAPI Vector2        CguiIVec2ToVec2(CguiIVector2 vec);                   // Convert a CguiIVector2 to a Vector2 by converting the values to float
CGAPI bool           CguiIsIRecEqual(CguiIRectangle a, CguiIRectangle b); // Check whether the CguiIRectangles are equal
CGAPI bool           CguiIsIVec2Equal(CguiIVector2 a, CguiIVector2 b);    // Check whether the CguiIVector2s are equal

// Geometry made of path and metadata
// Note: filled region is always enclosed in vertices that are arranged in counter-clockwise direction
typedef struct CguiGeom {
    CGUI_ARRAY(Vector2, vertices); // Vertices that trace a path of the geometry
} CguiGeom;

// Geometry's computed SDF per pixel covering the bounds of all the vertices
typedef struct CguiComputeGeom {
    CguiIRectangle rec;        // Bounds that is defined by vertices (rounded to enclosing integer)
    CGUI_ARRAY(float, fields); // SDF of individual pixels (count = width * height)
} CguiComputeGeom;

typedef float (*CguiAlterGeomFunc)(float a);            // Function to alter individual values of the geometry's pixel SDF
typedef float (*CguiCombineGeomFunc)(float a, float b); // Function to combine individual values of the geometry's pixel SDF

// Parameters for the rasterizer
typedef struct CguiRasterParam {
    int   posX; // The X position of the current pixel (in image space)
    int   posY; // The Y position of the current pixel (in image space)
    float dist; // The distance from the current pixel to the nearest edge, negative if filled and pixel is in the filled region
} CguiRasterParam;

// The rasterizer to convert the geometry into an image
// Return the appropriate color for the current pixel based on additional information
typedef Color (*CguiRasterizer)(
    void           *data, // Additional user data to provide to the rasterizer
    CguiRasterParam param // Parameter used by the rasterizer to produce a color
);

CGAPI void  CguiTransformGeom(CguiGeom *geom, CguiMatrix3 mat);                             // Transform all the vertices of the geometry with the matrix, also adjusts the width and height of the geometry
CGAPI Image CguiRasterizeGeom(CguiComputeGeom geom, CguiRasterizer rasterizer, void *data); // Rasterize the geometry and convert it into an image

// Default rasterizer

// A single gradient stop point
typedef struct CguiGradientStop {
    Color   color;    // Gradient color
    Vector2 position; // Stop point position
} CguiGradientStop;

// A 2D weighted gradient
typedef struct CguiGradientFill {
    CguiGradientStop stops[CGUI_MAX_GRADIENT_STOPS]; // Gradient stop points
    int              stopsCount;                     // Number of gradient stop points
} CguiGradientFill;

CGAPI Color CguiResolveGradientFill(CguiGradientFill *fill, Vector2 point); // Resolve a gradient fill color. It takes pointer to reduce copying

// Texture image mode for image filling
typedef enum CguiImageWrap {
    CGUI_IMAGE_WRAP_NO_WRAP,         // Use fallback color instead of wrapping the image
    CGUI_IMAGE_WRAP_CLAMP_TO_EDGE,   // Use the color at the image edge indifinitely
    CGUI_IMAGE_WRAP_REPEAT,          // Repeat the image in a tiled form
    CGUI_IMAGE_WRAP_MIRRORED_REPEAT, // Repeat the image in a mirrored form
} CguiImageWrap;

// An image-based filling
typedef struct CguiImageFill {
    Image       image;     // Image to use
    CguiMatrix3 transform; // Transformation of the point on image; Note: Uses nearest interpolation; Note: Homogeneous transformations are not defined when wrapping is on
    int         wrapX;     // Wrapping mode for X-axis
    int         wrapY;     // Wrapping mode for Y-axis
    Color       tint;      // Tint color on the image
    Color       fallback;  // Fallback color for no-wrapping mode
} CguiImageFill;

CGAPI Color CguiResolveImageFill(CguiImageFill *fill, Vector2 point); // Resolve a gradient fill color. It takes pointer to reduce copying

// The current fill type to use
typedef enum CguiRasterFillType {
    CGUI_RASTER_COLOR,    // Use a solid color for fill
    CGUI_RASTER_GRADIENT, // Use a gradient for fill
    CGUI_RASTER_IMAGE,    // Use an image for fill
} CguiRasterFillType;

// Fill for the default rasterizer
typedef struct CguiRasterFill {
    int fillType; // The current fill type in the union
    union {
        Color            color;    // Fill a geometry with a solid color
        CguiGradientFill gradient; // Fill a geometry with a gradient color
        CguiImageFill    image;    // Fill a gemoetry with an image
    };
} CguiRasterFill;

CGAPI Color CguiResolveRasterFill(CguiRasterFill *fill, Vector2 point); // Resolve a rasterizer fill color. It takes pointer to reduce copying

// Data for the default rasterizer
typedef struct CguiRasterData {
    CguiRasterFill outerFill;       // Fill for the outer region of the geometry
    CguiRasterFill innerFill;       // Fill for the inner region of the geometry if filled
    CguiRasterFill borderFill;      // Fill for the border
    float          borderThickness; // The border thickness (expands both inward and outward)
    float          borderShift;     // The border's shifting amount (shifts outwards or inwards if negative)
    CguiRasterFill shadowFill;      // Fill for the shadow
    float          shadowRadius;    // Radius of the shadow from edges (negative radius makes inward shadow)
} CguiRasterData;

CGAPI Color CguiDefaultRasterizer(void *data, CguiRasterParam param); // Default rasterizer that covers most of the use cases; Note: the pointer must be of type CguiRasterData * and user managed

// Helper rasterizers

CGAPI Image CguiRasterFilled(CguiComputeGeom geom, Color color);                                                            // Rasterize the geometry with a fill color
CGAPI Image CguiRasterFilledImg(CguiComputeGeom geom, Image image, Color tint);                                             // Rasterize the geometry with an image and a tint color
CGAPI Image CguiRasterFilledGrdV(CguiComputeGeom geom, Color start, Color end);                                             // Rasterize the geometry with a vertical gradient color
CGAPI Image CguiRasterFilledGrdH(CguiComputeGeom geom, Color start, Color end);                                             // Rasterize the geometry with a horizontal gradient color
CGAPI Image CguiRasterFilledGrdD(CguiComputeGeom geom, Color start, Color end);                                             // Rasterize the geometry with a diagonal gradient color
CGAPI Image CguiRasterFilledGrdC(CguiComputeGeom geom, Color topLeft, Color topRight, Color bottomLeft, Color bottomRight); // Rasterize the geometry with a corner gradient color
CGAPI Image CguiRasterBorder(CguiComputeGeom geom, Color border, float thickness, float shift);                             // Rasterize the geometry with a fill color and border
CGAPI Image CguiRasterShadow(CguiComputeGeom geom, Color shadow, float radius);                                             // Rasterize the geometry with a shadow color and radius

// Helper geometry generators

CGAPI CguiGeom CguiCreateLineGeom(Vector2 first, Vector2 second);                                                  // Create a line geometry
CGAPI CguiGeom CguiCreateRecGeom(Rectangle rec);                                                                   // Create a rectangle geometry
CGAPI CguiGeom CguiCreateRoundedRecGeom(Rectangle rec, Vector4 cornerRadius);                                      // Create a rounded rectangle geometry with corner radius
CGAPI CguiGeom CguiCreateCircleGeom(Vector2 center, float radius);                                                 // Create a circle geometry
CGAPI CguiGeom CguiCreateCircleArcGeom(Vector2 center, float radius, float startAngle, float endAngle);            // Create a circle geometry with different arc start/end
CGAPI CguiGeom CguiCreateEllipseGeom(Vector2 center, Vector2 radius);                                              // Create an ellipse geometry
CGAPI CguiGeom CguiCreateEllipseArcGeom(Vector2 center, Vector2 radius, float startAngle, float endAngle);         // Create an ellipse geometry with different arc start/end
CGAPI CguiGeom CguiCreateTriangleGeom(Vector2 v1, Vector2 v2, Vector2 v3);                                         // Create a triangle geometry
CGAPI CguiGeom CguiCreateRoundedTriangleGeom(Vector2 v1, Vector2 v2, Vector2 v3, Vector3 cornerRadius);            // Create a rounded triangle geometry with corner radius
CGAPI CguiGeom CguiCreatePolygonGeom(const Vector2 *vertices, int verticesCount);                                  // Create a polygon geometry
CGAPI CguiGeom CguiCreateRoundedPolygonGeom(const Vector2 *vertices, const float *cornerRadii, int verticesCount); // Create a rounded polygon geometry with corner radius
CGAPI void     CguiDeleteGeom(CguiGeom geom);                                                                      // Delete a geometry and release its memory

// Combinations

CGAPI CguiComputeGeom CguiComputeFields(CguiGeom geom);                                                             // Compute all the pixels' SDFs
CGAPI CguiComputeGeom CguiResizeFields(CguiComputeGeom geom, CguiIRectangle rec);                                   // Expand or contract a geometry while preserving it's coordinates
CGAPI CguiComputeGeom CguiCombineFields(CguiComputeGeom geomA, CguiComputeGeom geomB, CguiCombineGeomFunc combine); // Combine individual pixel SDFs of the geometries using function for result (expands the total geometry if dimensions are mismatched)
CGAPI CguiComputeGeom CguiCombineFieldsF(CguiComputeGeom geom, float value, CguiCombineGeomFunc combine);           // Combine the individual pixel SDFs of the geometry and value
CGAPI CguiComputeGeom CguiAlterFields(CguiComputeGeom geom, CguiAlterGeomFunc alter);                               // Alter the individual pixel SDFs of the geometries using function for result
CGAPI CguiComputeGeom CguiAddFields(CguiComputeGeom geomA, CguiComputeGeom geomB);                                  // Add individual pixel SDFs of the geometries
CGAPI CguiComputeGeom CguiAddFieldsF(CguiComputeGeom geom, float value);                                            // Add the individual pixel SDFs of the geometry and value
CGAPI CguiComputeGeom CguiMultiplyFields(CguiComputeGeom geomA, CguiComputeGeom geomB);                             // Multiply individual pixel SDFs of the geometries
CGAPI CguiComputeGeom CguiMultiplyFieldsF(CguiComputeGeom geom, float value);                                       // Multiply the individual pixel SDFs of the geometry and value
CGAPI CguiComputeGeom CguiNegateFields(CguiComputeGeom geom);                                                       // Negate the individual pixel SDFs of the geometry
CGAPI CguiComputeGeom CguiInvertFields(CguiComputeGeom geom);                                                       // Invert the individual pixel SDFs of the geometry
CGAPI CguiComputeGeom CguiPowFields(CguiComputeGeom geomA, CguiComputeGeom geomB);                                  // Raise individual pixel SDFs of the geometry
CGAPI CguiComputeGeom CguiPowFieldsF(CguiComputeGeom geom, float value);                                            // Raise the individual pixel SDFs of the geometry and value
CGAPI CguiComputeGeom CguiMinFields(CguiComputeGeom geomA, CguiComputeGeom geomB);                                  // Take minimum of individual pixel SDFs of the geometries
CGAPI CguiComputeGeom CguiMinFieldsF(CguiComputeGeom geom, float value);                                            // Take minimum of the individual pixel SDFs of the geometry and value
CGAPI CguiComputeGeom CguiMaxFields(CguiComputeGeom geomA, CguiComputeGeom geomB);                                  // Take maximum of individual pixel SDFs of the geometries
CGAPI CguiComputeGeom CguiMaxFieldsF(CguiComputeGeom geom, float value);                                            // Take maximum of to the individual pixel SDFs of the geometry and value
CGAPI void            CguiDeleteFields(CguiComputeGeom geom);                                                       // Delete a computed geometry and release its memory

#endif // CGUI_SHAPE_H_
