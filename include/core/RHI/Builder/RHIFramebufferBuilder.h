//
// Created by turiing on 2025/1/9.
//

#ifndef RHIFRAMEBUFFERBUILDER_H
#define RHIFRAMEBUFFERBUILDER_H

#include "core/RHI/RHIFrameBuffer.h"

class RHIFrameBufferBuilder final {
public:
    RHIFrameBufferBuilder(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHIRenderPass> &renderPass);
    RHIFrameBufferBuilder& AddAttachmentImageView(VkImageView imageView);
    RHIFrameBufferBuilder& SetSize(const Size &size);
    RHIFrameBufferBuilder& SetLayerCount(uint32_t layers);
    std::shared_ptr<RHIFrameBuffer> Build();

private:
    RHIFrameBufferCreateInfo m_createInfo {};
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<RHIDevice> m_pDevice;
};

#endif //RHIFRAMEBUFFERBUILDER_H
