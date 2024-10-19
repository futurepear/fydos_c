#include "tile.h"

#include "../constants.h"
#include "../physics/vector.h"
#include <stdint.h>
#include <vector>

TileGeometry::TileGeometry(Vector<float>* geometry, int size, int start) {
	m_geometry = geometry;
	m_size = size;
	m_start = start;
}
TileGeometry::~TileGeometry() {
	delete[] m_geometry;
}
int TileGeometry::size() const {
	return m_size;
}

int TileGeometry::start() const {
	return m_start;
}

Vector<float>* TileGeometry::ptr() const {
	return m_geometry;
}


Vector<float>& TileGeometry::operator[](const int index) const {
	return m_geometry[index];
}

const TileGeometry TileGeometry::index[] = {
	TileGeometry{new Vector<float>[] { { -0.5f, -0.5f }, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, -0.5f} }, 4, 0 }, //square
	TileGeometry{new Vector<float>[] { { -0.5f, -0.5f }, {-0.5f, 0.5f}, {0.5f, -0.5f} }, 3, 4 }, //slope
	TileGeometry{new Vector<float>[] { { -0.5f, -0.5f }, {-0.5f, 0.0f}, {0.5f, 0.0f}, {0.5f, -0.5f} }, 4, 7 }, //half rectangle
	TileGeometry{new Vector<float>[] { { -0.25f, -0.5f }, {-0.25f, 0.0f}, {0.25f, 0.0f}, {0.25f, -0.5f} }, 4, 11 }, //centered half rectangle
	TileGeometry{new Vector<float>[] { { -0.5f, -0.5f }, {-0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.5f} }, 4, 15 }, //corner square 
	TileGeometry{new Vector<float>[] { { -0.5f, -0.5f }, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, 0.0f} }, 4, 19 }, //2x1 slope big
};

const int TileGeometry::totalGeometry = sizeof(TileGeometry::index) / sizeof(TileGeometry);


int Tile::geometrySize() {
	return TileGeometry::index[geometry.type].size();
}

void Tile::getGeometryForRender(float* array) {
	Vector<>* geo = TileGeometry::index[geometry.type].ptr();
	int size = geometrySize();

	array[0] = 0.0f;
	array[1] = 0.0f;
	for (int i = 0; i < size; i++) {
		int cos = getRotationCos();
		int sin = getRotationSin();
		array[2 * i + 2] = geo[i].x * cos - geo[i].y * sin;
		array[2 * i + 3] = geo[i].x * sin + geo[i].y * cos;
	}
	array[2*size + 2] = array[2];
	array[2*size + 3] = array[3];
}

Vector<>* Tile::getPhysicsGeometry() {
	int size = geometrySize();
	int cos = getRotationCos();
	int sin = getRotationSin();

	Vector<>* geo = TileGeometry::index[geometry.type].ptr();
	Vector<>* physicsGeometry = new Vector<>[size] {};

	for (int i = 0; i < size; i++) {
		physicsGeometry[i].x = geo[i].x * cos - geo[i].y * sin;
		physicsGeometry[i].y = geo[i].x * sin + geo[i].y * cos;
		physicsGeometry[i].x *= Constants::tileWidth;
		physicsGeometry[i].y *= Constants::tileWidth;
	}
	return physicsGeometry;
}