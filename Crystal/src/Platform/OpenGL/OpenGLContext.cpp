#include "crystalpch.h"
#include "OpenglContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Crystal {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CRYSTAL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		CRYSTAL_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CRYSTAL_CORE_ASSERT(status, "Failed to initialize Glad!");

	#ifdef CRYSTAL_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CRYSTAL_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Crystal Requires at least OpenGL version 4.5!");
	#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		CRYSTAL_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}