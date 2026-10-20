file(GLOB_RECURSE RESOURCE_FILES
        RELATIVE "${RESOURCE_ROOT}"
        "${RESOURCE_ROOT}/*"
)

file(WRITE "${OUTPUT_FILE}" "#pragma once\n\n")
file(APPEND "${OUTPUT_FILE}" "namespace bpp::res {\n")

foreach (FILE ${RESOURCE_FILES})

    get_filename_component(EXT "${FILE}" EXT)

    if (NOT EXT STREQUAL "")
        get_filename_component(NAME "${FILE}" NAME_WE)

        string(TOLOWER "${NAME}" FUNC)

        string(REGEX REPLACE "[^a-z0-9]" "_" FUNC "${FUNC}")
        string(REGEX REPLACE "_+" "_" FUNC "${FUNC}")

        file(APPEND "${OUTPUT_FILE}" "    constexpr auto get_${FUNC}() noexcept -> const char * {\n        return \"./res/${FILE}\";\n    }\n\n")
    endif ()

endforeach ()

file(APPEND "${OUTPUT_FILE}" "} // namespace bpp::res\n")
