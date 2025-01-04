//
// Created by turiing on 2025/1/4.
//

#ifndef RHISAMPLER_H
#define RHISAMPLER_H

#include "common/common.h"

class RHIDevice;

class RHISampler {
public:
    RHISampler(const std::shared_ptr<RHIDevice>& device, VkFilter magFilter = VK_FILTER_NEAREST, VkFilter minFilter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
    ~RHISampler();
    [[nodiscard]] VkSampler GetHandle() const { return m_pSampler; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkSampler m_pSampler = nullptr;
};
#endif //RHISAMPLER_H
