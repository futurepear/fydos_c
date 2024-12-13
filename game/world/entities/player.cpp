#include "player.h"
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include "../../systems/crafting.h"
#include <cmath>

Player::Player(const char* playerID) {
	body = new Body<float>{};
	id = playerID;
	initializeHand();
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

	updateCrafting();
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

void Player::updateCrafting() {
	if (craftingQueue.empty()) return;
	CraftCommand& cmd = craftingQueue.front();
	
	updateCraftCommand(cmd, &inventory, &inventory);
	if (cmd.done) {
		if (cmd.success)
			std::cout << "success";
		craftingQueue.pop();
	}
}

void Player::craft(int recipeID) {
	craftingQueue.push(createCraftCommand(recipeID));
}