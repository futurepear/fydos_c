#include "tile.h"

#include "../../constants.h"
#include "../../physics/vector.h"
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

Tile::Tile(int loc, int id, Vector<float> chunkPosition) : location{ loc }, tileID{ id } {
	position.x = chunkPosition.x + x() + 0.5f;
	position.y = chunkPosition.y + y() + 0.5f;
}
Tile::~Tile() {

}
void Tile::interact(TileInteractionEvent t) {
}
void Tile::update() {

}
//Unique string id for every tile: cX.cY.Li
std::string Tile::uuid() {
	int chunkX = (position.x - 0.5f - x()) / Constants::chunkWidth;
	int chunkY = (position.y - 0.5f - y()) / Constants::chunkWidth;

	std::string str = std::to_string(chunkX) + "." + std::to_string(chunkY) + "." + std::to_string(2) + "." + std::to_string(location);
	return str;
}
void Tile::clientSideUpdate(bool canInteract) {

}
bool Tile::solid() {
	return !isAir();
}
bool Tile::isAir() {
	return (tileID == 0);
}
const BlockData& Tile::getData() {
	return Dictionary::blocks[tileID];
}
void Tile::setTile(int id) {
	tileID = id;
	const BlockData& data = getData();
	health = data.hp;

}
const Vector<float>& Tile::center() {
	return position;
}
bool Tile::broken() {
	return (health <= 0);
}
void Tile::damage(int hp) {
	if (isAir()) return;
	health -= hp;
	//if (health <= 0) setTile(0);
}
int Tile::getHealth() {
	return health;
}
int Tile::x() {
	return location % Constants::chunkWidth;
}
int Tile::y() {
	return location / Constants::chunkWidth;
}
int Tile::x(int location) {
	return location % Constants::chunkWidth;
}
int Tile::y(int location) {
	return location / Constants::chunkWidth;
}
int Tile::id() {
	return tileID;
}
void Tile::rotate() {
	rotation = (rotation + 1) % 4;
}
void Tile::TEST() {
	geometry.type = (geometry.type + 1) % 6;
}
int Tile::geometryType() {
	return geometry.type;
}
float Tile::getRotation() {
	return (float)rotation * 90.0f * 3.14159265358979f / 180.0f;
}
int Tile::getRotationSin(int offset) {
	int rot = (rotation + offset) % 4;
	return rot % 2 - 2 * (rot / 3);
}
int Tile::getRotationCos() {
	return getRotationSin(1);
}

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
		physicsGeometry[i].x *= 1;// Constants::tileWidth;
		physicsGeometry[i].y *= 1;// Constants::tileWidth;
	}
	return physicsGeometry;
}