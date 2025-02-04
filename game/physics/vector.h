#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <functional>

template <typename T = float> class Vector {
private:
	
public:
	T x{ 0 };
	T y{ 0 };

	Vector(T xComponent = 0, T yComponent = 0) {
		x = xComponent;
		y = yComponent;
	};

	struct hasher{
		size_t operator()(const Vector<int>& vector) const
		{
			size_t xComponent = std::hash<int>()(vector.x);
			size_t yComponent = std::hash<int>()(vector.y) << 1;
			return xComponent ^ yComponent;
		}
	};

	bool operator==(const Vector<T> vector) const {
		return (vector.x == x && vector.y == y);
	}

	Vector<T> operator+(const Vector<T>& vector) const {
		return Vector<T>(x + vector.x, y + vector.y);
	}
	Vector<T> operator-(const Vector<T>& vector) const {
		return Vector<T>(x - vector.x, y - vector.y);
	}
	Vector<T> operator*(const T scalar) const {
		return Vector<T>(x * scalar, y * scalar);
	}
	Vector<T> operator/(const T scalar) const {
		return Vector<T>(x / scalar, y / scalar);
	}
	Vector<T>& operator+=(const Vector<T>& vector) {
		x += vector.x;
		y += vector.y;
		return *this;
	}
	Vector<T>& operator-=(const Vector<T>& vector) {
		x -= vector.x;
		y -= vector.y;
		return *this;
	}
	Vector<T>& operator*=(const T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	T length() const {
		return std::hypot(x, y);
	}
	T lengthSquared() const {
		return x * x + y * y;
	}

	Vector<T> transform(const Vector<T>& displacement, T angle) {
		T sin = std::sin(angle);
		T cos = std::cos(angle);

		return Vector<T>(cos * x - sin * y + displacement.x, sin * x + cos * y + displacement.y);
	}
	
	static Vector<T> normalize(const Vector<T>& vector) {
		T length = vector.length();
		if (length == 0) return Vector<T>{0, 0};
		return Vector<T>(vector.x / length, vector.y / length);
	}
	static T dot(const Vector<T>& v1, const Vector<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static T cross(Vector<T>& v1, Vector<T>& v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}
	static T distance(const Vector<T>& v1, const Vector<T>& v2) {
		return (v1 - v2).length();
	}
	static T distanceSquared(Vector<T>& v1, Vector<T>& v2) {
		return (v1 - v2).lengthSquared();
	}
	static bool equal(Vector<T>& v1, Vector<T>& v2) {

	}

	void print() const {
		std::cout << "x:"  << x << "    y: " << y << "\n";
	}
	std::string toString() {
		return (std::string("x: ") + std::to_string(x) + std::string(" y: ") + std::to_string(y));
	}
};

