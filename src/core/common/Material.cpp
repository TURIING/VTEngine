//
// Created by turiing on 2025/1/5.
//

#include "core/common/Material.h"

#include "core/RHI/RHITexture.h"
#include "utility/File.h"

Material::Material(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool, std::vector<TextureBind>& binds): m_pDevice(device) {
    for(const auto & [bindIndex, path] : binds) {
        m_mapTexture[bindIndex] = std::make_shared<RHITexture>(m_pDevice, commandPool, File::FromStdString(path));
    }
}
