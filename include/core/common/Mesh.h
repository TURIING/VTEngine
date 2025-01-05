//
// Created by turiing on 2025/1/5.
//

#ifndef MESH_H
#define MESH_H

#include "common/common.h"

class RHIIndexBuffer;
class RHIVertexBuffer;
struct Vertex;
class File;
class RHICommandPool;
class RHIDevice;

class Mesh {
public:
    Mesh(const std::shared_ptr<RHIDevice> &device, const std::shared_ptr<RHICommandPool> &commandPool, File &&file);
    ~Mesh();
    [[nodiscard]] std::shared_ptr<RHIVertexBuffer> GetVertexBuffer() const { return m_pVertexBuffer; }
    [[nodiscard]] std::shared_ptr<RHIIndexBuffer> GetIndexBuffer() const { return m_pIndexBuffer; }

private:
    std::shared_ptr<RHIDevice> m_pDevice;
    std::shared_ptr<RHIVertexBuffer> m_pVertexBuffer;
    std::shared_ptr<RHIIndexBuffer> m_pIndexBuffer;
};
#endif //MESH_H
