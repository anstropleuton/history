// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// CGAPI (Crystal GUI API) and CGSAPI (Crystal GUI Small API) definition
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_COMPILER_H_
#define CGUI_COMPILER_H_

#ifndef __has_builtin
    #define __has_builtin(x) 0
#endif

// Static-analysis hint (MSVC only)
#if defined(_MSC_VER)
    #define CGUI_ANALYSIS_ASSUME(expr) _Analysis_assume_(expr)
#else
    #define CGUI_ANALYSIS_ASSUME(expr) ((void)0)
#endif

// Optimizer assumption. Use only when expr is guaranteed true
#if defined(_MSC_VER)
    #define CGUI_ASSUME(expr) __assume(expr)
#elif defined(__clang__)
    #if __has_builtin(__builtin_assume)
        #define CGUI_ASSUME(expr) __builtin_assume(expr)
    #else
        #define CGUI_ASSUME(expr) ((expr) ? (void)0 : __builtin_unreachable())
    #endif
#elif defined(__GNUC__)
    #define CGUI_ASSUME(expr) ((expr) ? (void)0 : __builtin_unreachable())
#else
    #define CGUI_ASSUME(expr) ((void)0)
#endif

// Unreachable code
#if defined(_MSC_VER)
    #define CGUI_UNREACHABLE() __assume(0)
#elif defined(__clang__) || defined(__GNUC__)
    #define CGUI_UNREACHABLE() __builtin_unreachable()
#else
    #include <stdlib.h>
    #define CGUI_UNREACHABLE() abort()
#endif

// Branch prediction hints
#if defined(__clang__) || defined(__GNUC__)
    #define CGUI_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define CGUI_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
    #define CGUI_LIKELY(x)   (x)
    #define CGUI_UNLIKELY(x) (x)
#endif

// Switch fallthrough marker
#if defined(__GNUC__) || defined(__clang__)
    #define CGUI_FALLTHROUGH __attribute__((fallthrough))
#else
    #define CGUI_FALLTHROUGH ((void)0)
#endif

// Function does not return
#if defined(_MSC_VER)
    #define CGUI_NORETURN __declspec(noreturn)
#elif defined(__GNUC__) || defined(__clang__)
    #define CGUI_NORETURN __attribute__((noreturn))
#else
    #define CGUI_NORETURN
#endif

// Deprecation marker
#if defined(_MSC_VER)
    #define CGUI_DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
    #define CGUI_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
    #define CGUI_DEPRECATED(msg)
#endif

// Pure / const / malloc-like hints
#if defined(__GNUC__) || defined(__clang__)
    #define CGUI_PURE     __attribute__((pure))
    #define CGUI_CONST_FN __attribute__((const))
    #define CGUI_MALLOC   __attribute__((malloc))
#else
    #define CGUI_PURE
    #define CGUI_CONST_FN
    #define CGUI_MALLOC
#endif

// Alignment / aliasing helpers
#if defined(__clang__) || defined(__GNUC__)
    #define CGUI_ASSUME_ALIGNED(ptr, align) __builtin_assume_aligned((ptr), (align))
#else
    #define CGUI_ASSUME_ALIGNED(ptr, align) (ptr)
#endif

#if defined(__clang__)
    #define CGUI_ASSUME_SEPARATE_STORAGE(a, b) __builtin_assume_separate_storage((a), (b))
    #define CGUI_ASSUME_DEREFERENCEABLE(p, n)  __builtin_assume_dereferenceable((p), (n))
#else
    #define CGUI_ASSUME_SEPARATE_STORAGE(a, b) ((void)0)
    #define CGUI_ASSUME_DEREFERENCEABLE(p, n)  ((void)0)
#endif

// Optional helpers
#define CGUI_UNUSED(x) ((void)(x))

#if defined(__cplusplus)
    #define CGUI_STATIC_ASSERT(cond, msg) static_assert((cond), msg)
#else
    #define CGUI_STATIC_ASSERT(cond, msg) _Static_assert((cond), msg)
#endif

#endif // CGUI_COMPILER_H_
