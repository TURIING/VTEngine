//
// Created by turiing on 2025/1/8.
//

#include "core/RHI/Builder/RHIDescriptorSetLayoutBuilder.h"

RHIDescriptorSetLayoutBuilder::RHIDescriptorSetLayoutBuilder(const std::shared_ptr<RHIDevice>& device) : m_pDevice(device) {
}

RHIDescriptorSetLayoutBuilder& RHIDescriptorSetLayoutBuilder::AddDescriptorBinding(RHIDescriptorSetLayoutBindInfo&& bindInfo) {
    m_createInfo.descriptorSetLayoutBindings.push_back(std::move(bindInfo));
    return *this;
}

std::shared_ptr<RHIDescriptorSetLayout> RHIDescriptorSetLayoutBuilder::Build() {
    return std::make_shared<RHIDescriptorSetLayout>(m_pDevice, m_createInfo);
}
