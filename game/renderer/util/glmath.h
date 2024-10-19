#pragma once

#include <iostream>

namespace glm {
	struct vec2 {
		float x;
		float y;
	};

	//padded 3x3 matrix for UBO
	class mat3_padded {
	private:
		float matrix[12] = {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0
		};
	public:
		mat3_padded() {}
		~mat3_padded() {
			//std::cout << "deleted :(";
		}
		void set(float x, float y, float rad, float scaleX, float scaleY);
		float* ptr();
	};

	//3x3 matrix
	class mat3 {
	private:
		float matrix[9] = {
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0
		};
	public:
		mat3() {}
		~mat3() {
			//std::cout << "deleted :(";
		}
		mat3& translate(vec2 vec);
		template<class T> mat3& translate(T x, T y);
		glm::mat3& set(float x, float y, float rad, float scaleX, float scaleY);
		void mult(float M[]);
		mat3& rotate(float rad);
		mat3& scale(float sX, float sY);
		std::unique_ptr<mat3> copy();
		void transpose_pad(float target[]);
		float* ptr();
		void print() {
			for (size_t i = 0; i < 9; i++) {
				std::cout << matrix[i] << " ";
				if ((i + 1) % 3 == 0) std::cout << "\n";
			}
		}
		float& operator[](int index) {
			return matrix[index];
		}
		float& i(int index) {
			return matrix[index];
		}
	};

	void mult(glm::mat3& m1, glm::mat3& m2, glm::mat3& dest);
}



