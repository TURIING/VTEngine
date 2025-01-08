/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:01
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/pass/ForwardPass.h"

#include <core/RHI/RHIDevice.h>

ForwardPass::ForwardPass(const std::shared_ptr<RHIDevice> &device, VkFormat colorFormat, VkFormat depthFormat): RHIRenderPass(device) {
}