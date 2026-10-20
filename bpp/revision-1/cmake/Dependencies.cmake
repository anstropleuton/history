function(bpp_resolve_dependency dep_name git_repo git_tag)
    string(TOUPPER "${dep_name}" dep_upper)

    set(path_var  "${dep_upper}_PATH")
    set(find_var  "${dep_upper}_FIND_PATH")
    set(fetch_var "${dep_upper}_FETCH_PATH")

    if(DEFINED ${path_var} AND EXISTS "${${path_var}}/CMakeLists.txt")
        add_subdirectory(
                "${${path_var}}"
                "${CMAKE_BINARY_DIR}/_deps/${dep_name}"
        )
        return()
    endif()

    if(DEFINED ${find_var})
        find_package(${dep_name} CONFIG QUIET PATHS "${${find_var}}")
    endif()

    if(TARGET ${dep_name} OR TARGET ${dep_name}::${dep_name})
        return()
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/${dep_name}/CMakeLists.txt")
        add_subdirectory(
                "${CMAKE_CURRENT_SOURCE_DIR}/deps/${dep_name}"
                "${CMAKE_BINARY_DIR}/_deps/${dep_name}"
        )
        return()
    endif()

    find_package(${dep_name} CONFIG QUIET)

    if(TARGET ${dep_name} OR TARGET ${dep_name}::${dep_name})
        return()
    endif()

    if(NOT git_repo OR NOT git_tag)
        message(FATAL_ERROR
                "Dependency '${dep_name}' not found. "
                "Provide ${dep_upper}_PATH, ${dep_upper}_FIND_PATH, "
                "deps/${dep_name}, or git_repo/git_tag."
        )
    endif()

    if(DEFINED ${fetch_var})
        set(_bpp_old_fetch_base_dir "${FETCHCONTENT_BASE_DIR}")
        set(FETCHCONTENT_BASE_DIR "${${fetch_var}}")
    endif()

    FetchContent_Declare(${dep_name}
            GIT_REPOSITORY "${git_repo}"
            GIT_TAG "${git_tag}"
            GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(${dep_name})

    if(DEFINED ${fetch_var})
        set(FETCHCONTENT_BASE_DIR "${_bpp_old_fetch_base_dir}")
    endif()
endfunction()

function(bpp_resolve_noncmake_dependency dep_name git_repo git_tag out_root_var)
    string(TOUPPER "${dep_name}" dep_upper)

    set(path_var  "${dep_upper}_PATH")
    set(fetch_var "${dep_upper}_FETCH_PATH")

    if(DEFINED ${path_var} AND EXISTS "${${path_var}}")
        set(${out_root_var} "${${path_var}}" PARENT_SCOPE)
        return()
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/${dep_name}")
        set(${out_root_var} "${CMAKE_CURRENT_SOURCE_DIR}/deps/${dep_name}" PARENT_SCOPE)
        return()
    endif()

    if(DEFINED ${fetch_var})
        set(_bpp_fetch_base "${${fetch_var}}")
    else()
        set(_bpp_fetch_base "${CMAKE_BINARY_DIR}/_deps")
    endif()

    include(FetchContent)

    FetchContent_Declare(${dep_name}
            GIT_REPOSITORY "${git_repo}"
            GIT_TAG        "${git_tag}"
            SOURCE_DIR     "${_bpp_fetch_base}/${dep_name}/src"
            BINARY_DIR     "${_bpp_fetch_base}/${dep_name}/build"
    )

    FetchContent_GetProperties(${dep_name})
    if(NOT ${dep_name}_POPULATED)
        FetchContent_Populate(${dep_name})
    endif()

    set(${out_root_var} "${${dep_name}_SOURCE_DIR}" PARENT_SCOPE)
endfunction()
