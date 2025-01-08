//
// Created by turiing on 2025/1/8.
//

#include "core/RHI/Builder/RHIDeviceBuilder.h"

RHIDeviceBuilder::RHIDeviceBuilder(const std::shared_ptr<RHIInstance>& instance, const std::shared_ptr<RHISurface>& surface): m_pInstance(instance), m_pSurface(surface) {

}

RHIDeviceBuilder& RHIDeviceBuilder::SetDeviceExtension(const std::vector<const char*>& extensions) {
    m_createInfo.deviceExtensions = extensions;
    return *this;
}

RHIDeviceBuilder& RHIDeviceBuilder::SetPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features) {
    m_createInfo.deviceFeatures = features;
    return *this;
}

std::shared_ptr<RHIDevice> RHIDeviceBuilder::Build() {
    return std::make_shared<RHIDevice>(m_pInstance, m_pSurface, m_createInfo);
}
