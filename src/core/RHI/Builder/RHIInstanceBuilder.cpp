//
// Created by turiing on 2025/1/7.
//

#include "core/RHI/Builder/RHIInstanceBuilder.h"

#include "core/RHI/RHIInstance.h"

RHIInstanceBuilder& RHIInstanceBuilder::SetEnableValidationLayer(bool on) {
    m_createInfo.enableValidationLayers = on;
    m_createInfo.layers.push_back("VK_LAYER_KHRONOS_validation");
    m_createInfo.instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return *this;
}

RHIInstanceBuilder& RHIInstanceBuilder::SetInstanceExtensions(const std::vector<const char*>& extensions) {
    m_createInfo.instanceExtensions = extensions;
    return *this;
}

RHIInstanceBuilder& RHIInstanceBuilder::SetLayer(const std::vector<const char*>& layers) {
    if(m_createInfo.layers.empty()) {
        m_createInfo.layers = layers;
    }
    else {
        m_createInfo.layers.insert(m_createInfo.layers.end(), layers.begin(), layers.end());
    }
    return *this;
}

std::shared_ptr<RHIInstance> RHIInstanceBuilder::Build() {
    return std::make_shared<RHIInstance>(m_createInfo);
}
