#pragma once

#include "Crystal/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Crystal
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
	private:
		uint32_t m_RendererID = 0;
	};
}