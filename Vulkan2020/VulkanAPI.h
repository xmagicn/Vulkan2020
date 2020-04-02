#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include <optional>
#include <iostream>
#include <array>

#include "ModelClass.h"

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

/* Rainbow triangle
const std::vector<Vertex> vertices =
{
	{{ 0.0, -0.5 }, {1.0, 0.0, 0.0}},
	{{ 0.5, 0.5 }, {0.0, 1.0, 0.0}},
	{{ -0.5, 0.5 }, {0.0, 0.0, 1.0}}
};

/* Blue/Green triangle
const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

//* Rainbow square
const std::vector<Vertex> vertices = 
{
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
	{ { 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
	{ { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },

	{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
	{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
	{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	{ {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },

};

const std::vector<uint16_t> indexes =
{
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4,
};
//*/

class VulkanAPI
{
public:
	VulkanAPI();
	~VulkanAPI();

	void Create();
	void Init();
	void Draw() { DrawFrame(); }
	void Cleanup();

	VkInstance* GetInstance() { return &vulkanInstance; }
	VkSurfaceKHR* GetSurface() { return &vulkanSurface; }

	void SetExtent( uint32_t width, uint32_t height )
	{
		Width = width;
		Height = height;
		bFrameBufferResized = true;
	}

	void WaitForFrameComplete();

private:
	VulkanAPI( const VulkanAPI& ) = delete;
	VulkanAPI& operator=( const VulkanAPI& ) = delete;

	void CreateInstance();
	void DrawFrame();
	void RecreateSwapChain();

	void CleanupSwapChain();

	void SetupDebugMessenger();
	void PopulateDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT& createInfo );

	void CreateBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory );
	void CopyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );

	void CreateImage( uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory );
	void TransitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels );
	void CopyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height );
	void CreateTextureImageView();
	void CreateTextureSampler();

	void PickPhysicalDevice();
	bool IsDeviceSuitable( VkPhysicalDevice device );
	QueueFamilyIndices FindQueueFamilies( VkPhysicalDevice device );
	bool CheckDeviceExtensionSupport( VkPhysicalDevice device );
	SwapChainSupportDetails QuerySwapChainSupport( VkPhysicalDevice device );

	void CreateLogicalDevice();

	void CreateSwapChain();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats );
	VkPresentModeKHR ChooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes );
	VkExtent2D ChooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

	VkImageView CreateImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels );
	void CreateImageViews();
	
	void CreateRenderPass();

	void CreateDescriptorSetLayout();

	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule( const std::vector<char>& code );

	void CreateFramebuffers();

	void CreateCommandPool();

	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands( VkCommandBuffer commandBuffer );
	void CreateCommandBuffers();

	void CreateSyncObjects();

	void GenerateMipmaps( VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels );
	void CreateTextureImage();

	void LoadModel();
	void CreateVertexBuffer( const std::vector<Vertex>& vertexData );
	void CreateIndexBuffer( const std::vector<uint32_t>& indexData );
	
	void CreateDepthResources();
	VkFormat FindDepthFormat();
	bool HasStencilComponent( VkFormat format );

	VkFormat FindSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void UpdateUniformBuffer( uint32_t currentImage );

	void CreateColorResources();

	VkSampleCountFlagBits GetMaxUsableSampleCount();

	uint32_t Width;
	uint32_t Height;

	VkInstance vulkanInstance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR vulkanSurface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice vulkanDevice;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass; 
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

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

	VkDescriptorPool DescriptorPool;
	std::vector<VkDescriptorSet> DescriptorSets;

	VkImage DepthImage;
	VkDeviceMemory DepthImageMemory;
	VkImageView DepthImageView;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indexes;

	VkImage ColorImage;
	VkDeviceMemory ColorImageMemory;
	VkImageView ColorImageView;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	bool bFrameBufferResized = false;

public:
	void RegisterExtensions( std::vector<const char*>& extensionVec )
	{
		for ( const char* extension : extensionVec )
		{
			extensions.push_back( extension );
		}
	}

	uint32_t FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties )
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties( physicalDevice, &memProperties );

		for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ )
		{
			if ( ( typeFilter & ( 1 << i ) ) && ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties )
			{
				return i;
			}
		}

		assert( false && "failed to find suitable memory type!" );
		return 0;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* )
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

private:
	// Validation Layers
	bool CheckValidationLayerSupport();

	std::vector<const char*> extensions;
};       