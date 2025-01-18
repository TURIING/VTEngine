/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 15:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIPipeLineLayout.h"

#include <core/RHI/RHIDevice.h>

RHIPipeLineLayout::RHIPipeLineLayout(const std::shared_ptr<RHIDevice> &device, const VkPipelineLayoutCreateInfo &pipelineLayoutCreateInfo): m_pDevice(device) {
    CALL_VK(vkCreatePipelineLayout(m_pDevice->GetHandle(), &pipelineLayoutCreateInfo, nullptr, &m_pPipelineLayout));
    LOG_INFO("Pipeline Layout Created");
}

RHIPipeLineLayout::~RHIPipeLineLayout() {
    LOG_ASSERT(m_pPipelineLayout);
    vkDestroyPipelineLayout(m_pDevice->GetHandle(), m_pPipelineLayout, nullptr);
}
