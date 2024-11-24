#include "player.h"
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include <cmath>

Player::Player(const char* playerID) {
	body = new Body<float>{};
	id = playerID;
	item = new Pickaxe<>{body->position};
}

Player::~Player() {
	delete body;
	delete item;
}

Vector<float>& Player::position() {
	return body->position;
}

void Player::update() {
	item->setFacing(facing());
	item->updatePivot(body->position);
}

const Vector<float> Player::mouse(){
	return { (float)input.mouseX / Constants::tileWidth + body->position.x, (float)input.mouseY / Constants::tileWidth + body->position.y };
}

float Player::facing() {
	return std::atan2(input.mouseY, input.mouseX);
}

void Player::addInput(InputMap i) {
	input = i;
}

void Player::initializeHand() {
	auto item = inventory.currentItem();
	if (item == nullptr) {
		setItem(ItemType::EMPTY);
		return;
	}
	setItem(item->getData().type);
}

void Player::switchHotbarSlot(int slot) {
	inventory.switchSlot(slot);
	initializeHand();
}

void Player::setItem(ItemType type) {
	delete item;

	switch (type) {
		case ItemType::WEAPON:
			item = pickaxeFactory( body->position );
			break;
		default:
			item = new Hand<>{ body->position };
	}
}