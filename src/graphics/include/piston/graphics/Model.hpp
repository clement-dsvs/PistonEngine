#ifndef MODEL_HPP
#define MODEL_HPP

#include "piston/graphics/Device.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace piston {
    class Model {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position
                       && color == other.color
                       && normal == other.normal
                       && uv == other.uv;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filepath);
        };

        Model(Device &a_device, const Model::Builder &a_builder);

        ~Model();

        Model(const Model &) = delete;

        Model &operator=(Model &) = delete;

        static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string filepath);

        void o_bind(VkCommandBuffer a_commandBuffer);

        void o_draw(VkCommandBuffer a_commandBuffer);

    private:
        void o_createVertexBuffers(const std::vector<Vertex> &a_vertices);

        void o_createIndexBuffers(const std::vector<uint32_t> &a_indices);

        Device &m_device;

        VkBuffer m_vertexBuffer;
        VkDeviceMemory m_vertexBufferMemory;
        uint32_t m_vertexCount;

        bool m_hasIndexBuffer = false;
        VkBuffer m_indexBuffer;
        VkDeviceMemory m_indexBufferMemory;
        uint32_t m_indexCount;
    };
}

#endif //MODEL_HPP
