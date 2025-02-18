cmake_minimum_required(VERSION 3.29)
project(VTEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SHADER_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/msvc2022_64")
    set(SHADER_COMPILER "C:/VulkanSDK/1.3.280.0/Bin/glslangValidator.exe")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_PREFIX_PATH "/Users/turiing/Qt/6.8.1/macos/lib/cmake")
    set(SHADER_COMPILER "/Users/turiing/VulkanSDK/1.3.296.0/macOS/bin/glslangValidator")
endif ()

include(cmake/common.cmake)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/external ${CMAKE_SOURCE_DIR}/include)

COMPILE_ALL_SHADER(shader/ ${SHADER_OUTPUT_DIR})

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.cpp src/*.ui)
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS include/*.h)
add_executable(${PROJECT_NAME} ${HEADER} ${SOURCE})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(${CMAKE_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()

# 复制资源文件夹到程序目录
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying directory ${SOURCE_FOLDER} to ${DESTINATION_FOLDER}"
)