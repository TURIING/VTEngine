//
// Created by turiing on 2025/1/4.
//

#ifndef FILE_H
#define FILE_H

#include "common/common.h"

struct Vertex;

enum class FileType { None, Image, Shader, Model };

struct ModelData {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class File {
public:
    File(const std::string &path);
    ~File();
    static File FromStdString(const std::string& path);
    [[nodiscard]] ImageInfo GetImageData();
    [[nodiscard]] std::vector<char> GetShaderData() const;
    [[nodiscard]] ModelData&& GetModelData() const;
    [[nodiscard]] FileType GetFileType() const { return m_fileType; };
    [[nodiscard]] std::string GetPath() const { return m_path; };

private:
    static FileType GetFileTypeFromSuffix(const std::string &suffix);

private:
    std::string m_path;
    FileType m_fileType;
    unsigned char *m_pImageData = nullptr;
};
#endif //FILE_H
