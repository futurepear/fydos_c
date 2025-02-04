#pragma once
#include "body.hpp"
#include <cmath>
#include <algorithm>

namespace collisions {
	template <typename T> bool intersectCircles(Body<T>& body1, Body<T>& body2, Vector<T>& normal, T& depth);
	template <typename T> bool SAT(Body<T>& body1, Body<T>& body2, Vector<T>& normal, T& depth);
	template <typename T> bool SATcircle(Vector<T>& center, T radius, Vector<T>* vertices, int size, const Vector<T>& verticesCenter, Vector<T>& normal, T& depth);
	template <typename T> void projectVertices(Vector<T>* vertices, int size, Vector<T>& axis, T& min, T& max);
	template <typename T> int circleClosestPolygonPoint(Vector<T>& center, Vector<T>* vertices, int size);
	template <typename T> void projectCircle(Vector<T>& center, T radius, Vector<T> axis, T& min, T& max);
	template <typename T> bool intersectRect(Vector<T> rectPos, Vector<T> rectDims, Vector<T> point);
}

template <typename T> bool collisions::intersectRect(Vector<T> rectPos, Vector<T> rectDims, Vector<T> point) {
	return (rectPos.x < point.x && point.x < rectPos.x + rectDims.x) && (rectPos.y < point.y && point.y < rectPos.y + rectDims.y);
}

template <typename T> bool collisions::intersectCircles(Body<T>& body1, Body<T>& body2, Vector<T>& normal, T& depth) {
	T distance = Vector<T>::distance(body1.position, body2.position);
	T radii = body1.radius + body2.radius;
	if (distance >= radii) return false;

	normal = Vector<T>::normalize(body2.position - body1.position);
	depth = radii - distance;
	return true;
}	

template <typename T> bool collisions::SAT(Body<T>& body1, Body<T>& body2, Vector<T>& normal, T& depth) {
	depth = 1000;

	Vector<T>* verts1 = body1.getTransformedVertices();
	Vector<T>* verts2 = body2.getTransformedVertices();

	for (int i = 0; i < body1.size(); i++) {
		Vector<T> startVertex = verts1[i];
		Vector<T> endVertex = verts1[(i + 1) % (body1.size())];

		Vector<T> edge = endVertex - startVertex;
		Vector<T> axis = Vector<T>::normalize(Vector<T>{-edge.y, edge.x});
		
		T min1; T min2; T max1; T max2;
		projectVertices(verts1, body1.size(), axis, min1, max1);
		projectVertices(verts2, body2.size(), axis, min2, max2);

		if (min1 >= max2 || min2 >= max1) return false;

		T axisDepth = std::min(max2 - min1, max1 - min2);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}
	for (int i = 0; i < body2.size(); i++) {
		Vector<T> startVertex = verts2[i];
		Vector<T> endVertex = verts2[(i + 1) % (body2.size())];

		auto edge = endVertex - startVertex;
		Vector<T> axis = Vector<T>::normalize(Vector<T>{-edge.y, edge.x});

		T min1; T min2; T max1; T max2;
		projectVertices(verts1, body1.size(), axis, min1, max1);
		projectVertices(verts2, body2.size(), axis, min2, max2);

		if (min1 >= max2 || min2 >= max1) return false;

		T axisDepth = std::min(max2 - min1, max1 - min2);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}

	auto center1 = body1.calculateCenter();
	auto center2 = body2.calculateCenter();
	auto direction = center2 - center1;
	if (Vector<T>::dot(direction, normal) < 0) normal *= -1;

	return true;
}


template <typename T> bool collisions::SATcircle(Vector<T>& center, T radius, Vector<T>* vertices, int size, const Vector<T>& verticesCenter, Vector<T>& normal, T& depth) {
	depth = 1000000;

	Vector<T> axis{};

	for (int i = 0; i < size; i++) {
		Vector<T> startVertex = vertices[i];
		Vector<T> endVertex = vertices[(i + 1) % (size)];

		Vector<T> edge = endVertex - startVertex;
		Vector<T> axis = Vector<T>::normalize(Vector<T>{-edge.y, edge.x});

		T min1; T min2; T max1; T max2;
		projectVertices(vertices, size, axis, min1, max1);
		projectCircle(center, radius, axis, min2, max2);

		if (min1 >= max2 || min2 >= max1) return false;

		T axisDepth = std::min(max2 - min1, max1 - min2);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}

	int closestPoint = circleClosestPolygonPoint(center, vertices, size);
	axis = Vector<T>::normalize(vertices[closestPoint] - center);

	T min1; T min2; T max1; T max2;
	projectVertices(vertices, size, axis, min1, max1);
	projectCircle(center, radius, axis, min2, max2);

	if (min1 >= max2 || min2 >= max1) return false;

	T axisDepth = std::min(max2 - min1, max1 - min2);
	if (axisDepth < depth) {
		depth = axisDepth;
		normal = axis;
	}

	Vector<T> direction = verticesCenter - center;
	if (Vector<T>::dot(direction, normal) < 0) normal *= -1;
	
	return true;
}

template <typename T> void collisions::projectVertices(Vector<T>* vertices, int size, Vector<T>& axis, T& min, T& max) {
	min = 10000;
	max = -10000;

	for (int i = 0; i < size; i++) {
		T projection = Vector<T>::dot(vertices[i], axis);
		if (projection < min) min = projection;
		if (projection > max) max = projection;
	}
}

template <typename T> void collisions::projectCircle(Vector<T>& center, T radius, Vector<T> axis, T& min, T& max) {
	Vector<T> direction = Vector<T>::normalize(axis) * radius;
	min = Vector<T>::dot(center + direction, axis);
	max = Vector<T>::dot(center - direction, axis);

	if (min >= max) {
		T t = min;
		min = max;
		max = t;
	}
}

template <typename T> int collisions::circleClosestPolygonPoint(Vector<T>& center, Vector<T>* vertices, int size) {
	int result = -1;
	T minDistance = 100000;

	for (int i = 0; i < size; i++) {
		T distance = Vector<T>::distance(vertices[i], center);
		if (distance < minDistance) {
			minDistance = distance;
			result = i;
		}
	}
	return result;
}