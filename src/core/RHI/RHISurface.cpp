/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/8 15:51
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHISurface.h"

#include <core/RHI/RHIInstance.h>

RHISurface::RHISurface(const std::shared_ptr<RHIInstance> &instance, const void *handle): m_pInstance(instance) {
#if PLATFORM_WINDOWS
    const VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = nullptr,
        .hwnd = (HWND)handle,
    };

    CALL_VK(vkCreateWin32SurfaceKHR(m_pInstance->GetHandle(), &surfaceCreateInfo, nullptr, &m_pSurface));
#elif PLATFORM_MACOS
    const VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .pView = handle,
    };

    CALL_VK(vkCreateMacOSSurfaceMVK(m_pInstance->GetHandle(), &surfaceCreateInfo, nullptr, &m_pSurface));
#endif
}

RHISurface::~RHISurface() {
    vkDestroySurfaceKHR(m_pInstance->GetHandle(), m_pSurface, nullptr);
}
