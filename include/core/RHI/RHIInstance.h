/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 22:30
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIINSTANCE_H
#define RHIINSTANCE_H

#include "common/common.h"

class RHIInstance {
public:
    explicit RHIInstance();
    ~RHIInstance();
    [[nodiscard]] VkInstance GetHandle() const;

private:
    void createInstance();
    [[nodiscard]] static bool checkValidationLayerSupport();
    void setupDebugMessenger();
    static VkResult createDebugUtilsMessengerExt(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);
    static void destroyDebugUtilsMessengerExt(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *);

private:
    VkInstance m_pInstance = nullptr;
    VkDebugUtilsMessengerEXT m_pDebugMessenger = nullptr;
    VkSurfaceKHR m_pSurface = nullptr;
};



#endif //RHIINSTANCE_H
