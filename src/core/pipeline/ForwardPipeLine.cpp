/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 15:04
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/pipeline/ForwardPipeLine.h"

#include <core/RHI/RHIPipeLineLayout.h>
#include <core/RHI/RHIRenderPass.h>

#include "core/RHI/RHIPipeLineState.h"
#include <core/RHI/RHIShaderModule.h>
#include "core/common/Vertex.h"

ForwardPipeLine::ForwardPipeLine(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass){
    RHIShaderModule vertexShaderModule(m_pDevice, "shader/triangle/vert.spv");
    RHIShaderModule fragmentShaderModule(m_pDevice, "shader/triangle/frag.spv");

    RHIPipelineVertexShaderStage vertexShaderStage;
    vertexShaderStage.shaderModule = vertexShaderModule.GetHandle();

    RHIPipelineFragmentShaderStage fragmentShaderStage;
    fragmentShaderStage.shaderModule = fragmentShaderModule.GetHandle();

    RHIPipelineVertexInputState vertexInputState;
    vertexInputState.bindingDescription = Vertex::GetBindingDescription();
    vertexInputState.attributeDescriptions = Vertex::GetAttributeDescriptions();

    RHIPipelineInputAssemblyState inputAssemblyState;
    RHIPipelineViewportState viewportState;
    RHIPipelineRasterizationState rasterizationState;
    RHIPipelineMultisampleState multisampleState;
    RHIPipelineColorBlendState colorBlendState;
    RHIPipelineDynamicState dynamicState;

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0,
    };
    m_pPipelineLayout = std::make_shared<RHIPipeLineLayout>(m_pDevice, pipelineLayoutCreateInfo);

    RHIPipelineState pipelineState {
        .vertexShaderStage = vertexShaderStage,
        .fragmentShaderStage = fragmentShaderStage,
        .pipeLineLayout = m_pPipelineLayout->GetHandle(),
        .renderPass = renderPass->GetHandle(),
        .vertexInputState = vertexInputState,
        .inputAssemblyState = inputAssemblyState,
        .viewportState = viewportState,
        .rasterizationState = rasterizationState,
        .multiSampleState = multisampleState,
        .colorBlendState = colorBlendState,
        .dynamicState = dynamicState,
        .subpassIndex = 0
    };
    m_pPipeline = std::make_shared<RHIPipeLine>(m_pDevice, pipelineState);
}

ForwardPipeLine::~ForwardPipeLine() {
}

