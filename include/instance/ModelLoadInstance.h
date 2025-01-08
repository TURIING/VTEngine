//
// Created by turiing on 2025/1/7.
//

#ifndef MODELLOADINSTANCE_H
#define MODELLOADINSTANCE_H

#include "common/common.h"
#include "instance/InstanceBase.h"

class RHIDescriptorSetLayout;
class RHIRenderPass;
class RHIDepthResource;
class RHISurface;
class RHIInstance;

class ModelLoadInstance: public InstanceBase {
public:
    ModelLoadInstance(const PlatformWindowInfo &info);

private:
    std::shared_ptr<RHIDepthResource> m_pDepthResource;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::shared_ptr<RHIDescriptorSetLayout> m_pDescriptorSetLayout;
};
#endif //MODELLOADINSTANCE_H
