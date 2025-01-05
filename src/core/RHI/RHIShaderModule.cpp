/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/11 22:24
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/RHI/RHIShaderModule.h"

#include <core/RHI/RHIDevice.h>

#include "utility/File.h"

RHIShaderModule::RHIShaderModule(const std::shared_ptr<RHIDevice> &device, File &&file): m_pDevice(device) {
    const auto shaderCode = file.GetShaderData();

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = shaderCode.size(),
        .pCode = reinterpret_cast<const uint32_t *>(shaderCode.data())
    };
    CALL_VK(vkCreateShaderModule(m_pDevice->GetLogicalDeviceHandle(), &shaderModuleCreateInfo, nullptr, &m_pShaderModule));
    LOG_INFO("Shader module created successfully! Path: {}", file.GetPath());
}

RHIShaderModule::~RHIShaderModule() {
    LOG_ASSERT(m_pShaderModule);
    vkDestroyShaderModule(m_pDevice->GetLogicalDeviceHandle(), m_pShaderModule, nullptr);
}
