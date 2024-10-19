//game is initialized here
#include <fmt/core.h>	
#include <thread>
#include <chrono>
#include <iostream>

#include "physics/physics.h"
#include "renderer/renderer.h"
#include "input/inputMap.h"
#include <GLFW/glfw3.h>

#include "world/terrain/generation.h"


float Game::getScale() {
	return windowSize.x / 1920 * scale;
}
double Game::cursorX() {
	return (cursor.x - windowSize.x / 2) / getScale() + camera.x;
}
double Game::cursorY() {
	return (cursor.y - windowSize.y / 2) / getScale() + camera.y;
}
Vector<double> Game::cursorWorldPosition() {
	return Vector<double>{cursorX(), cursorY()};
}
Vector<double> Game::getCursorPosition() {
	return cursor;
}
float Game::cameraX() {
	return (float)camera.x;
}
float Game::cameraY() {
	return (float)camera.y;
}
void Game::updateScreenSize(float x, float y) {
	windowSize.x = x;
	windowSize.y = y;
}

Player* Game::addPlayer(const char* id) {
	players[id] = new Player{ id };
	return players[id];
}

void Game::lockCamera(const char* id) {
	cameraTarget = players[id];
}
void Game::setYou(const char* id) {
	you = players[id];
}

void Game::tileNarrowPhase(Player* entity, Tile& tile, Chunk& parentChunk) {
	Body<> tileBody{};
	tileBody.setStatic(true);
	tileBody.setPosition(parentChunk.leftBorder() + tile.x() * Constants::tileWidth + Constants::tileWidth / 2, parentChunk.topBorder() + tile.y() * Constants::tileWidth + Constants::tileWidth / 2);
	int size = tile.geometrySize();
	tileBody.setVertices(tile.getPhysicsGeometry(), size);


	world.collide(*(entity->body), tileBody);
}

void Game::tileBroadPhaseInner(Player* entity, Chunk& chunk) {
	auto box = entity->body->getAABB();

	if (box.min.x < chunk.leftBorder()) box.min.x = chunk.leftBorder();
	if (box.min.y < chunk.topBorder()) box.min.y = chunk.topBorder();
	if (box.max.x >= chunk.rightBorder()) box.max.x = chunk.rightBorder();
	if (box.max.y >= chunk.bottomBorder()) box.max.y = chunk.bottomBorder();

	Vector<int> min = chunk.tileSpace(box.min);
	Vector<int> max = chunk.tileSpace(box.max);

	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			if (x >= Constants::chunkSize || y >= Constants::chunkSize) continue;

			int index = x + y * Constants::chunkSize;
			Tile& tile = chunk[index];
			if (chunk[index].solid()) {
				tileNarrowPhase(entity, chunk[index], chunk);
			}
		}
	}
}

void Game::tileBroadPhase(Player* entity) {
	auto box = entity->body->getAABB();

	Vector<int> min = chunks.chunkSpace(box.min);
	Vector<int> max = chunks.chunkSpace(box.max);

	for (int x = min.x; x <= max.x; x++) {
		for (int y = min.y; y <= max.y; y++) {
			Chunk& chunk = chunks[{x, y}];

			if (!chunks.isInvalid(chunk))
				tileBroadPhaseInner(entity, chunk);
		}
	}
}

void Game::update(float time) {
	//update physics
	world.collide(*(players["me"]->body), *(players["you"]->body));

	for (const auto& [key, value] : players) {
		tileBroadPhase(players[key]);
	}

	//camera after all positions are resolved
	if (cameraTarget != nullptr) {
		camera.x = cameraTarget->body->position.x;
		camera.y = cameraTarget->body->position.y;
	}


}

void Game::applyInput(Player* player, InputMap& input) {
	Vector<float> displacement{};

	float v = 5.0f;
	displacement.x = int(input.A) * -1 + int(input.D);
	displacement.y = int(input.W) * -1 + int(input.S);

	player->body->move(Vector<float>::normalize(displacement) * v);
}

void Game::processLocalInput(GLFWwindow* window) {
	InputMap input;

	glfwGetCursorPos(window, &cursor.x, &cursor.y);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	input.W = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	input.A = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	input.S = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	input.D = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
	input.R = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
	input.M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		scale += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		scale -= 0.02f;

	Chunk& chunk = chunks.vectorToChunk(cursorWorldPosition());

	if (!chunks.isInvalid(chunk)) {
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			generateChunk(chunk);
		}

		//place block
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			int loc = chunk.positionToLocation((int)cursorX(), (int)cursorY());
			if (loc != -1)
				chunk[loc].setTile(1);
		}

		//mine block
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			int loc = chunk.positionToLocation((int)cursorX(), (int)cursorY());
			if (loc != -1)
				chunk[loc].setTile(0);
		}

		//rotate block
		if (input.R && !localInput.R) {
			int loc = chunk.positionToLocation((int)cursorX(), (int)cursorY());
			if (loc != -1)
				chunk[loc].rotate();
		}

		//?
		if (input.M && !localInput.M) {
			int loc = chunk.positionToLocation((int)cursorX(), (int)cursorY());
			if (loc != -1)
				chunk[loc].TEST();
		}
	} else {
		//temp code lol
		chunks.createChunk(chunks.chunkSpace(cursorWorldPosition()));
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		players["me"]->body->position.print();
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		players["me"]->body->position.x = 49.98f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		players["me"]->inventory.print();
	}

	if (you != nullptr) {
		applyInput(you, input);
	}
	localInput = input;
}