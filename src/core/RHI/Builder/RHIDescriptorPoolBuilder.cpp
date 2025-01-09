//
// Created by turiing on 2025/1/9.
//

#include "core/RHI/Builder/RHIDescriptorPoolBuilder.h"

RHIDescriptorPoolBuilder::RHIDescriptorPoolBuilder(const std::shared_ptr<RHIDevice>& device): m_pDevice(device) {
}

RHIDescriptorPoolBuilder& RHIDescriptorPoolBuilder::AddPoolSize(RHIDescriptorPoolSizeInfo&& info) {
    m_createInfo.poolSizes.emplace_back(std::move(info));
    return *this;
}

std::shared_ptr<RHIDescriptorPool> RHIDescriptorPoolBuilder::Build() {
    return std::make_shared<RHIDescriptorPool>(m_pDevice, m_createInfo);
}
