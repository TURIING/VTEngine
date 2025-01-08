//
// Created by turiing on 2025/1/8.
//

#include "core/RHI/Builder/RHIRenderPassBuilder.h"

RHIRenderPassBuilder::RHIRenderPassBuilder(const std::shared_ptr<RHIDevice>& device) : m_pDevice(device) {
}

RHIRenderPassBuilder& RHIRenderPassBuilder::AddColorAttachment(const RHIAttachmentInfo&& attachmentInfo) {
    m_renderPassCreateInfo.enableColorAttachment = true;
    m_renderPassCreateInfo.colorAttachments.push_back(attachmentInfo);
    return *this;
}

RHIRenderPassBuilder& RHIRenderPassBuilder::AddDepthAttachment(const RHIAttachmentInfo&& attachmentInfo) {
    m_renderPassCreateInfo.enableDepthAttachment = true;
    m_renderPassCreateInfo.depthAttachment = attachmentInfo;
    return *this;
}

std::shared_ptr<RHIRenderPass> RHIRenderPassBuilder::Build() {
    return std::make_shared<RHIRenderPass>(m_pDevice, m_renderPassCreateInfo);
}
