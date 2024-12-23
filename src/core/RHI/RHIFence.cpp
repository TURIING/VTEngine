/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:46
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIFence.h"

#include <core/RHI/RHIDevice.h>

RHIFence::RHIFence(const std::shared_ptr<RHIDevice> &device): m_pDevice(device) {
    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,                                                  // 用于指定围栏在创建时的初始状态为“已触发”（signaled）
    };
    vkCreateFence(m_pDevice->GetLogicalDeviceHandle(), &fenceCreateInfo, nullptr, &m_pFence);
    LOG_INFO("Fence created");
}

RHIFence::~RHIFence() {
    vkDestroyFence(m_pDevice->GetLogicalDeviceHandle(), m_pFence, nullptr);
}

void RHIFence::Wait() const {
    vkWaitForFences(m_pDevice->GetLogicalDeviceHandle(), 1, &m_pFence, VK_TRUE, UINT64_MAX);
}

void RHIFence::Reset() const {
    vkResetFences(m_pDevice->GetLogicalDeviceHandle(), 1, &m_pFence);
}
