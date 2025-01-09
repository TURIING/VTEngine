//
// Created by turiing on 2025/1/9.
//
#include "core/RHI/Builder/RHIFramebufferBuilder.h"

RHIFrameBufferBuilder::RHIFrameBufferBuilder(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass): m_pDevice(device), m_pRenderPass(renderPass) { }

RHIFrameBufferBuilder& RHIFrameBufferBuilder::AddAttachmentImageView(VkImageView imageView) {
    m_createInfo.attachments.push_back(imageView);
    return *this;
}

RHIFrameBufferBuilder& RHIFrameBufferBuilder::SetSize(const Size& size) {
    m_createInfo.size = size;
    return *this;
}

RHIFrameBufferBuilder& RHIFrameBufferBuilder::SetLayerCount(uint32_t layers) {
    m_createInfo.layerCount = layers;
    return *this;
}

std::shared_ptr<RHIFrameBuffer> RHIFrameBufferBuilder::Build() {
    return std::make_shared<RHIFrameBuffer>(m_pDevice, m_pRenderPass, m_createInfo);
}
