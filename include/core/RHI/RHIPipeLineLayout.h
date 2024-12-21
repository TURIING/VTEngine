/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 15:20
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIPIPELINELAYOUT_H
#define RHIPIPELINELAYOUT_H

#include "common/common.h"

class RHIDevice;

class RHIPipeLineLayout {
public:
    RHIPipeLineLayout(const std::shared_ptr<RHIDevice> &device, const VkPipelineLayoutCreateInfo &pipelineLayoutCreateInfo);
    ~RHIPipeLineLayout();
    [[nodiscard]] VkPipelineLayout GetHandle() const { return m_pPipelineLayout; }

private:
    VkPipelineLayout m_pPipelineLayout = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};

#endif //RHIPIPELINELAYOUT_H
