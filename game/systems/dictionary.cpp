#include "dictionary.h"

Registry<BlockData> Dictionary::blocks{ {
	{0, "air", {0, 0, 0}, 0, BlockType::EMPTY, {}},
	{1, "stone", {128, 128, 128}, 100, BlockType::TILE, {{2, 10, 10}}},
	{2, "grass", {40, 120, 20}, 0, BlockType::BIOME, {}},
	{3, "copper ore", {180, 125, 25}, 100, BlockType::TILE, {{8, 1, 2}}},
	{4, "coal ore", {10, 10, 10}, 100, BlockType::TILE, {{9, 3, 5}}},
	{5, "oak trunk", {110, 90, 60}, 100, BlockType::TILE, {{4, 50, 100}}},
	{6, "clay", {230, 120, 90}, 100, BlockType::TILE, {{7}}},
	{7, "crafting bench", {130, 130, 60}, 50, BlockType::TILE, {}},
	{8, "kiln", {255, 255, 255}, 50, BlockType::TILE, {}},
	{9, "molding station", {255, 200, 200}, 50, BlockType::TILE, {}},
	{10, "water", {80, 80, 255}, 0, BlockType::BIOME, {}}
} };

Registry<ItemData> Dictionary::items{ {
	{0, "empty", 0, ItemType::RESOURCE},
	{1, "stone", 1, ItemType::BLOCK, 1},
	{2, "rock", 2, ItemType::RESOURCE},
	{3, "pickaxe", 3, ItemType::WEAPON},
	{4, "oak wood", 4, ItemType::RESOURCE},
	{5, "clay", 5, ItemType::RESOURCE},
	{6, "clay pot", 6, ItemType::RESOURCE},
	{7, "clay block", 6, ItemType::BLOCK, 6},
	{8, "copper ore", 0, ItemType::RESOURCE},
	{9, "coal ore", 0, ItemType::RESOURCE},
	{10, "crafting bench", 0, ItemType::BLOCK, 7},
	{11, "kiln", 0, ItemType::BLOCK, 8},
	{12, "molding station", 0, ItemType::BLOCK, 9},
	{13, "molten copper", 0, ItemType::RESOURCE},
	{14, "copper bar", 0, ItemType::RESOURCE}
} };

Registry<Recipe> Dictionary::recipes{ {
	{{{4, 30}}, {{10, 1}}, 0, 1}
} };
