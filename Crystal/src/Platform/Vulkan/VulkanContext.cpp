#include "crystalpch.h"
#include "VulkanContext.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Crystal {
	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CRYSTAL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void VulkanContext::Init()
	{
		CRYSTAL_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
	}
	void VulkanContext::SwapBuffers()
	{
		CRYSTAL_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}
