#include "inventory.h"
#include "../physics/vector.h"
#include <string>
#include <stdint.h>
#include <algorithm>
#include <iostream>
#include "dictionary.h"
#include <cmath>

const int Item::id() const {
	return m_id;
}
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
const bool Item::empty() const {
	return (m_id == 0 || m_quantity == 0);
}

Item ItemBuffer::emptyItem{ 0, 0 };

ItemBuffer::ItemBuffer(float capacity) : m_maxWeight{ capacity }  {}
ItemBuffer::ItemBuffer(ItemBuffer* sharedInventory) : parent{sharedInventory} {}
ItemBuffer::~ItemBuffer() {
	unbindAllCursors();
}
float ItemBuffer::maxWeight() {
	if (parent != nullptr)
		return parent->maxWeight();
	return m_maxWeight;
}
void ItemBuffer::unbindCursor(CursorCommand* cursor) {
	for (int i = 0; i < bindedCursors.size(); i++) {
		if (&cursor == &bindedCursors[i]) {
			bindedCursors.erase(bindedCursors.begin() + i);
			return;
		}
	}
}
void ItemBuffer::unbindAllCursors() {
	for (int i = 0; i < bindedCursors.size(); i++)
		bindedCursors[i]->unbind();
}
void ItemBuffer::bindCursor(CursorCommand* cursor) {
	bindedCursors.push_back(cursor);
}
const Vector<float>* ItemBuffer::position() {
	return physicalPosition;
}
void ItemBuffer::setPhysicalPosition(const Vector<float>* position) {
	physicalPosition = position;
}

void ItemStorageOrdered::eraseSlot(int i) {
	storage[i] = Item{ 0, 0 };
}
int ItemStorageOrdered::emptySlots() {
	int slots = 0;
	for (int i = 0; i < size(); i++)
		if (storage[i].id() == 0) slots++;
	return slots;
}

ItemStorageOrdered::ItemStorageOrdered(float capacity, int n) : ItemBuffer{ capacity } {
	initialize(n);
}
ItemStorageOrdered::ItemStorageOrdered(ItemBuffer* sharedInventory, int n) : ItemBuffer{ sharedInventory } {
	initialize(n);
}
void ItemStorageOrdered::initialize(int n) {
	storage.resize(n);
	for (int i = 0; i < n; i++)
		eraseSlot(i);
}
int ItemStorageOrdered::canAdd(int id, int quantity) {
	bool room = false;
	for (int i = 0; i < size(); i++)
		room = room || (storage[i].id() == 0 || storage[i].id() == id);
	if (!room) return 0;
	float remainingWeight = maxWeight() - totalWeight();
	return std::min(quantity, int(remainingWeight / Dictionary::items[id].weight));
}
int ItemStorageOrdered::addItem(int id, int quantity) {
	int firstEmpty = -1;
	//first check if its in here
	for (int i = 0; i < size(); i++) {
		if (storage[i].id() == id) {
			return addItem(Item{ id, quantity }, i);
		}
		if (firstEmpty == -1 && storage[i].id() == 0)
			firstEmpty = i;
	}
	//if not go into first empty slot
	return addItem(Item{ id, quantity }, firstEmpty);
}
int ItemStorageOrdered::addItem(const Item& item, int pos) {
	//weight checking
	int quantity = item.getQuantity();
	quantity = canAdd(item.id(), quantity); 

	if (quantity == 0) return 0;

	if (storage[pos].empty()) {
		storage[pos] = Item{ item.id(), 0 };
	}
	addQuantity(pos, quantity);

	return quantity;
}
void ItemStorageOrdered::addQuantity(int index, int quantity) {
	changed = true;
	storage[index].addQuantity(quantity);
	if (storage[index].getQuantity() == 0) {
		eraseSlot(index);
	}
}
bool ItemStorageOrdered::removeItem(int id, int quantity, int pos) {
	if (id == 0 || quantity == 0) return true;
	int start = 0;
	if (pos != -1) start = pos;
	//if theres a pos u start at pos and instantly find it..
	for (int i = start; i < size(); i++) {
		if (storage[i].id() == id && storage[i].getQuantity() >= quantity) {
			addQuantity(i, -quantity);
			return true;
		}
	}
	return false;
}

bool ItemStorageOrdered::has(int id, int quantity) {
	bool res = false;
	for (int i = 0; i < size(); i++)
		res = res || (storage[i].id() == id && storage[i].getQuantity() >= quantity);
	return res;
}
int ItemStorageOrdered::maxSlots() {
	return size();
}
int ItemStorageOrdered::size() const {
	return storage.size();
}
int ItemStorageOrdered::displaySize(int stride) {
	return size();
}
const Item& ItemStorageOrdered::at(int position) {
	return (storage[position]);
}
const Item& ItemStorageOrdered::get(int index) {
	for (int i = 0; i < size(); i++)
		if (storage[i].id() == index) return storage[i];
	return Inventory::emptyItem;
}
const Item& ItemStorageOrdered::fastAccess(int index, int position) {
	if(position != -1)
		return at(position);
	return get(index);
}
const std::vector<int> ItemStorageOrdered::order() {
	std::vector<int> itemOrder(size());
	for (int i = 0; i < storage.size(); i++)
		itemOrder[i] = storage[i].id();
	return itemOrder;
}
bool ItemStorageOrdered::canFitRecipeOutputs(const Recipe& recipe, bool subtractInputs) {
	int slots = emptySlots();
	float weight = 0;
	for (auto& item : recipe.outputs) {
		if (!has(item.id)) slots--;
		weight += item.amount * Dictionary::items[item.id].weight;
	}

	if (subtractInputs) {
		for (auto& item : recipe.inputs) {
			weight += item.amount * Dictionary::items[item.id].weight;
			if (item.amount == get(item.id).getQuantity()) slots++;
		}
	}
	
	return (slots >= 0 && weight + totalWeight() <= maxWeight());
}
float ItemStorageOrdered::totalWeight() {
	if (parent != nullptr)
		return parent->totalWeight();
	float total = 0;
	for (int i = 0; i < size(); i++)
		total += storage[i].weight();
	return total;
}
void ItemStorageOrdered::print() {
	std::cout << "\n--Storage--\n";
	for(int i = 0; i < size(); i++)
		std::cout << storage[i].getName() << ": " << storage[i].getQuantity() << "\n";
	std::cout << "-----------\n";
	std::cout << "Capacity: " << totalWeight() << " / " << maxWeight();
	std::cout << "\n-----------\n\n\n";
}
void ItemStorageOrdered::update(){}
void ItemStorageOrdered::addToPool(std::unordered_map<int, int>& pool) const {
	for (int i = 0; i < size(); i++) {
		const Item& item = storage[i];
		int key = item.id();
		if (pool.contains(key)) {
			pool[key] += item.getQuantity();
		} else {
			pool[key] = item.getQuantity();
		}
	}
}

ItemStorage::ItemStorage(float capacity) : ItemBuffer{ capacity }  {}
ItemStorage::ItemStorage(ItemBuffer* sharedInventory) : ItemBuffer{ sharedInventory } {}
int ItemStorage::canAdd(int id, int quantity) {
	float remainingWeight = maxWeight() - totalWeight();
	return std::min(quantity, int(remainingWeight / Dictionary::items[id].weight));
}
int ItemStorage::addItem(int id, int quantity) {
	//weight checking
	
	int quantityCanAdd = canAdd(id, quantity);
	if (quantityCanAdd == 0 || quantity == 0) return 0;

	if (!storage.contains(id)) {
		storage[id] = Item{ id, 0 };
	}
	addQuantity(id, quantityCanAdd);

	return quantityCanAdd;
}
int ItemStorage::addItem(const Item& item, int pos) {
	return addItem(item.id(), item.getQuantity());
}
void ItemStorage::addQuantity(int id, int quantity) {
	changed = true;
	storage[id].addQuantity(quantity);
	if (storage[id].getQuantity() == 0) {
		storage.erase(id);
	}
}

bool ItemStorage::removeItem(int id, int quantity, int pos) {
	if (id == 0 || quantity == 0) return true;
	if (storage.contains(id) && storage[id].getQuantity() >= quantity) {
		addQuantity(id, -quantity);
		return true;
	}
	return false;
}
bool ItemStorage::has(int id, int quantity) {
	return (storage.contains(id) && storage[id].getQuantity() >= quantity);
}
int ItemStorage::maxSlots() {
	return -1; //inf
}
int ItemStorage::size() const  {
	return storage.size();
}
int ItemStorage::displaySize(int stride) {
	int sz = std::ceil(size()/stride)*stride;
	if (sz % stride == 0) sz += stride;
	return sz;
}
const Item& ItemStorage::at(int position) {
	std::vector<int> itemOrder = order();
	if (position >= size()) return ItemBuffer::emptyItem; //some slots are empty items
	return storage[itemOrder[position]];
}
const Item& ItemStorage::get(int index) {
	if (storage.contains(index)) {
		return storage[index];
	} else {
		return Inventory::emptyItem;
	}
}
const Item& ItemStorage::fastAccess(int index, int position) {
	if(index != -1)
		return get(index);
	return at(position);
}
const std::vector<int> ItemStorage::order() {
	std::vector<int> itemOrder(size());

	int i = 0;
	for (const auto& [key, value] : storage) {
		Item& item = storage[key];
		itemOrder[i] = item.id();
		i++;
	}
	return itemOrder;
}
bool ItemStorage::canFitRecipeOutputs(const Recipe& recipe, bool subtractInputs) {
	float weight = 0;
	for(auto& item : recipe.outputs)
		weight += item.amount * Dictionary::items[item.id].weight;

	if (subtractInputs) {
		for (auto& item : recipe.inputs)
			weight += item.amount * Dictionary::items[item.id].weight;
	}

	return (weight + totalWeight() <= maxWeight());
}
float ItemStorage::totalWeight() {
	if (parent != nullptr)
		return parent->totalWeight();
	float total = 0;
	for (auto& [key, value] : storage)
		total += storage[key].weight();
	return total;
}
void ItemStorage::print() {
	std::cout << "\n--Storage--\n";
	for (const auto& [key, value] : storage) {
		Item item = storage[key];
		std::cout << item.getName() << ": " << item.getQuantity() << "\n";
	}
	std::cout << "-----------\n";
	std::cout << "Capacity: " << totalWeight() << " / " << maxWeight();
	std::cout << "\n-----------\n\n\n";
}
void ItemStorage::update(){}
void ItemStorage::addToPool(std::unordered_map<int, int>& pool) const {
	for (const auto& [key, value] : storage) {
		const Item& item = storage.at(key);
		if (pool.contains(key)) {
			pool[key] += item.getQuantity();
		} else {
			pool[key] = item.getQuantity();
		}
	}
}

bool CursorCommand::empty() {
	return (id == 0);
}
bool CursorCommand::bound() {
	return !(storage == nullptr);
}
void CursorCommand::bind(ItemBuffer* s) {
	storage = s;
	s->bindCursor(this);
}
void CursorCommand::unbind() {
	storage->unbindCursor(this);
	storage = nullptr;
	id = 0;
	quantity = 0;
	position = -1;
}
const Item& CursorCommand::item() {
	if(bound())
		return storage->fastAccess(id, position);
	return Inventory::emptyItem;
}
int CursorCommand::getQuantity() {
	return quantity;
}
void CursorCommand::update() {
	if (storage == nullptr) return;
	const Item& i = item();
	if(i.id() != id){
		return unbind();
	}
	if(i.getQuantity() < quantity){
		quantity -= (quantity - i.getQuantity());
	}
	if (quantity == 0) unbind();
}
//used to return items if the player is too far away, invnetory broken or closes 
void CursorCommand::returnSelf() {

}

Inventory::Inventory(float capacity) : ItemStorage{ capacity }, cursorBuffer{ 100, 1 } {
	//9th slot / end of hotbar is empty
	hotbar[9] = 0;
}
void Inventory::print() {
	std::cout << "--INVENTORY--\n";
	std::cout << "--Cursor--\n";
	std::cout << cursorBuffer.at(0).getName() << ": " << cursorBuffer.at(0).getQuantity() << "\n";
	std::cout << "CMD: " << cursorCommand.item().getName() << ": " << cursorCommand.getQuantity() << "\n";
	std::cout << "--Hotbar--\n";
	for (int i = 0; i < 10; i++) {
		const Item& item = getHotbarItem(i);
		if (currentSlot == i)
			std::cout << ">>>";
		if (item.empty()) {
			std::cout << "empty - ";
		}
		else {
			std::cout << item.getName() << " - ";
		}
	}
	std::cout << "\n--Storage--\n";
	for (const auto& [key, value] : storage) {
		Item item = storage[key];
		std::cout << item.getName() << ": " << item.getQuantity() << "\n";
	}
	std::cout << "-----------\n";
	std::cout << "Total Capacity: " << totalWeight() << " / " << maxWeight();
	std::cout << "\n-----------\n\n\n";
}


const Item& Inventory::getHotbarItem(int index) const {
	index = hotbar[index];
	if (storage.contains(index)) {
		return (storage.at(index));
	} else {
		return ItemBuffer::emptyItem; 
	}
}
const Item& Inventory::currentItem() {
	cursorCommand.update();
	if (cursorCommand.bound()) {
		return cursorCommand.item();
	}
	return getHotbarItem(currentSlot);
}
const Item& Inventory::activeItem() {
	return currentItem();
}
void Inventory::switchSlot(int slot) {
	currentSlot = std::clamp(slot, 0, 9);
}
void Inventory::setHotbarItem(int index, int itemID) {
	hotbar[index] = itemID;
}
int Inventory::slot() {
	//if (!cursorBuffer.at(0).empty()) return 9; //"empty" slot
	return currentSlot;
}
bool Inventory::isCurrentItem(ItemType type) {
	if (currentItem().empty()) return (type == ItemType::EMPTY);
	return (currentItem().type() == type);
}
bool Inventory::isWeaponType(WeaponType type) {
	if (currentItem().type() == ItemType::WEAPON) {
		return (Dictionary::weapons[currentItem().getData().target].type == type);
	}
	return false;
}
float Inventory::totalWeight() {
	return ItemStorage::totalWeight();
}
bool Inventory::isHotbarActive() {
	return !cursorCommand.bound();
}
//transfer an item into a known or unknown empty space
// RETURNS: how much of the amount was transferred

//TODO: make it so it can find empty space
int Inventory::transfer(ItemBuffer* source, int itemID, int pos, int quantity, ItemBuffer* dest, int destPos) {
	if (quantity == 0) return 0;
	int transferableAmount = dest->canAdd(itemID, quantity);
	if (source == dest) transferableAmount = quantity; //weight is conserved
	
	source->removeItem(itemID, transferableAmount, pos);
	dest->addItem(Item{ itemID, transferableAmount }, destPos);
	return transferableAmount;
}

//swap from one inventory to another:
//itemXid or posX can are optional - only one can be -1 / optional
void Inventory::swap(ItemBuffer* A, int itemAid, int posA, ItemBuffer* B, int itemBid, int posB) {
	int limitA = A->maxWeight();
	int limitB = B->maxWeight();
	//can add check????  like even room in ordered storage?? if(B.has(itemBid thing u transferring into) || B.hasRoom())

	//if itemAid == itemBid 
	//	all possible of B adds into A
		
	const Item itemA = A->fastAccess(itemAid, posA);
	const Item itemB = B->fastAccess(itemBid, posB);
	if (itemA.empty() && itemB.empty()) return;

	auto weightA = A->totalWeight(); 
	auto weightB = B->totalWeight();
	auto itemWeightA = Dictionary::items[itemA.id()].weight;
	auto itemWeightB = Dictionary::items[itemB.id()].weight;

	int qA = itemA.getQuantity();
	int qB = itemB.getQuantity();

	bool canB = (weightA - itemA.weight() + itemB.weight() <= limitA);
	bool canA = (weightB - itemB.weight() + itemA.weight() <= limitB);
	

	//cant swap directly into each object.. 
	if ((!canA && !itemB.empty()) || (!canB && !itemA.empty())) {
		std::cout << "cant\n";
		//cant do it...
	}
	
	if (!canA && itemB.empty()) {
		//partial swap can be done (A into B)
		int amount = (limitB-weightB) / itemWeightA;
		A->removeItem(itemA.id(), amount, posA);
		B->addItem({ itemA.id(), amount }, posB); //error warning? if posB is -1 and B is OrderedStorage, program crashes
		std::cout << "partia\n";
	}
	if (!canB && itemA.empty()) {
		int amount = (limitA - weightA) / itemWeightB;
		B->removeItem(itemB.id(), amount, posB);
		A->addItem({ itemB.id(), amount }, posA);
		std::cout << "partial\n";
	}
	if (canA && canB) {

		A->removeItem(itemA.id(), qA, posA);
		B->removeItem(itemB.id(), qB, posB);

		B->addItem({ itemA.id(), qA }, posB);
		A->addItem({ itemB.id(), qB }, posA);

		std::cout << "full\n";
	}

	A->print();
	B->print();
}