//
// Created by turiing on 2024/12/28.
//

#ifndef VTENGINE_RHIUNIFORMBUFFER_H
#define VTENGINE_RHIUNIFORMBUFFER_H

#include "common/common.h"

class RHIDevice;

class RHIUniformBuffer {
public:
    RHIUniformBuffer(const std::shared_ptr<RHIDevice> &device, size_t bufferSize);
    ~RHIUniformBuffer();
    void UpdateBuffer(const void *buffer);
    [[nodiscard]] VkDescriptorBufferInfo GetDescriptorBufferInfo() const { return m_descriptorInfo; }

private:
    VkBuffer m_pBuffer = nullptr;
    VkDeviceMemory m_pBufferMemory = nullptr;
    void *m_pBufferMapped = nullptr;
    VkDescriptorBufferInfo m_descriptorInfo {};
    std::shared_ptr<RHIDevice> m_pDevice;
    size_t m_bufferSize = 0;
};
#endif //VTENGINE_RHIUNIFORMBUFFER_H
