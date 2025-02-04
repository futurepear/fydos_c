#include "player.h"
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include "../../systems/crafting.h"
#include <cmath>

Player::Player(const char* playerID) : Entity{}, crafter { &inventory, & inventory } {
	
	id = playerID;
	inventory.setPhysicalPosition(&body->position);
	controller = new PlayerController();
	initializeHand();
}

Player::~Player() {
	delete item;
}

void Player::applyInput(InputMap& input) {
	controller->move(body, input);
}
void Player::update() {
	item->setFacing(facing());
	item->updatePivot(body->position);

	crafter.update();
}

const Vector<float> Player::mouse(){
	return { (float)input.mouseX / Constants::tileWidth + body->position.x, (float)input.mouseY / Constants::tileWidth + body->position.y };
}

float Player::facing() {
	return (float)std::atan2(input.mouseY, input.mouseX);
}

void Player::addInput(InputMap i) {
	input = i;
}

bool Player::canInteract(const Vector<float> pos) {
	float dist = Vector<float>::distance(pos, position());
	return (dist <= interactionDistance);
}

void Player::initializeHand() {
	auto& item = inventory.currentItem();
	if (item.empty()) {
		setItem(0);
		return;
	}
	setItem(item.id());
}

void Player::changeActiveItem(int item) {
	if (item == activeItem) return;
	activeItem = item; //set current active item to the new item id so it knows its this item its using
	std::cout << "Active item: " << Dictionary::items[activeItem].name << "\n";
	
	initializeHand();
}

void Player::setItem(int itemID) {
	delete item;

	const ItemData& itemData = Dictionary::items[itemID];

	switch (itemData.type) {
		case ItemType::WEAPON:
			item = weaponFactory(itemData.target, body->position);
			break;
		default:
			item = new Hand<>{ body->position };
	}
}


void Player::craft(int recipeID) {
	crafter.add(recipeID);
} 

void Player::checkCursorDistance() {
	CursorCommand& cmd = inventory.cursorCommand;
	if (cmd.storage == nullptr) return;
	if (cmd.storage == &inventory) return; //if storage is something else, it means something is in cursor
	if (!canInteract(*cmd.storage->position())) {
		cmd.unbind();
	}
}
void Player::applyStorageInputs(InputMap& input) {
	CursorCommand& cmd = inventory.cursorCommand;

	if (!input.storageInteraction) return;
	ItemBuffer* linkedInventory = input.cursor.storage;
	int position = input.cursor.position;
	int id = input.cursor.itemID;

	//check to see if inventories are within interaction distance
	auto pos = linkedInventory->position();
	if (pos != nullptr && !canInteract(*pos)) return;



	const Item item = linkedInventory->fastAccess(id, position);

	//empty slot, empty cursor = nothing
	//full slot, empty cursor 
	if (!item.empty() && !cmd.bound()) {
		cmd.bind(linkedInventory);
		cmd.id = item.id();
		cmd.quantity = item.getQuantity();
		cmd.position = position;
		return;
	}

	//empty slot, full cursor - deposit items , or same ids so adding
	//cmd.storage ---> linkedInventory
	if (cmd.bound() && (item.empty() || (cmd.id == item.id()))) {
		int transferred = Inventory::transfer(cmd.storage, cmd.id, cmd.position, cmd.getQuantity(), linkedInventory, position);
		//unbind if fully transferred
		if (transferred == cmd.getQuantity()) {
			cmd.unbind();
		}
		return;
	}
	//Inventory::swap(linkedInventory, item.id(), position, &e.cursorBuffer, cursorItem.id(), 0);
	//if e.shift
	/*
	*	linkedInventory <--> e.targetInventory
	*
		targetID = item.id() if e.targetInventory.has(item.id())
		targetID = 0 if e.targetInventory.hasRoom() // empty room
		//if found place to swap into, swap
		swap()
	*/
}