#include <fmt/core.h>
#include "fydos.h"

#include <thread>
#include <chrono>

#include "game/physics/physics.h"
#include "game/renderer/renderer.h"
#include <GLFW/glfw3.h>
#include "game/game.h"

#include "game/systems/animation.hpp"

void initGame() {
	GLFWwindow* window{ nullptr };
	window = renderer::createWindow();

	if (window == nullptr) {
		fmt::print("Error setting up window");
	}

	//glfwMakeContextCurrent(NULL);

	//create the renderer
	//std::thread renderThread{renderer::createRenderer, window};

	//single threaded version 


	RenderStateObject RenderState{};
	Game game{};
	
	game.addPlayer("me");
	game.addPlayer("you");
	game.lockCamera("you");
	game.setYou("me");
	game.players["you"]->body->position.x = 200;
	game.players["me"]->body->makeCircle();

	game.players["me"]->inventory.addItem(1, 10);

	renderer::setupRenderer(RenderState);

	float time = static_cast<float>(glfwGetTime()); //in seconds
	Animation<> test{ {0, 0, 3}, {10, 1, 3}, {40, 2, 3}, {0, 3, 1} };
	AnimationDiscrete<> test2{ {0, 0, 1}, {100, 60, 1}, {30, 120, 1}, {0, 180, 1} };


	////main game loop
	while (!glfwWindowShouldClose(window)) {
		time = static_cast<float>(glfwGetTime()); //in seconds

		glfwPollEvents();
		game.processLocalInput(window);
		game.update(time);		

		//test2.update();
		//game.players["me"]->body->position.x = test2.value();
		
		renderer::render(game, RenderState, window);

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	//program close - cleanup

	//renderThread.join();
	glfwTerminate();
}


int main(){
	fmt::print("Starting up...\n");

	initGame();

	fmt::print("closing..");
	//std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	return 0;
}	
