/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 13:50
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIRENDERPASS_H
#define RHIRENDERPASS_H

#include "common/common.h"

class RHIDevice;

class RHIRenderPass {
public:
    explicit RHIRenderPass(const std::shared_ptr<RHIDevice>& device);
    virtual ~RHIRenderPass();
    [[nodiscard]] VkRenderPass GetHandle() const { return m_pRenderPass; }

protected:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkRenderPass m_pRenderPass = nullptr;
};
#endif //RHIRENDERPASS_H
