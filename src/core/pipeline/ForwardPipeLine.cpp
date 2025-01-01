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
#include "core/RHI/RHIDescriptorSetLayout.h"

ForwardPipeLine::ForwardPipeLine(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass, const std::shared_ptr<RHIDescriptorSetLayout>& descriptorSetLayout): m_pDevice(device){
    RHIShaderModule vertexShaderModule(m_pDevice, "/Users/turiing/VTEngine/bin/shader/triangle/vert.spv");
    RHIShaderModule fragmentShaderModule(m_pDevice, "/Users/turiing/VTEngine/bin/shader/triangle/frag.spv");

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
    RHIPipelineMultisampleState multiSampleState;
    RHIPipelineColorBlendState colorBlendState;
    RHIPipelineDynamicState dynamicState;

    const auto descriptorLayout = descriptorSetLayout->GetHandle();
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &descriptorLayout,
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
        .multiSampleState = multiSampleState,
        .colorBlendState = colorBlendState,
        .dynamicState = dynamicState,
        .subpassIndex = 0
    };
    m_pPipeline = std::make_shared<RHIPipeLine>(m_pDevice, pipelineState);
}

ForwardPipeLine::~ForwardPipeLine() {
}

