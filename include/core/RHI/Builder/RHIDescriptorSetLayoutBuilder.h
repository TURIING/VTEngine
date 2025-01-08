//
// Created by turiing on 2025/1/8.
//

#ifndef RHIDESCRIPTORSETLAYOUTBUILDER_H
#define RHIDESCRIPTORSETLAYOUTBUILDER_H

#include "common/common.h"
#include "core/RHI/RHIDescriptorSetLayout.h"

struct RHIDescriptorSetLayoutBindInfo;
class RHIDevice;

class RHIDescriptorSetLayoutBuilder {
public:
    explicit RHIDescriptorSetLayoutBuilder(const std::shared_ptr<RHIDevice>& device);
    RHIDescriptorSetLayoutBuilder& AddDescriptorBinding(RHIDescriptorSetLayoutBindInfo &&bindInfo);
    std::shared_ptr<RHIDescriptorSetLayout> Build();

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    RHIDescriptorSetLayoutCreateInfo m_createInfo;
};
#endif //RHIDESCRIPTORSETLAYOUTBUILDER_H
