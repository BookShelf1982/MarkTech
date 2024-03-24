#include "GraphicsInterface.h"
#include "Platforms\OpenGL\OpenGLGraphicsContext.h"

void InitModule()
{
}

void UpdateModule()
{
}

void ShutdownModule()
{
}

IGraphicsContext* CreateGraphicsContext(EGraphicsAPI api, HWND hwnd)
{
	switch (api)
	{
	case EGraphicsAPI::OpenGL:
		return new COpenGLGraphicsContext(api, hwnd);
		break;
	}
	return nullptr;
}

void DestroyGraphicsContext(IGraphicsContext* pGraphicsContext)
{
	delete pGraphicsContext;
}
