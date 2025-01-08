//
// Created by turiing on 2025/1/7.
//

#include "instance/InstanceBase.h"

#include "core/RHI/RHICommandPool.h"
#include "core/RHI/RHISurface.h"
#include "core/RHI/RHISwapChain.h"
#include "core/RHI/Builder/RHIInstanceBuilder.h"
#include "core/RHI/Builder/RHIDeviceBuilder.h"

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
}
