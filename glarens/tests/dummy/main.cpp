// Glarens - GUI Framework.
//
// Dummy test file.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "glarens/glarens.hpp" // IWYU pragma: export
#include "doctest/doctest.h"

TEST_CASE("Test 2 + 2 = 4") {
    CHECK(2 + 2 == 4);
}
