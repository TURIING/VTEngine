//
// Created by turiing on 2025/1/5.
//

#ifndef RHIDEPTHRESOURCE_H
#define RHIDEPTHRESOURCE_H

#include "common/common.h"

class RHICommandPool;
class RHIImage;
class RHIDevice;

class RHIDepthResource {
public:
    RHIDepthResource(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, Size size);
    [[nodiscard]] VkImageView GetImageView() const;

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::unique_ptr<RHIImage> m_pImage;
};
#endif //RHIDEPTHRESOURCE_H
