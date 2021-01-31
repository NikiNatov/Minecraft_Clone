#include "pch.h"
#include <GLFW\glfw3.h>

#include "Core\Base.h"
#include "Window.h"

#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\WindowEvent.h"

Window::Window(const WindowProperties& properties)
	: m_WindowProperties(properties)
{
	int success = glfwInit();
	ASSERT(success, "Failed to initialize GLFW!");

	m_WindowHandle = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Title.c_str(), nullptr, nullptr);
	ASSERT(m_WindowHandle, "Failed to create a window!");

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_GraphicsContext = CreateScoped<GraphicsContext>(m_WindowHandle);

	glfwSetWindowUserPointer(m_WindowHandle, &m_WindowProperties);
	SetVSync(m_WindowProperties.VSync);

	glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int keycode, int scancode, int action, int mods)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(keycode, 0);
				properties.EventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent e(keycode, 1);
				properties.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(keycode);
				properties.EventCallback(e);
				break;
			}
		}

	});

	glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* window, uint32_t key)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		KeyTypedEvent e(key);
		properties.EventCallback(e);
	});

	glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent e(button);
				properties.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent e(button);
				properties.EventCallback(e);
				break;
			}
		}
	});

	glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		MouseMovedEvent e((float)xPos, (float)yPos);
		properties.EventCallback(e);
	});

	glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		MouseScrolledEvent e((float)xOffset, (float)yOffset);
		properties.EventCallback(e);
	});

	glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		properties.Width = width;
		properties.Height = height;

		WindowResizedEvent e(width, height);
		properties.EventCallback(e);
	});
	
	glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window)
	{
		WindowProperties& properties = *(WindowProperties*)glfwGetWindowUserPointer(window);
		WindowClosedEvent e;
		properties.EventCallback(e);
	});
}

Window::~Window()
{
	glfwDestroyWindow(m_WindowHandle);
	glfwTerminate();
}

void Window::OnUpdate()
{
	glfwPollEvents();
	m_GraphicsContext->SwapBuffers();
}

void Window::SetVSync(bool state)
{
	m_WindowProperties.VSync = state;

	if (state)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}
