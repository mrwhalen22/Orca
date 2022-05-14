#include "oapch.h"
#include "Orca/Core/Input.h"

#include "Orca/Core/Application.h"

#include <GLFW/glfw3.h>


namespace Orca {

	bool Input::IsKeyPressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePos();
		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = GetMousePos();
		return y;
	}

	std::pair<float,float> Input::GetMousePos() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x_pos, y_pos;
		glfwGetCursorPos(window, &x_pos, &y_pos);
		return { (float)x_pos, (float)y_pos };
	}

}