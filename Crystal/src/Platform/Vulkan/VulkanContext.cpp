#include "crystalpch.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Crystal {
	VulkanContext::VulkanContext(GLFWwindow* windowHandle, VulkanPipeline& pipeline)
		: m_WindowHandle(windowHandle)
	{
		CRYSTAL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void VulkanContext::Init()
	{
		CRYSTAL_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CRYSTAL_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void VulkanContext::SwapBuffers()
	{

	}
	void VulkanContext::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo
	}
	void VulkanContext::CreatePipeline()
	{

	}
	void VulkanContext::CreateCommandBuffers()
	{

	}
}
