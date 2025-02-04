#include "measurements.h"

using namespace GUIStyles;

Pixel::Pixel(int x) {
	this->value = x;
}
Percentage::Percentage(int x) {
	this->value = x;
}
Calc::Calc(Measurement* a, Measurement* b) : m_a{ a }, m_b{ b } {
	this->value = 0;
}
Calc::~Calc() {
	delete m_a;
	delete m_b;
}

int Pixel::compute(int edge) {
	return this->value;
}
int Percentage::compute(int edge) {
	return this->value * edge / 100;
}
int Calc::compute(int edge) {
	return m_a->compute(edge) + m_b->compute(edge);
}

Measurement* GUIStyles::createMeasurement(int x, int mode) {
	switch (mode) {
	case '%':
		return new Percentage(x);
	case 'p':
		return new Pixel(x);
	default:
		return new Pixel(x);
	}
}