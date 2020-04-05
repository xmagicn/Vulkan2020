#include "GraphicsInstance.h"

#include <cassert>

GraphicsInstance::GraphicsInstance()
{

}

GraphicsInstance::~GraphicsInstance()
{

}

bool GraphicsInstance::InitInstance( RenderWindow* pRenderWindow )
{
	return InitInstanceInternal( pRenderWindow );
}

bool GraphicsInstance::DestroyInstance()
{
	return DestroyInstanceInternal();
}

void GraphicsInstance::DrawFrame()
{
	DrawFrameInternal();
}