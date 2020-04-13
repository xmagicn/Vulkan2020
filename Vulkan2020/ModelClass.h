#pragma once

#include <glm/glm.hpp>
#include <array>

#pragma warning( disable : 4201 )

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "vulkan/vulkan.h"

class VulkanGraphicsInstance;

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 uv;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof( Vertex );
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 4> AttributeDescriptions = {};

		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[0].offset = offsetof( Vertex, pos );

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[1].offset = offsetof( Vertex, color );

		AttributeDescriptions[2].binding = 0;
		AttributeDescriptions[2].location = 2;
		AttributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[2].offset = offsetof( Vertex, normal );

		AttributeDescriptions[3].binding = 0;
		AttributeDescriptions[3].location = 3;
		AttributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescriptions[3].offset = offsetof( Vertex, uv );

		return AttributeDescriptions;
	}

	bool operator==( const Vertex& other ) const
	{
		return pos == other.pos && color == other.color && normal == other.normal && uv == other.uv;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()( Vertex const& vertex ) const
		{
			return ( 
				( hash<glm::vec3>()( vertex.pos ) ^
				( hash<glm::vec3>()( vertex.color ) << 1 ) ) >> 1 ) ^
				//( hash<glm::vec3>()( vertex.normal ) << 1 ) ) >> 1 ) ^
				( hash<glm::vec2>()( vertex.uv ) << 1 );
		}
	};
}

class VulkanTexture
{
public:
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
};

class Model
{
public:
	void Initialize( VulkanGraphicsInstance* pInstance );
	void BindToCommandBuffer( VkCommandBuffer& rBuffer, VkPipeline& rPipeline, VkPipelineLayout& rPipelineLayout, size_t idx );
	void Cleanup();

private:
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();

	void LoadModel();

	void CreateVertexBuffer();
	void CreateIndexBuffer();

public:
	void CreateDescriptorSets();

	VulkanGraphicsInstance* pGraphicsInstance;

public:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
	VkBuffer IndexBuffer;
	VkDeviceMemory IndexBufferMemory;

	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;

	uint32_t MipLevels;
	VkImage TextureImage;
	VkDeviceMemory TextureImageMemory;
	VkImageView TextureImageView;
	VkSampler TextureSampler;

	std::vector<VkDescriptorSet> DescriptorSets;
};