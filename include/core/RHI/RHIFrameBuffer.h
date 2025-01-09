/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 16:20
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIFRAMEBUFFER_H
#define RHIFRAMEBUFFER_H

#include "RHIObject.h"
#include "common/common.h"

class RHIRenderPass;
class RHIDevice;

struct RHIFrameBufferCreateInfo {
    std::vector<VkImageView> attachments;
    Size size;
    uint32_t layerCount = 1;
};

class RHIFrameBuffer final : public RHIObject<VkFramebuffer>{
public:
    RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, RHIFrameBufferCreateInfo &createInfo);
    ~RHIFrameBuffer() override;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIRenderPass> m_pRenderPass = nullptr;
};
#endif //RHIFRAMEBUFFER_H
