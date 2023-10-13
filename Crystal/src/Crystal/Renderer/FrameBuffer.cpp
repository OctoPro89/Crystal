#include "crystalpch.h"
#include "FrameBuffer.h"

#include "Crystal/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Crystal
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CRYSTAL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFrameBuffer>(spec);
		}

		CRYSTAL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}