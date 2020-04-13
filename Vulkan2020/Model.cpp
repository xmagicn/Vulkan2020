#include "ModelClass.h"

#pragma warning( disable : 4189 )

#include "FileUtils.h"
#include "VulkanGraphicsInstance.h"

#include <chrono>

void Model::Initialize( VulkanGraphicsInstance* pInstance )
{
	pGraphicsInstance = pInstance;

	CreateTextureImage();
	CreateTextureImageView();
	CreateTextureSampler();
	LoadModel();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateDescriptorSets();
}

void Model::BindToCommandBuffer( VkCommandBuffer& rBuffer, VkPipeline& rPipeline, VkPipelineLayout& rPipelineLayout, size_t idx )
{
	vkCmdBindPipeline( rBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipeline );

	
	VkBuffer vertexBuffers[] = { VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers( rBuffer, 0, 1, vertexBuffers, offsets );

	vkCmdBindIndexBuffer( rBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT32 );

	vkCmdBindDescriptorSets( rBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rPipelineLayout, 0, 1, &DescriptorSets[idx], 0, nullptr );
	vkCmdDrawIndexed( rBuffer, static_cast< uint32_t >( indices.size() ), 1, 0, 0, 0 );
}

void Model::Cleanup()
{

	vkDestroySampler( *pGraphicsInstance->GetDevice(), TextureSampler, nullptr );
	vkDestroyImageView( *pGraphicsInstance->GetDevice(), TextureImageView, nullptr );
	vkDestroyImage( *pGraphicsInstance->GetDevice(), TextureImage, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), TextureImageMemory, nullptr );

	//vkDestroyDescriptorSetLayout( vulkanDevice, descriptorSetLayout, nullptr );

	vkDestroyBuffer( *pGraphicsInstance->GetDevice(), IndexBuffer, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), IndexBufferMemory, nullptr );

	vkDestroyBuffer( *pGraphicsInstance->GetDevice(), VertexBuffer, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), VertexBufferMemory, nullptr );

}

void Model::CreateTextureImage()
{
	int texWidth;
	int texHeight;
	int texChannels;

	void* pixels = FileUtils::OpenTexture( "chaletTex.jpg", texWidth, texHeight, texChannels );
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	MipLevels = static_cast< uint32_t >( std::floor( std::log2( std::max( texWidth, texHeight ) ) ) ) + 1;

	assert( pixels && "failed to load texture image!" );

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	pGraphicsInstance->CreateBuffer( imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, 0, imageSize, 0, &data );
	memcpy( data, pixels, static_cast< size_t >( imageSize ) );
	vkUnmapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory );

	FileUtils::CloseTexture( pixels );

	pGraphicsInstance->CreateImage( texWidth, texHeight, MipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, TextureImage, TextureImageMemory );

	pGraphicsInstance->TransitionImageLayout( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, MipLevels );
	pGraphicsInstance->CopyBufferToImage( stagingBuffer, TextureImage, static_cast< uint32_t >( texWidth ), static_cast< uint32_t >( texHeight ) );

	//transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps
	//TransitionImageLayout( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, MipLevels );
	pGraphicsInstance->GenerateMipmaps( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, MipLevels );

	vkDestroyBuffer( *pGraphicsInstance->GetDevice(), stagingBuffer, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, nullptr );
}

void Model::CreateTextureImageView()
{
	TextureImageView = pGraphicsInstance->CreateImageView( TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, MipLevels );
}

void Model::CreateTextureSampler()
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	//	samplerInfo.minLod = static_cast< float >( MipLevels / 2 );
	//	samplerInfo.maxLod = 0;
	samplerInfo.maxLod = static_cast< float >( MipLevels );
	samplerInfo.mipLodBias = 0.0f;

	VkResult result = vkCreateSampler( *pGraphicsInstance->GetDevice(), &samplerInfo, nullptr, &TextureSampler );
	assert( VK_SUCCESS == result && "failed to create texture sampler!" );
}

void Model::LoadModel()
{
	//FileUtils::LoadModel( "../assets/models/chalet.obj", vertices, indices );
	FileUtils::LoadModel( "../assets/models/Cactus_4.obj", vertices, indices );
}

void Model::CreateVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof( vertices[0] ) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	pGraphicsInstance->CreateBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data );
	memcpy( data, vertices.data(), ( size_t )bufferSize );
	vkUnmapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory );

	pGraphicsInstance->CreateBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory );

	pGraphicsInstance->CopyBuffer( stagingBuffer, VertexBuffer, bufferSize );

	vkDestroyBuffer( *pGraphicsInstance->GetDevice(), stagingBuffer, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, nullptr );
}

void Model::CreateIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof( indices[0] ) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	pGraphicsInstance->CreateBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

	void* data;
	vkMapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data );
	memcpy( data, indices.data(), ( size_t )bufferSize );
	vkUnmapMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory );

	pGraphicsInstance->CreateBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory );

	pGraphicsInstance->CopyBuffer( stagingBuffer, IndexBuffer, bufferSize );

	vkDestroyBuffer( *pGraphicsInstance->GetDevice(), stagingBuffer, nullptr );
	vkFreeMemory( *pGraphicsInstance->GetDevice(), stagingBufferMemory, nullptr );
}

void Model::CreateDescriptorSets()
{
	pGraphicsInstance->CreateImageSamplerDescriptorSet( DescriptorSets, TextureImageView, TextureSampler );
}