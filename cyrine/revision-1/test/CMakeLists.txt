include("${CYRINE_SOURCE_DIR}/cmake/subdep.cmake")

subdep_add(doctest)

set(CYRINE_TESTS
)

foreach(TEST ${CYRINE_TESTS})
    set(TEST_TARGET cyrine_${TEST}_test)
    add_executable(${TEST_TARGET} ${TEST}.cpp)
    target_link_libraries(${TEST_TARGET} PRIVATE cyrine doctest::doctest)
    target_include_directories(${TEST_TARGET} PRIVATE ${CYRINE_SOURCE_DIR}/test)
    add_test(NAME ${TEST} COMMAND ${TEST_TARGET})
endforeach()
