#include "pch.h"
#include <glad\glad.h>

#include "Application.h"
#include "Base.h"


Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	Window::WindowProperties properties("Minecraft Clone", 800, 600, true, BIND_FN(Application::OnEvent));
	m_Window = CreateScoped<Window>(properties);
}

Application::~Application()
{
}

void Application::Run()
{
	while (m_Running)
	{
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowClosedEvent>(BIND_FN(Application::OnWindowClosed));
}

bool Application::OnWindowClosed(WindowClosedEvent& e)
{
	m_Running = false;
	return true;
}
