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

class RHIInstance;

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

class RHIDevice {
public:
    explicit RHIDevice(std::shared_ptr<RHIInstance> instance);
    [[nodiscard]] vkPhysicalDevice GetPhysicalDeviceHandle() const { return m_pPhysicalDevice; }
    [[nodiscard]] VkDevice GetLogicalDeviceHandle() const { return m_pLogicalDevice; }

private:
    [[nodiscard]] bool checkDeviceSupport(VkPhysicalDevice device) const;
    [[nodiscard]] static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    [[nodiscard]] bool checkPresentSupport(VkPhysicalDevice device, uint32_t queueFamilyIndex) const;
    void createPhysicalDevice();
    void createLogicalDevice();

private:
    std::shared_ptr<RHIInstance> m_pInstance = nullptr;
    VkPhysicalDevice m_pPhysicalDevice = nullptr;
    VkDevice m_pLogicalDevice = nullptr;
    VkQueue m_pGraphicsQueue = nullptr;
    VkQueue m_pPresentQueue = nullptr;
};


#endif //RHIDEVICE_H
