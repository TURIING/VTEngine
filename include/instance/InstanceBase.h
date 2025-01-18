//
// Created by turiing on 2025/1/7.
//

#ifndef INSTANCEBASE_H
#define INSTANCEBASE_H

#include "common/common.h"

class RHIRenderPass;
class RHIFrameBuffer;
class RHIDepthResource;
class RenderScene;
class RHICommandPool;
class RHIDevice;
class RHISwapChain;
class RHISurface;
class RHIInstance;

class InstanceBase {
public:
    explicit InstanceBase(const PlatformWindowInfo &info);
    virtual ~InstanceBase() = default;
    virtual void OnMouseMove(MouseButton button, const Point &pos) const;
    virtual void OnMouseScroll(double delta) const;
    virtual void OnResize(const Size &size);
    virtual void OnRender() = 0;

protected:
    virtual void cleanSwapChain() = 0;

protected:
    Size m_size;
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHISwapChain> m_pSwapChain;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    PlatformWindowInfo m_platformWindowInfo;
    std::shared_ptr<RenderScene> m_pRenderScene;
    std::shared_ptr<RHIDepthResource> m_pDepthResource;
    std::shared_ptr<RHIRenderPass> m_pRenderPass;
    std::vector<std::shared_ptr<RHIFrameBuffer>> m_vecFrameBuffer;
};
#endif //INSTANCEBASE_H
