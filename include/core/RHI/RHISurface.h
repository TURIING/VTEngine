/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/8 15:51
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHISURFACE_H
#define RHISURFACE_H

#include "common/common.h"

class RHIInstance;

class RHISurface {
public:
    RHISurface(const std::shared_ptr<RHIInstance> &instance, const void *);
    [[nodiscard]] VkSurfaceKHR GetHandle() const { return m_pSurface; }

private:
    VkSurfaceKHR m_pSurface = nullptr;
    std::shared_ptr<RHIInstance> m_pInstance;
};

#endif //RHISURFACE_H
