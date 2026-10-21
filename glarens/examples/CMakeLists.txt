file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach(example_dir ${EXAMPLE_DIRS})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}")
        file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
            ${example_dir}/*.cpp
        )

        if (EXAMPLE_SOURCES)
            set(example_target "glarens_example_${example_dir}")

            add_executable(${example_target} ${EXAMPLE_SOURCES})

            target_include_directories(${example_target}
                PRIVATE
                    ${example_dir}
            )

            target_link_libraries(${example_target}
                PRIVATE
                    glarens::glarens
            )

            set(example_out_dir "${CMAKE_CURRENT_BINARY_DIR}/${example_dir}")

            set_target_properties(${example_target} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${example_out_dir}"
                LIBRARY_OUTPUT_DIRECTORY "${example_out_dir}"
                ARCHIVE_OUTPUT_DIRECTORY "${example_out_dir}"
            )

            set(SOURCE_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}/assets")
            set(BUILD_ASSETS_DIR  "${CMAKE_CURRENT_BINARY_DIR}/${example_dir}/assets")

            if (EXISTS "${SOURCE_ASSETS_DIR}")
                # build-tree assets dir (we compile shaders into here)
                file(MAKE_DIRECTORY "${BUILD_ASSETS_DIR}")

                # copy source assets into build assets dir (this is a separate custom target)
                add_custom_target("${example_target}_copy_assets"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${SOURCE_ASSETS_DIR}"
                        "${BUILD_ASSETS_DIR}"
                    COMMENT "Preparing assets for ${example_target}"
                )

                # find shader sources in the source assets folder
                file(GLOB_RECURSE SHADERS
                    "${SOURCE_ASSETS_DIR}/*.vert.hlsl"
                    "${SOURCE_ASSETS_DIR}/*.frag.hlsl"
                )

                # collect all per-shader target names for this example
                set(_example_shader_targets)

                foreach(shader_src ${SHADERS})
                    file(RELATIVE_PATH shader_rel "${SOURCE_ASSETS_DIR}" "${shader_src}")
                    string(REGEX REPLACE "\\.hlsl$" "" shader_rel_base "${shader_rel}")
                    set(shader_dest "${BUILD_ASSETS_DIR}/${shader_rel_base}")

                    # ensure dest dir exists
                    get_filename_component(sd_dir "${shader_dest}" DIRECTORY)
                    file(MAKE_DIRECTORY "${sd_dir}")

                    # call compile_shader to create per-shader target and outputs in build tree
                    compile_shader(
                        TARGET ${example_target}
                        SOURCE "${shader_src}"
                        DEST   "${shader_dest}"
                    )

                    # compute the deterministic shader target name (must match compile_shader's naming)
                    get_filename_component(shader_file "${shader_src}" NAME) # e.g. foo.vert.hlsl
                    string(REPLACE ".vert.hlsl" "_vert" shader_file "${shader_file}")
                    string(REPLACE ".frag.hlsl" "_frag" shader_file "${shader_file}")
                    set(_shader_target "${example_target}_shader_${shader_file}")

                    list(APPEND _example_shader_targets "${_shader_target}")
                endforeach()

                # create a single per-example assets target that depends on:
                #  - copying source assets into build assets
                #  - building all per-shader targets (so shader compilation runs)
                add_custom_target("${example_target}_assets"
                    DEPENDS "${example_target}_copy_assets" ${_example_shader_targets}
                    COMMENT "Assembling assets (copy + compile shaders) for ${example_target}"
                )

                # make the example executable depend on the assets target so building the exe
                # will prepare/compile assets first
                add_dependencies(${example_target} "${example_target}_assets")

                # After the example is built, copy the build assets (including compiled shaders) into the runtime dir
                add_custom_command(TARGET ${example_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${example_target}>/assets"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${BUILD_ASSETS_DIR}"
                        "$<TARGET_FILE_DIR:${example_target}>/assets"
                    COMMENT "Copying built assets (including compiled shaders) for ${example_target}"
                )
            endif()
        endif()
    endif()
endforeach()
