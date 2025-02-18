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
#include "utility/File.h"

ForwardPipeLine::ForwardPipeLine(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass, const std::shared_ptr<RHIDescriptorSetLayout>& descriptorSetLayout): m_pDevice(device){
    RHIShaderModule vertexShaderModule(m_pDevice, File::FromStdString(SHADER_DIR + "triangle/vert.spv"));
    RHIShaderModule fragmentShaderModule(m_pDevice, File::FromStdString(SHADER_DIR + "triangle/frag.spv"));

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
    RHIPipelineMultiSampleState multiSampleState;
    RHIPipelineColorBlendState colorBlendState;
    RHIPipelineDynamicState dynamicState;
    RHIPipelineDepthStencilState depthStencilState;

    const auto descriptorLayout = descriptorSetLayout->GetHandle();
    std::vector<VkPushConstantRange> pushConstantRanges = { { VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant) } };
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &descriptorLayout,
        .pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size()),
        .pPushConstantRanges = pushConstantRanges.data(),
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
        .depthStencilState = depthStencilState,
        .colorBlendState = colorBlendState,
        .dynamicState = dynamicState,
        .subPassIndex = 0
    };
    m_pPipeline = std::make_shared<RHIPipeLine>(m_pDevice, pipelineState);
}

ForwardPipeLine::~ForwardPipeLine() {
}

