//
// Created by turiing on 2024/12/29.
//

#ifndef RHIDESCRIPTORPOOL_H
#define RHIDESCRIPTORPOOL_H

#include "common/common.h"

class RHIDevice;

class RHIDescriptorPool {
public:
    explicit RHIDescriptorPool(const std::shared_ptr<RHIDevice>& device);
    ~RHIDescriptorPool();
    [[nodiscard]] VkDescriptorPool GetHandle() const { return m_pDescriptorPool; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkDescriptorPool m_pDescriptorPool = nullptr;
};


#endif //RHIDESCRIPTORPOOL_H
