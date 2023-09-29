#pragma once

#include <string>

namespace Crystal {
	class Shader
	{
	public:
		//Shader(const std::string& vertexSource, const std::string& fragmentSrc);
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSrc);
	private:
		//uint32_t m_RendererID = 0;
	};
}