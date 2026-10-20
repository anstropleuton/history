set(CYRINE_EXAMPLES
)

foreach(EXAMPLE ${CYRINE_EXAMPLES})
    set(EXAMPLE_TARGET cyrine_${EXAMPLE}_example)
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE_TARGET} PRIVATE cyrine)
    target_include_directories(${EXAMPLE_TARGET} PRIVATE ${CYRINE_SOURCE_DIR}/example)
endforeach()
