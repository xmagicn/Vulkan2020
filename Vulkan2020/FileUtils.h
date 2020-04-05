#pragma once

#include <string>
#include <vector>

#include "ModelClass.h"

constexpr const char* TEXTURE_PATH = "../assets/textures/";
constexpr const char* MODEL_PATH = "../assets/models/";

struct FileUtils
{
	static std::vector<char> ReadFile( const std::string& filename );
	static void* OpenTexture( const char* filename, int& texWidth, int& texHeight, int& texChannels );
	static void CloseTexture( void* );
	static void LoadModel( const char* filename, std::vector<Vertex>& uniqueVertices, std::vector<uint32_t>& indices );
};