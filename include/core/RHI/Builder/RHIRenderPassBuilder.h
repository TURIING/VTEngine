//
// Created by turiing on 2025/1/8.
//

#ifndef RHIRENDERPASSBUILDER_H
#define RHIRENDERPASSBUILDER_H

#include "common/common.h"
#include "core/RHI/RHIRenderPass.h"

class RHIRenderPass;
struct RHIAttachmentInfo;
class RHIDevice;


class RHIRenderPassBuilder final {
public:
    explicit RHIRenderPassBuilder(const std::shared_ptr<RHIDevice>& device);
    RHIRenderPassBuilder& AddColorAttachment(const RHIAttachmentInfo && attachmentInfo);
    RHIRenderPassBuilder& AddDepthAttachment(const RHIAttachmentInfo && attachmentInfo);
    std::shared_ptr<RHIRenderPass> Build();

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    RHIRenderPassCreateInfo m_renderPassCreateInfo {};
};
#endif //RHIRENDERPASSBUILDER_H
