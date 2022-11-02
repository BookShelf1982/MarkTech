#include "mtpch.h"
#include "WinInput.h"

#include "MarkTech/Application.h"
#include "GLFW/glfw3.h"


namespace MarkTech {

	Input* Input::s_Instance = new WinInput();

	bool WinInput::IsKeyPressedImp1(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WinInput::IsMouseButtonPressedImp1(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WinInput::GetMousePosImp1()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float WinInput::GetMouseXImp1()
	{
		auto [x, y] = GetMousePosImp1();
		return x;
	}

	float WinInput::GetMouseYImp1()
	{
		auto [x, y] = GetMousePosImp1();
		return y;
	}

}