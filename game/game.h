#pragma once
#include <unordered_map>

#include "world/entities/player.h"
#include "physics/physics.h"
#include "input/inputMap.h"
#include "world/chunk.h"
#include <GLFW/glfw3.h>
#include "input/processInput.h"
#include "world/chunkManager.h"
#include <vector>
#include <string>
#include <set>
#include "gui/events.h"

void initGame();

class Client {
private:
public:
	std::set<ItemBuffer*> openStorages{};
	Player* you = nullptr;
	InputMap lastInput{};
	InputMap input;
};
//only one of these should exist at a time 
class Game {
private:
	bool m_quit;
	Vector<double> cursor{ 0, 0 };
	Vector<float> camera{ 0, 0 };
	Vector<float> windowSize{ 0, 0 };
	float scale{ 1.0f };
	World<> world;
	
	
	Player* cameraTarget = nullptr;
	Player* you = nullptr;
	Client client{};

	void tileNarrowPhase(Entity* entity, Tile* tile, Chunk& parentChunk);
	void tileBroadPhaseInner(Entity* entity, Chunk& chunk);
public:
	InputBuffer inputBuffer{};
	std::unordered_map<const char*, Player*> players;
	std::vector<Entity*> entities{};

	ChunkManager chunks{};

	Game() {
		m_quit = false;

	}
	~Game() {
		for (const auto & [key, value] : players) {
			delete players[key];
		}
	}
	void quit() {
		m_quit = true;
	}
	float getScale();
	double cursorX();
	double cursorY();
	Vector<double> getCursorPosition();
	Vector<double> cursorWorldPosition();
	float cameraX();
	float cameraY();
	void updateScreenSize(float x, float y);
	Player* addPlayer(const char* id);
	Entity* addEntity(Entity* entity);
	void lockCamera(const char* id);
	void setYou(const char* id);
	bool placeBlock(Player* entity, Vector<float> mouse);
	
	void tileBroadPhase(Entity* entity);
	void updateChunk(Chunk& chunk, float time);
	void updateChunks(float time);
	void update(float time);
	void updateClient(float time);
	void applyStorageInputs(Player* player, InputMap& input);
	void applyInput(Player* player, InputMap& input);
	void processLocalInput(GLFWwindow* window);
};