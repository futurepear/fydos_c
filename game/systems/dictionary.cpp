#include "dictionary.h"

using enum WeaponType;

Registry<BlockData> Dictionary::blocks{ {
//__BLOCKSTART__
	{0, "air", {0, 0, 0}, 0, BlockType::EMPTY, {}},
	{1, "stone", {128, 128, 128}, 100, BlockType::TILE, {{2, 10, 10}}},
	{2, "grass", {115, 190, 90}, 0, BlockType::BIOME, {}},
	{3, "copper ore", {180, 125, 25}, 100, BlockType::TILE, {{8, 1, 2}}},
	{4, "coal ore", {10, 10, 10}, 100, BlockType::TILE, {{9, 3, 5}}},
	{5, "oak trunk", {110, 90, 60}, 100, BlockType::TILE, {{4, 50, 100}}},
	{6, "clay", {178, 111, 80}, 100, BlockType::TILE, {{7}}},
	{7, "crafting bench", {130, 130, 60}, 100, BlockType::TILE, {{10}}},
	{8, "kiln", {255, 255, 255}, 50, BlockType::TILE, {{11}}},
	{9, "molding station", {255, 200, 200}, 50, BlockType::TILE, {{12}}},
	{10, "water", {60, 100, 170}, 0, BlockType::BIOME, {}},
	{11, "forest floor", {135, 170, 85}, 0, BlockType::BIOME, {}},
	{12, "river rock", {160, 160, 160}, 0, BlockType::BIOME, {}},
	{13, "rock", {100, 100, 100}, 0, BlockType::BIOME, {}},
	{14, "water rock", {70, 115, 230}, 0, BlockType::BIOME, {}},
	{15, "tilled soil", {90, 70, 50}, 0, BlockType::BIOME, {}},
	{16, "forest cover", {40, 120, 20}, 20, BlockType::FLOOR, {}},
	{17, "mushroom cover", {40, 100, 20}, 20, BlockType::FLOOR, {}},
	{18, "tall grass", {125, 190, 100}, 0, BlockType::FLOOR, {}},
	{19, "wildflowers", {40, 120, 200}, 0, BlockType::FLOOR, {}},
	{20, "bushy cover", {100, 190, 80}, 0, BlockType::FLOOR, {}},
	{21, "pine trunk", {130, 110, 80}, 100, BlockType::TILE, {{4, 50, 100}}},
	{22, "dirt", {120, 100, 80}, 100, BlockType::TILE, {}},
	{23, "iron ore", {30, 30, 20}, 100, BlockType::TILE, {}},
	{24, "oak canopy", {10, 10, 10}, 100, BlockType::ROOF, {}},
	{25, "pine canopy", {10, 10, 10}, 100, BlockType::ROOF, {}},
	{26, "wood planks floor", {100, 80, 50}, 100, BlockType::FLOOR, {}},
	{27, "wood planks", {120, 100, 70}, 100, BlockType::TILE, {}},
	{28, "wood plank roof", {100, 80, 50}, 100, BlockType::ROOF, {}},
	{29, "campfire", {255, 255, 0}, 100, BlockType::TILE, {}},
	{30, "chest", {255, 255, 255}, 100, BlockType::TILE, {{19}}}
//__BLOCKEND__
} };

Registry<ItemData> Dictionary::items{ {
//__ITEMSTART__	
	{0, "empty", 0, ItemType::RESOURCE},
	{1, "Stone Block", 1, ItemType::BLOCK, 1},
	{2, "Rock", 2, ItemType::RESOURCE},
	{3, "Stone Pickaxe", 3, ItemType::WEAPON, 0},
	{4, "Oak Wood", 0.1, ItemType::RESOURCE},
	{5, "Clay", 1, ItemType::RESOURCE},
	{6, "Clay Bowl", 6, ItemType::RESOURCE},
	{7, "Hardened Clay Block", 6, ItemType::BLOCK, 6},
	{8, "Copper Ore", 5, ItemType::RESOURCE},
	{9, "Coal Ore", 1, ItemType::RESOURCE},
	{10, "Crafting Bench", 1, ItemType::BLOCK, 7},
	{11, "Kiln", 1, ItemType::BLOCK, 8},
	{12, "Molding Station", 1, ItemType::BLOCK, 9},
	{13, "Molten Copper", 1, ItemType::RESOURCE},
	{14, "Copper Bar", 1, ItemType::RESOURCE},
	{15, "Sharpened Stick", 1, ItemType::WEAPON},
	{16, "Iron Ore", 1, ItemType::RESOURCE},
	{17, "Oak Planks", 1, ItemType::BLOCK, 27},
	{18, "Oak Planks Floor", 1, ItemType::BLOCK, 26},
	{19, "Chest", 1, ItemType::BLOCK, 30}
//__ITEMEND__
} };

Registry<Recipe> Dictionary::recipes{ {
//__RECIPESTART__
	{0, "Empty", {{}}, {{}}, 0, 0},
	{1, "Crafting Bench", {{4, 30}}, {{10, 1}}, 0, 1},
	{2, "Sharpened Stick", {{4, 40}}, {{15, 1}}, 0, 0.5},
	{3, "Kiln", {{5, 40}, {2, 20}}, {{11, 1}}, 0, 2},
	{4, "Molding Station", {{5, 40}, {2, 20}, {4, 20}}, {{12, 1}}, 0, 2},
	{5, "Copper Smelting", {{8, 1}, {6, 1}}, {{13, 1}}, 0, 10},
	{6, "Copper Bar Molding", {{13, 1}}, {{14, 5}, {6, 1}}, 0, 10},
	{7, "Clay", {{7, 1}}, {{5, 10}}, 0, 1},
	{8, "Chest", {{4, 40}}, {{{19, 1}}}, 0, 1}
//__RECIPEEND__
} };

Registry<WeaponData> Dictionary::weapons{ {
//__WEAPONSTART__
	{0, PICKAXE, 50, 40, 40, 4}
//__WEAPONEND__
} };

Registry<CraftingAreaData> Dictionary::crafters{ {
//__CRAFTSTART
	{"Inventory", true, false, 0.5, {1, 2, 7}},
	{"Crafting Bench", false, false, 1.0, {1, 2, 3, 4, 7, 8}},
	{"Kiln", true, true, 1.0, {5}},
	{"Molding Station", true, false, 1.0, {6}}
//__CRAFTEND__
} };
