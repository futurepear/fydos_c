#include "player.h"
#include "../../physics/physics.h"
#include "../../systems/inventory.h"

Player::Player(const char* playerID) {
	body = new Body<float>{};
	id = playerID;
}

Player::~Player() {
	delete body;
}

Vector<float>& Player::position() {
	return body->position;
}