//
// Created by turiing on 2024/12/29.
//

#include "core/RHI/RHIDescriptorPool.h"

#include "core/RHI/RHIDevice.h"

constexpr uint32_t DESCRIPTOR_POOL_MAX_SET_COUNT = 100;

RHIDescriptorPool::RHIDescriptorPool(const std::shared_ptr<RHIDevice>& device): m_pDevice(device) {
    VkDescriptorPoolSize poolSize {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1000,
    };

    const VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = DESCRIPTOR_POOL_MAX_SET_COUNT,
        .poolSizeCount = 1,
        .pPoolSizes = &poolSize,
    };
    CALL_VK(vkCreateDescriptorPool(m_pDevice->GetLogicalDeviceHandle(), &poolCreateInfo, nullptr, &m_pDescriptorPool));
    LOG_INFO("Created descriptor pool");
}

RHIDescriptorPool::~RHIDescriptorPool() {
    vkDestroyDescriptorPool(m_pDevice->GetLogicalDeviceHandle(), m_pDescriptorPool, nullptr);
}