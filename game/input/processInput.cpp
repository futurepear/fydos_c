#include <GLFW/glfw3.h>
#include "processInput.h"
#include <iostream>

void Input::processTextInput(InputBuffer& inputBuffer, unsigned int codepoint) {
	if (inputBuffer.chat.length() == 0) return;
	inputBuffer.chat += (char)codepoint;
	std::cout << inputBuffer.chat << "\n";
};

void Input::processScrollInput(InputBuffer& inputBuffer, double scrollX, double scrollY) {
	inputBuffer.scrollX = static_cast<int>(scrollX);
	inputBuffer.scrollY = static_cast<int>(scrollY);
};

void Input::processInputTick(GLFWwindow* window, InputMap& input, InputMap& lastInput, MouseEvent& e, InputBuffer& inputBuffer) {
	//glfwGetCursorPos(window, &cursor.x, &cursor.y);
	double cursorX = 0;
	double cursorY = 0;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	bool mouseDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	bool rightMouseDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	bool mouseDownGUI = false;

	e.mousedown = mouseDown;
	e.click = mouseDown && !lastInput.mouseDown;
	e.mouseRelease = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE);
	e.shiftDown = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	e.ctrlDown = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);
	e.hold = mouseDown && lastInput.mouseDown;
	e.x = cursorX;
	e.y = cursorY;
	e.dX = cursorX - lastInput.rawMouseX;
	e.dY = cursorY - lastInput.rawMouseY;
	e.scrollX = inputBuffer.scrollX;
	e.scrollY = inputBuffer.scrollY;

	input.W = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	input.A = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	input.S = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	input.D = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
	input.R = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
	input.M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
	input.E = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
	input.mouseDownGUI = mouseDownGUI;
	input.rightMouseDown = rightMouseDown;
	input.mouseDown = mouseDown;
	input.rawMouseX = e.x;
	input.rawMouseY = e.y;
}