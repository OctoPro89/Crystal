#pragma once

#include "OrthographicCamera.h"

namespace Crystal
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		// Primitives
	};
}