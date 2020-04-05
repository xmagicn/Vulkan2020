#pragma once

#include "GraphicsInstance.h"

#include <optional>
#include <vector>

#include "vulkan/vulkan.h"

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

class VulkanGraphicsInstance : public GraphicsInstance
{
public:
	VulkanGraphicsInstance();
	virtual ~VulkanGraphicsInstance();

	void PreInitInstance( std::vector<const char*> requiredExtensions );

	virtual void WaitForFrameComplete() override;

	virtual void ResizeFrame( unsigned int width, unsigned int height ) override;

	VkInstance* GetInstance() { return &vulkanInstance; }
	VkDevice* GetDevice() { return &vulkanDevice; }

private:
	VulkanGraphicsInstance( const VulkanGraphicsInstance& ) = delete;
	VulkanGraphicsInstance& operator=( const VulkanGraphicsInstance& ) = delete;

	virtual bool InitInstanceInternal( RenderWindow* pRenderWindow ) override;
	virtual bool DestroyInstanceInternal() override;

	virtual void DrawFrameInternal() override;

/////////////////////////////////////////
// VulkanAPI Functions
/////////////////////////////////////////

/////////////////////////////////////////
// Initialization Functions
/////////////////////////////////////////

	void CreateInstance();

	void RecreateSwapChain();

	void PickPhysicalDevice();
	bool IsDeviceSuitable( VkPhysicalDevice device );
	VkSampleCountFlagBits GetMaxUsableSampleCount();
	QueueFamilyIndices FindQueueFamilies( VkPhysicalDevice device );
	bool CheckDeviceExtensionSupport( VkPhysicalDevice device );
	SwapChainSupportDetails QuerySwapChainSupport( VkPhysicalDevice device );

	void CreateLogicalDevice();
	
	void CreateSwapChain();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats );
	VkPresentModeKHR ChooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes );
	VkExtent2D ChooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities );

public:
	VkImageView CreateImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels );
private:
	void CreateImageViews();

	void CreateRenderPass();
	VkFormat FindDepthFormat();
	VkFormat FindSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features );

	void CreateDescriptorSetLayout();

	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule( const std::vector<char>& code );

	void CreateCommandPool();

	void CreateColorResources();
public:
	void GenerateMipmaps( VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels );
	void CreateImage( uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory );
private:
	uint32_t FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties );

	void CreateDepthResources();
	bool HasStencilComponent( VkFormat format );


	void CreateUniformBuffers();

	void CreateDescriptorPool();

	void CreateCommandBuffers();

	void CreateSyncObjects();

	//////////////////////////////
	// Buffer Functions
	//////////////////////////////
public:
	void CreateBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory );
	void CopyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );
	void CopyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height );

	//////////////////////////////
	// Command Functions
	//////////////////////////////
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands( VkCommandBuffer commandBuffer );
	void TransitionImageLayout( VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels );

	void CreateFramebuffers();
	void CreateDescriptorSets();

/////////////////////////////////////////
// Cleanup Functions
/////////////////////////////////////////

private:
	void CleanupSwapChain();

/////////////////////////////////////////
// Update Functions
/////////////////////////////////////////

	void UpdateUniformBuffer( uint32_t );

/////////////////////////////////////////
// Debug Functions
/////////////////////////////////////////

#ifdef _DEBUG
	void SetupDebugMessenger();
	bool CheckValidationLayerSupport();
	void PopulateDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT& createInfo );
	VkResult CreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger );
	void DestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator );
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* );
#endif

	bool bFrameBufferResized = false;

	uint32_t Width;
	uint32_t Height;

	VkInstance vulkanInstance;
	VkSurfaceKHR vulkanSurface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

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

	VkImage ColorImage;
	VkDeviceMemory ColorImageMemory;
	VkImageView ColorImageView;

	VkImage DepthImage;
	VkDeviceMemory DepthImageMemory;
	VkImageView DepthImageView;

	VkDescriptorPool DescriptorPool;
	std::vector<VkDescriptorSet> DescriptorSets;

	std::vector<VkBuffer> UniformBuffers;
	std::vector<VkDeviceMemory> UniformBuffersMemory;


	const int MAX_FRAMES_IN_FLIGHT = 2;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	std::vector<const char*> extensions;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT debugMessenger;
#endif

	void InitializeRenderObjects() { TEST_MODEL.Initialize( this ); }

	Model TEST_MODEL;
};