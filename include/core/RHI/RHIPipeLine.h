/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:51
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIPIPELINE_H
#define RHIPIPELINE_H

#include "RHIObject.h"
#include "common/common.h"

struct RHIPipelineState;
class RHIDevice;

class RHIPipeLine : public RHIObject<VkPipeline> {
public:
    explicit RHIPipeLine(const std::shared_ptr<RHIDevice>& device, RHIPipelineState &pipeLineState);
    ~RHIPipeLine() override;

private:
    void createPipeline(RHIPipelineState &pipeLineState);

protected:
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //RHIPIPELINE_H
