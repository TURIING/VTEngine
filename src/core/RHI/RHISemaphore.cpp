/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHISemaphore.h"

#include <core/RHI/RHIDevice.h>

RHISemaphore::RHISemaphore(const std::shared_ptr<RHIDevice> &device): m_pDevice(device) {
    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    CALL_VK(vkCreateSemaphore(m_pDevice->GetHandle(), &semaphoreInfo, nullptr, &m_pSemaphore));
    LOG_INFO("Semaphore created");
}

RHISemaphore::~RHISemaphore() {
    LOG_ASSERT(m_pSemaphore);
    vkDestroySemaphore(m_pDevice->GetHandle(), m_pSemaphore, nullptr);
}
