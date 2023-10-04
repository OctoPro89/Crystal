#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Crystal {
	class Renderer 
	{
	public:
		static void BeginScene(OrthographicCamera& camera); //TODO: Take in scene
		static void EndScene();

		static void Submit(const Crystal::Ref<Shader>& shader, const Crystal::Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}