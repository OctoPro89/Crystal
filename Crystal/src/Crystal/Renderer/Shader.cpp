#include "crystalpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace Crystal
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		CRYSTAL_CORE_ASSERT(false, "RendererAPI::NONE is curerntly not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLShader(vertexSource, fragmentSrc);
		}
	}
}