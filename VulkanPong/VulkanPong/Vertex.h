#pragma once
#include "pch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>

struct Vertex {
    glm::vec2 pos; // absolute 2d position
    glm::vec3 color; // absolute rgb color

    // for populating the right data onto the vertex input binder
    static VkVertexInputBindingDescription getBindingDescription() {
        // data formatting
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    // for describing vertex attribute description
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        // position attributes
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SINT; // is always in order RGB bc of how GPU works, and can be 64 bit (i.e. 64b SFLOAT = signed double, 32b uint = unsinged int)
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        // color attributes
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SINT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }

    // equality operator overload
    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec2>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1);
        }
    };
}