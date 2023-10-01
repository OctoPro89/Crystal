#include "crystalpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Crystal {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}