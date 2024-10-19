#pragma once
#include "vector.h"
#include "AABB.h"
#include "transform.h"

enum Shape {
	circle,
	polygon
};

template <typename T = float> class Body {
private:
	Vector<T>* vertices = nullptr;
	Vector<T>* transformedVertices = nullptr;
	int verticesLength = 0; 
	bool transformUpdateRequired = true;

	T inverseMass = 1;
	T inverseRotationalInertia = 1;
	T restitution = 0.5;

	bool m_isStatic = false;

	Shape shapeType = polygon;

	Vector<T> center{ 0, 0 };

	void calculateRotationalInertia();
	
public:
	Vector<T> position{ 0, 0 };
	Vector<T> velocity{ 0, 0 };
	Vector<T> force{ 0, 0 };
	T angle = 0;
	T angularVelocity = 0;

	T mass = 1;
	T rotationalInertia = 1;

	T staticFriction = 1;
	T kineticFriction = 1;

	T radius = 50;
	T width = 100;
	T height = 100;

	Body();
	~Body();
	AABB<T> getAABB();
	void setStatic(bool staticState);
	void setMass(T mass);
	void step(T time);
	void setVertices(Vector<T>* verts, int length);
	void setPosition(T x, T y);
	void move(const Vector<T>& displacement);
	void rotate(T angle);
	void applyForce(Vector<T>& appliedForce);
	void makeCircle();
	Vector<T>* getTransformedVertices();
	bool isCircle();
	bool isPolygon();
	bool isStatic();
	int size();
	Vector<T> calculateCenter();
};


template <typename T> Body<T>::Body() {
	vertices = new Vector<T>[4] { { -width/2, -height/2 }, { -width/2, height/2 }, { width/2, height/2 }, {width/2, -height/2}};
	verticesLength = 4;
	transformedVertices = new Vector<T>[verticesLength];
}

template <typename T> Body<T>::~Body() {
	delete[] vertices;
	delete[] transformedVertices;
}

template <typename T> void Body<T>::calculateRotationalInertia() {
	if (shapeType == circle)
		rotationalInertia = 1 / 2 * mass * radius * radius;
	if (shapeType == polygon)
		rotationalInertia = 1 / 12 * mass * (width * width + height * height);
	inverseRotationalInertia = 1 / rotationalInertia;
}

template <typename T> void Body<T>::setStatic(bool staticState) {
	m_isStatic = staticState;
	if (staticState) {
		inverseMass = 0;
		inverseRotationalInertia = 0;
	}
}

template <typename T> void Body<T>::setMass(T mass) {
	this->mass = mass;
	inverseMass = 1 / mass;
	calculateRotationalInertia();
}

template <typename T> AABB<T> Body<T>::getAABB() {
	if (shapeType == circle) {
		return AABB<T>{{position.x - radius, position.y - radius}, { position.x + radius, position.y + radius }};
	}
	if (shapeType == polygon) {
		T minX = 100000;
		T minY = 100000;
		T maxX = -100000;
		T maxY = -100000;

		Vector<T>* verts = getTransformedVertices();

		for (int i = 0; i < verticesLength; i++) {
			Vector<T>& v = verts[i];
			if (v.x < minX) { minX = v.x; }
			if (v.x > maxX) { maxX = v.x; }
			if (v.y < minY) { minY = v.y; }
			if (v.y > maxY) { maxY = v.y; }
		}
		return AABB<T>{{minX, minY}, { maxX, maxY }};
	}
	return AABB<T>({ 0, 0 }, { 0, 0 });
}

template <typename T> Vector<T>* Body<T>::getTransformedVertices() {
	if (transformUpdateRequired) {
		for (int i = 0; i < verticesLength; i++) {
			transformedVertices[i] = vertices[i].transform(position, angle);
		}
		transformUpdateRequired = false;
	}
	return transformedVertices;
}

template <typename T> Vector<T> Body<T>::calculateCenter() {
	Vector<T>* verts = getTransformedVertices();
	T sumX = 0;
	T sumY = 0;
	for (int i = 0; i < verticesLength; i++) {
		sumX += verts[i].x;
		sumY += verts[i].y;
	}
	sumX /= verticesLength;
	sumY /= verticesLength;
	return Vector<T>{sumX, sumY};
}

template <typename T> void Body<T>::step(T time) {
	velocity += force * (time * inverseMass);
	move(velocity * time);
	angle += angularVelocity * time;

	force.x = 0;
	force.y = 0;
}

template <typename T> void Body<T>::setVertices(Vector<T>* verts, int length) {
	delete vertices;
	delete transformedVertices;

	vertices = verts;
	verticesLength = length;
	transformedVertices = new Vector<T>[verticesLength];
	transformUpdateRequired = true;
}

template <typename T> void Body<T>::setPosition(T x, T y) {
	position.x = x;
	position.y = y;
}

template <typename T> void Body<T>::applyForce(Vector<T>& appliedForce) {
	force += appliedForce;
}

template <typename T> void Body<T>::move(const Vector<T>& displacement) {
	position += displacement;
	transformUpdateRequired = true;
}

template <typename T> void Body<T>::rotate(T rotation) {
	angle += rotation;
	transformUpdateRequired = true;
}

template <typename T> bool Body<T>::isCircle() {
	return (shapeType == circle);
}

template <typename T> bool Body<T>::isPolygon() {
	return (shapeType == polygon);
}

template <typename T> bool Body<T>::isStatic() {
	return m_isStatic;
}

template <typename T> int Body<T>::size() {
	return verticesLength;
}

template <typename T> void Body<T>::makeCircle() {
	shapeType = circle;
}