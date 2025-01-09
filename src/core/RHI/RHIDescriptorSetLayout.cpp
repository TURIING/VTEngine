//
// Created by turiing on 2024/12/28.
//
#include "core/RHI/RHIDescriptorSetLayout.h"
#include "core/RHI/RHIDevice.h"

RHIDescriptorSetLayout::RHIDescriptorSetLayout(const std::shared_ptr<RHIDevice> &device, const RHIDescriptorSetLayoutCreateInfo &createInfo): m_pDevice(device) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    for(const auto & [bindIndex, descriptorType] : createInfo.descriptorSetLayoutBindings) {
        VkDescriptorSetLayoutBinding binding {
            .binding = static_cast<uint32_t>(bindIndex),
            .descriptorType = gDescriptorTypeMap[descriptorType],
            .descriptorCount = 1,
            .stageFlags = gDescriptorShaderStageMap[descriptorType],
            .pImmutableSamplers = nullptr,
        };
        bindings.push_back(binding);
    }

    const VkDescriptorSetLayoutCreateInfo layoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };
    CALL_VK(vkCreateDescriptorSetLayout(m_pDevice->GetHandle(), &layoutCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Descriptor set layout created.");
}

RHIDescriptorSetLayout::~RHIDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(m_pDevice->GetHandle(), m_pHandle, nullptr);
}