cmake_minimum_required(VERSION 3.29)
project(VTEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/external ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS src/*.h)
add_executable(${PROJECT_NAME} ${HEADER} ${SOURCE})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(${CMAKE_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()
