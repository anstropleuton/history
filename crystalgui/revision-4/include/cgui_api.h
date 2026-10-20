// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// CGAPI (Crystal GUI API) and CGSAPI (Crystal GUI Small API) definition
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_API_H_
#define CGUI_API_H_

// [Stolen from raylib:]

// Function specifiers in case library is build/used as a shared library
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
// NOTE: visibility("default") attribute makes symbols "visible" when compiled with -fvisibility=hidden
#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif
    #if defined(BUILD_LIBTYPE_SHARED)
        #define CGAPI __declspec(dllexport) // Building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define CGAPI __declspec(dllimport) // Using the library as a Win32 shared library (.dll)
    #endif
#else
    #if defined(BUILD_LIBTYPE_SHARED)
        #define CGAPI __attribute__((visibility("default"))) // Building as a Unix shared library (.so/.dylib)
    #endif
#endif

#ifndef CGAPI
    #define CGAPI // Functions defined as 'extern' by default (implicit specifiers)
#endif

#if defined(CGS_IMPLEMENTATION) && defined(CGS_STATIC_INLINE)
    #error "Specifying both CGS_IMPLEMENTATION and CGS_STATIC_INLINE is contradictory"
#endif

// Function specifiers definition
#if defined(CGS_IMPLEMENTATION)
    #if defined(_WIN32) && defined(BUILD_LIBTYPE_SHARED)
        #define CGSAPI __declspec(dllexport) extern inline // Building the library as a Win32 shared library (.dll)
    #elif defined(BUILD_LIBTYPE_SHARED)
        #define CGSAPI __attribute__((visibility("default"))) // Building the library as a Unix shared library (.so/.dylib)
    #elif defined(_WIN32) && defined(USE_LIBTYPE_SHARED)
        #define CGSAPI __declspec(dllimport) // Using the library as a Win32 shared library (.dll)
    #else
        #define CGSAPI extern inline // Provide external definition
    #endif
#elif defined(CGS_STATIC_INLINE)
    #define CGSAPI static inline // Functions may be inlined, no external out-of-line definition
#else
    #if defined(__TINYC__)
        #define CGSAPI static inline // plain inline not supported by tinycc (See issue #435)
    #else
        #define CGSAPI inline // Functions may be inlined or external definition used
    #endif
#endif

#endif // CGUI_API_H_
