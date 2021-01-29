#pragma once

#include "Core\Base.h"
#include "Core\Window.h"

#include "Core\Events\Event.h"
#include "Core\Events\WindowEvent.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\MouseEvent.h"

#include "Core\LayerStack.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	inline Window& GetWindow() const { return *m_Window; }
private:
	bool OnWindowClosed(WindowClosedEvent& e);
public:
	static Application& GetInstance() { return *s_Instance; }
private:
	ScopedPtr<Window> m_Window;
	LayerStack m_LayerStack;

	bool m_Running = true;

	float m_LastFrameTime = 0.0f;
	float m_CurrentFrameTime = 0.0f;
private:
	static Application* s_Instance;
};