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

class RHIPipeLineLayout;
class RHIRenderPass;
class RHIDevice;

class ForwardPipeLine final{
public:
    ForwardPipeLine(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass);
    ~ForwardPipeLine();
    [[nodiscard]] VkPipeline GetHandle() const { return m_pPipeline->GetHandle(); }

private:
    std::shared_ptr<RHIPipeLineLayout> m_pPipelineLayout;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIPipeLine> m_pPipeline;
};
#endif //FORWARDPIPELINE_H
