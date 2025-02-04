#include "entity.h"

Entity::Entity() {
	body = new Body<float>{};
}
Entity::~Entity() {
	delete body;
}
Vector<float>& Entity::position() {
	return body->position;
}