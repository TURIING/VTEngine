/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 19:54
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIPIPELINESTATE_H
#define RHIPIPELINESTATE_H

#include "common/common.h"

struct RHIPipelineVertexShaderStage
{
    std::string entry = "main";
    VkShaderModule shaderModule;

    [[nodiscard]] VkPipelineShaderStageCreateInfo GetCreateInfo() const {
        VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
        vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module = shaderModule;
        vertexShaderStageInfo.pName = entry.c_str();
        return vertexShaderStageInfo;
    }
};

struct RHIPipelineFragmentShaderStage
{
    std::string entry = "main";
    VkShaderModule shaderModule;

    [[nodiscard]] VkPipelineShaderStageCreateInfo GetCreateInfo() const {
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = shaderModule;
        fragShaderStageInfo.pName = entry.c_str();
        return fragShaderStageInfo;
    }
};

struct RHIPipelineVertexInputState
{
    VkVertexInputBindingDescription bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    [[nodiscard]] VkPipelineVertexInputStateCreateInfo GetCreateInfo() const {
        VkPipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.vertexBindingDescriptionCount = 1;
        vertexInputState.pVertexBindingDescriptions = &bindingDescription;
        vertexInputState.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
        return vertexInputState;
    }
};

struct RHIPipelineInputAssemblyState
{
    [[nodiscard]] static VkPipelineInputAssemblyStateCreateInfo GetCreateInfo() {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyState.primitiveRestartEnable = VK_FALSE;
        return inputAssemblyState;
    }
};

struct RHIPipelineViewportState
{
    static VkPipelineViewportStateCreateInfo GetCreateInfo()
    {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;
        return viewportState;
    }
};

struct RHIPipelineRasterizationState
{
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    [[nodiscard]] VkPipelineRasterizationStateCreateInfo GetCreateInfo() const {
        VkPipelineRasterizationStateCreateInfo rasterizationState{};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = VK_FALSE;
        rasterizationState.polygonMode = polygonMode;
        rasterizationState.lineWidth = 1.0f;
        rasterizationState.cullMode = cullMode;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.depthBiasEnable = VK_FALSE;
        return rasterizationState;
    }
};

struct RHIPipelineMultisampleState
{
    bool enable = false;
    VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
    [[nodiscard]] VkPipelineMultisampleStateCreateInfo GetCreateInfo() const {
        VkPipelineMultisampleStateCreateInfo multiSampleState{};
        multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSampleState.sampleShadingEnable = enable ? VK_TRUE : VK_FALSE;
        multiSampleState.rasterizationSamples = sampleCount;
        return multiSampleState;
    }
};

struct RHIPipelineColorBlendState
{
    bool enable = false;
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
    // TODO: translucency render
    [[nodiscard]] VkPipelineColorBlendStateCreateInfo GetCreateInfo()
    {
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachmentState.blendEnable = enable ? VK_TRUE : VK_FALSE;
        colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // 正确的枚举值
        colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachmentState;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        return colorBlending;
    }
};

struct RHIPipelineDynamicState
{
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    [[nodiscard]] VkPipelineDynamicStateCreateInfo GetCreateInfo() const {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();
        return dynamicState;
    }
};

struct RHIPipelineState
{
    RHIPipelineVertexShaderStage vertexShaderStage{};
    RHIPipelineFragmentShaderStage fragmentShaderStage{};
    VkPipelineLayout pipeLineLayout{};
    VkRenderPass renderPass{};
    // TODO: constant state
    RHIPipelineVertexInputState vertexInputState{};
    RHIPipelineInputAssemblyState inputAssemblyState{};
    // TODO: tessellation state
    RHIPipelineViewportState viewportState{};
    RHIPipelineRasterizationState rasterizationState{};
    RHIPipelineMultisampleState multiSampleState{};
    // RHIPipelineDepthStencilState depthStencilState{};
    RHIPipelineColorBlendState colorBlendState{};
    RHIPipelineDynamicState dynamicState{};
    uint32_t subpassIndex{ 0 };
};

#endif //RHIPIPELINESTATE_H
