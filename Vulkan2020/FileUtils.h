#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr const char* TEXTURE_PATH = "../assets/textures/";

static std::vector<char> ReadFile( const std::string& filename )
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

static stbi_uc* LoadTexture( const char* filename, int& texWidth, int& texHeight, int& texChannels )
{
	stbi_uc* pixels = stbi_load( ( std::string( TEXTURE_PATH ) + filename ).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
	//VkDeviceSize imageSize = texWidth * texHeight * 4;

	assert( pixels && "failed to load texture image!" );

	return pixels;
}