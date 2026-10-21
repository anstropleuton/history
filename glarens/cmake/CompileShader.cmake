find_program(SH_COMPILER NAMES shadercross shadercross.exe)
if(NOT SH_COMPILER)
    message(FATAL_ERROR "shadercross not found")
endif()

function(compile_shader)
    cmake_parse_arguments(CS "" "TARGET;SOURCE;DEST" "" ${ARGN})

    if(NOT CS_TARGET OR NOT CS_SOURCE OR NOT CS_DEST)
        message(FATAL_ERROR "compile_shader requires TARGET, SOURCE, and DEST")
    endif()

    get_filename_component(shader_src "${CS_SOURCE}" ABSOLUTE)

    if(shader_src MATCHES "\\.vert\\.hlsl$")
        set(stage "vertex")
    elseif(shader_src MATCHES "\\.frag\\.hlsl$")
        set(stage "fragment")
    else()
        message(FATAL_ERROR
            "compile_shader: shader must end with .vert.hlsl or .frag.hlsl: ${CS_SOURCE}")
    endif()

    set(out_spv  "${CS_DEST}.spv")
    set(out_msl  "${CS_DEST}.msl")
    set(out_dxil "${CS_DEST}.dxil")

    get_filename_component(out_dir "${out_spv}" DIRECTORY)
    if(NOT IS_DIRECTORY "${out_dir}")
        file(MAKE_DIRECTORY "${out_dir}")
    endif()

    add_custom_command(
        OUTPUT "${out_spv}"
        COMMAND "${SH_COMPILER}" "${shader_src}" -t ${stage} -d SPIRV -o "${out_spv}"
        DEPENDS "${shader_src}"
        COMMENT "Compiling shader ${shader_src} -> ${out_spv}"
        VERBATIM
    )

    add_custom_command(
        OUTPUT "${out_msl}"
        COMMAND "${SH_COMPILER}" "${shader_src}" -t ${stage} -d MSL -o "${out_msl}"
        DEPENDS "${shader_src}"
        COMMENT "Compiling shader ${shader_src} -> ${out_msl}"
        VERBATIM
    )

    add_custom_command(
        OUTPUT "${out_dxil}"
        COMMAND "${SH_COMPILER}" "${shader_src}" -t ${stage} -d DXIL -o "${out_dxil}"
        DEPENDS "${shader_src}"
        COMMENT "Compiling shader ${shader_src} -> ${out_dxil}"
        VERBATIM
    )

    get_filename_component(shader_file "${CS_SOURCE}" NAME) # e.g. foo.vert.hlsl
    string(REPLACE ".vert.hlsl" "_vert" shader_file "${shader_file}")
    string(REPLACE ".frag.hlsl" "_frag" shader_file "${shader_file}")
    set(shader_target_name "${CS_TARGET}_shader_${shader_file}")

    if(NOT TARGET "${shader_target_name}")
        add_custom_target("${shader_target_name}"
            DEPENDS "${out_spv}" "${out_msl}" "${out_dxil}"
        )
    else()
        add_dependencies("${shader_target_name}" "${out_spv}" "${out_msl}" "${out_dxil}")
    endif()
endfunction()
