#include "inventory.h"
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include "items.hpp"


const ItemData& Item::getData() const {
	return itemIndex[m_id];
}
const char* Item::getName() const {
	return getData().name;
}
float Item::weight() const {
	return m_quantity * getData().weight;
}
int Item::getQuantity() const {
	return m_quantity;
}
void Item::addQuantity(int amount) {
	m_quantity += amount;
}
const ItemType Item::type() const {
	return getData().type;
}

void Inventory::print() {
	std::cout << "--Hotbar--\n";
	for (int i = 0; i < 10; i++) {
		const Item* item = getHotbarItem(i);
		if (currentSlot == i)
			std::cout << ">>>";
		if (item == nullptr) {
			std::cout << "empty - ";
		}
		else {
			std::cout << item->getName() << " - ";
		}
	}
	std::cout << "\n--Inventory--\n";
	for (const auto& [key, value] : storage) {
		Item item = storage[key];
		std::cout << item.getName() << ": " << item.getQuantity() << "\n";
	}
	std::cout << "-----------\n\n\n";
}

const Item* Inventory::getHotbarItem(int index) const {
	if (hotbar[index] == nullptr) {

	}
	return hotbar[index];
}
const Item* Inventory::currentItem() const {
	return getHotbarItem(currentSlot);
}

void Inventory::addItem(int id, int quantity) {
	if (storage.contains(id)) {
		storage[id].addQuantity(quantity);
	} else {
		storage[id] = Item{ id, quantity };
	}
}

void Inventory::addQuantity(int id, int quantity) {
	storage[id].addQuantity(quantity);
	if (storage[id].getQuantity() == 0)
		storage.erase(id);
}

bool Inventory::removeItem(int id, int quantity) {
	if (storage.contains(id) && storage[id].getQuantity() >= quantity) {
		storage[id].addQuantity(-quantity);
		return true;
	}
	return false;
}

void Inventory::switchSlot(int slot) {
	currentSlot = std::clamp(slot, 0, 10);
}

void Inventory::setHotbarItem(int index, int itemID) {
	hotbar[index] = &storage[itemID];
}
int Inventory::slot() {
	return currentSlot;
}
bool Inventory::isCurrentItem(ItemType type) {
	if (getHotbarItem(currentSlot) == nullptr) return (type == ItemType::EMPTY);
	return (getHotbarItem(currentSlot)->type() == type);
}