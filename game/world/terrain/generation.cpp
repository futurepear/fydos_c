#include "generation.h"
#include <cmath>
#include "../../physics/vector.h"
#include "../../constants.h"
#include "../chunk.h"
#include <stdlib.h> 


static float fract(float x) {
	return x - std::floor(x);
}

static float mix(float x, float y, float a) {
	return x * (1.0f - a) + y * a;
}

static float random(const Vector<float>& st) {
	return fract(std::sin(st.x * 12.9898f + st.y * 78.233f)* 43758.5453123f);
}

static float noise(const Vector<float>& st) {
	Vector<float> i = Vector<float>{ std::floor(st.x), std::floor(st.y) };
	Vector<float> f = Vector<float>{ fract(st.x), fract(st.y) };
	
	float a = random(i);
	float b = random(i + Vector<float>(1.0, 0.0));
	float c = random(i + Vector<float>(0.0, 1.0));
	float d = random(i + Vector<float>(1.0, 1.0));

	Vector<float> u = Vector<float>{ f.x * f.x * (3.0f - 2.0f * f.x), f.y * f.y * (3.0f - 2.0f * f.y) };

	return mix(a, b, u.x) + (c - a) * u.y * (1.0f - u.x) + (d - b) * u.x * u.y;
}

static float fbm(Vector<float> st) {
	const int OCTAVES = 6;

	float value = 0.0f;
	float amplitude = 0.5f;
	float frequency = 0.0f;

	for (int i = 0; i < OCTAVES; i++) {
		value += amplitude * noise(st);
		st *= 2.0f;
		amplitude *= 0.5f;
	}

	return value;
}

void generateChunk(Chunk& chunk) {
	//tree
	int max = Constants::chunkWidth * Constants::chunkWidth;

	for (int i = 0; i < 10; i++) {
		std::srand(i * 942134899123 + 1237824214 + chunk.leftBorder() * 1123 + chunk.topBorder() * 23148);
		float r = std::rand() / (float)RAND_MAX;
		int index = static_cast<int>(r * max);
		chunk[2][index]->setTile(5);
	}

	
	for (int i = 0; i < Constants::chunkWidth * Constants::chunkWidth; i++) {
		Vector<float> st = { (float)chunk[2][i]->x() + chunk.leftBorder(), (float)chunk[2][i]->y() + chunk.topBorder() };


		float elevation = fbm(st * 0.1f);
		bool forestBiome = (fbm(st * 0.02f) > 0.4);

		//carving rivers & lakes
		float riverValue = fbm(st * 0.05f);

		bool water = (elevation < 0.3f);


		//biomes
		if (forestBiome) {
			chunk[0][i]->setTile(11);
		}

		//rock
		float rockValue = fbm(st * 0.1f);
		if (elevation > 0.5)
			chunk[2][i]->setTile(1);

		//copper ore
		float copperValue = fbm(st * 0.3f + Vector<float>{100.0f, 100.0f});
		if (copperValue > 0.7 && rockValue > 0.6 && !water)
			chunk[2][i]->setTile(3);

		//coal ore
		float coalValue = fbm(st * 0.3f + Vector<float>{32.0f, 238.0f});	
		if (coalValue > 0.7 && rockValue > 0.63 && !water)
			chunk[2][i]->setTile(4);

		//clay
		float clayValue = fbm(st * 0.3f + Vector<float>{43.0f, 935.0f});
		if (clayValue > 0.6 && 0.55 > rockValue && rockValue > 0.5 && !water)
			chunk[2][i]->setTile(6);

		if (water) {
			chunk[0][i]->setTile(10);
			chunk[2][i]->setTile(0);
		}
		
	}
}