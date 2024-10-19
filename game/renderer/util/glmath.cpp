#include "glmath.h"
#include <iostream>
#include <cmath>
#include <memory>


void glm::mat3_padded::set(float x, float y, float rad, float scaleX, float scaleY) {
	float c = cos(rad);
	float s = sin(rad);
	matrix[0] = scaleX * c;
	matrix[4] = scaleX * s;
	matrix[8] = x;
	matrix[1] = -scaleY * s;
	matrix[5] = scaleY * c;
	matrix[9] = y;
}

float* glm::mat3_padded::ptr() {
	return matrix;
}

glm::mat3& glm::mat3::translate(glm::vec2 vec) {
	matrix[2] += vec.x;
	matrix[5] += vec.y;
	return *this;
}

template<class T> glm::mat3& glm::mat3::translate(T x, T y) {
	matrix[2] += x;
	matrix[5] += y;
	return *this;
}

glm::mat3& glm::mat3::set(float x, float y, float rad, float scaleX, float scaleY) {
	float c = cos(rad);
	float s = sin(rad);
	matrix[0] = scaleX * c;
	matrix[1] = scaleY * s;
	matrix[2] = x;
	matrix[3] = -scaleX * s;
	matrix[4] = scaleY * c;
	matrix[5] = y;
	return *this;
}

void glm::mat3::mult(float M[]) {
	float temp[9] = { matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8] };

	matrix[0] = temp[0] * M[0] + temp[1] * M[3] + temp[2] * M[6];
	matrix[1] = temp[0] * M[1] + temp[1] * M[4] + temp[2] * M[7];
	matrix[2] = temp[0] * M[2] + temp[1] * M[5] + temp[2] * M[8];
	matrix[3] = temp[3] * M[0] + temp[4] * M[3] + temp[5] * M[6];
	matrix[4] = temp[3] * M[1] + temp[4] * M[4] + temp[5] * M[7];
	matrix[5] = temp[3] * M[2] + temp[4] * M[5] + temp[5] * M[8];
	matrix[6] = temp[6] * M[0] + temp[7] * M[3] + temp[8] * M[6];
	matrix[7] = temp[6] * M[1] + temp[7] * M[4] + temp[8] * M[7];
	matrix[8] = temp[6] * M[2] + temp[7] * M[5] + temp[8] * M[8];
}

float* glm::mat3::ptr() {
	return matrix;
}

void glm::mat3::transpose_pad(float target[]) {
	target[0] = matrix[0];
	target[1] = matrix[3];
	target[2] = matrix[6];
	target[4] = matrix[1];
	target[5] = matrix[4];
	target[6] = matrix[7];
	target[8] = matrix[2];
	target[9] = matrix[5];
	target[10] = matrix[8];
}

std::unique_ptr<glm::mat3> glm::mat3::copy() {
	std::unique_ptr<mat3> newMat3{ new glm::mat3{} };
	for (int i = 0; i < 9; i++) {
		newMat3->i(i) = matrix[i];
	}
	newMat3->print();
	return newMat3;
}

namespace glm {
	void mult(mat3& m1, mat3& m2, mat3& dest) {
		dest[0] = m1[0] * m2[0] + m1[1] * m2[3] + m1[2] * m2[6];
		dest[1] = m1[0] * m2[1] + m1[1] * m2[4] + m1[2] * m2[7];
		dest[2] = m1[0] * m2[2] + m1[1] * m2[5] + m1[2] * m2[8];
		dest[3] = m1[3] * m2[0] + m1[4] * m2[3] + m1[5] * m2[6];
		dest[4] = m1[3] * m2[1] + m1[4] * m2[4] + m1[5] * m2[7];
		dest[5] = m1[3] * m2[2] + m1[4] * m2[5] + m1[5] * m2[8];
		dest[6] = m1[6] * m2[0] + m1[7] * m2[3] + m1[8] * m2[6];
		dest[7] = m1[6] * m2[1] + m1[7] * m2[4] + m1[8] * m2[7];
		dest[8] = m1[6] * m2[2] + m1[7] * m2[5] + m1[8] * m2[8];

		// for(size_t i = 0; i < 9; i++){
		//     std::cout << dest[i] << " ";  
		//     if((i+1) % 3 == 0) std::cout << "\n";
		// }
	}

}

glm::mat3& glm::mat3::rotate(float rad) {
	float c = cos(rad);
	float s = sin(rad);
	float R[9] = {
		c, -s, 0,
		s, c, 0,
		0, 0, 1
	};
	mult(R);
	return *this;
}

glm::mat3& glm::mat3::scale(float sX, float sY) {
	float R[9] = {
		sX, 0, 0,
		0, sY, 0,
		0, 0, 1
	};
	mult(R);
	return *this;
}

//
//int main() {
//    mat3 bruh{};
//    bruh.translate(2, 3);//.rotate(1.57079).scale(2, 2);
//    bruh[0] = 3;
//
//
//    mat3 bruh2{};
//
//    mat3 final{};
//    //mult(bruh, bruh2, final);
//
//    std::cout << "\n";
//
//    bruh.copy()->print();
//
//    //mult(bruh.copy(), bruh2, bruh);
//
//
//}