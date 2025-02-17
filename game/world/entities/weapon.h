#pragma once

#include <unordered_map>
#include <utility>
#include "../chunkManager.h"
#include "../../systems/dictionary.h"
#include "../../systems/animation.hpp"
#include "../../physics/physics.h"

template <typename T = float> class Usable {
private:
	AnimationDiscrete<T> m_x{ {0,0}, {0, 1}, {0, 2} };
	AnimationDiscrete<T> m_y{ {0,0}, {0, 1}, {0, 2} };
	AnimationDiscrete<T> m_angle{ {0,0}, {0, 1}, {0, 2} };
	Vector<T> pivot;
	float m_pivotAngle{ 0 };
protected:
	int cooldown{ 2 };
	int m_tick = 0;
public:
	Usable(Vector<T>& center) : pivot{ center } {

	}
	void updatePivot(Vector<T>& center) {
		pivot = center;
	}
	bool canUse() {
		return (m_tick == 0);
	}
	int frame() {
		return cooldown - m_tick;
	}
	virtual void update(const Vector<float>& mouse, ChunkManager& map) {
		if (m_tick > 0) m_tick--;

		m_x.update();
		m_y.update();
		m_angle.update();
	}
	void setFacing(float a) {
		m_pivotAngle = a;
	}
	virtual bool use() {
		if (!canUse()) return false;

		m_tick = cooldown;

		m_x.start();
		m_y.start();
		m_angle.start();
		return true;
	}
	T x() {
		return m_x.value() * std::cos(angle()) + pivot.x;
	}
	T y() {
		return m_y.value() * std::sin(angle()) + pivot.y;
	}
	Vector<T> position() {
		Vector<T> pos{ m_x.value(), m_y.value() };

		pos = pos.transform({ 0, 0 }, angle());

		pos.x += pivot.x;
		pos.y += pivot.y;

		return pos;
	}
	T angle() {
		return -m_angle.value() + m_pivotAngle + 3.14f / 2.0f;
	}

	void setAnimation(const char* property, const AnimationDiscrete<T>& animation) {
		AnimationDiscrete<T>* target = nullptr;

		if (property == "x")
			target = &m_x;
		if (property == "y")
			target = &m_y;
		if (property == "angle")
			target = &m_angle;

		if (target == nullptr) return;
		*target = animation;
		cooldown = animation.length();
	}
};

template <typename T = float> class Hand : public Usable<T> {
private:
public:
	Hand(Vector<T>& center) : Usable<T>{ center } {

	}
	bool use() override {
		if (!Usable<T>::use()) return false;
		return true;
	}
};

template <typename T = float> class Melee : public Usable<T> {
protected:
	std::unordered_map<const char*, bool> hitList{};
	int damage;
public:
	Melee(Vector<T>& center, int dmg) : Usable<T>{ center }, damage{ dmg } {

	}
	void update(const Vector<float>& mouse, ChunkManager& map) override {
		Usable<T>::update(mouse, map);
	}
	bool checkCollision() {

	}

};

template <typename T = float> class Pickaxe : public Melee<T> {
private:
public:
	Pickaxe(Vector<T>& center, int dmg) : Melee<T>{ center, dmg } {

	}
	bool use() override {
		if (!Usable<T>::use()) return false;
		return true;
	}
	void update(const Vector<float>& mouse, ChunkManager& map) override {
		Melee<T>::update(mouse, map);
		
		Chunk& chunk = map.vectorToChunk(mouse);
		int loc = chunk.positionToLocation(mouse.x, mouse.y);

		if (this->frame() == this->cooldown / 3 && loc != -1) {
			chunk[2][loc]->damage(this->damage);
		}
	}
};

/**/

Pickaxe<float>* pickaxeFactory(int cooldown, int damage, Vector<float>& center);

Usable<float>* weaponFactory(int weaponID, Vector<float>& center);