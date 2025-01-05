//
// Created by turiing on 2025/1/5.
//
#include "core/Render/RenderEntity.h"

#include "core/common/Material.h"
#include "core/common/Mesh.h"
#include "utility/File.h"

RenderEntity::RenderEntity(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool): m_pDevice(device) {
    m_pMesh = std::make_shared<Mesh>(m_pDevice, commandPool, File::FromStdString(MODEL_DIR + "viking room/viking_room.obj"));

    std::vector<TextureBind> binds = {
        TextureBind { 1, MODEL_DIR + "viking room/viking_room.png" }
    };
    m_pMaterial = std::make_shared<Material>(m_pDevice, commandPool, binds);
}
