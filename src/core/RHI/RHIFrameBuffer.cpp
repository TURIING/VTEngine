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

RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass, VkImageView colorImageView, Size size)
    : m_pDevice(device), m_pRenderPass(renderPass), m_pColorImageView(colorImageView), m_size(size) {
    std::vector<VkImageView> attachments = { colorImageView };

    VkFramebufferCreateInfo framebufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = m_pRenderPass->GetHandle(),
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = size.width,
        .height = size.height,
        .layers = 1,
    };
    CALL_VK(vkCreateFramebuffer(m_pDevice->GetLogicalDeviceHandle(), &framebufferCreateInfo, nullptr, &m_pFramebuffer));
    LOG_INFO("Framebuffer created");
}

RHIFrameBuffer::~RHIFrameBuffer() {
    LOG_ASSERT(m_pFramebuffer);
    vkDestroyFramebuffer(m_pDevice->GetLogicalDeviceHandle(), m_pFramebuffer, nullptr);
}
