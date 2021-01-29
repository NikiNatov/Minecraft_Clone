#pragma once

#include <string>
#include <GLFW\glfw3.h>

#include "Core\Base.h"

#include "Core\Events\Event.h"
#include "Graphics\GraphicsContext.h"

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync;
		EventCallbackFn EventCallback;

		WindowProperties(const std::string& title, uint32_t width, uint32_t height, bool vsync, const EventCallbackFn& callback)
			: Title(title), Width(width), Height(height), VSync(vsync), EventCallback(callback)
		{
		}
	};
public:
	Window(const WindowProperties& properties);
	~Window();

	void OnUpdate();

	inline GLFWwindow* GetWindowhandle() const { return m_WindowHandle; }
	inline const std::string& GetTitle() const { return m_WindowProperties.Title; }
	inline uint32_t GetWidth() const { return m_WindowProperties.Width; }
	inline uint32_t GetHeight() const { return m_WindowProperties.Height; }
	inline bool IsVSyncOn() const { return m_WindowProperties.VSync; }

	void SetVSync(bool state);
	inline void SetWindowEventCallback(const EventCallbackFn& callback) { m_WindowProperties.EventCallback = callback; }
private:
	GLFWwindow* m_WindowHandle;
	ScopedPtr<GraphicsContext> m_GraphicsContext;
	WindowProperties m_WindowProperties;
	
};

