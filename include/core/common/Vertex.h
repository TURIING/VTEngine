/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/12/14 20:09
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef VERTEX_H
#define VERTEX_H
#include "common/common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    // glm::vec3 normal;
    // glm::vec4 tangent;

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;                                                 // 绑定的索引号
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;                     // 在每个顶点之后移动到下一个数据条目

        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
        // vertex object position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        // proper color
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        // uv
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        // // normal
        // attributeDescriptions[3].binding = 0;
        // attributeDescriptions[3].location = 3;
        // attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        // attributeDescriptions[3].offset = offsetof(Vertex, normal);
        // // tangent
        // attributeDescriptions[4].binding = 0;
        // attributeDescriptions[4].location = 4;
        // attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        // attributeDescriptions[4].offset = offsetof(Vertex, tangent);
        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

template<> struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const noexcept {
        return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};
#endif //VERTEX_H
