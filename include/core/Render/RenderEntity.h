//
// Created by turiing on 2025/1/5.
//

#ifndef RENDERENTITY_H
#define RENDERENTITY_H

#include "common/common.h"

class RHICommandPool;
class RHIDevice;
class Transform;
class Mesh;
class Material;

class RenderEntity {
public:
    RenderEntity(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool);
    [[nodiscard]] std::shared_ptr<Material> GetMaterial() const { return m_pMaterial; }
    [[nodiscard]] std::shared_ptr<Mesh> GetMesh() const { return m_pMesh; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<Material> m_pMaterial;
    std::shared_ptr<Mesh> m_pMesh;
    std::shared_ptr<Transform> m_pTransform;
};
#endif //RENDERENTITY_H
