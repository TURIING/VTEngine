/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/28 22:31
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHIDevice.h"

#include <core/RHI/RHISemaphore.h>
#include "core/RHI/RHISingleTimeCommandBuffer.h"
#include "core/RHI/RHIInstance.h"
#include "core/RHI/RHISurface.h"
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

RHIDevice::RHIDevice(const std::shared_ptr<RHIInstance> &instance, const std::shared_ptr<RHISurface> &surface, RHIDeviceCreateInfo &createInfo) : m_pInstance(instance), m_pSurface(surface), m_pCreateInfo(&createInfo) {
    this->createPhysicalDevice();
    this->createLogicalDevice();
}

void RHIDevice::Present(const std::shared_ptr<RHISemaphore> &waitSemaphore, const std::shared_ptr<RHISwapChain> &swapChain, uint32_t imageIndex) const {
    VkSemaphore waitSemaphores[] = { waitSemaphore->GetHandle() };
    VkSwapchainKHR swapChains[] = { swapChain->GetHandle() };

    const VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
    };
    vkQueuePresentKHR(m_pPresentQueue, &presentInfo);
}

void RHIDevice::WaitIdle() const {
    LOG_ASSERT(m_pHandle);
    vkDeviceWaitIdle(m_pHandle);
}

bool RHIDevice::checkDeviceSupport(VkPhysicalDevice device) const {
    const auto indices = QueueFamilyIndices::GetQueueFamilyIndices(device, m_pSurface->GetHandle());
    const auto extensionSupported = checkDeviceExtensionSupport(device);

    auto isSwapChainAdequate = false;
    if(extensionSupported) {
        const auto swapChainSupportDetails = SwapChainSupportDetails::GetSwapChainSupportDetails(device, m_pSurface->GetHandle());
        isSwapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportFeatures);

    return indices.isComplete() && extensionSupported && isSwapChainAdequate && supportFeatures.samplerAnisotropy;
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
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_pPhysicalDevice, queueFamilyIndex, m_pSurface->GetHandle(), &presentSupport);
    return presentSupport;
}

void RHIDevice::createPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, nullptr);
    LOG_ASSERT_INFO(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, devices.data());
    for(const auto &device : devices) {
        if(this->checkDeviceSupport(device)) {
            m_pPhysicalDevice = device;
            break;
        }
    }
    LOG_ASSERT_INFO(m_pPhysicalDevice != VK_NULL_HANDLE, "failed to find a suitable GPU!");

    vkGetPhysicalDeviceProperties(m_pPhysicalDevice, &m_physicalGpuInfo);

    LOG_INFO("Physical GPU found, name: {}", m_physicalGpuInfo.deviceName);
}

void RHIDevice::createLogicalDevice() {
    const auto indices = QueueFamilyIndices::GetQueueFamilyIndices(m_pPhysicalDevice, m_pSurface->GetHandle());
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

    VkDeviceCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(m_pCreateInfo->deviceExtensions.size()),
        .ppEnabledExtensionNames = m_pCreateInfo->deviceExtensions.data(),
        .pEnabledFeatures = &m_pCreateInfo->deviceFeatures,
    };
    if (m_pInstance->IsEnableValidationLayers()) {
        createInfo.enabledLayerCount = 1;
        createInfo.ppEnabledLayerNames = &VK_LAYER_KHRONOS_VALIDATION;
    }
    CALL_VK(vkCreateDevice(m_pPhysicalDevice, &createInfo, nullptr, &m_pHandle));
    LOG_INFO("Logical Device created!");

    if(this->checkPresentSupport(m_pPhysicalDevice, indices.graphicsFamily.value())) {
        vkGetDeviceQueue(m_pHandle, indices.graphicsFamily.value(), 0, &m_pGraphicsQueue);
    }
    if(this->checkPresentSupport(m_pPhysicalDevice, indices.presentFamily.value())) {
        vkGetDeviceQueue(m_pHandle, indices.presentFamily.value(), 0, &m_pPresentQueue);
    }
}

VkFormat RHIDevice::GetDepthFormatDetail() const {
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    constexpr VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    for(const auto format : formats) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_pPhysicalDevice, format, &formatProperties);

        if(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = format;
            break;
        }
    }
    return depthFormat;
}

void RHIDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const {
    // 创建缓冲
    VkBufferCreateInfo bufferInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    CALL_VK(vkCreateBuffer(m_pHandle, &bufferInfo, nullptr, &buffer));

    // 获取缓冲的内存需求
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_pHandle, buffer, &memoryRequirements);

    // 分配内存
    const VkMemoryAllocateInfo allocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = this->FindMemoryType(memoryRequirements.memoryTypeBits, properties)
    };
    CALL_VK(vkAllocateMemory(m_pHandle, &allocateInfo, nullptr, &bufferMemory));

    vkBindBufferMemory(m_pHandle, buffer, bufferMemory, 0);
}

uint32_t RHIDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    // 查询物理设备可用的内存类型
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_pPhysicalDevice, &memoryProperties);

    for(auto i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG_CRITICAL("Failed to find suitable memory type.");
}

void RHIDevice::CopyBuffer(const std::shared_ptr<RHICommandPool> &commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    const auto commandBuffer = std::make_unique<RHISingleTimeCommandBuffer>(this, commandPool);
    commandBuffer->CopyBuffer(srcBuffer, dstBuffer, size);
}
