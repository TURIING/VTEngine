//
// Created by turiing on 2024/12/28.
//

#ifndef VTENGINE_RHIINDEXBUFFER_H
#define VTENGINE_RHIINDEXBUFFER_H

#include "common/common.h"
#include "core/RHI/RHIDevice.h"
#include "core/common/Vertex.h"

class RHICommandPool;

class RHIIndexBuffer {
public:
    RHIIndexBuffer(std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHICommandPool> &commandPool, const std::vector<uint32_t> &buffer);
    ~RHIIndexBuffer();
    [[nodiscard]] VkBuffer GetHandle() const { return m_pBuffer; }

private:
    std::shared_ptr<RHIDevice> m_pDevice = nullptr;
    VkBuffer m_pBuffer = nullptr;
    VkDeviceMemory m_pBufferMemory = nullptr;
    void *m_pBufferMapped = nullptr;
};

#endif //VTENGINE_RHIINDEXBUFFER_H
