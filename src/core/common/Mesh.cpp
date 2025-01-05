//
// Created by turiing on 2025/1/5.
//
#include "core/common/Mesh.h"

#include "core/RHI/RHIIndexBuffer.h"
#include "core/RHI/RHIVertexBuffer.h"
#include "utility/File.h"


Mesh::Mesh(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandPool>& commandPool, File&& file): m_pDevice(device) {
    const auto &[vertices, indices] = file.GetModelData();

    m_indexCount = indices.size();
    m_pVertexBuffer = std::make_shared<RHIVertexBuffer>(m_pDevice, commandPool, vertices);
    m_pIndexBuffer = std::make_shared<RHIIndexBuffer>(m_pDevice, commandPool, indices);
}

Mesh::~Mesh() {
}
