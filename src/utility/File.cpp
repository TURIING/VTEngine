//
// Created by turiing on 2025/1/4.
//
#include "utility/File.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "core/common/Vertex.h"

File::File(const std::string& path): m_path(path) {
    LOG_ASSERT(!path.empty());

    m_fileType = GetFileTypeFromSuffix(Utility::GetFileExtension(path));
}

File::~File() {
    if(m_pImageData) {
        stbi_image_free(m_pImageData);
    }
}

File File::FromStdString(const std::string& path) {
    return File { path };
}

ImageInfo File::GetImageData() {
    LOG_ASSERT(m_fileType == FileType::Image);

    int width, height, channels;
    m_pImageData = stbi_load(m_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    LOG_ASSERT(m_pImageData != nullptr);

    return ImageInfo {
        .data = m_pImageData,
        .size = { .width = static_cast<uint32_t>(width), .height = static_cast<uint32_t>(height) },
    };
}

std::vector<char> File::GetShaderData() const {
    LOG_ASSERT(m_fileType == FileType::Shader);

    return Utility::readFile(m_path);
}

ModelData&& File::GetModelData() const {
    LOG_ASSERT(m_fileType == FileType::Model);

    ModelData modelData;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, m_path.c_str())) {
        LOG_CRITICAL("Failed to load obj file '{}': {}", m_path.c_str(), err.c_str());
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for(const auto &shape : shapes) {
        for(const auto &index : shape.mesh.indices) {
            Vertex vertex;

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.vertex_index + 0],
                1.0f - attrib.texcoords[2 * index.vertex_index + 1]                             // 反转纹理的Y坐标
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if(!uniqueVertices.contains(vertex)) {
                uniqueVertices[vertex] = static_cast<uint32_t>(modelData.vertices.size());
                modelData.vertices.push_back(vertex);
            }

            modelData.indices.push_back(uniqueVertices[vertex]);
        }
    }

    return std::move(modelData);
}


FileType File::GetFileTypeFromSuffix(const std::string& suffix) {
    if(const std::unordered_set<std::string> imageSuffixes { "jpg", "jpeg", "png", "gif", "bmp"}; imageSuffixes.contains(suffix)) {
        return FileType::Image;
    }
    else if(const std::unordered_set<std::string> shaderSuffixes { "spv"}; shaderSuffixes.contains(suffix)) {
        return FileType::Shader;
    }
    else if(const std::unordered_set<std::string> modelSuffixes { "obj"}; modelSuffixes.contains(suffix)) {
        return FileType::Model;
    }
    return FileType::None;
}
