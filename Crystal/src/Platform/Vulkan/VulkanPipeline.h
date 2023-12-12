#pragma once
#include "VulkanDevice.h"

struct PipelineConfigInfo {
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};
namespace Crystal {
	class VulkanPipeline {
	public:
		VulkanPipeline(VulkanDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		VulkanDevice& GetDevice() { return m_Device; }

		~VulkanPipeline();
		VulkanPipeline(const VulkanPipeline&) = delete;
		void operator=(const VulkanPipeline&) = delete;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);
	private:
		void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		std::string ReadFile(const std::string& filepath);
		VulkanDevice& m_Device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}
