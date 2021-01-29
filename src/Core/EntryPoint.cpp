#include "pch.h"

#include "Application.h"


int main(char** argv, int argc)
{
	Application* app = new Application();

	app->Run();

	delete app;
}