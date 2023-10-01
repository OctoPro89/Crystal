#pragma once

#include "RendererAPI.h"

namespace Crystal {
	class RenderCommand
	{
	public:
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}