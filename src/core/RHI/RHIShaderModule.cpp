/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/11 22:24
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIShaderModule.h"

#include <core/RHI/RHIDevice.h>

RHIShaderModule::RHIShaderModule(const std::shared_ptr<RHIDevice> &device, const std::string &shaderPath): m_pDevice(device) {
    const auto shaderCode = Utility::readFile(shaderPath);

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = shaderCode.size(),
        .pCode = reinterpret_cast<const uint32_t *>(shaderCode.data())
    };
    CALL_VK(vkCreateShaderModule(m_pDevice->GetLogicalDeviceHandle(), &shaderModuleCreateInfo, nullptr, &m_pShaderModule));
    LOG_INFO("Shader module created successfully! Path: {}", shaderPath);
}

RHIShaderModule::~RHIShaderModule() {
    LOG_ASSERT(m_pShaderModule);
    vkDestroyShaderModule(m_pDevice->GetLogicalDeviceHandle(), m_pShaderModule, nullptr);
}
