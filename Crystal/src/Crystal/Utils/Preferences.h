#pragma once
#include <Crystal/Renderer/RendererAPI.h>

#include <glm/vec4.hpp>

namespace Crystal
{
	struct Preferences
	{
	public:
		enum ColorScheme
		{
			Default = 0, DefaultLight, DefaultDark, Dark, Monochrome
		};

		enum EditorFont
		{
			OpenSans = 0, OpenSansBold, Roboto, RobotoBold, Kalam, KalamBold
		};

		glm::vec4 SphereColliderColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 QuadColliderColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 EntityOutlineColor = glm::vec4(1, 0, 0, 1);
		float LineThickness = 3.0f;
		RendererAPI::API RenderingAPI;
		ColorScheme colorScheme = ColorScheme::Dark;
		EditorFont editorFont = EditorFont::OpenSans;
		bool VSync = false;
		bool ShowPhysicsColliders = false;
		bool NearestFiltering = false;
	};
}
