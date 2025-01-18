/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 15:03
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef FORWARDPIPELINE_H
#define FORWARDPIPELINE_H

#include <core/RHI/RHIPipeLine.h>
#include "common/common.h"
#include "core/RHI/RHIPipeLineLayout.h"

class RHIDescriptorSetLayout;
class RHIPipeLineLayout;
class RHIRenderPass;
class RHIDevice;

struct ForwardPipeLineCreateInfo {
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::vector<VkPushConstantRange> pushConstantRanges;
};

class ForwardPipeLine final {
public:
    ForwardPipeLine(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIDescriptorSetLayout>& descriptorSetLayout, ForwardPipeLineCreateInfo &createInfo);
    [[nodiscard]] VkPipeline GetHandle() const { return m_pPipeline->GetHandle(); }
    [[nodiscard]] VkPipelineLayout GetPipelineLayout() const { return m_pPipelineLayout->GetHandle(); }

private:
    std::shared_ptr<RHIPipeLineLayout> m_pPipelineLayout;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIPipeLine> m_pPipeline;
};

class ForwardPipeLineBuilder final {
public:
    ForwardPipeLineBuilder(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIDescriptorSetLayout>& descriptorSetLayout);
    ForwardPipeLineBuilder& SetVertexShaderPath(const std::string &vertexShaderPath);
    ForwardPipeLineBuilder& SetFragmentShaderPath(const std::string &fragmentShaderPath);
    ForwardPipeLineBuilder& SetPushConstantsRanges(const std::vector<VkPushConstantRange> &vecPushConstantRange);
    std::shared_ptr<ForwardPipeLine> Build();

private:
    ForwardPipeLineCreateInfo m_createInfo {};
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<RHIDescriptorSetLayout> m_pDescriptorSetLayout;
};
#endif //FORWARDPIPELINE_H
