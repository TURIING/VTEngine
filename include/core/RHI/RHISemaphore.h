/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:41
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHISEMAPHORE_H
#define RHISEMAPHORE_H

#include "common/common.h"

class RHIDevice;

class RHISemaphore {
public:
    explicit RHISemaphore(const std::shared_ptr<RHIDevice>& device);
    ~RHISemaphore();
    [[nodiscard]] VkSemaphore GetHandle() const { return m_pSemaphore; }
private:
    VkSemaphore m_pSemaphore = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //RHISEMAPHORE_H
