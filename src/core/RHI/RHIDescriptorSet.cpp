//
// Created by turiing on 2024/12/29.
//

#include "core/RHI/RHIDescriptorSet.h"

#include "core/RHI/RHIDescriptorPool.h"
#include "core/RHI/RHIDescriptorSetLayout.h"
#include "core/RHI/RHIDevice.h"
#include "core/RHI/RHIUniformBuffer.h"

RHIDescriptorSet::RHIDescriptorSet(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIDescriptorPool>& descriptorPool, const std::shared_ptr<RHIDescriptorSetLayout> &descriptorSetLayout)
    : m_pDevice(device), m_pDescriptorPool(descriptorPool) {
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts =  { descriptorSetLayout->GetHandle() };

    const VkDescriptorSetAllocateInfo allocateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_pDescriptorPool->GetHandle(),
        .descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size()),
        .pSetLayouts = descriptorSetLayouts.data(),
    };
    CALL_VK(vkAllocateDescriptorSets(m_pDevice->GetLogicalDeviceHandle(), &allocateInfo, &m_pDescriptorSet));
    LOG_INFO("RHI Descriptor Set Allocated");
}

RHIDescriptorSet::~RHIDescriptorSet() {

}

void RHIDescriptorSet::UpdateUniformBuffer(const std::shared_ptr<RHIUniformBuffer>& uniformBuffer, uint32_t bindingIndex) const {
    const auto bufferInfo = uniformBuffer->GetDescriptorBufferInfo();

    const VkWriteDescriptorSet writeDescriptorSet {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = m_pDescriptorSet,
        .dstBinding = bindingIndex,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = gDescriptorTypeMap[RHIDescriptorType::ConstantBuffer],
        .pBufferInfo = &bufferInfo,
    };
    vkUpdateDescriptorSets(m_pDevice->GetLogicalDeviceHandle(), 1, &writeDescriptorSet, 0, nullptr);
}

void RHIDescriptorSet::UpdateTextureImage(VkDescriptorImageInfo imageInfo, uint32_t bindingIndex) const {
    const VkWriteDescriptorSet writeDescriptorSet {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .dstSet = m_pDescriptorSet,
        .dstBinding = bindingIndex,
        .dstArrayElement = 0,
        .descriptorType = gDescriptorTypeMap[RHIDescriptorType::Sampler],
        .descriptorCount = 1,
        .pImageInfo = &imageInfo,
    };
    vkUpdateDescriptorSets(m_pDevice->GetLogicalDeviceHandle(), 1, &writeDescriptorSet, 0, nullptr);
}
