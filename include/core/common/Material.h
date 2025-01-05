//
// Created by turiing on 2025/1/5.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#endif //MATERIAL_H

#include "common/common.h"

class RHITexture;
class RHICommandPool;
class RHIDevice;

struct TextureBind {
    uint32_t bindIndex = 0;
    std::string path;
};

class Material {
public:
    Material(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool> &commandPool, std::vector<TextureBind>& binds);

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::vector<TextureBind> m_vecTextureBind;
    std::vector<std::shared_ptr<RHITexture>> m_vecTexture;
}