#pragma once
#include "entity.h"
#include "../../input/inputMap.h"
#include "../../systems/crafting.h"
#include "controllers.h"
#include "weapon.h"
#include <queue>

class Player : public Entity {
private:
	const char* id;
	int activeItem = 0; 
	float interactionDistance = 3.0f; 
	Controller* controller; 
public:
	Inventory inventory{ 100 };
	Usable<>* item{ nullptr }; 
	InputMap input{}; 

	ItemStorage craftingBuffer; 
	Crafter crafter;

	Player(const char* playerID);
	~Player();
	
	void applyInput(InputMap& input); 
	void update();
	float facing();
	void addInput(InputMap i); 
	bool canInteract(const Vector<float> pos);
	const Vector<float> mouse(); 
	void setItem(int itemID); 
	void initializeHand(); 
	void changeActiveItem(int activeItem);
	
	void craft(int recipeID); 
	void checkCursorDistance(); 
	void applyStorageInputs(InputMap& input);
};