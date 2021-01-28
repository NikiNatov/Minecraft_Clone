#pragma once

class Application
{
public:
	Application();
	~Application();

	void Run();

	void OnUpdate();
	void OnEvent();

public:
	static Application& GetInstance() { return *s_Instance; }
private:
	static Application* s_Instance;
};