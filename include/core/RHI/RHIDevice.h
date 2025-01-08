/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/28 22:31
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIDEVICE_H
#define RHIDEVICE_H

#include "common/common.h"

class RHISurface;
class RHISwapChain;
class RHISemaphore;
class RHIInstance;
class RHICommandPool;

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

struct RHIDeviceCreateInfo {
    std::vector<const char*> deviceExtensions;
    VkPhysicalDeviceFeatures deviceFeatures;
};

class RHIDevice {
public:
    explicit RHIDevice(const std::shared_ptr<RHIInstance> &instance, const std::shared_ptr<RHISurface> &surface, RHIDeviceCreateInfo &createInfo);
    [[nodiscard]] VkPhysicalDevice GetPhysicalDeviceHandle() const { return m_pPhysicalDevice; }
    [[nodiscard]] VkDevice GetLogicalDeviceHandle() const { return m_pLogicalDevice; }
    [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_pGraphicsQueue; }
    [[nodiscard]] VkPhysicalDeviceProperties GetPhysicalGpuInfo() const { return m_physicalGpuInfo; }
    [[nodiscard]] VkFormat GetDepthFormatDetail() const;
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory) const;
    void CopyBuffer(const std::shared_ptr<RHICommandPool> &commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void Present(const std::shared_ptr<RHISemaphore>& waitSemaphore, const std::shared_ptr<RHISwapChain> &swapChain, uint32_t imageIndex) const;
    void WaitIdle() const;
    [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

private:
    [[nodiscard]] bool checkDeviceSupport(VkPhysicalDevice device) const;
    [[nodiscard]] static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    [[nodiscard]] bool checkPresentSupport(VkPhysicalDevice device, uint32_t queueFamilyIndex) const;
    void createPhysicalDevice();
    void createLogicalDevice();

private:
    std::shared_ptr<RHIInstance> m_pInstance = nullptr;
    std::shared_ptr<RHISurface> m_pSurface = nullptr;
    VkPhysicalDevice m_pPhysicalDevice = nullptr;
    VkDevice m_pLogicalDevice = nullptr;
    VkQueue m_pGraphicsQueue = nullptr;
    VkQueue m_pPresentQueue = nullptr;
    VkPhysicalDeviceProperties m_physicalGpuInfo {};
    RHIDeviceCreateInfo *m_pCreateInfo = nullptr;
};


#endif //RHIDEVICE_H
