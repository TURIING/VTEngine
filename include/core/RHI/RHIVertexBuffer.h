//
// Created by turiing on 2024/12/28.
//

#ifndef VTENGINE_RHIVERTEXBUFFER_H
#define VTENGINE_RHIVERTEXBUFFER_H

#include "common/common.h"
#include "core/RHI/RHIDevice.h"
#include "core/common/Vertex.h"

class RHICommandPool;

class RHIVertexBuffer {
public:
    RHIVertexBuffer(std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHICommandPool> &commandPool, const std::vector<Vertex> &buffer);
    ~RHIVertexBuffer();
    [[nodiscard]] VkBuffer GetHandle() const { return m_pBuffer; }

private:
    std::shared_ptr<RHIDevice> m_pDevice = nullptr;
    VkBuffer m_pBuffer = nullptr;
    VkDeviceMemory m_pBufferMemory = nullptr;
    void *m_pBufferMapped = nullptr;
};

#endif //VTENGINE_RHIVERTEXBUFFER_H
