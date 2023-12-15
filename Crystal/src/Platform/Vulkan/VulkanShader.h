#pragma once
#include "Crystal/Renderer/Shader.h"
#include <glm/glm.hpp>

enum VkShaderStageFlagBits;

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

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::string& filepath);
		VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<VkShaderStageFlagBits, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<VkShaderStageFlagBits, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(VkShaderStageFlagBits stage, const std::vector<uint32_t>& shaderData);
	private:
		//uint32_t m_RendererID;
		VkShaderModule m_Module;
		std::string m_FilePath;
		std::string m_Name;

		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<VkShaderStageFlagBits, std::string> m_VulkanSourceCode;
	};
	class VulkanPipeline {
	public:
		VulkanPipeline(VulkanDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		VulkanDevice& GetDevice() { return m_Device; }
		void Bind(VkCommandBuffer& commandBuffer);
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
