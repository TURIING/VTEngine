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

struct RHIAttachmentInfo {
    uint32_t attachmentIndex;
    VkFormat format;
};

struct RHIRenderPassCreateInfo {
    bool enableColorAttachment = false;
    bool enableDepthAttachment = false;

    std::vector<RHIAttachmentInfo> colorAttachments;
    RHIAttachmentInfo depthAttachment;
};

class RHIRenderPass {
public:
    RHIRenderPass(const std::shared_ptr<RHIDevice>& device, RHIRenderPassCreateInfo &createInfo);
    virtual ~RHIRenderPass() = 0;
    [[nodiscard]] VkRenderPass GetHandle() const { return m_pRenderPass; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkRenderPass m_pRenderPass = nullptr;
};
#endif //RHIRENDERPASS_H
