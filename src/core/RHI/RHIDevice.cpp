/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/28 22:31
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIInstance.h"
#include "core/RHI/RHISwapChain.h"


QueueFamilyIndices QueueFamilyIndices::GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for(int familyIndex = 0; const auto &queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = familyIndex;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupport);
        if(presentSupport) indices.presentFamily = familyIndex;

        if(indices.isComplete()) break;

        familyIndex++;
    }
    return indices;
}

RHIDevice::RHIDevice(std::shared_ptr<RHIInstance> instance) : m_pInstance(std::move(instance)){
    this->createPhysicalDevice();
}

bool RHIDevice::checkDeviceSupport(VkPhysicalDevice device) const {
    const auto indices = QueueFamilyIndices::GetQueueFamilyIndices(device, m_pInstance->GetSurface());
    const auto extensionSupported = checkDeviceExtensionSupport(device);

    auto isSwapChainAdequate = false;
    if(extensionSupported) {
        const auto swapChainSupportDetails = SwapChainSupportDetails::GetSwapChainSupportDetails(device, m_pInstance->GetSurface());
        isSwapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }

    return indices.isComplete() && extensionSupported && isSwapChainAdequate;
}

bool RHIDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(REQUIRE_DEVICE_EXTENSION.begin(), REQUIRE_DEVICE_EXTENSION.end());
    for(const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool RHIDevice::checkPresentSupport(VkPhysicalDevice device, uint32_t queueFamilyIndex) const {
    LOG_ASSERT(m_pInstance->GetSurface() != nullptr);

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_pPhysicalDevice, queueFamilyIndex, m_pInstance->GetSurface(), &presentSupport);
    return presentSupport;
}

void RHIDevice::createPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, nullptr);
    LOG_ASSERT_INFO(deviceCount != 0, "failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, devices.data());
    for(const auto &device : devices) {
        if(this->checkDeviceSupport(device)) {
            m_pPhysicalDevice = device;
            break;
        }
    }
    LOG_ASSERT_INFO(m_pPhysicalDevice != VK_NULL_HANDLE, "failed to find a suitable GPU!");
}

void RHIDevice::createLogicalDevice() {
    const auto indices = QueueFamilyIndices::GetQueueFamilyIndices(m_pPhysicalDevice, m_pInstance->GetSurface());
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for(const auto queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(REQUIRE_DEVICE_EXTENSION.size()),
        .ppEnabledExtensionNames = REQUIRE_DEVICE_EXTENSION.data(),
        .pEnabledFeatures = &deviceFeatures,
    };
    if constexpr(ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = 1;
        createInfo.ppEnabledLayerNames = &VK_LAYER_KHRONOS_VALIDATION;
    }
    CALL_VK(vkCreateDevice(m_pPhysicalDevice, &createInfo, nullptr, &m_pLogicalDevice));

    if(this->checkPresentSupport(m_pPhysicalDevice, indices.graphicsFamily.value())) {
        vkGetDeviceQueue(m_pLogicalDevice, indices.graphicsFamily.value(), 0, &m_pGraphicsQueue);
    }
    if(this->checkPresentSupport(m_pPhysicalDevice, indices.presentFamily.value())) {
        vkGetDeviceQueue(m_pLogicalDevice, indices.presentFamily.value(), 0, &m_pPresentQueue);
    }
}