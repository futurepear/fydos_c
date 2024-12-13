#pragma once

struct Drop {
	int target;
	int min = 1;
	int max = 1;
	float chance = 1;
};

float random();

int pollDropAmount(const Drop& drop);