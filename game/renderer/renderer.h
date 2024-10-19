#pragma once

#include "glHelper.h"
#include <GLFW/glfw3.h>
#include "../game.h"

namespace renderer {
	GLFWwindow* createWindow();
	void createRenderer(GLFWwindow* window);
	void render(Game& game, RenderStateObject& RenderState, GLFWwindow* window);
	void setupRenderer(RenderStateObject& RenderState);
}