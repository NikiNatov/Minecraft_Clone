#include "pch.h"

#include "Application.h"
#include "Base.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;
}

Application::~Application()
{
}

void Application::Run()
{
	while (true)
	{
		OnUpdate();
		OnEvent();
	}
}

void Application::OnUpdate()
{
}

void Application::OnEvent()
{
}
