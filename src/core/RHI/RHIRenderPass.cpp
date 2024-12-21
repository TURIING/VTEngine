/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 13:50
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIRenderPass.h"

#include <core/RHI/RHIDevice.h>

RHIRenderPass::RHIRenderPass(const std::shared_ptr<RHIDevice> &device): m_pDevice(device) {

}

RHIRenderPass::~RHIRenderPass() {
    LOG_ASSERT(m_pRenderPass);
    vkDestroyRenderPass(m_pDevice->GetLogicalDeviceHandle(), m_pRenderPass, nullptr);
}
