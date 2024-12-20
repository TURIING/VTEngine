/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:18
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHICommandBuffer.h"

#include <core/pipeline/ForwardPipeLine.h>
#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHIFence.h>
#include <core/RHI/rhiframebuffer.h>
#include <core/RHI/RHIPipeLine.h>
#include <core/RHI/RHIRenderPass.h>
#include <core/RHI/RHISemaphore.h>

#include "core/RHI/RHICommandPool.h"

RHICommandBuffer::RHICommandBuffer(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHICommandPool> &commandPool, uint32_t count): m_pDevice(device), m_pCommandPool(commandPool) {
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_pCommandPool->GetHandle(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = count,
    };
    m_vecCommandBuffer.resize(count);
    CALL_VK(vkAllocateCommandBuffers(m_pDevice->GetLogicalDeviceHandle(), &allocInfo, m_vecCommandBuffer.data()));
    LOG_INFO("Command buffer created");
}

RHICommandBuffer::~RHICommandBuffer() {
}

void RHICommandBuffer::Reset(uint32_t index) const {
    vkResetCommandBuffer(m_vecCommandBuffer[index], 0);
}

void RHICommandBuffer::BeginRecord(uint32_t currentframeIndex) {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        // .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    CALL_VK(vkBeginCommandBuffer(m_vecCommandBuffer[currentframeIndex], &beginInfo));
}

void RHICommandBuffer::EndRecord(uint32_t currentframeIndex) {
    CALL_VK(vkEndCommandBuffer(m_vecCommandBuffer[currentframeIndex]));
}

void RHICommandBuffer::BeginRenderPass(const std::shared_ptr<RHIRenderPass> &renderPass, const std::shared_ptr<RHIFrameBuffer> &framebuffer, uint32_t currentFrame, Size size) const {
    std::vector<VkClearValue> clearValues(1);
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = renderPass->GetHandle(),
        .framebuffer = framebuffer->GetHandle(),
        .renderArea.offset = {0, 0},
        .renderArea.extent = { size.width, size.height },
        .clearValueCount = 1,
        .pClearValues = clearValues.data(),
    };
    vkCmdBeginRenderPass(m_vecCommandBuffer[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void RHICommandBuffer::EndRenderPass(uint32_t currentFrameIndex) const {
    vkCmdEndRenderPass(m_vecCommandBuffer[currentFrameIndex]);
}

void RHICommandBuffer::BindPineLine(const std::shared_ptr<ForwardPipeLine> &pineLine, uint32_t currentFrameIndex) const {
    vkCmdBindPipeline(m_vecCommandBuffer[currentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pineLine->GetHandle());
}

void RHICommandBuffer::SetViewport(uint32_t currentFrameIndex, uint32_t firstViewportIndex, uint32_t viewportCount, const VkViewport &viewport) const {
    vkCmdSetViewport(m_vecCommandBuffer[currentFrameIndex], firstViewportIndex, viewportCount, &viewport);
}

void RHICommandBuffer::SetScissor(uint32_t currentFrameIndex, uint32_t firstScissorIndex, uint32_t scissorCount, const VkRect2D &scissor) const {
    vkCmdSetScissor(m_vecCommandBuffer[currentFrameIndex], firstScissorIndex, scissorCount, &scissor);
}

void RHICommandBuffer::Draw(uint32_t currentFrameIndex) {
    vkCmdDraw(m_vecCommandBuffer[currentFrameIndex], 3, 1, 0, 0);
}

void RHICommandBuffer::Submit(uint32_t currentFrameIndex, const std::shared_ptr<RHISemaphore> &waitSemaphore, const std::shared_ptr<RHISemaphore> &signalSemaphore, const std::shared_ptr<RHIFence> &inFightFence) const {
    VkSemaphore waitSemaphores[] = { waitSemaphore->GetHandle() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signalSemaphores[] = { signalSemaphore->GetHandle() };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_vecCommandBuffer[currentFrameIndex],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };
    CALL_VK(vkQueueSubmit(m_pDevice->GetGraphicsQueue(), 1, &submitInfo, inFightFence->GetHandle()));
}
