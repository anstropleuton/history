#include <doctest/doctest.h>
#include "test_util.h"
#include "diag.h"

TEST_CASE("if statement") {
    auto out = run_program_capture(R"(
        if true {
            print 1;
        }
        if false {
            print 2;
        }
    )");
    CHECK(out == "1\n");
}

TEST_CASE("while loop") {
    auto out = run_program_capture(R"(
        let i = 0;
        while i < 3 {
            print i;
            i = i + 1;
        }
    )");
    CHECK(out == "0\n1\n2\n");
}

TEST_CASE("for loop") {
    auto out = run_program_capture(R"(
        let sum = 0;
        for let i: int = 0; i < 5; i = i + 1 {
            sum = sum + i;
        }
        print sum;
    )");
    CHECK(out == "10\n");
}

TEST_CASE("scope shadowing") {
    auto out = run_program_capture(R"(
        let x = 1;
        {
            let x = 2;
            print x;
        }
        print x;
    )");
    CHECK(out == "2\n1\n");
}
