#pragma once
#include "chunk.h"
#include <unordered_map>
#include <cmath>
#include "../physics/vector.h"


/*
TODO:
______________________
- dynamic deleting and creating chunks inside the hash (should be easy, also could be pointer to chunks)
- use I/O system to write and read saves 

*/

class ChunkManager {
private:
	std::unordered_map<Vector<int>, Chunk, Vector<int>::hasher> chunks{};
	Chunk invalid{ 0, 0 };
public:
	ChunkManager(){
		createChunk({ 0, 0 });
		createChunk({ 0, -1 });
		createChunk({ -1, -1 });
		createChunk({ -1, 0 });
	}
	void createChunk(Vector<int> location) {
		chunks[location] = Chunk{ location.x * Constants::chunkSize * Constants::tileWidth, location.y * Constants::chunkSize * Constants::tileWidth };
	}
	~ChunkManager(){}
	Chunk& getChunk(const Vector<int>& index){
		if (!chunks.contains(index)) {
			return invalid;
		}
		return chunks[index];
	}
	Chunk& operator[](const Vector<int>& index) {
		return getChunk(index);
	}
	template <typename T> Vector<int> chunkSpace(const Vector<T>& vector) const {
		T x = vector.x / (Constants::chunkSize * Constants::tileWidth);
		T y = vector.y / (Constants::chunkSize * Constants::tileWidth);
		return Vector<int>{ (int)std::floor(x), (int)std::floor(y) };
	}
	template <typename T> Chunk& vectorToChunk(const Vector<T>& vector) {
		return getChunk(chunkSpace(vector));
	}
	bool isInvalid(Chunk& chunk) {
		return (&chunk == &invalid);
	}


	//wtf
	using container = std::unordered_map<Vector<int>, Chunk, Vector<int>::hasher>;
	using iterator = typename container::iterator;
	using const_iterator = typename container::const_iterator;

	iterator begin() { return chunks.begin(); }
	iterator end() { return chunks.end(); }
	const_iterator begin() const { return chunks.begin(); }
	const_iterator end() const { return chunks.end(); }
};
