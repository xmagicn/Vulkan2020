#include "FileUtils.h"

#include <iostream>
#include <fstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static std::string GetBaseDir( const std::string& filepath )
{
	if ( filepath.find_last_of( "/\\" ) != std::string::npos )
		return filepath.substr( 0, filepath.find_last_of( "/\\" ) );
	return "";
}

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

	if ( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, filename, GetBaseDir( filename ).c_str() ) )
	{
		throw std::runtime_error( warn + err );
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for ( const auto& shape : shapes )
	{
		size_t faceIndex = 0;
		size_t vertCt = 0;

		for ( const auto& index : shape.mesh.indices )
		{
			Vertex vertex = {};

			vertex.pos =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			if ( !attrib.texcoords.empty() )
			{
				vertex.uv =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			if ( !attrib.normals.empty() )
			{
				vertex.color = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if ( !materials.empty() )
			{
				size_t matIdx = shape.mesh.material_ids[faceIndex];

				vertex.color = {
					materials[matIdx].diffuse[0],
					materials[matIdx].diffuse[1],
					materials[matIdx].diffuse[2],
				};
			}
			else
			{
				vertex.color = {
					1.0f,
					1.0f,
					1.0f,
				};
			}

			if ( uniqueVertices.count( vertex ) == 0 )
			{
				uniqueVertices[vertex] = static_cast< uint32_t >( vertices.size() );
				vertices.push_back( vertex );
			}

			indices.push_back( uniqueVertices[vertex] );

			if ( ++vertCt >= shape.mesh.num_face_vertices[faceIndex] )
			{
				faceIndex++;
				vertCt = 0;
			}
		}
	}
}