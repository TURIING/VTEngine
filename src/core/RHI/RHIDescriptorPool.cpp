//
// Created by turiing on 2024/12/29.
//

#include "core/RHI/RHIDescriptorPool.h"

#include "core/RHI/RHIDevice.h"

constexpr uint32_t DESCRIPTOR_POOL_MAX_SET_COUNT = 100;

RHIDescriptorPool::RHIDescriptorPool(const std::shared_ptr<RHIDevice>& device, const RHIDescriptorPoolCreateInfo &createInfo): m_pDevice(device) {
    std::vector<VkDescriptorPoolSize> poolSizes(createInfo.poolSizes.size());

    for(auto i = 0; i < createInfo.poolSizes.size(); ++i) {
        poolSizes[i] = {
            .type = gDescriptorTypeMap[createInfo.poolSizes[i].type],
            .descriptorCount = createInfo.poolSizes[i].count,
        };        
    }

    const VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = DESCRIPTOR_POOL_MAX_SET_COUNT,
        .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
        .pPoolSizes = poolSizes.data(),
    };
    CALL_VK(vkCreateDescriptorPool(m_pDevice->GetHandle(), &poolCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Created descriptor pool");
}

RHIDescriptorPool::~RHIDescriptorPool() {
    vkDestroyDescriptorPool(m_pDevice->GetHandle(), m_pHandle, nullptr);
}