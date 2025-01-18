//
// Created by turiing on 2025/1/6.
//

#include "core/Render/RenderScene.h"

#include "core/common/Camera.h"
#include "core/common/Material.h"
#include "core/Render/RenderEntity.h"
#include "core/RHI/RHIDescriptorSet.h"
#include "core/RHI/RHITexture.h"

RenderScene::RenderScene(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool): m_pDevice(device), m_pCommandPool(commandPool) {
    m_pCamera = std::make_shared<Camera>();
}

void RenderScene::OnMouseMove(MouseButton button, const Point &pos) const {
    m_pCamera->OnMouseMove(pos.x, pos.y, button);
}

void RenderScene::OnMouseScroll(double delta) const {
    m_pCamera->OnMouseWheelScroll(delta);
}

void RenderScene::OnResize(const Size& size) const {
    m_pCamera->SetWindowSize(size);
}

void RenderScene::AddRenderEntity(const std::string& path) {
    LOG_ASSERT(!path.empty());

    m_vecRenderEntity.emplace_back(std::make_shared<RenderEntity>(m_pDevice, m_pCommandPool, path));
    LOG_INFO("Added render entity");
}

void RenderScene::UpdateEntityTexture(const std::shared_ptr<RHIDescriptorSet>& descriptorSet) const {
    for(auto &&entity : m_vecRenderEntity) {
        for(const auto &[bindIndex, texture] : entity->GetMaterial()->GetTextures()) {
            descriptorSet->UpdateTextureImage(texture->GetDescriptorImageInfo(), bindIndex);
        }
    }
}
