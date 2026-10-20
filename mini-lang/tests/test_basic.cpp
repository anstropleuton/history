#include <doctest/doctest.h>
#include "test_util.h"

TEST_CASE("print literals and simple expressions") {
    auto out = run_program_capture(R"(
        print 1;
        print 2.5;
        print true;
        print "hello";
    )");
    CHECK(out == "1\n2.5\ntrue\nhello\n");
}

TEST_CASE("variable declaration and assignment") {
    auto out = run_program_capture(R"(
        let x: int = 5;
        let y = x + 2;
        print y;
    )");
    CHECK(out == "7\n");
}

TEST_CASE("string concatenation") {
    auto out = run_program_capture(R"(
        print "a" + "b";
    )");
    CHECK(out == "ab\n");
}
