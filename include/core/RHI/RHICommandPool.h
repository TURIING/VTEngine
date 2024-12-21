/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/21 16:35
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHICOMMANDPOOL_H
#define RHICOMMANDPOOL_H

#include "common/common.h"

class RHIDevice;

class RHICommandPool {
public:
    RHICommandPool(const std::shared_ptr<RHIDevice>& device, uint32_t queueFamilyIndex);
    ~RHICommandPool();
    [[nodiscard]] VkCommandPool GetHandle() const { return m_pCommandPool; }

private:
    VkCommandPool m_pCommandPool = nullptr;
    std::shared_ptr<RHIDevice> m_pDevice;
};
#endif //RHICOMMANDPOOL_H
