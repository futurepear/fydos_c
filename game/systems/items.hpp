#pragma once

enum class ItemType {
	EMPTY,
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
	int target = 0; //ID of BLOCK/WEAPON/EFFECT created

	//[] of items consumed:
	/*
		Resource: null
		Block: ID of block used
		Resource: null
		Consumable: null (target is itself)
		WEAPON: list of items consumed
		equiable: null
		EMPTY: null
	*//*
	const char* consumes;*/
};

constexpr ItemData itemIndex[4]{
	{0, "empty", 0, ItemType::RESOURCE},
	{1, "stone", 1, ItemType::BLOCK, 1},
	{2, "rock", 2, ItemType::RESOURCE},
	{3, "pickaxe", 3, ItemType::WEAPON}
};