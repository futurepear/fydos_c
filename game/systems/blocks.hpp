enum class BlockType {
	EMPTY,
	BIOME,
	FLOOR,
	TILE,
	ROOF
};

struct BlockData {
	int id;
	const char* name;
};