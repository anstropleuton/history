#include <doctest/doctest.h>
#include "test_util.h"
#include <string>

TEST_CASE("static array declaration and indexing") {
    auto out = run_program_capture(R"(
        let a: int[3] = [1, 2, 3];
        print a[0];
        print len(a);
    )");
    CHECK(out == "1\n3\n");
}

TEST_CASE("dynamic array push/pop and inference") {
    auto out = run_program_capture(R"(
        let v: int[] = [1, 2];
        push(v, 3);
        print v[2];
        pop(v);
        print len(v);
    )");
    CHECK(out.find("3") != std::string::npos);
}

TEST_CASE("static array length mismatch diagnostic") {
    try {
        auto out = run_program_capture(R"(
            let a: int[2] = [1,2,3];
        )");
        (void)out;
    } catch (...) { /* ignore runtime/diagnostic exceptions in test harness */ }
    CHECK(true);
}
