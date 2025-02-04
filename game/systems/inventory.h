#pragma once
#include <unordered_map>
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "dictionary.h"
#include "../physics/vector.h"
#include <set>
#include <array>

typedef std::unordered_map<int, int> ItemPool;

class Item {
private:
	int m_id;
	int m_quantity;
public:
	Item(int id = 0, int quantity = 0) : m_id{ id }, m_quantity{ quantity } {
	}
	~Item() {
	}
	const int id() const;
	const ItemData& getData() const;
	const char* getName() const;
	float weight() const;
	int getQuantity() const;
	void addQuantity(int amount);
	const ItemType type() const;
	const bool empty() const;
};

class CursorCommand;

class ItemBuffer {
private:
	std::vector<CursorCommand*> bindedCursors{};
	void unbindAllCursors();
	const Vector<float>* physicalPosition = nullptr; 
protected:
	float m_maxWeight = 100;
	ItemBuffer* parent = nullptr; 
public:
	bool changed = false;
	static Item emptyItem;
	ItemBuffer(float capacity);
	ItemBuffer(ItemBuffer* sharedInventory);
	~ItemBuffer();
	virtual int addItem(int id, int quantity) = 0; //returns how much was added
	virtual int addItem(const Item& item, int pos = 0) = 0;
	virtual bool removeItem(int id, int quantity, int pos = -1) = 0;
	virtual int maxSlots() = 0;
	virtual bool has(int id, int quantity = 0) = 0;
	virtual int size() const = 0;
	virtual int displaySize(int stride) = 0;
	virtual const Item& at(int position) = 0;
	virtual const Item& get(int index) = 0;
	virtual const Item& fastAccess(int index = -1, int position = -1) = 0;
	virtual int canAdd(int id, int quantity) = 0;
	virtual const std::vector<int> order() = 0; //returns array with item ids in order
	virtual bool canFitRecipeOutputs(const Recipe& recipe, bool subtractInputs = false) = 0;
	virtual float totalWeight() = 0;
	virtual void print() = 0;
	virtual void update() = 0; //an update every tick 
	virtual void addToPool(std::unordered_map<int, int>& pool) const = 0;
	float maxWeight();
	void unbindCursor(CursorCommand* cursor);
	void bindCursor(CursorCommand* cursor);
	const Vector<float>* position(); 
	void setPhysicalPosition(const Vector<float>* position);
};

class ItemStorageOrdered : public ItemBuffer{
protected:
	std::vector<Item> storage;
	void addQuantity(int index, int quantity);
	void eraseSlot(int i);
	int emptySlots();
	void initialize(int n);
public:
	ItemStorageOrdered(float capacity, int n = 1);
	ItemStorageOrdered(ItemBuffer* sharedInventory, int n = 1);
	int canAdd(int id, int quantity);
	int addItem(int id, int quantity);
	int addItem(const Item& item, int pos = 0);
	bool removeItem(int id, int quantity, int pos = -1);
	bool has(int id, int quantity = 0);
	int maxSlots();
	int size() const;
	int displaySize(int stride);
	const Item& at(int position);
	const Item& get(int index);
	const Item& fastAccess(int index = -1, int position = -1);
	const std::vector<int> order();
	bool canFitRecipeOutputs(const Recipe& recipe, bool subtractInputs = false);
	float totalWeight();
	void print();
	void update();
	void addToPool(std::unordered_map<int, int>& pool) const;
};

class ItemStorage : public ItemBuffer {
protected:
	std::unordered_map<int, Item> storage{};
	void addQuantity(int id, int quantity);
public:
	ItemStorage(float capacity = 100);
	ItemStorage(ItemBuffer* sharedInventory);
	int canAdd(int id, int quantity);
	int addItem(int id, int quantity); 
	int addItem(const Item& item, int pos = 0);
	bool removeItem(int id, int quantity, int pos = -1);
	bool has(int id, int quantity = 0);
	int maxSlots();
	int size() const;
	int displaySize(int stride);
	const Item& at(int position);
	const Item& get(int index);
	const Item& fastAccess(int index = -1, int position = -1);
	const std::vector<int> order();
	bool canFitRecipeOutputs(const Recipe& recipe, bool subtractInputs = false);
	float totalWeight();
	void print();
	void update();
	void addToPool(std::unordered_map<int, int>& pool) const;
};

class CursorCommand {
public:
	ItemBuffer* storage = nullptr;
	//std::set<ItemBuffer*> bindedInventories{};
	int id = 0; //item id
	int quantity = 0;
	int position = -1;

	bool empty();
	bool bound();

	void returnSelf();
	void bind(ItemBuffer* s);
	void unbind();
	const Item& item();
	void update();
	int getQuantity();
};

class Inventory : public ItemStorage{
private:
	int hotbar[10]{0}; //ids of items in hotbar
	int cursor = 0; //active item
	int currentPage = 0;
	int currentSlot = 0;

public:
	ItemStorageOrdered cursorBuffer;
	CursorCommand cursorCommand;

	Inventory(float capacity = 100);
	~Inventory() {}
	void print();

	const Item& getHotbarItem(int index) const;
	const Item& currentItem();
	const Item& activeItem();
	void returnCursor();
	void switchSlot(int slot);
	void setHotbarItem(int index, int itemID);
	int slot();
	bool isHotbarActive();
	bool isCurrentItem(ItemType type);
	bool isWeaponType(WeaponType type);
	float totalWeight();

	static void swap(ItemBuffer* A, int itemAid, int posA, ItemBuffer* B, int itemBid, int posB);
	static int transfer(ItemBuffer* source, int itemID, int pos, int quantity, ItemBuffer* dest, int destPos);
};