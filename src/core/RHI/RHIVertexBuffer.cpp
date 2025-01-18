//
// Created by turiing on 2024/12/28.
//
#include "core/RHI/RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHICommandPool> &commandPool, const std::vector<Vertex> &bufferVertex): m_pDevice(device) {
    VkDeviceSize bufferSize = sizeof(Vertex) * bufferVertex.size();
    VkBuffer stageBuffer;
    VkDeviceMemory stageBufferMemory;
    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

    // 传输数据到暂存缓冲
    vkMapMemory(m_pDevice->GetHandle(), stageBufferMemory, 0, bufferSize, 0, &m_pBufferMapped);
    memcpy(m_pBufferMapped, bufferVertex.data(), bufferSize);
    vkUnmapMemory(m_pDevice->GetHandle(), stageBufferMemory);

    // 从暂存缓冲复制到真正的缓冲区
    m_pDevice->CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_pBuffer, m_pBufferMemory);
    m_pDevice->CopyBuffer(commandPool, stageBuffer, m_pBuffer, bufferSize);

    vkDestroyBuffer(m_pDevice->GetHandle(), stageBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetHandle(), stageBufferMemory, nullptr);

    LOG_INFO("Vertex buffer created.");
}

RHIVertexBuffer::~RHIVertexBuffer() {
    vkDestroyBuffer(m_pDevice->GetHandle(), m_pBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetHandle(), m_pBufferMemory, nullptr);
    LOG_INFO("Vertex buffer destroyed.");
}