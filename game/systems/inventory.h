#pragma once
#include <unordered_map>
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "dictionary.h"


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
	const ItemType type() const;
};

class ItemBuffer {
private:
public:
	virtual void addItem(int id, int quantity) = 0;
	virtual bool removeItem(int id, int quantity) = 0;
};

class ItemStorageOrdered : public ItemBuffer{
private:
	
public:
};

class ItemStorage : public ItemBuffer {
protected:
	std::unordered_map<int, Item> storage{};
	void addQuantity(int id, int quantity);
public:
	void addItem(int id, int quantity);
	bool removeItem(int id, int quantity);
};


class Inventory : public ItemStorage{
private:
	
	Item* hotbar[10]{};
	int currentPage = 0;
	int currentSlot = 0;

public:
	Inventory() {}
	~Inventory() {}
	void print();

	const Item* getHotbarItem(int index) const;
	const Item* currentItem() const;
	void switchSlot(int slot);
	void setHotbarItem(int index, int itemID);
	int slot();
	bool isCurrentItem(ItemType type);

};
