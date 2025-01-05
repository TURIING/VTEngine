//
// Created by turiing on 2025/1/5.
//

#ifndef RENDERENTITY_H
#define RENDERENTITY_H

#include "common/common.h"

class Transform;
class Mesh;
class Material;

class RenderEntity {
public:

private:
    std::shared_ptr<Material> m_pMaterial;
    std::shared_ptr<Mesh> m_pMesh;
    std::shared_ptr<Transform> m_pTransform;
};
#endif //RENDERENTITY_H
