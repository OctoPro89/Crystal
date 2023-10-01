#pragma once

#include "RenderCommand.h"

namespace Crystal {
	class Renderer 
	{
	public:
		static void BeginScene(); //TODO: Take in scene
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}