//
// Created by turiing on 2024/12/28.
//
#include "core/RHI/RHIUniformBuffer.h"
#include "core/RHI/RHIDevice.h"

RHIUniformBuffer::RHIUniformBuffer(const std::shared_ptr<RHIDevice> &device, size_t bufferSize): m_pDevice(device), m_bufferSize(bufferSize) {
    m_pDevice->CreateBuffer(m_bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_pBuffer, m_pBufferMemory);
    m_descriptorInfo = {
        .buffer = m_pBuffer,
        .offset = 0,
        .range = m_bufferSize
    };
    LOG_INFO("Created uniform buffer with size {}", m_bufferSize);
}

RHIUniformBuffer::~RHIUniformBuffer() {
    vkDestroyBuffer(m_pDevice->GetLogicalDeviceHandle(), m_pBuffer, nullptr);
    vkFreeMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory, nullptr);
}

void RHIUniformBuffer::UpdateBuffer(const void *buffer) {
    LOG_ASSERT(buffer);

    vkMapMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory, 0, m_bufferSize, 0, &m_pBufferMapped);
    memcpy(m_pBufferMapped, buffer, m_bufferSize);
    vkUnmapMemory(m_pDevice->GetLogicalDeviceHandle(), m_pBufferMemory);
    m_pBufferMapped = nullptr;
}

