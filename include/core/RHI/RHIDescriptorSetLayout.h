//
// Created by turiing on 2024/12/28.
//

#ifndef VTENGINE_RHIDESCRIPTORSETLAYOUT_H
#define VTENGINE_RHIDESCRIPTORSETLAYOUT_H

#include "common/common.h"

class RHIDevice;

enum class RHIDescriptorType
{
    Sampler,
    TextureSRV,
    TextureUAV,
    ConstantBuffer,
    StructuredBuffer
};

static std::unordered_map<RHIDescriptorType, VkDescriptorType> gDescriptorTypeMap = {
    { RHIDescriptorType::Sampler, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER },
    { RHIDescriptorType::TextureSRV, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE },
    { RHIDescriptorType::TextureUAV, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE },
    { RHIDescriptorType::ConstantBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
    { RHIDescriptorType::StructuredBuffer, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER }
};

static std::unordered_map<RHIDescriptorType, VkShaderStageFlags> gDescriptorShaderStageMap = {
    { RHIDescriptorType::Sampler, VK_SHADER_STAGE_FRAGMENT_BIT },
    { RHIDescriptorType::ConstantBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
    { RHIDescriptorType::TextureSRV, VK_SHADER_STAGE_FRAGMENT_BIT}
};

class RHIDescriptorSetLayout {
public:
    RHIDescriptorSetLayout(const std::shared_ptr<RHIDevice> &device, const std::vector<RHIDescriptorType> &layoutDescriptor);
    ~RHIDescriptorSetLayout();
    [[nodiscard]] VkDescriptorSetLayout GetHandle() const { return m_pLayout; }

private:
    VkDescriptorSetLayout m_pLayout = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //VTENGINE_RHIDESCRIPTORSETLAYOUT_H
