#pragma once
#include <unordered_map>
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>

enum class ItemType {
	RESOURCE,
	WEAPON,
	CONSUMABLE,
	BLOCK,
	EQUIPABLE
};

struct ItemData {
	int id;
	const char* name;
	float weight;
	ItemType type;
};

constexpr ItemData itemIndex[3]{ 
	{0, "empty", 0, ItemType::RESOURCE},
	{1, "stone", 1, ItemType::BLOCK},
	{2, "rock", 2, ItemType::RESOURCE}
};

class Item {
private:
	int m_id;
	int m_quantity;
public:
	Item(int id = 0, int quantity = 0) : m_id{ id }, m_quantity{ quantity } {
	}
	~Item() {
	}
	const ItemData& getData() const;
	const char* getName() const;
	float weight() const;
	int getQuantity() const;
	void addQuantity(int amount);
};



class Inventory {
private:
	std::unordered_map<int, Item> storage{};
	Item* hotbar[10]{};
	int currentPage = 0;
	int currentSlot = 0;
public:
	Inventory() {}
	~Inventory() {}
	void print();
	const Item* getHotbarItem(int index) const;
	void addItem(int id, int quantity);
	void removeItem(int id, int quantity);
	void switchSlot(int slot);
};
