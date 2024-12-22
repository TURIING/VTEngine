add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
add_definitions(-DPLATFORM_MACOS)

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets ${VULKAN_LIB})