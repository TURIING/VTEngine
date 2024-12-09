cmake_minimum_required(VERSION 3.29)
project(VTEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(SHADER_COMPILER "C:/VulkanSDK/1.3.280.0/Bin/glslangValidator.exe")
set(SHADER_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader)

include(cmake/common.cmake)

include_directories(${CMAKE_SOURCE_DIR}/external ${CMAKE_SOURCE_DIR}/include)

COMPILE_ALL_SHADER(shader/ ${SHADER_OUTPUT_DIR})

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS src/*.h)
add_executable(${PROJECT_NAME} ${HEADER} ${SOURCE})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(${CMAKE_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()

