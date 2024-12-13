#include "utility.h"
#include <cstdlib>
#include <iostream>

float random() {
	return std::rand() / (float)RAND_MAX;
}

int pollDropAmount(const Drop& drop) {
	float amountRandom = random();
	float chanceRandom = random();
	int chanceFactor = 1;
	
	if (chanceRandom > drop.chance) chanceFactor = 0;

	int span = drop.max - drop.min;
	int amount = span * amountRandom + drop.min;

	return amount * chanceFactor;
}