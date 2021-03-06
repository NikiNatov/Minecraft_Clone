#include "pch.h"
#include <glad\glad.h>

#include <GLFW\glfw3.h>

#include "Application.h"
#include "Base.h"

#include "Core\Input.h"
#include "Core\KeyCodes.h"
#include "Core\MouseCodes.h"

#include "Game\GameLayer.h"
#include "Graphics\Renderer.h"
#include "Graphics\Renderer2D.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;


	Window::WindowProperties properties("Minecraft Clone", 1280, 720, true, BIND_FN(Application::OnEvent));
	m_Window = CreateScoped<Window>(properties);

	Renderer::Init();
	Renderer2D::Init();

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

		std::cout << dt << std::endl;

		for (auto layer : m_LayerStack)
			layer->OnUpdate(dt);

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowClosedEvent>(BIND_FN(Application::OnWindowClosed));
	dispatcher.Dispatch<WindowResizedEvent>(BIND_FN(Application::OnWindowResized));

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

bool Application::OnWindowResized(WindowResizedEvent& e)
{
	Renderer::SetViewportSize(0, 0, e.GetWidth(), e.GetHeight());
	return false;
}
