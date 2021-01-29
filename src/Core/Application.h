#pragma once

#include "Core\Base.h"
#include "Core\Window.h"

#include "Core\Events\Event.h"
#include "Core\Events\WindowEvent.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\MouseEvent.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

	void OnEvent(Event& e);

	inline Window& GetWindow() const { return *m_Window; }
private:
	bool OnWindowClosed(WindowClosedEvent& e);
public:
	static Application& GetInstance() { return *s_Instance; }
private:
	ScopedPtr<Window> m_Window;
	bool m_Running = true;
private:
	static Application* s_Instance;
};