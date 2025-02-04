#pragma once
#include "inputMap.h"
#include "../gui/events.h"

namespace Input {
	void processTextInput(InputBuffer& inputBuffer, unsigned int codepoint);
	void processScrollInput(InputBuffer& inputBuffer, double scrollX, double scrollY);
	void processInputTick(GLFWwindow* window, InputMap& input, InputMap& lastInput, MouseEvent& e, InputBuffer& inputBuffer);
}

