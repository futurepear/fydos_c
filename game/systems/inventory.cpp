#include "inventory.h"
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include "dictionary.h"

const ItemData& Item::getData() const {
	return Dictionary::items[m_id];
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


//repetitive code i honestly dont care	
void ItemStorage::addItem(int id, int quantity) {
	if (quantity == 0) return;
	if (storage.contains(id)) {
		storage[id].addQuantity(quantity);
	}else {
		storage[id] = Item{ id, quantity };
	}
}
void ItemStorage::addQuantity(int id, int quantity) {
	storage[id].addQuantity(quantity);
	if (storage[id].getQuantity() == 0)
		storage.erase(id);
}

bool ItemStorage::removeItem(int id, int quantity) {
	if (storage.contains(id) && storage[id].getQuantity() >= quantity) {
		storage[id].addQuantity(-quantity);
		return true;
	}
	return false;
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