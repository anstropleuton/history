add_library(cyrine
    cyrine.cpp
)
target_include_directories(cyrine PUBLIC
    $<BUILD_INTERFACE:${CYRINE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CYRINE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(cyrine PUBLIC cxx_std_23)

install(TARGETS cyrine
    EXPORT cyrine_EXPORT
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY "${CYRINE_SOURCE_DIR}/include" DESTINATION "include")
install(EXPORT cyrine_EXPORT
    FILE "cyrine-config.cmake"
    NAMESPACE cyrine::
    DESTINATION "lib/cmake/cyrine"
)
export(TARGETS cyrine
    FILE "${CYRINE_BINARY_DIR}/cyrine_export.cmake"
    NAMESPACE cyrine::
)

configure_file(
    "${CYRINE_SOURCE_DIR}/cmake/cyrine.pc.in"
    "${CYRINE_BINARY_DIR}/cmake/cyrine.pc"
    @ONLY
)
install(FILES "${CYRINE_BINARY_DIR}/cmake/cyrine.pc" DESTINATION "lib/pkgconfig")

include("${CYRINE_SOURCE_DIR}/cmake/subdep.cmake")

subdep_add(optrone)

add_executable(cyrine_executable
    main.cpp
)
target_link_libraries(cyrine_executable PRIVATE cyrine optrone)

install(TARGETS cyrine_executable
    RUNTIME DESTINATION bin
)
