#include "pch.h"

#include "Application.h"

#include "Events\KeyEvent.h"
#include "Events\MouseEvent.h"
#include "Events\WindowEvent.h"

int main(char** argv, int argc)
{
	Application* app = new Application();

	MouseButtonPressedEvent e1(2);
	MouseButtonReleasedEvent e2(2);
	KeyReleasedEvent e3(2);
	KeyPressedEvent e4(2, 0);
	WindowResizedEvent e5(500, 500);
	MouseMovedEvent e6(400, 500);
	MouseButtonPressedEvent e7(6);
	MouseButtonPressedEvent e8(6);
	MouseScrolledEvent e9(400, 500);

	std::cout << e1.ToString() << std::endl;
	std::cout << e2.ToString() << std::endl;
	std::cout << e3.ToString() << std::endl;
	std::cout << e4.ToString() << std::endl;
	std::cout << e5.ToString() << std::endl;
	std::cout << e6.ToString() << std::endl;
	std::cout << e7.ToString() << std::endl;
	std::cout << e8.ToString() << std::endl;
	std::cout << e9.ToString() << std::endl;

	app->Run();

	delete app;
}