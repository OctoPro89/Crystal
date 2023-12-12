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
		VulkanContext(GLFWwindow* windowHandle, VulkanPipeline& pipeline);
		VulkanDevice& GetDevice() { return vkDevice; }

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();

		GLFWwindow* m_WindowHandle;
		VulkanDevice vkDevice{ m_WindowHandle };
		VulkanSwapChain m_SwapChain{ vkDevice, VkExtent2D{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()} };
		Ref<VulkanPipeline> m_Pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
