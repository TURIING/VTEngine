/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 22:30
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIINSTANCE_H
#define RHIINSTANCE_H

#include "RHIObject.h"
#include "common/common.h"

struct RHIInstanceCreateInfo {
    bool enableValidationLayers = false;
    std::vector<const char *> instanceExtensions;
    std::vector<const char *> layers;
};

class RHIInstance : public RHIObject<VkInstance>{
public:
    explicit RHIInstance(const RHIInstanceCreateInfo &createInfo);
    ~RHIInstance();
    [[nodiscard]] bool IsEnableValidationLayers() const { return m_createInfo.enableValidationLayers; }

private:
    void createInstance();
    [[nodiscard]] bool checkValidationLayerSupport() const;
    void setupDebugMessenger();
    static VkResult createDebugUtilsMessengerExt(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);
    static void destroyDebugUtilsMessengerExt(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *);

private:
    VkDebugUtilsMessengerEXT m_pDebugMessenger = nullptr;
    VkSurfaceKHR m_pSurface = nullptr;
    RHIInstanceCreateInfo m_createInfo {};
};



#endif //RHIINSTANCE_H
