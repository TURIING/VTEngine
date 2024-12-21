/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:51
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIPIPELINE_H
#define RHIPIPELINE_H

#include "common/common.h"

struct RHIPipelineState;
class RHIDevice;

class RHIPipeLine {
public:
    explicit RHIPipeLine(const std::shared_ptr<RHIDevice>& device, RHIPipelineState &pipeLineState);
    virtual ~RHIPipeLine();
    [[nodiscard]] VkPipeline GetHandle() const { return m_pPipeLine; }

private:
    void createPipeline(RHIPipelineState &pipeLineState);

protected:
    VkPipeline m_pPipeLine = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //RHIPIPELINE_H
