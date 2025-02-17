﻿#include <fmt/core.h>
#include "fydos.h"

#include <thread>
#include <chrono>

#include "game/physics/physics.h"
#include "game/renderer/renderer.h"
#include <GLFW/glfw3.h>
#include "game/game.h"
#include "game/input/processInput.h"

#include "game/systems/animation.hpp"

#include "game/gui/components.h"

void g(){}

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

	GUI::compute(&GUI::root);

	RenderStateObject RenderState{};
	Game game{};

	game.addPlayer("me");
	game.addPlayer("you");

	game.lockCamera("me");
	game.setYou("me");
	game.players["you"]->body->position.x = 2;
	game.players["me"]->body->makeCircle();

	game.players["me"]->inventory.addItem(1, 10);
	game.players["me"]->inventory.addItem(3, 1);
	game.players["me"]->inventory.addItem(10, 4);
	game.players["me"]->inventory.addItem(19, 3);
	game.players["me"]->inventory.setHotbarItem(1, 3);
	game.players["me"]->inventory.setHotbarItem(0, 1);
	game.players["me"]->inventory.setHotbarItem(2, 19);
	game.players["me"]->initializeHand();

	//BUILDING GUI TESTS HERE
	{
		GUIElement* root = &GUI::root;
		root->align = GUIStyles::Flex::all;
		
		auto inventory = Components::inventory(&game.players["me"]->inventory);
		GUI::setID(inventory, "inventory");

		
		//root->appendChild(c);
		root->appendChild(inventory);
		root->appendChild(Components::hotbar());
	}

	renderer::setupRenderer(RenderState);

	float time = static_cast<float>(glfwGetTime()); //in seconds

	glfwSetWindowUserPointer(window, &game);
	//input for chat and commands
	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
		Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
		Input::processTextInput(game->inputBuffer, codepoint);
	});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
		Input::processScrollInput(game->inputBuffer, xoffset, yoffset);
	});

	////main game loop
	while (!glfwWindowShouldClose(window)) {
		time = static_cast<float>(glfwGetTime()); //in seconds

		glfwPollEvents();
		game.processLocalInput(window);
		game.updateClient(time);
		game.update(time);		

		
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
