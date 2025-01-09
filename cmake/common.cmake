function(COMPILE_SHADER shader_file output_dir)
    # 获取文件的扩展名（例如，.vert 或 .frag）
    get_filename_component(shader_name ${shader_file} EXT)
    string(SUBSTRING ${shader_name} 1 -1 shader_name)

    # 提取最近一级的目录名
    get_filename_component(DIR_NAME ${shader_file} DIRECTORY)
    get_filename_component(PARENT_DIR "${DIR_NAME}" NAME)

    # 构建输出文件路径
    set(output_file "${output_dir}/${PARENT_DIR}/${shader_name}.spv")

    # 检查输出目录是否存在，如果不存在则创建
    if(NOT EXISTS ${output_dir}/${PARENT_DIR})
        file(MAKE_DIRECTORY ${output_dir}/${PARENT_DIR})
    endif()

    # 调用 glslangValidator 编译 GLSL 文件
    execute_process(
            COMMAND ${SHADER_COMPILER} -V ${shader_file} -o ${output_file}
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
            ERROR_VARIABLE error
    )

    # 检查命令是否成功
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to compile shader: ${shader_file}\nError: ${output}")
    else()
        message(STATUS "Shader compiled successfully: ${shader_file} -> ${output_file}")
    endif()
endfunction()

# 遍历 shader 目录及其子目录，编译所有 GLSL 文件
function(COMPILE_ALL_SHADER input_dir output_dir)
    # 获取所有的 .vert 和 .frag 文件（包括子目录）
    file(GLOB_RECURSE shaders "${input_dir}*.vert" "${input_dir}*.frag")

    # 遍历所有 GLSL 文件，调用 compile_shader 函数进行编译
    foreach(shader IN LISTS shaders)
        compile_shader(${shader} ${output_dir})
    endforeach()
endfunction()
