#pragma once
#include "Crystal/Renderer/GraphicsContext.h"
#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include <Crystal/Core/Application.h>

struct GLFWwindow;

namespace Crystal {
	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
		friend class VulkanRendererAPI;
	};
}
