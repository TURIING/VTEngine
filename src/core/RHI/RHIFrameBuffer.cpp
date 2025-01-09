/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 16:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIFrameBuffer.h"

#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHIRenderPass.h>

RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass, RHIFrameBufferCreateInfo &createInfo)
    : m_pDevice(device), m_pRenderPass(renderPass) {

    const VkFramebufferCreateInfo frameBufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = m_pRenderPass->GetHandle(),
        .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
        .pAttachments = createInfo.attachments.data(),
        .width = createInfo.size.width,
        .height = createInfo.size.height,
        .layers = createInfo.layerCount,
    };
    CALL_VK(vkCreateFramebuffer(m_pDevice->GetHandle(), &frameBufferCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("FrameBuffer created");
}

RHIFrameBuffer::~RHIFrameBuffer() {
    LOG_ASSERT(m_pHandle);
    vkDestroyFramebuffer(m_pDevice->GetHandle(), m_pHandle, nullptr);
}
