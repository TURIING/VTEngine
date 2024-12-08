set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/vulkan-1.lib)
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
target_link_libraries(${PROJECT_NAME} ${VULKAN_LIB})
