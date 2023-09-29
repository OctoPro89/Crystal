#pragma once

#include "Crystal/Renderer/Shader.h"

namespace Crystal
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void Unbind() const;
	private:
		uint32_t m_RendererID = 0;
	};
}