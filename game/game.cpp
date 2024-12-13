//game is initialized here
#include <fmt/core.h>	
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

#include "physics/physics.h"
#include "renderer/renderer.h"
#include "input/inputMap.h"
#include <GLFW/glfw3.h>
#include "systems/utility.h"

#include "world/terrain/generation.h"


float Game::getScale() {
	return windowSize.x / 1920 * scale;
}
double Game::cursorX() {
	return (cursor.x - windowSize.x / 2) / (getScale()) + camera.x * Constants::tileWidth;
}
double Game::cursorY() {
	return (cursor.y - windowSize.y / 2) / (getScale()) + camera.y * Constants::tileWidth;
}
Vector<double> Game::cursorWorldPosition() {
	return Vector<double>{cursorX()/Constants::tileWidth, cursorY()/Constants::tileWidth};
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
	tileBody.setPosition(parentChunk.leftBorder() + tile.x() * 1 + 0.5f, parentChunk.topBorder() + tile.y() * 1 + 0.5f);
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
			if (x >= Constants::chunkWidth || y >= Constants::chunkWidth) continue;

			int index = x + y * Constants::chunkWidth;
			Tile& tile = chunk[2][index];
			if (chunk[2][index].solid()) {
				tileNarrowPhase(entity, chunk[2][index], chunk);
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


void Game::updateChunk(Chunk& chunk, float time) {
	for (int i = 0; i < Constants::chunkLayerSize; i++) {
		//X layer block broke?
		if (chunk.tiles[i].broken()) {
			//give items
			const BlockData& data = chunk.tiles[i].getData();

			for (int i = 0; i < data.drops.size(); i++) {
				int result = pollDropAmount(data.drops[i]);
				int target = data.drops[i].target;

				//FIx dis later!
				players["me"]->inventory.addItem(target, result);
			}

			chunk.tiles[i].setTile(0);
		}
	}
}

void Game::updateChunks(float time) {
	/*checks for:
		-broken block
		-blocks that grow
		-blocks that change over time
	*/

	for (const auto& [key, chunk] : chunks.chunks) {
		updateChunk(chunks[key], time);
	}
}

void Game::update(float time) {
	//update physics
	world.collide(*(players["me"]->body), *(players["you"]->body));

	for (const auto& [key, value] : players) {
		
		players[key]->item->update(players[key]->mouse(), chunks);

		players[key]->body->step(1.0/60.0);
		tileBroadPhase(players[key]);
		
		players[key]->update();
	}

	updateChunks(time);

	//camera after all positions are resolved
	if (cameraTarget != nullptr) {
		camera.x = cameraTarget->body->position.x;
		camera.y = cameraTarget->body->position.y;
	}
}

void Game::applyInput(Player* player, InputMap& input) {
	//convert input to be usable
	//change mouse to world space
	const Vector<float> mouse{ (float)input.mouseX / Constants::tileWidth + player->body->position.x, (float)input.mouseY / Constants::tileWidth + player->body->position.y };

	//TODO: better movement
	Vector<float> displacement{};

	float v = 2.0 / 60.0f;
	displacement.x = int(input.A) * -1 + int(input.D);
	displacement.y = int(input.W) * -1 + int(input.S);

	bool isRunning = input.A || input.W || input.D || input.S;
	bool isStill = player->body->velocity.lengthSquared() < 0.1;
	//isStill = true;

	displacement = Vector<float>::normalize(displacement);
	Vector<float> vN = Vector<float>::normalize(player->body->velocity);

	Vector<float> force = { displacement.x * v - (0.2f * player->body->velocity.x + 200.0f * 1 * 9.8f * vN.x * !isRunning) * !isStill,
						   displacement.y * v - (0.2f * player->body->velocity.y + 200.0f * 1 * 9.8f * vN.y * !isRunning) * !isStill
						  };

	
	player->body->move(displacement * v);
	//player->body->applyForce(force);
	//END TODO

	//switching items
	if (player->inventory.slot() != input.hotbar) {
		player->switchHotbarSlot(input.hotbar);
	}
	//end switching items
	
	if (input.mouseDown && player->inventory.isCurrentItem(ItemType::BLOCK)) {
		placeBlock(player, mouse);
	}
	
	if (input.mouseDown && player->inventory.isCurrentItem(ItemType::WEAPON)) {
		player->item->use();
	}

	player->addInput(input);
}

bool Game::placeBlock(Player* entity, Vector<float> mouse) {
	Chunk& chunk = chunks.vectorToChunk(mouse);
	if (chunks.isInvalid(chunk)) return false;
	int loc = chunk.positionToLocation(mouse.x, mouse.y);
	
	//not empty space OR cant place OR invalid block
	if (!chunk[2][loc].isAir() || !entity->item->canUse() || loc == -1) return false;
	
	auto itemData = entity->inventory.currentItem()->getData();
	bool hasItems = entity->inventory.removeItem(itemData.id, 1);

	if (!hasItems) return false;

	entity->item->use();

	chunk[2][loc].setTile(1);

	return true;
}

void Game::processLocalInput(GLFWwindow* window) {
	InputMap input;

	glfwGetCursorPos(window, &cursor.x, &cursor.y);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//chat & commands
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
		if(chatBuffer.length() > 0)
			chatBuffer.pop_back();
		std::cout << chatBuffer << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		std::cout << chatBuffer << "\n";
		if (chatBuffer == "/craft") {
			players["me"]->craft(0);
		}
		chatBuffer = "";
	}
	if (chatBuffer.length() > 0) {
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS)
		chatBuffer += "/";
	//end chat & commands

	input.W = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	input.A = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	input.S = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	input.D = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
	input.R = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
	input.M = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
	input.mouseDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

	//todo change camera to 'you'
	input.mouseX = (int)cursorX() - camera.x * Constants::tileWidth;
	input.mouseY = (int)cursorY() - camera.y * Constants::tileWidth;

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		scale += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		scale -= 0.02f;

	Chunk& chunk = chunks.vectorToChunk(cursorWorldPosition());

	if (!chunks.isInvalid(chunk)) {
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			generateChunk(chunk);
		}

		//rotate block
		if (input.R && !localInput.R) {
			int loc = chunk.positionToLocation(std::floor(cursorX()/Constants::tileWidth), std::floor(cursorY() / Constants::tileWidth));
			if (loc != -1)
				chunk[2][loc].rotate();
		}

		//?
		if (input.M && !localInput.M) {
			int loc = chunk.positionToLocation(std::floor(cursorX() / Constants::tileWidth), std::floor(cursorY() / Constants::tileWidth));
			if (loc != -1)
				chunk[2][loc].TEST();
		}
	} else {
		//temp code lol
		auto a = chunks.chunkSpace(cursorWorldPosition());
		//chunks.createChunk(a);
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		players["me"]->body->position.print();
		std::cout << "chunks--\n";
		chunks.printActive();
		std::cout << "---\n";
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		players["me"]->body->position.x = 49.98f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		players["me"]->inventory.print();
	}

	if (you != nullptr) {
		//hotbar
		int hotbarSlot = you->inventory.slot();
		for (int i = 49; i <= 57; i++) {
			int slot = i - 49;
			if (glfwGetKey(window, i) == GLFW_PRESS) {
				hotbarSlot = slot;
			}
		}

		input.hotbar = hotbarSlot;

		applyInput(you, input);
	}
	localInput = input;
}

void Game::processChatInputCallback(unsigned int codepoint) {
	if (chatBuffer.length() == 0) return;
	chatBuffer += (char)codepoint;
	std::cout << chatBuffer << "\n";
}