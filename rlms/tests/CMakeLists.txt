set(MINESWEEPER_TESTS
    "${CMAKE_CURRENT_SOURCE_DIR}/tester.cpp"
)

add_executable(minesweeper_tester ${MINESWEEPER_TESTS})
target_include_directories(minesweeper_tester PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(minesweeper_tester PRIVATE minesweeper_lib)
target_link_libraries(minesweeper_tester PRIVATE confer)
