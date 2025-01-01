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
    RHIDescriptorSet(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIDescriptorPool> &descriptorPool, const std::shared_ptr<RHIDescriptorSetLayout> &descriptorSetLayout);
    ~RHIDescriptorSet();
    [[nodiscard]] VkDescriptorSet GetHandle() const { return m_pDescriptorSet; }
    void UpdateUniformBuffer(const std::shared_ptr<RHIUniformBuffer> &uniformBuffer, uint32_t bindingIndex) const;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIDescriptorPool> m_pDescriptorPool;
    VkDescriptorSet m_pDescriptorSet = nullptr;
};
#endif //RHIDESCRIPTORSET_H
