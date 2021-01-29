#include "pch.h"
#include "Input.h"

#include "Core\Application.h"

bool Input::IsKeyPressed(int keycode)
{
    const Application& app = Application::GetInstance();
    int state = glfwGetKey(app.GetWindow().GetWindowHandle(), keycode);
    return (state == GLFW_PRESS || state == GLFW_REPEAT);
}

bool Input::IsMouseButtonPressed(int button)
{
    const Application& app = Application::GetInstance();
    int state = glfwGetMouseButton(app.GetWindow().GetWindowHandle(), button);
    return state == GLFW_PRESS;
}

const glm::vec2& Input::GetMousePosition()
{
    const Application& app = Application::GetInstance();
    double xPos, yPos;
    glfwGetCursorPos(app.GetWindow().GetWindowHandle(), &xPos, &yPos);
    return { xPos, yPos };
}
