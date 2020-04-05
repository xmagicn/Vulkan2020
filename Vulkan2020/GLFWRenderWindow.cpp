#include "GLFWRenderWindowClass.h"

#pragma warning( disable : 4189 )

#include <cassert>

void GLFWRenderWindow::InitWindow()
{
	glfwInit();

	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

	window = glfwCreateWindow( WIDTH, HEIGHT, "Vulkan", nullptr, nullptr );
	glfwSetWindowUserPointer( window, this );
	glfwSetFramebufferSizeCallback( window, FramebufferResizeCallback );
}

void GLFWRenderWindow::TerminateWindow()
{
	glfwDestroyWindow( window );

	glfwTerminate();
}

void GLFWRenderWindow::CreateSurface( VkInstance instance, VkSurfaceKHR* surface )
{
	VkResult result = glfwCreateWindowSurface( instance, window, nullptr, surface );
	assert( VK_SUCCESS == result && "failed to create window surface!" );
}

bool GLFWRenderWindow::WindowShouldClose()
{
	return glfwWindowShouldClose( window );
}

void GLFWRenderWindow::PollEvents()
{
	glfwPollEvents();
}