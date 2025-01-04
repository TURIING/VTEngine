//
// Created by turiing on 2024/12/29.
//

#include "core/RHI/RHIDescriptorPool.h"

#include "core/RHI/RHIDevice.h"

constexpr uint32_t DESCRIPTOR_POOL_MAX_SET_COUNT = 100;

RHIDescriptorPool::RHIDescriptorPool(const std::shared_ptr<RHIDevice>& device): m_pDevice(device) {
    std::vector<VkDescriptorPoolSize> poolSizes(2);

    poolSizes[0] = {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1000,
    };

    poolSizes[1] = {
        .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1000,
    };

    const VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = DESCRIPTOR_POOL_MAX_SET_COUNT,
        .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
        .pPoolSizes = poolSizes.data(),
    };
    CALL_VK(vkCreateDescriptorPool(m_pDevice->GetLogicalDeviceHandle(), &poolCreateInfo, nullptr, &m_pDescriptorPool));
    LOG_INFO("Created descriptor pool");
}

RHIDescriptorPool::~RHIDescriptorPool() {
    vkDestroyDescriptorPool(m_pDevice->GetLogicalDeviceHandle(), m_pDescriptorPool, nullptr);
}