//
// Created by turiing on 2025/1/8.
//

#ifndef RHIDEVICEBUILDER_H
#define RHIDEVICEBUILDER_H

#include "common/common.h"
#include "core/RHI/RHIDevice.h"

class RHISurface;
class RHIInstance;

class RHIDeviceBuilder {
public:
    RHIDeviceBuilder(const std::shared_ptr<RHIInstance> &instance, const std::shared_ptr<RHISurface> &surface);
    RHIDeviceBuilder& SetDeviceExtension(const std::vector<const char *> &extensions);
    RHIDeviceBuilder& SetPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures &features);
    std::shared_ptr<RHIDevice> Build();

private:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    RHIDeviceCreateInfo m_createInfo {};
};
#endif //RHIDEVICEBUILDER_H
