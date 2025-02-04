#pragma once
#include "../../physics/physics.h"
#include "../../systems/inventory.h"
#include <queue>

class Entity {
private:
protected:
public:
	Body<float>* body;

	Entity();
	virtual ~Entity();
	virtual void update() = 0;
	Vector<float>& position();
};