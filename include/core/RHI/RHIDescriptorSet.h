//
// Created by turiing on 2024/12/29.
//

#ifndef RHIDESCRIPTORSET_H
#define RHIDESCRIPTORSET_H

#include "common/common.h"

class RHIUniformBuffer;
class RHIDescriptorSetLayout;
class RHIDescriptorPool;
class RHIDevice;

class RHIDescriptorSet {
public:
    RHIDescriptorSet(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIDescriptorPool> &descriptorPool, const std::vector<VkDescriptorSetLayout> &descriptorSetLayouts);
    ~RHIDescriptorSet();
    [[nodiscard]] VkDescriptorSet GetHandle(uint32_t index) { return m_vecDescriptorSet[index]; }
    void UpdateUniformBuffer(const std::shared_ptr<RHIUniformBuffer> &uniformBuffer, uint32_t index, uint32_t bindingIndex) const;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIDescriptorPool> m_pDescriptorPool;
    std::vector<VkDescriptorSet> m_vecDescriptorSet;
};
#endif //RHIDESCRIPTORSET_H
