/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 17:46
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHIFENCE_H
#define RHIFENCE_H

#include "common/common.h"

class RHIDevice;

class RHIFence {
public:
    RHIFence(const std::shared_ptr<RHIDevice> &device);
    ~RHIFence();
    void Wait() const;
    void Reset() const;
    [[nodiscard]] VkFence GetHandle() const { return m_pFence; }
private:
    VkFence m_pFence = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};

#endif //RHIFENCE_H
