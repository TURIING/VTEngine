/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 14:01
* @version: 1.0
* @description: 
********************************************************************************/
#include "core/pass/ForwardPass.h"

#include <core/RHI/RHIDevice.h>

ForwardPass::ForwardPass(const std::shared_ptr<RHIDevice> &device, VkFormat colorFormat): RHIRenderPass(device) {
    VkAttachmentDescription colorAttachment = {
        .format = colorFormat,
        .samples = VK_SAMPLE_COUNT_1_BIT,

        // 对颜色和深度缓冲起效
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,                                      // 指定在渲染之前对附着中的数据进行的操作,使用一个常量值来清除附着的内容
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,                                    // 渲染之后对附着中的数据进行的操作,渲染的内容会被存储起来，以便之后读取

        // 对模板缓冲起效
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,                           // 设置对模板缓冲不关心
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

        // 图像的像素数据在内存中的分布取决于我们要对图像进行的操作
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,                                 // 用于指定渲染流程开始前的图像布局方式,我们不关心之前的图像布局方式
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR                              // 用于指定渲染流程结束后的图像布局方式,图像被用在交换链中进行呈现操作
    };

    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,                                                            // 用于指定要引用的附着在附着描述结构体数组中的索引
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL                          // 用于指定进行子流程时引用的附着使用的布局方式，Vulkan会在子流程开始时自动将引用的附着转换到layout成员变量指定的图像布局
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,                       // Vulkan在未来也可能会支持计算子流程，所以，我们还需要显式地指定这是一个图形渲染的子流程
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    const VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };
    CALL_VK(vkCreateRenderPass(m_pDevice->GetLogicalDeviceHandle(), &renderPassInfo, nullptr, &m_pRenderPass));
    LOG_INFO("RenderPass created");
}