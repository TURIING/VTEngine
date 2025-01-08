//
// Created by turiing on 2025/1/7.
//

#ifndef INSTANCEBASE_H
#define INSTANCEBASE_H

#include "common/common.h"

class RHICommandPool;
class RHIDevice;
class RHISwapChain;
class RHISurface;
class RHIInstance;

class InstanceBase {
public:
    explicit InstanceBase(const PlatformWindowInfo &info);

protected:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHISwapChain> m_pSwapChain;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    PlatformWindowInfo m_platformWindowInfo;
};
#endif //INSTANCEBASE_H
