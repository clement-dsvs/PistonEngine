//
// Created by cleme on 20/12/2024.
//

#include "piston/graphics/Model.hpp"

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <iostream>
//#include <tiny_obj_loader.h>
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/hash.hpp>


template<typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
	seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

template<>
struct std::hash<piston::Model::Vertex>
{
	size_t operator()(piston::Model::Vertex const& vertex) const
	{
		size_t seed = 0;
		hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
		return seed;
	}
};

std::vector<VkVertexInputBindingDescription> piston::Model::Vertex::getBindingDescriptions()
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> piston::Model::Vertex::getAttributeDescriptions()
{
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
	attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
	attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
	attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

	return attributeDescriptions;
}

piston::Model::Model(piston::Device& a_device, const piston::Model::Builder& a_builder)
	: m_device(a_device)
{
	o_createVertexBuffers(a_builder.vertices);
	o_createIndexBuffers(a_builder.indices);
}

piston::Model::~Model()
{
	vkDestroyBuffer(m_device.device(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_device.device(), m_vertexBufferMemory, nullptr);

	if (m_hasIndexBuffer)
	{
		vkDestroyBuffer(m_device.device(), m_indexBuffer, nullptr);
		vkFreeMemory(m_device.device(), m_indexBufferMemory, nullptr);
	}
}

std::unique_ptr<piston::Model> piston::Model::createModelFromFile(Device& device, const std::string filepath)
{
	Builder l_builder{};
	l_builder.loadModel(filepath);
	std::cout << "Vertex count: " << l_builder.vertices.size() << "\n";
	return std::make_unique<piston::Model>(device, l_builder);
}

void piston::Model::o_bind(VkCommandBuffer a_commandBuffer)
{
	const VkBuffer l_buffers[] = {m_vertexBuffer};
	constexpr VkDeviceSize l_offsets[] = {0};

	vkCmdBindVertexBuffers(a_commandBuffer, 0, 1, l_buffers, l_offsets);
	if (m_hasIndexBuffer)
	{
		vkCmdBindIndexBuffer(a_commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void piston::Model::o_draw(VkCommandBuffer a_commandBuffer)
{
	if (m_hasIndexBuffer)
	{
		vkCmdDrawIndexed(a_commandBuffer, m_indexCount, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(a_commandBuffer, m_vertexCount, 1, 0, 0);
	}
}

void piston::Model::o_createVertexBuffers(const std::vector<Vertex>& a_vertices)
{
	m_vertexCount = static_cast<uint32_t>(a_vertices.size());
	assert(m_vertexCount >= 3 && "Vertex count must be at least 3");

	VkDeviceSize bufferSize = sizeof(a_vertices[0]) * m_vertexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, a_vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_vertexBuffer,
		m_vertexBufferMemory
	);

	m_device.copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

void piston::Model::o_createIndexBuffers(const std::vector<uint32_t>& a_indices)
{
	m_indexCount = static_cast<uint32_t>(a_indices.size());
	m_hasIndexBuffer = m_indexCount > 0;

	if (!m_hasIndexBuffer)
	{
		return;
	}

	VkDeviceSize bufferSize = sizeof(a_indices[0]) * m_indexCount;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	m_device.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory
	);

	void* data;
	vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, a_indices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(m_device.device(), stagingBufferMemory);

	m_device.createBuffer(
		bufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_indexBuffer,
		m_indexBufferMemory
	);

	m_device.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

	vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
}

void piston::Model::Builder::loadModel(const std::string& filepath)
{
	throw std::runtime_error("not yet implemented");
	// tinyobj::attrib_t attrib;
	// std::vector<tinyobj::shape_t> shapes;
	// std::vector<tinyobj::material_t> materials;
	// std::string warn, err;
	//
	// if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
	// {
	// 	throw std::runtime_error(warn + err);
	// }
	//
	// vertices.clear();
	// indices.clear();
	//
	// std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	//
	// for (const auto& shape : shapes)
	// {
	// 	for (const auto& index : shape.mesh.indices)
	// 	{
	// 		Vertex l_vertex{};
	//
	// 		if (index.vertex_index >= 0)
	// 		{
	// 			l_vertex.position = {
	// 				attrib.vertices[3 * index.vertex_index + 0],
	// 				attrib.vertices[3 * index.vertex_index + 1],
	// 				attrib.vertices[3 * index.vertex_index + 2]
	// 			};
	//
	// 			l_vertex.color = {
	// 				attrib.colors[3 * index.vertex_index + 0],
	// 				attrib.colors[3 * index.vertex_index + 1],
	// 				attrib.colors[3 * index.vertex_index + 2]
	// 			};
	// 		}
	//
	// 		if (index.normal_index >= 0)
	// 		{
	// 			l_vertex.normal = {
	// 				attrib.normals[3 * index.normal_index + 0],
	// 				attrib.normals[3 * index.normal_index + 1],
	// 				attrib.normals[3 * index.normal_index + 2]
	// 			};
	// 		}
	//
	// 		if (index.texcoord_index >= 0)
	// 		{
	// 			l_vertex.uv = {
	// 				attrib.texcoords[2 * index.texcoord_index + 0],
	// 				attrib.texcoords[2 * index.texcoord_index + 1]
	// 			};
	// 		}
	//
	// 		if (uniqueVertices.count(l_vertex) == 0)
	// 		{
	// 			uniqueVertices[l_vertex] = static_cast<uint32_t>(vertices.size());
	// 			vertices.push_back(l_vertex);
	// 		}
	//
	// 		indices.push_back(uniqueVertices[l_vertex]);
	// 	}
	// }
}
