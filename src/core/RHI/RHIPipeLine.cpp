/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:51
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIPipeLine.h"

#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHIPipeLineState.h>

RHIPipeLine::RHIPipeLine(const std::shared_ptr<RHIDevice> &device, RHIPipelineState &pipeLineState): m_pDevice(device) {
    this->createPipeline(pipeLineState);
}

RHIPipeLine::~RHIPipeLine() {
    vkDestroyPipeline(m_pDevice->GetLogicalDeviceHandle(), m_pPipeLine, nullptr);
}

void RHIPipeLine::createPipeline(RHIPipelineState &pipeLineState) {
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {
        pipeLineState.vertexShaderStage.GetCreateInfo(),
        pipeLineState.fragmentShaderStage.GetCreateInfo()
    };
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = pipeLineState.vertexInputState.GetCreateInfo();
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = RHIPipelineInputAssemblyState::GetCreateInfo();
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = RHIPipelineViewportState::GetCreateInfo();
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = pipeLineState.rasterizationState.GetCreateInfo();
    VkPipelineMultisampleStateCreateInfo multiSampleStateCreateInfo = pipeLineState.multiSampleState.GetCreateInfo();
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = pipeLineState.colorBlendState.GetCreateInfo();
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = pipeLineState.dynamicState.GetCreateInfo();
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = pipeLineState.depthStencilState.GetCreateInfo();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(shaderStages.size()),
        .pStages = shaderStages.data(),
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multiSampleStateCreateInfo,
        .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = pipeLineState.pipeLineLayout,
        .renderPass = pipeLineState.renderPass,
        .subpass = pipeLineState.subPassIndex,
        .basePipelineHandle = VK_NULL_HANDLE,
    };
    CALL_VK(vkCreateGraphicsPipelines(m_pDevice->GetLogicalDeviceHandle(), nullptr, 1, &pipelineCreateInfo, nullptr, &m_pPipeLine));
    LOG_INFO("Pipeline created");
}
