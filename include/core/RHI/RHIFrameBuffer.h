/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 16:20
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIFRAMEBUFFER_H
#define RHIFRAMEBUFFER_H

#include "common/common.h"

class RHIRenderPass;
class RHIDevice;

class RHIFrameBuffer {
public:
    RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, VkImageView colorImageView, Size size);
    RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, VkImageView colorImageView, VkImageView depthImageView, Size size);
    ~RHIFrameBuffer();
    [[nodiscard]] VkFramebuffer GetHandle() const { return m_pFramebuffer; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkFramebuffer m_pFramebuffer = nullptr;
    std::shared_ptr<RHIRenderPass> m_pRenderPass = nullptr;
    Size m_size;
};
#endif //RHIFRAMEBUFFER_H
