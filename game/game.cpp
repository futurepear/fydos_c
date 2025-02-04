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
#include "gui/components.h"
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
	return Vector<double>{cursorX() / Constants::tileWidth, cursorY() / Constants::tileWidth};
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
	addEntity(players[id]);
	return players[id];
}
Entity* Game::addEntity(Entity* entity) {
	entities.push_back(entity);
	return entity;
}

void Game::lockCamera(const char* id) {
	cameraTarget = players[id];
}
void Game::setYou(const char* id) {
	you = players[id];
}

void Game::tileNarrowPhase(Entity* entity, Tile* tile, Chunk& parentChunk) {
	Body<> tileBody{};
	tileBody.setStatic(true);
	tileBody.setPosition(parentChunk.leftBorder() + tile->x() * 1 + 0.5f, parentChunk.topBorder() + tile->y() * 1 + 0.5f);
	int size = tile->geometrySize();
	tileBody.setVertices(tile->getPhysicsGeometry(), size);

	world.collide(*(entity->body), tileBody);
}

void Game::tileBroadPhaseInner(Entity* entity, Chunk& chunk) {
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
			Tile* tile = chunk[2][index];
			if (chunk[2][index]->solid()) {
				tileNarrowPhase(entity, chunk[2][index], chunk);
			}
		}
	}
}

void Game::tileBroadPhase(Entity* entity) {
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
		chunk.tiles[i]->update();
		if (chunk.tiles[i]->broken()) {
			//give items
			const BlockData& data = chunk.tiles[i]->getData();

			for (int i = 0; i < data.drops.size(); i++) {
				int result = pollDropAmount(data.drops[i]);
				int target = data.drops[i].target;

				//FIx dis later!
				players["me"]->inventory.addItem(target, result);
			}

			chunk.replaceTile(2, i, 0);
		}
	}
}

void Game::updateChunks(float time) {
	/*checks for:
		-broken block
		-blocks that grow
		-blocks that change over time
	*/

	for (const auto& [key, chunk] : chunks) {
		updateChunk(chunks[key], time);
	}
}

void Game::update(float time) {
	//apply inputs here

	//update physics

	int totalEntities = entities.size();

	for (int i = 0; i < totalEntities; i++) {
		for (int j = 0; j < totalEntities; j++) {
			if (entities[i] != entities[j])
				world.collide(*entities[i]->body, *entities[j]->body);
		}
	}

	for (int i = 0; i < totalEntities; i++) {
		tileBroadPhase(entities[i]);
		entities[i]->body->step(1.0f / 60.0f);
		entities[i]->update();
	}

	for (const auto& [key, value] : players) {
		players[key]->item->update(players[key]->mouse(), chunks);
	}

	updateChunks(time); 

	//camera after all positions are resolved
	if (cameraTarget != nullptr) {
		camera.x = cameraTarget->body->position.x;
		camera.y = cameraTarget->body->position.y;
	}
}


//only affects client side
void Game::updateClient(float time) {
	Chunk& chunk = chunks.vectorToChunk(cursorWorldPosition());
	int loc = chunk.positionToLocation(std::floor(cursorX() / Constants::tileWidth), std::floor(cursorY() / Constants::tileWidth));
	bool validTile = !chunks.isInvalid(chunk) && (loc != -1);
	bool rightClick = client.input.rightMouseDown && !client.lastInput.rightMouseDown;

	TileInteractionEvent tileInteraction{};

	if (rightClick && validTile && players["me"]->canInteract(chunk[2][loc]->center())) {
		chunk[2][loc]->interact(tileInteraction);
	}

	//client side tile updates - tiles close their inventories
	for (const auto& [key, chunk] : chunks) {
		for (int i = 0; i < Constants::chunkLayerSize; i++) {
			Tile* tile = chunks[key][2][i];
			bool inRange = you->canInteract(tile->center());
			tile->clientSideUpdate(inRange);
		}
	}

	//update recipe browser - get inventories from GUI. TODO: move to own file
	std::vector<GUIElement*> inventoryContainers = GUI::getElementsByClassName("storage");
	std::vector<const ItemBuffer*> recipeStorageInputs{};	

	bool includedPlayerInventory = false;
	for (int i = 0; i < recipeStorageInputs.size(); i++) {
		if (recipeStorageInputs[i] == &you->inventory)
			includedPlayerInventory = true;
	}
	if(!includedPlayerInventory)
		recipeStorageInputs.push_back(&you->inventory);

	for (int i = 0; i < inventoryContainers.size(); i++) {
		recipeStorageInputs.push_back(Components::getContainerStorage(inventoryContainers[i]));
	}

	std::vector<GUIElement*> recipeBrowsers = GUI::getElementsByClassName("recipeBrowser");

	for (int i = 0; i < recipeBrowsers.size(); i++) {
		reinterpret_cast<Components::RecipeBrowser*>(recipeBrowsers[i])->updateRecipeList(recipeStorageInputs);
	}
}

void Game::applyStorageInputs(Player* player, InputMap& input) {
	CursorCommand& cmd = player->inventory.cursorCommand;
	cmd.update();
	//if player takes item out of reach, add it back
	player->checkCursorDistance();
	player->applyStorageInputs(input);
}

void Game::applyInput(Player* player, InputMap& input) {
	//convert input to be usable - change mouse to world space
	const Vector<float> mouse{ (float)input.mouseX / Constants::tileWidth + player->body->position.x, (float)input.mouseY / Constants::tileWidth + player->body->position.y };

	player->applyInput(input);
	
	//item transfers

	//crafting
	for (int i = 0; i < input.crafts.size(); i++)
		input.crafts[i].target->add(input.crafts[i].recipeID);

	//switch items
	player->inventory.switchSlot(input.hotbarSlot);
	player->changeActiveItem(player->inventory.activeItem().id());

	bool inDistance = player->canInteract(mouse);
	bool isWeapon = player->inventory.isCurrentItem(ItemType::WEAPON);
	bool isBlock = player->inventory.isCurrentItem(ItemType::BLOCK);
	bool isPickaxe = player->inventory.isWeaponType(WeaponType::PICKAXE);

	if(input.mouseDown){
		if (inDistance && isBlock)
			placeBlock(player, mouse);

		if (inDistance && isPickaxe)
			player->item->use();

		if(isWeapon && !isPickaxe)
			player->item->use();
	}

	player->addInput(input);
}

bool Game::placeBlock(Player* entity, Vector<float> mouse) {
	Chunk& chunk = chunks.vectorToChunk(mouse);
	if (chunks.isInvalid(chunk)) return false;
	int loc = chunk.positionToLocation(mouse.x, mouse.y);

	//not empty space OR cant place OR invalid block
	if (!chunk[2][loc]->isAir() || !entity->item->canUse() || loc == -1) return false;

	auto& item = entity->inventory.currentItem();
	auto& itemData = item.getData();
	
	//item can be from cursor/external storage or inventory so find where
	ItemBuffer* source = &entity->inventory;
	if (entity->inventory.cursorCommand.storage != nullptr)
		source = entity->inventory.cursorCommand.storage;

	bool hasItems = source->removeItem(itemData.id, 1);
	if (!hasItems) return false;

	entity->item->use();

	chunk.replaceTile(2, loc, itemData.target);
	return true;
}

void Game::processLocalInput(GLFWwindow* window) {
	InputMap input;
	MouseEvent e{ you->inventory, input };

	Input::processInputTick(window, input, client.input, e, inputBuffer);
	
	cursor.x = e.x;
	cursor.y = e.y;
	input.mouseDownGUI = GUI::findTargetGUIElement(e, { static_cast<int>(cursor.x), static_cast<int>(cursor.y) });

	//chat & commands
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		if (inputBuffer.chat.length() > 0)
			inputBuffer.chat.pop_back();
		std::cout << inputBuffer.chat << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		std::cout << inputBuffer.chat << "\n";
		if (inputBuffer.chat == "/craft") {
			
			//you->crafter.add(0);
		}
		inputBuffer.chat = "";
	}
	if (inputBuffer.chat.length() > 0) {
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS)
		inputBuffer.chat += "/";
	//end chat & commands

	
	//todo change camera to 'you'
	input.mouseX = (int)cursorX() - camera.x * Constants::tileWidth;
	input.mouseY = (int)cursorY() - camera.y * Constants::tileWidth;

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		scale += 0.02f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		scale -= 0.02f;

	Chunk& chunk = chunks.vectorToChunk(cursorWorldPosition());

	if (!chunks.isInvalid(chunk)) {
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
			generateChunk(chunk);
		int loc = chunk.positionToLocation(std::floor(cursorX() / Constants::tileWidth), std::floor(cursorY() / Constants::tileWidth));
		//rotate block
		if (input.R && !client.input.R) {
			if (loc != -1) chunk[2][loc]->rotate();
		}
		if (input.M && !client.input.M) {
			if (loc != -1) chunk[2][loc]->TEST();
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			you->body->position.print();
		/*if (loc != -1 && input.rightMouseDown && !client.input.rightMouseDown)
			if(players["me"]->canInteract(chunk[2][loc]->center()))
				chunk[2][loc]->interact();*/
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !client.input.E) {
		players["me"]->inventory.print();
		GUI::$("inventory")->toggleVisibility();
	}

	inputBuffer.scrollX = 0;
	inputBuffer.scrollY = 0;

	if (you != nullptr) {
		//hotbar
		int hotbarSlot = you->inventory.slot();
		for (int i = 49; i <= 57; i++) {
			int slot = i - 49;
			if (glfwGetKey(window, i) == GLFW_PRESS) {
				hotbarSlot = slot;
			}
		}
		//0 key / empty slot
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			hotbarSlot = 9;

		input.hotbarSlot = hotbarSlot;

		applyStorageInputs(you, input);
		applyInput(you, input);
	}
	client.lastInput = client.input;
	client.input = input;
}