#include "ShaderClass.h"

#pragma warning( disable : 4189 )

#include "FileUtils.h"
#include "VulkanGraphicsInstance.h"

VulkanShader::VulkanShader( VulkanGraphicsInstance* pInstance, const char* filename )
	: pGraphicsInstance( pInstance )
{
	shaderCode = FileUtils::ReadFile( filename );
	shaderModule = CreateShaderModule( shaderCode );
}

VulkanShader::~VulkanShader()
{
	vkDestroyShaderModule( *pGraphicsInstance->GetDevice(), shaderModule, nullptr );
}

void VulkanShader::GetCreateInfoInternal( VkPipelineShaderStageCreateInfo& info )
{
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.module = shaderModule;
	info.pName = "main";
	info.pSpecializationInfo = nullptr;
}

VkShaderModule VulkanShader::CreateShaderModule( const std::vector<char>& code )
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast< const uint32_t* >( code.data() );

	VkShaderModule module;
	VkResult result = vkCreateShaderModule( *pGraphicsInstance->GetDevice(), &createInfo, nullptr, &module );
	assert( VK_SUCCESS == result && "failed to create shader module!" );

	return module;
}

VulkanVertexShader::VulkanVertexShader( VulkanGraphicsInstance* pInstance, const char* filename )
	: VulkanShader( pInstance, filename )
{}

VkPipelineShaderStageCreateInfo VulkanVertexShader::GetCreateInfo()
{
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	
	GetCreateInfoInternal( vertShaderStageInfo );
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	
	return vertShaderStageInfo;
}

VulkanFragmentShader::VulkanFragmentShader( VulkanGraphicsInstance* pInstance, const char* filename )
	: VulkanShader(pInstance, filename)
{}

VkPipelineShaderStageCreateInfo VulkanFragmentShader::GetCreateInfo()
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};

	GetCreateInfoInternal( shaderStageInfo ); 
	shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

	return shaderStageInfo;
}