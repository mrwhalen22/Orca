#include "oapch.h"
#include "OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Orca {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle)
	{
		OA_CORE_ASSERT(windowHandle, "Window Handle is NULL");
	}

	void OpenGLContext::Init() {
		OA_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OA_CORE_ASSERT(status, "Failed to initialize Glad");

		OA_CORE_INFO("OpenGL Info:");
		OA_CORE_INFO(" -- Vendor: {0}", glGetString(GL_VENDOR));
		OA_CORE_INFO(" -- Renderer: {0}", glGetString(GL_RENDERER));
		OA_CORE_INFO(" -- Version: {0}", glGetString(GL_VERSION));


	}

	void OpenGLContext::SwapBuffers() {
		OA_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);

	}
}