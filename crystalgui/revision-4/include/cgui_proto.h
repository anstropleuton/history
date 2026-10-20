// Crystal GUI - A Crystal-clear Modern-style Retained-mode GUI framework for Raylib
// Prototype system for overridable parameters in GUI nodes
// Copyright (c) 2026 Anstro Pleuton
// MIT License (see LICENSE.md)

#ifndef CGUI_PROTO_H_
#define CGUI_PROTO_H_

#include <stdbool.h>

#define CGUI_PROTO(TYPE, NAME) \
    TYPE NAME;                 \
    TYPE NAME##Proto;          \
    bool NAME##Override

// Note: Requires defining <TYPE> <NAME>; and <TYPE> <NAME>Proto; manually
#define CGUI_PROTO_NO_PROTO(TYPE, NAME) \
    bool NAME##Override

#define CGUI_PROTO_GET(NAME) \
    (NAME##Override ? NAME : NAME##Proto)

#define CGUI_PROTO_SET_PROTO(NAME, VALUE) \
    do {                                  \
        NAME##Proto = (VALUE);            \
    } while (0)

#define CGUI_PROTO_SET(NAME, VALUE) \
    do {                            \
        NAME           = (VALUE);   \
        NAME##Override = true;      \
    } while (0)

#define CGUI_PROTO_UNSET(NAME)  \
    do {                        \
        NAME##Override = false; \
    } while (0)

#endif // CGUI_PROTO_H_
