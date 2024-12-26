add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
add_definitions(-DPLATFORM_MACOS)

set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/libvulkan.1.3.296.dylib)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets ${VULKAN_LIB})