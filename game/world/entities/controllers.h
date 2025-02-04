#pragma once
#include "../../physics/physics.h"
#include "../../input/inputMap.h"
#include "../../constants.h"

class Controller {
private:
public:
	virtual void move(Body<float>* body, InputMap& input) = 0;
};

class PlayerController : public Controller {
private:
public:
	void move(Body<float>* body, InputMap& input);
};