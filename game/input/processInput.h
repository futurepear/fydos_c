#pragma once

namespace input {
	struct keyMap {
		bool w;
		bool a;
		bool s;
		bool d;
	};
	void processInput(GLFWwindow* window);
}

