#include "crystalpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Crystal
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		CRYSTAL_CORE_ASSERT(false, "RendererAPI::NONE is curerntly not supported"); return nullptr;
		case RendererAPI::API::OpenGL:	return new OpenGLVertexArray();
		}

		CRYSTAL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
