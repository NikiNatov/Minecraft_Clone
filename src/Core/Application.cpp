#include "pch.h"
#include <glad\glad.h>

#include <GLFW\glfw3.h>

#include "Application.h"
#include "Base.h"

#include "Core\Input.h"
#include "Core\KeyCodes.h"
#include "Core\MouseCodes.h"

#include "Game\GameLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	Window::WindowProperties properties("Minecraft Clone", 800, 600, true, BIND_FN(Application::OnEvent));
	m_Window = CreateScoped<Window>(properties);

	PushLayer(new GameLayer());
}

Application::~Application()
{

}

void Application::Run()
{
	while (m_Running)
	{
		m_CurrentFrameTime = (float)glfwGetTime();
		float dt = m_CurrentFrameTime - m_LastFrameTime;
		m_LastFrameTime = m_CurrentFrameTime;

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto layer : m_LayerStack)
			layer->OnUpdate(dt);

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowClosedEvent>(BIND_FN(Application::OnWindowClosed));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
	{
		if (e.Handled)
			break;

		(*--it)->OnEvent(e);
	}
}

void Application::PushLayer(Layer* layer)
{
	layer->OnAttach();
	m_LayerStack.PushLayer(layer);
}

void Application::PopLayer(Layer* layer)
{
	layer->OnDetach();
	m_LayerStack.PopLayer(layer);
}

bool Application::OnWindowClosed(WindowClosedEvent& e)
{
	m_Running = false;
	return true;
}
