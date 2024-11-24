#pragma once
#include <cmath>
#include <algorithm>
#include <vector>

template <typename T = float> struct Keyframe {
	T value;
	float time;
	int interpolation = 1;
};

template <typename T = float> struct KeyframeDiscrete {
	T value;
	int tick;
	int interpolation = 1;
};

//continuous animation - NOT deterministic 
template <typename T = float> class Animation {
private:
	float m_start = 0;
	bool m_loop = false;
	std::vector<Keyframe<T>> keyframes{};
	float m_end;
public:
	Animation(std::initializer_list<Keyframe<T>> k) {
		keyframes.insert(keyframes.end(), k.begin(), k.end());
		m_end = keyframes[keyframes.size() - 1].time;
	}
	void start(float time) {
		m_start = time;
	}
	T value(float now) {
		
		float nowC = std::fmod(now - m_start, m_end);

		// O(n) searching for current keyframe
		int currentFrame = 0;
		for (int i = 0; i < keyframes.size() - 1; i++) {
			if (keyframes[i].time <= nowC && nowC < keyframes[i + 1].time) {
				currentFrame = i; 
				break;
			}
		}

		Keyframe<>& a = keyframes[currentFrame];
		Keyframe<>& b = keyframes[currentFrame + 1];
		float f = (nowC - a.time) / (b.time - a.time);

		switch (b.interpolation) {
		case 0:
			return Animation::constantInterpolation(a.value, b.value, f);
		case 1:
			return Animation::linearInterpolation(a.value, b.value, f);
		case 3:
			return Animation::cubicInterpolation(a.value, b.value, f);
		default:
			return Animation::linearInterpolation(a.value, b.value, f);
		}
	}
	static T constantInterpolation(T a, T b, float t = 0) {
		return a;
	}
	static T linearInterpolation(T a, T b, float t) {
		return a * (1 - t) + b * t;
	}
	static T cubicInterpolation(T a, T b, float t) {		
		return (b-a) * t * t * (3.0f - 2.0f * t) + a;
	}
};


//discrete steps in ticks - useful for deterministic calculations
template <typename T = float> class AnimationDiscrete {
private:
	int m_tick = 0;
	bool m_loop = false;
	std::vector<KeyframeDiscrete<T>> keyframes{};
	int m_currentFrame = 0;
	int m_end;
public:
	AnimationDiscrete(std::initializer_list<KeyframeDiscrete<T>> k) {
		keyframes.insert(keyframes.end(), k.begin(), k.end());
		m_end = keyframes[keyframes.size() - 1].tick;
		m_tick = m_end;
	}
	void start(int time = 0) {
		m_tick = time;
	}
	int length() const {
		return m_end;
	}
	void update() {
		if (!m_loop && m_tick >= m_end) {
			return;
		}

		m_tick++;
		int tickc = m_tick % m_end;

		int &i = m_currentFrame; //ease of access
		while (true) {
			if (keyframes[i].tick <= tickc && tickc < keyframes[i + 1].tick) {
				break;
			} else {
				i = (i + 1) % (keyframes.size() - 1);
			}
		}
	}
	T value() {
		KeyframeDiscrete<T>& a = keyframes[m_currentFrame]; //current frame
		if (keyframes.size() == 1)
			return a.value;

		KeyframeDiscrete<T> &b = keyframes[(m_currentFrame+1)]; //next frame

		float f = ((m_tick % m_end) - a.tick) / float(b.tick - a.tick);

		switch (b.interpolation) {
		case 0:
			return Animation<T>::constantInterpolation(a.value, b.value, f);
		case 1:
			return Animation<T>::linearInterpolation(a.value, b.value, f);
		case 3:
			return Animation<T>::cubicInterpolation(a.value, b.value, f);
		default:
			return Animation<T>::linearInterpolation(a.value, b.value, f);
		}
	}
	const int tick() const {
		return m_tick;
	}
};