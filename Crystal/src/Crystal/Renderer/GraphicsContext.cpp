#include "crystalpch.h"
#include "Crystal/Renderer/GraphicsContext.h"

#include "Crystal/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Crystal {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CRYSTAL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		CRYSTAL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}