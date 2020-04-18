#pragma once

#include <vector>

#include "GraphicsCommon.h"

class VulkanGraphicsInstance;

class VulkanShader
{
public:
	VulkanShader( VulkanGraphicsInstance* pInstance, const char* filename );
	VulkanShader( const VulkanShader& ) = default;
	VulkanShader& operator=( const VulkanShader& ) = default;
	virtual ~VulkanShader();

	virtual VkPipelineShaderStageCreateInfo GetCreateInfo() = 0;

protected:
	void GetCreateInfoInternal( VkPipelineShaderStageCreateInfo& info );

	VkShaderModule CreateShaderModule( const std::vector<char>& code );

	std::vector<char> shaderCode;
	VkShaderModule shaderModule;
	VulkanGraphicsInstance* pGraphicsInstance;
};

class VulkanVertexShader : public VulkanShader
{
public:
	VulkanVertexShader( VulkanGraphicsInstance* pInstance, const char* filename );
	VulkanVertexShader( const VulkanVertexShader& ) = default;
	VulkanVertexShader& operator=( const VulkanVertexShader& ) = default;
	virtual ~VulkanVertexShader() = default;

	virtual VkPipelineShaderStageCreateInfo GetCreateInfo() override;
};

class VulkanFragmentShader : public VulkanShader
{
public:
	VulkanFragmentShader( VulkanGraphicsInstance* pInstance, const char* filename );
	VulkanFragmentShader( const VulkanFragmentShader& ) = default;
	VulkanFragmentShader& operator=( const VulkanFragmentShader& ) = default;
	virtual ~VulkanFragmentShader() = default;

	virtual VkPipelineShaderStageCreateInfo GetCreateInfo() override;
};