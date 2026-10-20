/* Anstro's Game of Life remake
Edition-independent core game of life implementations

Copyright (c) 2023 Anstro Pleuton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.  */

#ifndef ANSTRO_GAME_OF_LIFE_REMAKE_H
#define ANSTRO_GAME_OF_LIFE_REMAKE_H

#ifndef AGOLR_API
    // Implement all the functions as inline by default
    #define AGOLR_API inline
#endif

#ifdef USE_NO_STANDARD_LIBRARY

// Non-standard memory allocation functions
#ifndef AGOLR_MALLOC
#error "Memory allocation functions is required"
#endif
#ifndef AGOLR_REALLOC
#error "Memory reallocation functions is required"
#endif
#ifndef AGOLR_FREE
#error "Memory allocation functions is required"
#endif

#ifndef NULL
#define NULL (void)0
#endif

// Non-standard boolean types
#ifndef bool
#define bool chad
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#else
#include <stdlib.h>
#include <stdbool.h>

#define AGOLR_MALLOC malloc
#define AGOLR_REALLOC realloc
#define AGOLR_FREE free
#endif

// No standard library is in use

// Vector2Int, 2 int components
typedef struct Vector2Int {
    int x;                  // Vector x component
    int y;                  // Vector y component
} Vector2Int;

// Cell data type, determine the state of a cell
// NOTE: The dead state should come before fade
// And the fade state should come at the last
typedef enum Cell {
    live = 0,               // A cell is live
    dead = 3,               // A cell is dead
    born = 1,               // A cell needs to be live in next generation
    kill = 2,               // A cell needs to be dead in next generation
    fade = 255              // A cell is set to fade when clearing the grid for smooth transition of live to dead colors
} Cell;

// Cell data stored isn't an enumerator
#define CELL_TYPE unsigned char

// The core of the game of life
typedef struct Life {
    CELL_TYPE *cells;       // Dynamically allocated array as a grid of cells
    Vector2Int gridSize;    // Size of the grid
} Life;

// Modulus function on integer with support for negative values of x
AGOLR_API int IntMod(int x, int y);

// Get the value in the range of min and max
AGOLR_API int IntRange(int value, int min, int max);

// Modulus function to wrap a position in a given bounds
AGOLR_API Vector2Int GetWrappedPosition(Vector2Int position, Vector2Int bounds);

// Clear the grid
AGOLR_API void ClearLife(Life *life);

// Resize the gird with new size
AGOLR_API void ResizeLife(Life *life, Vector2Int newGridSize);

// Allocate memory for Life data
AGOLR_API Life *LoadLife(Vector2Int gridSize);

// Unload the memory for life
AGOLR_API void UnloadLife(Life *life);

// Return calculated cell index from the position
// NOTE: The cell index cannot be invalid unless forced
AGOLR_API int GetCellIndex(Life *life, Vector2Int cellPosition);

// Check wether a cell is live by determining the calculated and not calculated state
AGOLR_API bool IsCellResolvedLive(Life *life, int cellIndex);

// Get the number of live cells in it's neighors
AGOLR_API int GetCellNeighors(Life *life, Vector2Int cellPosition);

// Caclulate the generation of life using the rules
AGOLR_API void CalculateGeneration(Life *life);

// Make the cells dead or live based on kill or born state
AGOLR_API void ResolveGeneration(Life *life);

// One generation step function: Calculate and reesolve generation
AGOLR_API void StepGeneration(Life *life);

#ifdef ANSTRO_GAME_OF_LIFE_REMAKE_IMPLEMENTATION

// Modulus function on integer with support for negative values of x
int IntMod(int x, int y)
{
    if (y == 0) return 0;
    if (x < 0) return y - ((-x) % y);
    return x % y;
}

// Get the value in the range of min and max
int IntRange(int value, int min, int max)
{
    // Swap min amd max if they are opposite or if they overlap
    if (min > max)
    {
        int temp = min;
        min = max;
        max = temp;
    }

    if (value < min) value = min;
    if (value > max) value = max;

    return value;
}

// Modulus function to wrap a position in a given bounds
Vector2Int GetWrappedPosition(Vector2Int position, Vector2Int bounds)
{
    Vector2Int wrappedPosition = { 0 };
    wrappedPosition.x = IntMod(position.x, bounds.x);
    wrappedPosition.y = IntMod(position.y, bounds.y);
    return wrappedPosition;
}

// Clear the grid
void ClearLife(Life *life)
{
    if (!life) return;
    // NOTE: The cell is set to fade instead of dead
    for (int i = 0; i < life->gridSize.x * life->gridSize.y; i++) life->cells[i] = fade;
}

// Resize the gird with new size
void ResizeLife(Life *life, Vector2Int newGridSize)
{
    if (!life) return;
    life->cells = AGOLR_REALLOC(life->cells, newGridSize.x * newGridSize.y * sizeof(CELL_TYPE));
    life->gridSize = newGridSize;
    ClearLife(life);
}

// Allocate memory for Life data
Life *LoadLife(Vector2Int gridSize)
{
    Life *life = AGOLR_MALLOC(sizeof(Life));
    if (!life) return NULL;
    life->gridSize = gridSize;
    life->cells = AGOLR_MALLOC(gridSize.x * gridSize.y * sizeof(CELL_TYPE));
    if (!life->cells) return NULL;
    ClearLife(life);
    return life;
}

// Unload the memory for life
void UnloadLife(Life *life)
{
    AGOLR_FREE(life->cells);
    AGOLR_FREE(life);
}

// Return calculated cell index from the position
// NOTE: The cell index cannot be invalid unless forced
int GetCellIndex(Life *life, Vector2Int cellPosition)
{
    if (!life) return -1;
    Vector2Int wrappedCellPosition = GetWrappedPosition(cellPosition, life->gridSize);
    return wrappedCellPosition.y * life->gridSize.x + wrappedCellPosition.x;
}

// Check wether a cell is live by determining the calculated and not calculated state
bool IsCellResolvedLive(Life *life, int cellIndex)
{
    if (!life) return -1;

    // NOTE: The cell was previously live when it is set to kill
    return (life->cells[cellIndex] == live || life->cells[cellIndex] == kill);
}

// Get the number of live cells in it's neighors
int GetCellNeighors(Life *life, Vector2Int cellPosition)
{
    if (!life) return -1;

    return (
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x - 1, cellPosition.y - 1 })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x - 1, cellPosition.y     })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x - 1, cellPosition.y + 1 })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x,     cellPosition.y - 1 })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x,     cellPosition.y + 1 })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x + 1, cellPosition.y - 1 })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x + 1, cellPosition.y     })) +
        IsCellResolvedLive(life, GetCellIndex(life, (Vector2Int){ cellPosition.x + 1, cellPosition.y + 1 }))
    );
}

// Caclulate the generation of life using the rules
void CalculateGeneration(Life *life)
{
    if (!life) return;

    for (int x = 0; x < life->gridSize.x; x++)
    {
        for (int y = 0; y < life->gridSize.y; y++)
        {
            Vector2Int cellPosition = { x, y };
            int cellIndex = GetCellIndex(life, cellPosition);
            CELL_TYPE *cell = &life->cells[cellIndex];
            int neighorsCount = GetCellNeighors(life, cellPosition);

            if (IsCellResolvedLive(life, cellIndex))
            {
                // Set the cell to be dead in next generation
                if (neighorsCount < 2 || neighorsCount > 3) *cell = kill;
            }
            else
            {
                // Set the cell to be live in next generation
                if (neighorsCount == 3) *cell = born;
            }
        }
    }
}

// Make the cells dead or live based on kill or born state
void ResolveGeneration(Life *life)
{
    if (!life) return;

    for (int x = 0; x < life->gridSize.x; x++)
    {
        for (int y = 0; y < life->gridSize.y; y++)
        {
            Vector2Int cellPosition = { x, y };
            CELL_TYPE *cell = &life->cells[GetCellIndex(life, cellPosition)];

            if (*cell == born) *cell = live;
            else if (*cell == kill) *cell = dead;
            
            // Fade calculation
            else if (*cell >= dead && *cell < fade) (*cell)++;
        }
    }
}

// One generation step function: Calculate and reesolve generation
void StepGeneration(Life *life)
{
    if (!life) return;

    CalculateGeneration(life);
    ResolveGeneration(life);
}

#endif // ANSTRO_GAME_OF_LIFE_REMAKE_IMPLEMENTATION

#endif // ANSTRO_GAME_OF_LIFE_REMAKE_H
