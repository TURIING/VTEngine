/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 22:30
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHIInstance.h"

VkBool32 VKAPI_CALL debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                  const VkDebugUtilsMessengerCallbackDataEXT *pCallBackData,
                                  void *pUserData) {
    if (messageServerity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LOG_WARNING("Vulkan validation layer: {}", pCallBackData->pMessage);
    }
    else if (messageServerity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        LOG_CRITICAL("Vulkan validation layer: {}", pCallBackData->pMessage);
    }
    return VK_FALSE;
}

RHIInstance::RHIInstance() {
    this->createInstance();
    this->setupDebugMessenger();
}

RHIInstance::~RHIInstance() {
    LOG_ASSERT(m_pInstance != nullptr);

    if constexpr(ENABLE_VALIDATION_LAYERS) {
        destroyDebugUtilsMessengerExt(m_pInstance, m_pDebugMessenger, nullptr);
    }

    vkDestroyInstance(m_pInstance, nullptr);
}

VkInstance RHIInstance::GetHandle() const {
    return m_pInstance;
}

void RHIInstance::createInstance() {
    if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
        LOG_CRITICAL("validation layers requested, but not available!");
    }

    if constexpr (ENABLE_VALIDATION_LAYERS) {
        // REQUIRE_INSTANCE_EXT.push_back();
    }

    // appInfo
    auto [major, minor, patch] = APP_VERSION;
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = APP_NAME,
        .applicationVersion = VK_MAKE_VERSION(major, minor, patch),
        .pEngineName = APP_NAME,
        .engineVersion = VK_MAKE_VERSION(major, minor, patch),
        .apiVersion = VK_API_VERSION_1_3,
    };


    // createInfo
    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if PLATFORM_MACOS
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
#endif
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = static_cast<uint32_t>(REQUIRE_INSTANCE_EXT.size()),
        .ppEnabledExtensionNames = REQUIRE_INSTANCE_EXT.data()
    };
    if(ENABLE_VALIDATION_LAYERS) {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            // .pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallBack,
            .pUserData = nullptr,
        };

        createInfo.enabledLayerCount = static_cast<uint32_t>(REQUIRE_VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = REQUIRE_VALIDATION_LAYERS.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    CALL_VK(vkCreateInstance(&createInfo, nullptr, &m_pInstance));
    LOG_INFO("Instance created");
}

bool RHIInstance::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : REQUIRE_VALIDATION_LAYERS) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if(!layerFound) {
            return false;
        }
    }

    return true;
}

void RHIInstance::setupDebugMessenger() {
    if constexpr (!ENABLE_VALIDATION_LAYERS) return;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallBack,
        .pUserData = nullptr,
    };

    CALL_VK(createDebugUtilsMessengerExt(m_pInstance, &debugCreateInfo, nullptr, &m_pDebugMessenger));
}

/**
 * 载入vkCreateDebugUtilsMessengerEXT函数
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pDebugMessenger
 * @return
 */
VkResult RHIInstance::createDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                        const VkAllocationCallbacks *pAllocator,
                                        VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void RHIInstance::destroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}