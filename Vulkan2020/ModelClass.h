#pragma once

#include <glm/glm.hpp>


#pragma warning( disable : 4201 )

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 uv;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof( Vertex );
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> AttributeDescriptions = {};

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
		AttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescriptions[2].offset = offsetof( Vertex, uv );

		return AttributeDescriptions;
	}

	bool operator==( const Vertex& other ) const
	{
		return pos == other.pos && color == other.color && uv == other.uv;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()( Vertex const& vertex ) const
		{
			return ( ( hash<glm::vec3>()( vertex.pos ) ^
				( hash<glm::vec3>()( vertex.color ) << 1 ) ) >> 1 ) ^
				( hash<glm::vec2>()( vertex.uv ) << 1 );
		}
	};
}

class Model
{
public:
	std::vector<Vertex> vertices;

	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
};