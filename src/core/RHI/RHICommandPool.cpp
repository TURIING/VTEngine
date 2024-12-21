/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 16:35
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHICommandPool.h"

#include <core/RHI/RHIDevice.h>

RHICommandPool::RHICommandPool(const std::shared_ptr<RHIDevice> &device, uint32_t queueFamilyIndex) {
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex,
    };
    CALL_VK(vkCreateCommandPool(m_pDevice->GetLogicalDeviceHandle(), &poolInfo, nullptr, &m_pCommandPool));
    LOG_INFO("Command pool created");
}

RHICommandPool::~RHICommandPool() {
}
