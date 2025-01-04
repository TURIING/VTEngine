//
// Created by turiing on 2025/1/4.
//
#include "utility/File.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
