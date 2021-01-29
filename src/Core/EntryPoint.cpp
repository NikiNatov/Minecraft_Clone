#include "pch.h"

#include "Application.h"

int main(char** argv, int argc)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application* app = new Application();
	app->Run();
	delete app;
}