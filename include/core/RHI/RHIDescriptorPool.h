//
// Created by turiing on 2024/12/29.
//

#ifndef RHIDESCRIPTORPOOL_H
#define RHIDESCRIPTORPOOL_H

#include "common/common.h"
#include "core/RHI/RHIDescriptorSetLayout.h"

class RHIDevice;

struct RHIDescriptorPoolSizeInfo {
    RHIDescriptorType type;
    uint32_t count;
};

struct RHIDescriptorPoolCreateInfo {
    std::vector<RHIDescriptorPoolSizeInfo> poolSizes;
};

class RHIDescriptorPool final : public RHIObject<VkDescriptorPool>{
public:
    explicit RHIDescriptorPool(const std::shared_ptr<RHIDevice>& device, const RHIDescriptorPoolCreateInfo &createInfo);
    ~RHIDescriptorPool() override;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
};


#endif //RHIDESCRIPTORPOOL_H
