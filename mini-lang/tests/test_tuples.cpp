#include <doctest/doctest.h>
#include "test_util.h"

TEST_CASE("positional tuple and indexing") {
    auto out = run_program_capture(R"(
        let t = (1, "hi");
        print t[1];
    )");
    CHECK(out == "hi\n");
}

TEST_CASE("named tuple fields and member access") {
    auto out = run_program_capture(R"(
        let p: (x: int, y: int) = (10, 20);
        print p.x;
        p.y = 30;
        print p[1];
    )");
    CHECK(out == "10\n30\n");
}

TEST_CASE("tuple as map key") {
    auto out = run_program_capture(R"(
        let m: (int, int) => int = {};
        m[(1,2)] = 5;
        print get(m, (1,2));
    )");
    CHECK(out == "5\n");
}
