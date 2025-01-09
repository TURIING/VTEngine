/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/8 16:36
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef RHISWAPCHAIN_H
#define RHISWAPCHAIN_H

#include "RHIObject.h"
#include "common/common.h"

class RHISemaphore;
class RHISurface;
class RHIDevice;
class RHIInstance;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    static SwapChainSupportDetails GetSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

class RHISwapChain: public RHIObject<VkSwapchainKHR> {
public:
    RHISwapChain(const std::shared_ptr<RHIInstance> &instance, const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHISurface> &surface, const Size &size);
    ~RHISwapChain();
    [[nodiscard]] uint32_t GetImageCount() const { return m_vecSwapChainImages.size(); }
    [[nodiscard]] VkImage GetImage(uint32_t index) const;
    [[nodiscard]] VkImageView GetImageView(uint32_t index) const;
    [[nodiscard]] Size GetSize() const { return m_size; }
    [[nodiscard]] VkFormat GetColorFormat() const { return m_pSwapChainSurfaceFormat.format; }
    VkResult AcquireNextImage(const std::shared_ptr<RHISemaphore> &semaphore, uint32_t &imageIndex) const;

private:
    [[nodiscard]] VkSurfaceFormatKHR chooseSwapSurfaceFormat() const;
    [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode() const;
    [[nodiscard]] VkExtent2D getSwapChainExtent();
    [[nodiscard]] uint32_t getSwapChainImageCount() const;
    void createSwapChainImagesAndViews();

private:
    std::shared_ptr<RHIInstance> m_pInstance;
    std::shared_ptr<RHIDevice> m_pDevice;
    Size m_size;
    std::vector<VkImage> m_vecSwapChainImages;
    std::vector<VkImageView> m_vecSwapChainImageViews;
    VkSurfaceFormatKHR m_pSwapChainSurfaceFormat {};
    SwapChainSupportDetails m_swapChainSupportDetails;
};

#endif //RHISWAPCHAIN_H
