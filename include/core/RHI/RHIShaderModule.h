/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/11 22:24
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHISHADERMODULE_H
#define RHISHADERMODULE_H

#include "common/common.h"

class RHIDevice;

class RHIShaderModule {
public:
    RHIShaderModule(const std::shared_ptr<RHIDevice>& device, const std::string &shaderPath);
    ~RHIShaderModule();
    [[nodiscard]] VkShaderModule GetHandle() const { return m_pShaderModule; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    VkShaderModule m_pShaderModule = nullptr;
};

#endif //RHISHADERMODULE_H
