/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/9 20:32
* @version: 1.0
* @description: 
********************************************************************************/

#include "core/RHI/RHIContext.h"

#include <core/RHI/RHIDevice.h>
#include <core/RHI/RHISurface.h>
#include <core/RHI/RHISwapChain.h>

#include "core/RHI/RHIInstance.h"

RHIContext::RHIContext(const PlatformWindowInfo &info) {
    m_pInstance = std::make_shared<RHIInstance>();
    m_pSurface = std::make_shared<RHISurface>(m_pInstance, info.handle);
    m_pDevice = std::make_shared<RHIDevice>(m_pInstance);
    m_pSwapChain = std::make_shared<RHISwapChain>(m_pInstance, m_pDevice, m_pSurface, info.size);
}
