#pragma once

#include "RenderWindowClass.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWRenderWindow : public RenderWindow
{
public:

	virtual ~GLFWRenderWindow() = default;

	void InitWindow();
	void TerminateWindow();

	virtual void CreateSurface( VkInstance instance, VkSurfaceKHR* surface ) override;

	bool WindowShouldClose();
	void PollEvents();

	static void FramebufferResizeCallback( GLFWwindow*, int, int )
	{
		//auto app = reinterpret_cast< Vulkan2020App* >( glfwGetWindowUserPointer( window ) );
		//app->SubmitExtent();
	}

	GLFWwindow* window;
	int WIDTH = 800;
	int HEIGHT = 600;
};