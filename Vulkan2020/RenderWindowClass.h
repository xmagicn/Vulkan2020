#pragma once

#include "vulkan/vulkan.h"

class RenderWindow
{
public:
	virtual ~RenderWindow() = default;

	virtual void CreateSurface( VkInstance instance, VkSurfaceKHR* surface ) = 0;
};