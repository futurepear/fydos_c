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

enum BlockType {
	EMPTY = -1,
	BIOME = 0,
	FLOOR = 1,
	TILE = 2,
	ROOF = 3
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
	int id;
	const char* processName;
	std::vector<Pair> inputs;
	std::vector<Pair> outputs;
	int where;
	float time;
};

enum class WeaponType {
	PICKAXE,
	SPEAR,
	DAGGER,
	SWORD,
	BOW
};

struct WeaponData {
	int id;
	WeaponType type;
	int damage;
	int animationLength;
	int cooldown;
	int knockback;
};

struct CraftingAreaData {
	const char* name;
	bool automatic;
	bool automaticFindRecipe;
	float craftingSpeed;
	std::vector<int> recipes;
};

class Dictionary {
public:
	static Registry<BlockData> blocks;
	static Registry<Recipe> recipes;
	static Registry<ItemData> items;
	static Registry<WeaponData> weapons;
	static Registry<CraftingAreaData> crafters;
};



