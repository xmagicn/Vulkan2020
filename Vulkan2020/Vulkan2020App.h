#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>

#include "VulkanAPI.h"
#include "VulkanGraphicsInstance.h"

#include "GLFWRenderWindowClass.h"

const int WIDTH = 800;
const int HEIGHT = 600;

class Vulkan2020App
{
public:
	void Run()
	{
		InitWindow();
		InitGraphicsInstance();
		/*
		SubmitExtensions();
		CreateVulkanInstance();
		CreateSurface();
		InitVulkan();
		SubmitExtent();
		//*/
		MainLoop();
		Cleanup();
	}

private:
	GLFWwindow* window;

	VulkanAPI VulkanLayer;

	GLFWRenderWindow* pRenderWindow;
	VulkanGraphicsInstance* pGraphicsInstance;

	size_t currentFrame = 0;

	bool framebufferResized = false;

private:
	void InitWindow()
	{
		pRenderWindow = new GLFWRenderWindow();
		pRenderWindow->InitWindow();
	}

	void InitGraphicsInstance()
	{
		pGraphicsInstance = new VulkanGraphicsInstance();

		auto extensions = GetRequiredExtensions();
		pGraphicsInstance->PreInitInstance( extensions );

		pGraphicsInstance->InitInstance( pRenderWindow );
	}

	static void FramebufferResizeCallback( GLFWwindow* window, int, int )
	{
		auto app = reinterpret_cast< Vulkan2020App* >( glfwGetWindowUserPointer( window ) );
		app->SubmitExtent();
	}

	void CreateVulkanInstance()
	{
		VulkanLayer.Create();
	}

	void InitVulkan()
	{
		VulkanLayer.Init();
	}

	void MainLoop()
	{
		while ( !pRenderWindow->WindowShouldClose() )
		{
			pRenderWindow->PollEvents();
			pGraphicsInstance->DrawFrame();
		}

		pGraphicsInstance->WaitForFrameComplete();
	}

	void Cleanup()
	{
		pGraphicsInstance->DestroyInstance();
		delete pGraphicsInstance;
		pGraphicsInstance = nullptr;

		pRenderWindow->TerminateWindow();
		delete pRenderWindow;
		pRenderWindow = nullptr;
	}

	void SubmitExtensions()
	{
		auto extensions = GetRequiredExtensions();
		VulkanLayer.RegisterExtensions( extensions );
	}

	void SubmitExtent()
	{
		int width, height;
		glfwGetFramebufferSize( window, &width, &height );
		VulkanLayer.SetExtent( static_cast< uint32_t >( width ), static_cast< uint32_t >( height ) );
	}

	void CreateSurface()
	{
		VkResult result = glfwCreateWindowSurface( *VulkanLayer.GetInstance(), window, nullptr, VulkanLayer.GetSurface() );
		if ( result != VK_SUCCESS )
		{
			throw std::runtime_error( "failed to create window surface!" );
		}
	}

	std::vector<const char*> GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

		std::vector<const char*> extensions( glfwExtensions, glfwExtensions + glfwExtensionCount );

		return extensions;
	}
};