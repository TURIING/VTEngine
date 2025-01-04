//
// Created by turiing on 2025/1/4.
//

#ifndef FILE_H
#define FILE_H

#include "common/common.h"

enum class FileType { None, Image, Shader, Model };

class File {
public:
    File(const std::string &path);
    ~File();
    static File FromStdString(const std::string& path);
    [[nodiscard]] ImageInfo GetImageData();
    [[nodiscard]] std::vector<char> GetShaderData() const;
    [[nodiscard]] FileType GetFileType() const { return m_fileType; };

private:
    static FileType GetFileTypeFromSuffix(const std::string &suffix);

private:
    std::string m_path;
    FileType m_fileType;
    unsigned char *m_pImageData = nullptr;
};
#endif //FILE_H
