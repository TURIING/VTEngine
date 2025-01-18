//
// Created by turiing on 2025/1/7.
//

#include "instance/InstanceBase.h"

#include "core/Render/RenderScene.h"
#include "core/RHI/RHICommandPool.h"
#include "core/RHI/RHIDepthResource.h"
#include "core/RHI/RHIFrameBuffer.h"
#include "core/RHI/RHISurface.h"
#include "core/RHI/RHISwapChain.h"
#include "core/RHI/Builder/RHIInstanceBuilder.h"
#include "core/RHI/Builder/RHIDeviceBuilder.h"
#include "core/RHI/Builder/RHIFramebufferBuilder.h"

InstanceBase::InstanceBase(const PlatformWindowInfo& info): m_platformWindowInfo(info) {
    m_pInstance = RHIInstanceBuilder()
                    .SetEnableValidationLayer(true)
                    .SetInstanceExtensions(REQUIRE_INSTANCE_EXT)
                    .Build();

    m_pSurface = std::make_shared<RHISurface>(m_pInstance, info.handle);

    constexpr VkPhysicalDeviceFeatures deviceFeatures {
        .samplerAnisotropy = VK_TRUE,
    };
    m_pDevice = RHIDeviceBuilder(m_pInstance, m_pSurface)
                    .SetDeviceExtension(REQUIRE_DEVICE_EXTENSION)
                    .SetPhysicalDeviceFeatures(deviceFeatures)
                    .Build();

    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, info.size);

    const auto queueFamilyIndices = QueueFamilyIndices::GetQueueFamilyIndices(m_pDevice->GetPhysicalDeviceHandle(), m_pSurface->GetHandle());
    m_pCommandPool = std::make_shared<RHICommandPool>(m_pDevice, queueFamilyIndices.graphicsFamily.value());

    m_pRenderScene = std::make_shared<RenderScene>(m_pDevice, m_pCommandPool);
    m_pRenderScene->OnResize(info.size);
}

void InstanceBase::OnMouseMove(MouseButton button, const Point &pos) const {
    m_pRenderScene->OnMouseMove(button, pos);
}

void InstanceBase::OnMouseScroll(double delta) const {
    m_pRenderScene->OnMouseScroll(delta);
}

void InstanceBase::OnResize(const Size& size) {
    m_size = size;
    m_pRenderScene->OnResize(size);
    m_pDevice->WaitIdle();
    this->cleanSwapChain();

    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, size);

    const auto newSize = m_pSwapChain->GetSize();
    m_pDepthResource = std::make_shared<RHIDepthResource>(m_pDevice, m_pCommandPool, newSize);

    for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
        m_vecFrameBuffer.emplace_back(RHIFrameBufferBuilder(m_pDevice, m_pRenderPass)
                                            .AddAttachmentImageView(m_pSwapChain->GetImageView(i))
                                            .AddAttachmentImageView(m_pDepthResource->GetImageView())
                                            .SetSize(newSize)
                                            .Build());
    }
}