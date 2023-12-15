#pragma once
#include "Crystal/Renderer/RendererAPI.h"
#include "VulkanSwapChain.h"
#include "VulkanPipeline.h"
#include <Crystal/Core/Application.h>

namespace Crystal {
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI() { api = this; }
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4 color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetLineWidth(float width) override;

		inline std::vector<VkCommandBuffer>& GetCommandBuffers() { return commandBuffers; }
		static VulkanRendererAPI& GetInstance() { return *api; }
		VulkanDevice& GetDevice() { return vkDevice; }
	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		VulkanDevice vkDevice{ m_WindowHandle };
		VulkanSwapChain m_SwapChain{ vkDevice, VkExtent2D{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()} };
		Ref<VulkanPipeline> m_Pipeline{};
		VkPipelineLayout pipelineLayout{};
		std::vector<VkCommandBuffer> commandBuffers{};
		static VulkanRendererAPI* api;
	};
}
