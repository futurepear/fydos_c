#include "controllers.h"

void PlayerController::move(Body<float>* body, InputMap& input) {

	//TODO: better movement
	Vector<float> displacement{};

	float v = 2.0 / 60.0f;
	displacement.x = int(input.A) * -1 + int(input.D);
	displacement.y = int(input.W) * -1 + int(input.S);

	bool isRunning = input.A || input.W || input.D || input.S;
	bool isStill = body->velocity.lengthSquared() < 0.1;
	//isStill = true;

	displacement = Vector<float>::normalize(displacement);
	Vector<float> vN = Vector<float>::normalize(body->velocity);

	Vector<float> force = { displacement.x * v - (0.2f * body->velocity.x + 200.0f * 1 * 9.8f * vN.x * !isRunning) * !isStill,
						   displacement.y * v - (0.2f * body->velocity.y + 200.0f * 1 * 9.8f * vN.y * !isRunning) * !isStill
	};

	body->move(displacement * v);
}