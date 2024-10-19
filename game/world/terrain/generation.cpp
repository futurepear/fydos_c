#include "generation.h"
#include <cmath>
#include "../../physics/vector.h"
#include "../../constants.h";
#include "../chunk.h";

static float fract(float x) {
	return x - std::floor(x);
}

static float mix(float x, float y, float a) {
	return x * (1.0f - a) + y * a;
}

static float random(const Vector<float>& st) {
	return fract(std::sin(st.x * 12.9898f + st.y * 78.233)* 43758.5453123);
}

static float noise(const Vector<float>& st) {
	Vector<float> i = Vector<float>{ std::floor(st.x), std::floor(st.y) };
	Vector<float> f = Vector<float>{ fract(st.x), fract(st.y) };
	
	float a = random(i);
	float b = random(i + Vector<float>(1.0, 0.0));
	float c = random(i + Vector<float>(0.0, 1.0));
	float d = random(i + Vector<float>(1.0, 1.0));

	Vector<float> u = Vector<float>{ f.x * f.x * (3.0f - 2.0f * f.x), f.y * f.y * (3.0f - 2.0f * f.y) };

	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
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
	for (int i = 0; i < Constants::chunkSize * Constants::chunkSize; i++) {
		Vector<float> st = { (float)chunk[i].x() + chunk.leftBorder() / Constants::tileWidth, (float)chunk[i].y() + chunk.topBorder() / Constants::tileWidth };
		float value = fbm(st * 0.2f);
		if (value > 0.5)
			chunk[i].setTile(1);
	}
}