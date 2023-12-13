#include "crystalpch.h"
#include "VulkanRendererAPI.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Crystal
{
	void VulkanRenderAPI::Init()
	{
		CRYSTAL_PROFILE_FUNCTION();
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}
	void VulkanRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}
	void VulkanRenderAPI::SetClearColor(const glm::vec4 color)
	{
	}
	void VulkanRenderAPI::Clear()
	{
	}
	void VulkanRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t imageIndex;
		VkResult result = m_SwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			CRYSTAL_CORE_FATAL("Failed to acquire swap chain image!");

		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		vkCmdDrawIndexed(commandBuffers[imageIndex], indexCount,);

		result = m_SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
			CRYSTAL_CORE_FATAL("Failed to present swap chain image");
	}
	void VulkanRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
	}
	void VulkanRenderAPI::SetLineWidth(float width)
	{
		vkCmdSetLineWidth(commandBuffers[0], width);
	}
	VulkanRenderAPI::~VulkanRenderAPI()
	{
		vkDestroyPipelineLayout(vkDevice.device(), pipelineLayout, nullptr);
	}
	void VulkanRenderAPI::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vkDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout));
	}
	void VulkanRenderAPI::CreatePipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height());
		pipelineConfig.renderPass = m_SwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		m_Pipeline = std::make_unique<VulkanPipeline>(vkDevice, "assets/cache/shader/vulkan/simple.vert.spv", "assets/cache/shader/vulkan/simple.frag.spv", pipelineConfig);
	}
	void VulkanRenderAPI::CreateCommandBuffers()
	{
		commandBuffers.resize(m_SwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vkDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
		if (vkAllocateCommandBuffers(vkDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			CRYSTAL_CORE_FATAL("Failed to allocate command buffers!");
		}

		for (int i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				CRYSTAL_CORE_FATAL("Failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_SwapChain.getRenderPass();
			renderPassInfo.framebuffer = m_SwapChain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = m_SwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};

			// ATTACHMENTS -- COME BACK HERE FOR RED INTEGER
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			m_Pipeline->Bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				CRYSTAL_CORE_FATAL("Failed to record command buffer!");
			}
		}
	}
}
