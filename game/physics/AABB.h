#pragma once
#include "vector.h"

template <typename T = float> struct AABB {
	Vector<T> min;
	Vector<T> max;
};