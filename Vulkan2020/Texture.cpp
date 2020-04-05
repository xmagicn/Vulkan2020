#include "TextureClass.h"

#include "FileUtils.h"

Texture::Texture( const char* filename )
{
	int width;
	int height;
	int channels;
	FileUtils::LoadTexture( filename, width, height, channels );
}

Texture::~Texture()
{

}

void Texture::CreateTextureImage()
{
	/*
	int texWidth;
	int texHeight;
	int texChannels;

	stbi_uc* pixels = LoadTexture( "chaletTex.jpg", texWidth, texHeight, texChannels );
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	MipLevels = static_cast< uint32_t >( std::floor( std::log2( std::max( texWidth, texHeight ) ) ) ) + 1;

	assert( pixels && "failed to load texture image!" );

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory( vulkanDevice, stagingBufferMemory, 0, imageSize, 0, &data );
	memcpy( data, pixels, static_cast< size_t >( imageSize ) );
	vkUnmapMemory( vulkanDevice, stagingBufferMemory );

	stbi_image_free( pixels );

	CreateImage( texWidth, texHeight, MipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TextureImage, TextureImageMemory );

	TransitionImageLayout( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, MipLevels );
	CopyBufferToImage( stagingBuffer, TextureImage, static_cast< uint32_t >( texWidth ), static_cast< uint32_t >( texHeight ) );

	//transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps
	//TransitionImageLayout( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, MipLevels );
	GenerateMipmaps( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, MipLevels );

	vkDestroyBuffer( vulkanDevice, stagingBuffer, nullptr );
	vkFreeMemory( vulkanDevice, stagingBufferMemory, nullptr );
	//*/
}