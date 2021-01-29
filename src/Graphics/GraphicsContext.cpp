#include "pch.h"
#include <glad\glad.h>
#include "GraphicsContext.h"

#include "Core\Base.h"


GraphicsContext::GraphicsContext(GLFWwindow* handle)
	: m_WindowHandle(handle)
{
	ASSERT(m_WindowHandle, "Window is null!");

	glfwMakeContextCurrent(m_WindowHandle);
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(result, "Failed to initialize glad!");

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
