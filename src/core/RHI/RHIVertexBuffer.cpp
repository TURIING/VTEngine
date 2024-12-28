//
// Created by turiing on 2024/12/28.
//
#include "core/RHI/RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(std::shared_ptr<RHIDevice> &device, const std::vector<Vertex> &bufferVertex): m_pDevice(device) {
    VkDeviceSize bufferSize = sizeof(Vertex) * bufferVertex.size();
    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_pBuffer, m_pBufferMemory);

    void *data = nullptr;
    vkMapMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, bufferVertex.data(), bufferSize);
    vkUnmapMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory);
    LOG_INFO("Vertex buffer created.");
}

RHIVertexBuffer::~RHIVertexBuffer() {
    vkDestroyBuffer(m_pDevice->GetLogicalDeviceHandle(), m_pBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory, nullptr);
    LOG_INFO("vertex buffer destroyed.");
}