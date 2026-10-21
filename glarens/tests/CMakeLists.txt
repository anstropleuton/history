include(FetchContent)

find_package(doctest REQUIRED)

file(GLOB TEST_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach(test_dir ${TEST_DIRS})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${test_dir}")
        file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
            ${test_dir}/*.cpp
        )

        if (TEST_SOURCES)
            set(test_target "glarens_test_${test_dir}")

            add_executable(${test_target} ${TEST_SOURCES})

            target_include_directories(${test_target}
                PRIVATE
                    ${test_dir}
            )

            target_link_libraries(${test_target}
                PRIVATE
                    glarens::glarens
                    doctest::doctest
            )

            add_test(
                NAME ${test_target}
                COMMAND ${test_target}
            )

            set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${test_dir}/assets")

            if (EXISTS "${ASSETS_SRC_DIR}")
                add_custom_command(
                    TARGET ${test_target}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                            "${ASSETS_SRC_DIR}"
                            "$<TARGET_FILE_DIR:${test_target}>/assets"
                    COMMENT "Copying assets for ${test_target}"
                )

                file(GLOB_RECURSE SHADERS
                    "${ASSETS_DST_DIR}/*.hlsl"
                )

                foreach(shader ${SHADERS})
                    compile_shader("${shader}")
                endforeach()
            endif()
        endif()
    endif()
endforeach()
