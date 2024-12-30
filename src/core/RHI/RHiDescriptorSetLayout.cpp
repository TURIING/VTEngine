//
// Created by turiing on 2024/12/28.
//
#include "core/RHI/RHIDescriptorSetLayout.h"
#include "core/RHI/RHIDevice.h"

RHIDescriptorSetLayout::RHIDescriptorSetLayout(const std::shared_ptr<RHIDevice> &device, const std::vector<RHIDescriptorType> &layoutDescriptor): m_pDevice(device) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    for(auto i = 0; i < layoutDescriptor.size(); i++) {
        VkDescriptorSetLayoutBinding binding {
            .binding = static_cast<uint32_t>(i),
            .descriptorType = gDescriptorTypeMap[layoutDescriptor[i]],
            .descriptorCount = 1,
            .stageFlags = gDescriptorShaderStageMap[layoutDescriptor[i]],
            .pImmutableSamplers = nullptr,
        };
        bindings.push_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };
    CALL_VK(vkCreateDescriptorSetLayout(m_pDevice->GetLogicalDeviceHandle(), &layoutCreateInfo, nullptr, &m_pLayout));
    LOG_INFO("Descriptor set layout created.");
}

RHIDescriptorSetLayout::~RHIDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(m_pDevice->GetLogicalDeviceHandle(), m_pLayout, nullptr);
}