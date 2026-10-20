// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Array utilities for dynamic arrays of any type
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_ARRAY_H_
#define CGUI_ARRAY_H_

#include "cgui_api.h"
#include "cgui_compiler.h"
#include <stdlib.h>
#include <string.h>

#define CGUI_ARRAY_TYPE(TYPE) \
    typedef TYPE *TYPE##Ptr;

#define CGUI_ARRAY(TYPE, NAME) \
    TYPE##Ptr NAME;            \
    int       NAME##Count;     \
    int       NAME##Capacity

#define CGUI_ARRAY_IS_VALID(TYPE, NAME) (NAME != NULL && NAME##Capacity > 0)

#define CGUI_ARRAY_CREATE(TYPE, NAME)                     \
    do {                                                  \
        NAME##Count    = 0;                               \
        NAME##Capacity = 0;                               \
        NAME           = (TYPE##Ptr)malloc(sizeof(TYPE)); \
        if (!NAME) break;                                 \
        memset(NAME, 0, sizeof(TYPE));                    \
        NAME##Count    = 0;                               \
        NAME##Capacity = 1;                               \
    } while (0)

#define CGUI_ARRAY_CREATE_EX(TYPE, NAME, COUNT)                      \
    do {                                                             \
        int count = (COUNT);                                         \
        if (count < 0) count = 0;                                    \
        int capacity = count;                                        \
        if (capacity <= 0) capacity = 1;                             \
        capacity       = CguiArrayNextPow2(capacity);                \
        NAME##Count    = 0;                                          \
        NAME##Capacity = 0;                                          \
        NAME           = (TYPE##Ptr)malloc(sizeof(TYPE) * capacity); \
        if (!NAME) break;                                            \
        memset(NAME, 0, sizeof(TYPE));                               \
        NAME##Count    = count;                                      \
        NAME##Capacity = capacity;                                   \
    } while (0)

#define CGUI_ARRAY_DELETE(TYPE, NAME)                \
    do {                                             \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break; \
        free(NAME);                                  \
        NAME           = NULL;                       \
        NAME##Count    = 0;                          \
        NAME##Capacity = 0;                          \
    } while (0)

#define CGUI_ARRAY_CLEAR(TYPE, NAME)                    \
    do {                                                \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;    \
        NAME##Count      = 0;                           \
        TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE)); \
        if (newPtr) {                                   \
            NAME           = newPtr;                    \
            NAME##Capacity = 1;                         \
        }                                               \
    } while (0)

// Note: Reserves for additional items, not total capacity
// Note: Keeps growth strategy intact, so it may reserve more than requested
// Note: Creates a new array if invalid array is provided
#define CGUI_ARRAY_RESERVE(TYPE, NAME, ITEMS)                                                 \
    do {                                                                                      \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) {                                               \
            CGUI_ARRAY_CREATE_EX(TYPE, NAME, (ITEMS));                                        \
            break;                                                                            \
        }                                                                                     \
        int requiredCapacity = NAME##Count + (ITEMS);                                         \
        int oldCapacity      = NAME##Capacity;                                                \
        if (NAME##Capacity < requiredCapacity) {                                              \
            NAME##Capacity   = CguiArrayNextPow2(requiredCapacity);                           \
            TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE) * NAME##Capacity);                  \
            if (newPtr) {                                                                     \
                NAME = newPtr;                                                                \
                memset(NAME + oldCapacity, 0, sizeof(TYPE) * (NAME##Capacity - oldCapacity)); \
            } else {                                                                          \
                NAME##Capacity = oldCapacity;                                                 \
                break;                                                                        \
            }                                                                                 \
        }                                                                                     \
    } while (0)

// Note: Keeps growth strategy intact, so it may keep more capacity than requested
#define CGUI_ARRAY_SHRINK(TYPE, NAME)                                        \
    do {                                                                     \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;                         \
        int oldCapacity = NAME##Capacity;                                    \
        NAME##Capacity  = CguiArrayNextPow2(NAME##Count);                    \
        if (NAME##Capacity < 1) NAME##Capacity = 1;                          \
        if (NAME##Capacity != oldCapacity) {                                 \
            TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE) * NAME##Capacity); \
            if (newPtr) {                                                    \
                NAME = newPtr;                                               \
            } else {                                                         \
                NAME##Capacity = oldCapacity;                                \
                break;                                                       \
            }                                                                \
        }                                                                    \
    } while (0)

// Note: Fills the existing items, not new items
#define CGUI_ARRAY_FILL(TYPE, NAME, VALUE)           \
    do {                                             \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break; \
        for (int i = 0; i < NAME##Count; i++) {      \
            NAME[i] = (VALUE);                       \
        }                                            \
    } while (0)

// Note: Creates a new array if invalid array is provided
#define CGUI_ARRAY_RESIZE(TYPE, NAME, ITEMS)                                                  \
    do {                                                                                      \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) {                                               \
            CGUI_ARRAY_CREATE_EX(TYPE, NAME, (ITEMS));                                        \
            break;                                                                            \
        }                                                                                     \
        if ((ITEMS) < 0) break;                                                               \
        int oldCount = NAME##Count;                                                           \
        if ((ITEMS) > NAME##Capacity) {                                                       \
            int oldCapacity  = NAME##Capacity;                                                \
            NAME##Capacity   = CguiArrayNextPow2((ITEMS));                                    \
            TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE) * NAME##Capacity);                  \
            if (newPtr) {                                                                     \
                NAME = newPtr;                                                                \
                memset(NAME + oldCapacity, 0, sizeof(TYPE) * (NAME##Capacity - oldCapacity)); \
            } else {                                                                          \
                NAME##Capacity = oldCapacity;                                                 \
                break;                                                                        \
            }                                                                                 \
        }                                                                                     \
        NAME##Count = (ITEMS);                                                                \
    } while (0)

// Note: TO_NAME must be uninitialized or deleted
#define CGUI_ARRAY_COPY(TYPE, TO_NAME, FROM_NAME)                  \
    do {                                                           \
        if (!CGUI_ARRAY_IS_VALID(TYPE, FROM_NAME)) break;          \
        CGUI_ARRAY_CREATE_EX(TYPE, TO_NAME, FROM_NAME##Count);     \
        if (!TO_NAME) break;                                       \
        memcpy(TO_NAME, FROM_NAME, sizeof(TYPE) * TO_NAME##Count); \
    } while (0)

// Note: TO_NAME must be uninitialized or deleted
#define CGUI_ARRAY_MOVE(TYPE, TO_NAME, FROM_NAME)         \
    do {                                                  \
        if (!CGUI_ARRAY_IS_VALID(TYPE, FROM_NAME)) break; \
        TO_NAME##Count      = FROM_NAME##Count;           \
        TO_NAME##Capacity   = FROM_NAME##Capacity;        \
        TO_NAME             = FROM_NAME;                  \
        FROM_NAME           = NULL;                       \
        FROM_NAME##Count    = 0;                          \
        FROM_NAME##Capacity = 0;                          \
    } while (0)

// Note: TO_NAME must be uninitialized or deleted
#define CGUI_ARRAY_SLICE(TYPE, TO_NAME, FROM_NAME, START, COUNT)                                             \
    do {                                                                                                     \
        if (!CGUI_ARRAY_IS_VALID(TYPE, FROM_NAME)) break;                                                    \
        if ((START) < 0 || (START) >= FROM_NAME##Count || (COUNT) < 0) break;                                \
        int sliceCount    = ((START) + (COUNT) > FROM_NAME##Count) ? (FROM_NAME##Count - (START)) : (COUNT); \
        int sliceCapacity = CguiArrayNextPow2(sliceCount);                                                   \
        TO_NAME           = (TYPE##Ptr)malloc(sizeof(TYPE) * sliceCapacity);                                 \
        if (!TO_NAME) break;                                                                                 \
        TO_NAME##Count    = sliceCount;                                                                      \
        TO_NAME##Capacity = sliceCapacity;                                                                   \
        memcpy(TO_NAME, FROM_NAME + (START), sizeof(TYPE) * sliceCount);                                     \
    } while (0)

// Note: Creates a new array if invalid array is provided
#define CGUI_ARRAY_INSERT(TYPE, NAME, INDEX, VALUE)                                           \
    do {                                                                                      \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) {                                               \
            CGUI_ARRAY_CREATE(TYPE, NAME);                                                    \
        }                                                                                     \
        if ((INDEX) < 0 || (INDEX) > NAME##Count) break;                                      \
        if (NAME##Count >= NAME##Capacity) {                                                  \
            int oldCapacity  = NAME##Capacity;                                                \
            NAME##Capacity   = CguiArrayNextPow2(NAME##Count + 1);                            \
            TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE) * NAME##Capacity);                  \
            if (newPtr) {                                                                     \
                NAME = newPtr;                                                                \
                memset(NAME + oldCapacity, 0, sizeof(TYPE) * (NAME##Capacity - oldCapacity)); \
            } else {                                                                          \
                NAME##Capacity = oldCapacity;                                                 \
                break;                                                                        \
            }                                                                                 \
        }                                                                                     \
        memmove(&NAME[(INDEX) + 1], &NAME[(INDEX)], sizeof(TYPE) * (NAME##Count - (INDEX)));  \
        CGUI_ANALYSIS_ASSUME((INDEX) < NAME##Capacity);                                       \
        NAME[(INDEX)] = (VALUE);                                                              \
        NAME##Count++;                                                                        \
    } while (0)

#define CGUI_ARRAY_REMOVE(TYPE, NAME, INDEX)                                                     \
    do {                                                                                         \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;                                             \
        if ((INDEX) < 0 || (INDEX) >= NAME##Count) break;                                        \
        memmove(&NAME[(INDEX)], &NAME[(INDEX) + 1], sizeof(TYPE) * (NAME##Count - (INDEX) - 1)); \
        NAME##Count--;                                                                           \
        if (NAME##Count <= NAME##Capacity / 4) {                                                 \
            int oldCapacity = NAME##Capacity;                                                    \
            NAME##Capacity  = CguiArrayNextPow2(NAME##Count);                                    \
            if (NAME##Capacity < 1) NAME##Capacity = 1;                                          \
            TYPE##Ptr newPtr = realloc(NAME, sizeof(TYPE) * NAME##Capacity);                     \
            if (newPtr)                                                                          \
                NAME = newPtr;                                                                   \
            else {                                                                               \
                NAME##Capacity = oldCapacity;                                                    \
                break;                                                                           \
            }                                                                                    \
        }                                                                                        \
    } while (0)

#define CGUI_ARRAY_POP(TYPE, NAME, INDEX, RESULT)         \
    do {                                                  \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;      \
        if ((INDEX) < 0 || (INDEX) >= NAME##Count) break; \
        (RESULT) = NAME[(INDEX)];                         \
        CGUI_ARRAY_REMOVE(TYPE, NAME, (INDEX));           \
    } while (0)

#define CGUI_ARRAY_INSERT_FRONT(TYPE, NAME, VALUE) CGUI_ARRAY_INSERT(TYPE, NAME, 0, (VALUE))
#define CGUI_ARRAY_INSERT_BACK(TYPE, NAME, VALUE)  CGUI_ARRAY_INSERT(TYPE, NAME, NAME##Count, (VALUE))
#define CGUI_ARRAY_REMOVE_FRONT(TYPE, NAME)        CGUI_ARRAY_REMOVE(TYPE, NAME, 0)
#define CGUI_ARRAY_REMOVE_BACK(TYPE, NAME)         CGUI_ARRAY_REMOVE(TYPE, NAME, NAME##Count - 1)
#define CGUI_ARRAY_POP_FRONT(TYPE, NAME, RESULT)   CGUI_ARRAY_POP(TYPE, NAME, 0, (RESULT))
#define CGUI_ARRAY_POP_BACK(TYPE, NAME, RESULT)    CGUI_ARRAY_POP(TYPE, NAME, NAME##Count - 1, (RESULT))

#define CGUI_ARRAY_INSERT_MANY_FRONT(TYPE, NAME, ...)                         \
    do {                                                                      \
        TYPE values[] = {__VA_ARGS__};                                        \
        int  count    = sizeof(values) / sizeof(TYPE);                        \
        CGUI_ARRAY_RESERVE(TYPE, NAME, count);                                \
        for (int i = 0; i < count; i++) {                                     \
            CGUI_ARRAY_INSERT(TYPE, NAME, i, values[i]); /* Todo: optimize */ \
        }                                                                     \
    } while (0)

#define CGUI_ARRAY_INSERT_MANY_BACK(TYPE, NAME, ...)               \
    do {                                                           \
        TYPE values[] = {__VA_ARGS__};                             \
        int  count    = sizeof(values) / sizeof(TYPE);             \
        CGUI_ARRAY_RESERVE(TYPE, NAME, count);                     \
        for (int i = 0; i < count; i++) {                          \
            CGUI_ARRAY_INSERT(TYPE, NAME, NAME##Count, values[i]); \
        }                                                          \
    } while (0)

// Note: Requires indices to be in ascending order
#define CGUI_ARRAY_REMOVE_MANY(TYPE, NAME, ...)        \
    do {                                               \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;   \
        int indices[] = {__VA_ARGS__};                 \
        int count     = sizeof(indices) / sizeof(int); \
        for (int i = count - 1; i >= 0; i--) {         \
            CGUI_ARRAY_REMOVE(TYPE, NAME, indices[i]); \
        }                                              \
    } while (0)

// Warning: Using break does not work as intended; this vehicle is breakless
#define CGUI_ARRAY_FOR(TYPE, NAME, VAR)                                  \
    if (CGUI_ARRAY_IS_VALID(TYPE, NAME))                                 \
        for (int VAR##Index = 0; VAR##Index < NAME##Count; VAR##Index++) \
            for (TYPE##Ptr VAR = &NAME[VAR##Index]; VAR; VAR = NULL)

// Warning: Using break does not work as intended; this vehicle is breakless
#define CGUI_ARRAY_FORR(TYPE, NAME, VAR)                                      \
    if (CGUI_ARRAY_IS_VALID(TYPE, NAME))                                      \
        for (int VAR##Index = NAME##Count - 1; VAR##Index >= 0; VAR##Index--) \
            for (TYPE##Ptr VAR = &NAME[VAR##Index]; VAR; VAR = NULL)

// Warning: Using break does not work as intended; this vehicle is breakless
#define CGUI_ARRAY_FOR_RANGE(TYPE, NAME, VAR, BEGIN, END)                                            \
    if (CGUI_ARRAY_IS_VALID(TYPE, NAME))                                                             \
        for (int VAR##Index = (BEGIN); VAR##Index < (END) && VAR##Index < NAME##Count; VAR##Index++) \
            for (TYPE##Ptr VAR = &NAME[VAR##Index]; VAR; VAR = NULL)

// Warning: Using break does not work as intended; this vehicle is breakless
#define CGUI_ARRAY_FORR_RANGE(TYPE, NAME, VAR, BEGIN, END)                                                          \
    if (CGUI_ARRAY_IS_VALID(TYPE, NAME))                                                                            \
        for (int VAR##Index = ((END) < NAME##Count ? (END) : NAME##Count) - 1; VAR##Index >= (BEGIN); VAR##Index--) \
            for (TYPE##Ptr VAR = &NAME[VAR##Index]; VAR; VAR = NULL)

#define CGUI_ARRAY_COMPARE(TYPE, ...)                                     \
    static inline int TYPE##Compare(const void *aPtr, const void *bPtr) { \
        if (!aPtr || !bPtr) return 0;                                     \
        const TYPE##Ptr a = (const TYPE##Ptr)aPtr;                        \
        const TYPE##Ptr b = (const TYPE##Ptr)bPtr;                        \
        __VA_ARGS__                                                       \
    }

#define CGUI_ARRAY_COMPARE_DEFAULT(TYPE) CGUI_ARRAY_COMPARE(TYPE, if (a < b) { return -1; } if (a > b) { return 1; } return 0;)

#define CGUI_ARRAY_SORT(TYPE, NAME)                            \
    do {                                                       \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;           \
        qsort(NAME, NAME##Count, sizeof(TYPE), TYPE##Compare); \
    } while (0)

#define CGUI_ARRAY_FIND_IF(TYPE, NAME, VAR, PRED, RESULT) \
    do {                                                  \
        (RESULT) = -1;                                    \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;      \
        for (int i = 0; i < NAME##Count; i++) {           \
            TYPE##Ptr VAR = &NAME[i];                     \
            if ((PRED)) {                                 \
                (RESULT) = i;                             \
                break;                                    \
            }                                             \
        }                                                 \
    } while (0)

#define CGUI_ARRAY_FIND(TYPE, NAME, VALUE, RESULT) CGUI_ARRAY_FIND_IF(TYPE, NAME, item, TYPE##Compare(item, &(VALUE)) == 0, (RESULT))

// Note: Requires the array to be sorted
#define CGUI_ARRAY_BIN_FIND(TYPE, NAME, VALUE, RESULT)     \
    do {                                                   \
        (RESULT) = -1;                                     \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;       \
        int left  = 0;                                     \
        int right = NAME##Count - 1;                       \
        while (left <= right) {                            \
            int mid = left + (right - left) / 2;           \
            int res = TYPE##Compare(&NAME[mid], &(VALUE)); \
            if (res < 0) {                                 \
                left = mid + 1;                            \
            } else if (res > 0) {                          \
                right = mid - 1;                           \
            } else {                                       \
                (RESULT) = mid;                            \
                break;                                     \
            }                                              \
        }                                                  \
    } while (0)

#define CGUI_ARRAY_COUNT_IF(TYPE, NAME, VAR, PRED, RESULT) \
    do {                                                   \
        (RESULT) = 0;                                      \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;       \
        for (int i = 0; i < NAME##Count; i++) {            \
            TYPE##Ptr VAR = &NAME[i];                      \
            if ((PRED)) {                                  \
                (RESULT)++;                                \
            }                                              \
        }                                                  \
    } while (0)

#define CGUI_ARRAY_COUNT(TYPE, NAME, VALUE, RESULT) CGUI_ARRAY_COUNT_IF(TYPE, NAME, item, TYPE##Compare(item, &(VALUE)) == 0, (RESULT))

#define CGUI_ARRAY_CONTAINS_IF(TYPE, NAME, VAR, PRED, RESULT) \
    do {                                                      \
        (RESULT) = false;                                     \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;          \
        for (int i = 0; i < NAME##Count; i++) {               \
            TYPE##Ptr VAR = &NAME[i];                         \
            if ((PRED)) {                                     \
                (RESULT) = true;                              \
                break;                                        \
            }                                                 \
        }                                                     \
    } while (0)

#define CGUI_ARRAY_CONTAINS(TYPE, NAME, VALUE, RESULT) CGUI_ARRAY_CONTAINS_IF(TYPE, NAME, item, TYPE##Compare(item, &(VALUE)) == 0, (RESULT))

// Note: Requires the array to be sorted
#define CGUI_ARRAY_BIN_CONTAINS(TYPE, NAME, VALUE, RESULT)           \
    do {                                                             \
        (RESULT) = false;                                            \
        if (!CGUI_ARRAY_IS_VALID(TYPE, NAME)) break;                 \
        int result = CGUI_ARRAY_BIN_FIND(TYPE, NAME, VALUE, result); \
        if (result != -1) {                                          \
            (RESULT) = true;                                         \
        }                                                            \
    } while (0)

CGSAPI int CguiArrayNextPow2(int x) {
    if (x <= 1) return 1;

    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    // x |= x >> 32; // For int64
    x++;

    return x;
}

#endif // CGUI_ARRAY_H_
