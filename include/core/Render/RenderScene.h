//
// Created by turiing on 2025/1/6.
//

#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "common/common.h"

class RHIDescriptorSet;
class RHICommandPool;
class RenderEntity;
class Camera;
class RHIDevice;

class RenderScene {
public:
    RenderScene(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool);
    void OnMouseMove(MouseButton button, const Point &pos) const;
    void OnMouseScroll(double delta) const;
    void OnResize(const Size &size) const;
    void AddRenderEntity(const std::string &path);
    void UpdateEntityTexture(const std::shared_ptr<RHIDescriptorSet> &descriptorSet) const;
    [[nodiscard]] std::shared_ptr<Camera> GetCamera() const { return m_pCamera; }
    [[nodiscard]] std::shared_ptr<RenderEntity> GetEntity() { return m_vecRenderEntity[0]; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHICommandPool> m_pCommandPool;
    std::shared_ptr<Camera> m_pCamera;
    std::vector<std::shared_ptr<RenderEntity>> m_vecRenderEntity;
};
#endif //RENDERSCENE_H
