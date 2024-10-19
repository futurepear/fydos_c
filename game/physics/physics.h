#pragma once
#include "body.hpp"
#include "collisions.h"

//world file

template <typename T = float> class World {
private:
public:
	World() {

	}
	~World() {

	}
	void seperateBodies(Body<T>& body1, Body<T>& body2, Vector<T> mtv) {
		T totalMass = body1.mass + body2.mass;
		T massRatio = body1.mass / totalMass;

		if (body1.isStatic()) massRatio = 0;
		if (body2.isStatic()) massRatio = 1;

		body1.move(Vector<T>{0, 0});
		body1.move(mtv * (-1 * massRatio));
		body2.move(mtv * massRatio);
	}

	bool collide(Body<T>& body1, Body<T>& body2) {
		Vector<T> normal{};
		T depth{ 0 };
		bool result = false;

		
		if (body1.isCircle() && body2.isCircle()) 
			result = collisions::intersectCircles<T>(body1, body2, normal, depth);

		if (body1.isPolygon() && body2.isPolygon())
			result = collisions::SAT(body1, body2, normal, depth);

		if (body1.isPolygon() && body2.isCircle()) {
			result = collisions::SATcircle(body2.position, body2.radius, body1.getTransformedVertices(), body1.size(), body1.calculateCenter(), normal, depth);
			if (result) normal *= -1;
		}

		if (body1.isCircle() && body2.isPolygon()) {
			result = collisions::SATcircle(body1.position, body1.radius, body2.getTransformedVertices(), body2.size(), body2.calculateCenter(), normal, depth);
		}

		////
		if (result) {
			seperateBodies(body1, body2, normal * depth);
		}
		////

		return result;
	}
};