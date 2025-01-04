//
// Created by turiing on 2025/1/4.
//

#ifndef RHITEXTURE_H
#define RHITEXTURE_H

#include "common/common.h"

class File;
class RHISampler;
class RHICommandPool;
class RHIImage;
class RHIDevice;

class RHITexture {
public:
    RHITexture(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, File &&file);
    ~RHITexture();
    [[nodiscard]] VkDescriptorImageInfo GetDescriptorImageInfo() const { return m_imageInfo; }

private:
    void copyBufferToImage(VkBuffer buffer, Size size);

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    std::shared_ptr<RHIImage> m_pImage;
    std::shared_ptr<RHISampler> m_pSampler;
    VkDescriptorImageInfo m_imageInfo;
};
#endif //RHITEXTURE_H