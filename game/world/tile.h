#pragma once	
#include "../constants.h"
#include "../physics/vector.h"
#include <stdint.h>
#include <vector>
#include <cmath>

class TileGeometry {
private:
	Vector<float>* m_geometry;
	int m_size;
	int m_start;
public:
	TileGeometry(Vector<float>* geometry, int size, int start);
	~TileGeometry();
	int size() const;
	int start() const;
	Vector<float>* ptr() const;
	Vector<float>& operator[](const int index) const;
	
	const static TileGeometry index[];
	const static int totalGeometry;
};


class Tile {
private:
	struct geometryInfo {
		int group;
		int type;
	};

	int location = 0;
	int tileID = 0;
	geometryInfo geometry{ 0, 0 };
	uint8_t rotation = 0;
public:
	Tile(int loc = 0, int id = 0) : location{ loc }, tileID{ id } {

	}
	bool solid() {
		return (tileID == 1);
	}
	void setTile(int id) {
		tileID = id;
	}
	int x() {
		return location % Constants::chunkSize;
	}
	int y() {
		return location / Constants::chunkSize;
	}
	static int x(int location) {
		return location % Constants::chunkSize;
	}
	static int y(int location) {
		return location / Constants::chunkSize;
	}
	int id() {
		return tileID;
	}
	void rotate() {
		rotation = (rotation + 1) % 4;
	}
	void TEST() {
		geometry.type = (geometry.type + 1) % 6;
	}
	int geometryType() {
		return geometry.type;
	}
	float getRotation() {
		return (float)rotation * 90.0f * 3.14159265358979f / 180.0f;
	}
	int getRotationSin(int offset = 0) {
		int rot = (rotation + offset) % 4;
		return rot % 2 - 2 * (rot / 3);
	}
	int getRotationCos() {
		return getRotationSin(1);
	}
	int geometrySize();
	void getGeometryForRender(float* array);
	Vector<>* getPhysicsGeometry();
};