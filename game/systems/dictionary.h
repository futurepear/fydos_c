#pragma once
#include "registry.hpp"
#include <vector>
#include "utility.h"
/*
All game data

Blocks
Items
Enemies
etc idk

*/

//maybe move this later
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

enum class BlockType {
	EMPTY,
	BIOME,
	FLOOR,
	TILE,
	ROOF
};

struct BlockData {
	struct Color {
		int r;
		int g;
		int b;
	};

	int id;
	const char* name;
	Color color;
	int hp;
	BlockType type;
	std::vector<Drop> drops;
};

struct Recipe {
	struct Pair {
		int id;
		int amount;
	};
	
	std::vector<Pair> inputs;
	std::vector<Pair> outputs;
	int where;
	float time;
};

class Dictionary {
public:
	static Registry<BlockData> blocks;
	static Registry<Recipe> recipes;
	static Registry<ItemData> items;
};



