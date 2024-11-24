#pragma once
#include <unordered_map>

#include "world/entities/player.h"
#include "physics/physics.h"
#include "input/inputMap.h"
#include "world/chunk.h"
#include <GLFW/glfw3.h>
#include "input/inputMap.h"
#include "world/chunkManager.h"
#include <vector>

void initGame();

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
	InputMap localInput{};

	void tileNarrowPhase(Player* entity, Tile& tile, Chunk& parentChunk);
	void tileBroadPhaseInner(Player* entity, Chunk& chunk);
public:
	std::unordered_map<const char*, Player*> players;

	std::vector<Vector<int>> debug{};

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
	void lockCamera(const char* id);
	void setYou(const char* id);
	bool placeBlock(Player* entity, Vector<float> mouse);
	
	void tileBroadPhase(Player* entity);
	void update(float time);
	void applyInput(Player* player, InputMap& input);
	void processLocalInput(GLFWwindow* window);
};