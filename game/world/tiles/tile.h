#pragma once	
#include "../../constants.h"
#include "../../physics/vector.h"
#include "../../systems/dictionary.h"
#include "../../systems/crafting.h"
#include "../../gui/components.h"
#include <stdint.h>
#include <vector>
#include <cmath>

struct TileInteractionEvent {
	bool openStorage = false;
};

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
	int health = 100;
	geometryInfo geometry{ 0, 0 };
	uint8_t rotation = 0;
protected:
	Vector<float> position; 
public:
	Tile(int loc = 0, int id = 0, Vector<float> chunkPosition = {0, 0});
	virtual ~Tile();
	virtual void interact(TileInteractionEvent t);
	virtual void update(); 
	virtual void clientSideUpdate(bool canInteract);
	std::string uuid();
	//add virtual method to get items that spill when breaking block
	bool solid();
	bool isAir();
	const Vector<float>& center();
	const BlockData& getData();
	void setTile(int id);
	bool broken();
	void damage(int hp);
	int getHealth();
	int x();
	int y();
	static int x(int location);
	static int y(int location);
	int id();
	void rotate();
	void TEST();
	int geometryType();
	float getRotation();
	int getRotationSin(int offset = 0);
	int getRotationCos();
	int geometrySize();
	void getGeometryForRender(float* array);
	Vector<>* getPhysicsGeometry();
};

