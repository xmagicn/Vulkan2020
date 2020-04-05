#pragma once

struct GLFWwindow;
class RenderWindow;

class GraphicsInstance
{
public:
	GraphicsInstance();
	virtual ~GraphicsInstance();

	bool InitInstance( RenderWindow* pRenderWindow );
	bool DestroyInstance();

	void DrawFrame();

	virtual void ResizeFrame( unsigned int width, unsigned int height ) = 0;

private:
	GraphicsInstance( const GraphicsInstance& ) = delete;
	GraphicsInstance& operator=( const GraphicsInstance& ) = delete;

protected:
	virtual bool InitInstanceInternal( RenderWindow* pRenderWindow ) = 0;
	virtual bool DestroyInstanceInternal() = 0;

	virtual void DrawFrameInternal() = 0;
	virtual void WaitForFrameComplete() = 0;
};
