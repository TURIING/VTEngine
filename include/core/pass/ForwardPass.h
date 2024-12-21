/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:00
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef FORWARDPASS_H
#define FORWARDPASS_H

#include "core/RHI/RHIRenderPass.h"
#include "common/common.h"

class RHIDevice;

class ForwardPass: public RHIRenderPass{
public:
    ForwardPass(const std::shared_ptr<RHIDevice>& device, VkFormat colorFormat);
    ~ForwardPass() = default;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
};

#endif //FORWARDPASS_H
