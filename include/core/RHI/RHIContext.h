/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/9 20:32
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHICONTEXT_H
#define RHICONTEXT_H

#include "common/common.h"

class RHISurface;
class RHISwapChain;
class RHIDevice;
class RHIInstance;

class RHIContext {
public:
    explicit RHIContext(const PlatformWindowInfo &info);

private:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHISurface> m_pSurface;
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHISwapChain> m_pSwapChain;
};

#endif //RHICONTEXT_H
