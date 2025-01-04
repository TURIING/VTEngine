//
// Created by turiing on 2025/1/4.
//

#ifndef RHIIMAGE_H
#define RHIIMAGE_H

#include "common/common.h"

class RHICommandPool;
class RHIDevice;

enum class ImageType { Texture2D, Texture3D };

struct RHIImageCreateInfo {
    Size size;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspectFlags;
    VkMemoryPropertyFlags memoryPropertyFlags;
    ImageType imageType = ImageType::Texture2D;
    uint32_t mipLevels = 1;
};

class RHIImage {
public:
    RHIImage(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, const RHIImageCreateInfo& info);
    ~RHIImage();
    void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
    [[nodiscard]] VkImage GetHandle() const { return m_pImage; }
    [[nodiscard]] VkImageView GetImageViewHandle() const { return m_pImageView; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    VkImage m_pImage = nullptr;
    VkImageView m_pImageView = nullptr;
    VkDeviceMemory m_pDeviceMemory = nullptr;
    uint32_t m_mipLevels = 0;
    ImageType m_imageType = ImageType::Texture2D;
};

#endif //RHIIMAGE_H
