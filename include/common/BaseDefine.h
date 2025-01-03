/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 21:45
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef BASEDEFINE_H
#define BASEDEFINE_H

#include "common.h"

/************************************************* Type *************************************************************/

struct Version {
    int major;
    int minor;
    int patch;
};

struct Size {
    uint32_t width = 0;
    uint32_t height = 0;
};

struct PlatformWindowInfo {
    void *handle = nullptr;
    Size size;
};

enum class MouseButton { Left, Middle, Right };

struct Point {
    int x = 0;
    int y = 0;
};

/************************************************* Marco ************************************************************/
#define BIT(x) (1 << (x))

/************************************************* Variable *********************************************************/

constexpr auto APP_NAME = "VTEngine";
constexpr Version APP_VERSION = { 1, 0, 0 };
constexpr Size WINDOW_SIZE = { 1200, 1000 };
inline constexpr float FRAME_TIME = 1/60.0;                                                                         // 每秒里每帧的耗时

constexpr bool ENABLE_VALIDATION_LAYERS = true;                                                                     // 是否开启校验层
const std::vector<const char*> REQUIRE_VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };
constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

#if PLATFORM_WINDOWS
const std::vector<const char*> REQUIRE_INSTANCE_EXT = { "VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
const std::vector<const char*> REQUIRE_DEVICE_EXTENSION = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#elif PLATFORM_MACOS
const std::vector<const char*> REQUIRE_INSTANCE_EXT = { "VK_KHR_surface", "VK_MVK_macos_surface", VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
const std::vector<const char*> REQUIRE_DEVICE_EXTENSION = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset" };
#endif

#endif //BASEDEFINE_H
