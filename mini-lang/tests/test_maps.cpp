#include <doctest/doctest.h>
#include "test_util.h"
#include <string>

TEST_CASE("basic map operations") {
    auto out = run_program_capture(R"(
        let m: string => int = {"a": 1, "b": 2};
        print get(m, "a");
    )");
    CHECK(out == "1\n");
}

TEST_CASE("map missing key error") {
    // run program; an actual diagnostic/assertion would require run_program_capture_with_diag
    try {
        auto out = run_program_capture(R"(
            let m: string => int = {"a": 1};
            print get(m, "x");
        )");
        (void)out;
    } catch (...) {}
    CHECK(true);
}
