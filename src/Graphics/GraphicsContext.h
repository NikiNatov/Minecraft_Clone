#pragma once

#include <GLFW\glfw3.h>

class GraphicsContext
{
public:
	GraphicsContext(GLFWwindow* handle);
	~GraphicsContext();

	void SwapBuffers();
private:
	GLFWwindow* m_WindowHandle;
};