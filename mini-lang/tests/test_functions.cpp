#include <doctest/doctest.h>
#include "test_util.h"
#include "diag.h"
#include "parser.h"
#include "typechecker.h"
#include "interpreter.h"

TEST_CASE("simple function call") {
    auto out = run_program_capture(R"(
        fn add(a: int, b: int) -> int {
            return a + b;
        }
        print add(2, 3);
    )");
    CHECK(out == "5\n");
}

TEST_CASE("recursive factorial") {
    auto out = run_program_capture(R"(
        fn fact(n: int) -> int {
            if n == 0 {
                return 1;
            }
            return n * fact(n - 1);
        }
        print fact(5);
    )");
    CHECK(out == "120\n");
}

TEST_CASE("return type inference") {
    auto out = run_program_capture(R"(
        fn f(a: int) {
            return a + 1;
        }
        print f(3);
    )");
    CHECK(out == "4\n");
}
