#include "crystalpch.h"
#include "Renderer.h"

namespace Crystal {
	


	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		RenderCommand::DrawIndexed(vertexArray);
	}

}