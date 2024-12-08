cmake_minimum_required(VERSION 3.29)
project(VTEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.h src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

if(CMAKE_SYSTEM_NAME STREQUAL "windows")
    include(${CMAKE_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()