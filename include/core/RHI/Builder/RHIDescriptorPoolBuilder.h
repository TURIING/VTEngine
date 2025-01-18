//
// Created by turiing on 2025/1/9.
//

#ifndef RHIDESCRIPTORPOOLBUILDER_H
#define RHIDESCRIPTORPOOLBUILDER_H

#include "common/common.h"
#include "core/RHI/RHIDescriptorPool.h"

class RHIDevice;
class RHIDescriptorPool;

class RHIDescriptorPoolBuilder final {
public:
    explicit RHIDescriptorPoolBuilder(const std::shared_ptr<RHIDevice>& device);
    RHIDescriptorPoolBuilder& AddPoolSize(RHIDescriptorPoolSizeInfo &&info);
    std::shared_ptr<RHIDescriptorPool> Build();

private:
    RHIDescriptorPoolCreateInfo m_createInfo {};
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //RHIDESCRIPTORPOOLBUILDER_H
