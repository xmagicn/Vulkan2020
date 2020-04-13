#include "FileUtils.h"

#include <iostream>
#include <fstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::vector<char> FileUtils::ReadFile( const std::string& filename )
{
	std::ifstream file( filename, std::ios::ate | std::ios::binary );

	assert( file.is_open() && "failed to open file!" );

	size_t fileSize = ( size_t )file.tellg();
	std::vector<char> buffer( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
}

void* FileUtils::OpenTexture( const char* filename, int& texWidth, int& texHeight, int& texChannels )
{
	stbi_uc* pixels = stbi_load( ( std::string( TEXTURE_PATH ) + filename ).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );

	assert( pixels && "failed to load texture image!" );

	return pixels;
}

void FileUtils::CloseTexture( void* pData )
{
	stbi_image_free( pData );
}

void FileUtils::LoadModel( const char* filename, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices )
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if ( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, filename ) )
	{
		throw std::runtime_error( warn + err );
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for ( const auto& shape : shapes )
	{
		for ( const auto& index : shape.mesh.indices )
		{
			Vertex vertex = {};

			vertex.pos =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			/*
			vertex.uv =
			{
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			//*/
			vertex.color = { 1.0f, 1.0f, 1.0f };

			if ( uniqueVertices.count( vertex ) == 0 )
			{
				uniqueVertices[vertex] = static_cast< uint32_t >( vertices.size() );
				vertices.push_back( vertex );
			}

			indices.push_back( uniqueVertices[vertex] );
		}
	}
}