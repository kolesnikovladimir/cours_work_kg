#pragma once
#include "stdio.h"
#include "math.h"
#define X 0
#define Y 1
#define Z 2
#define W 3

class Dot
{
	float* multiply(float _m[4][4])
	{
		float x_ = x;
		float y_ = y;
		float z_ = z;
		float w_ = w;
		float cpy[4] = { x_, y_, z_, w_ };
		float res[4] = { 0,0,0,0 };

		res[X] = cpy[0] * _m[0][0] + cpy[1] * _m[1][0] + cpy[2] * _m[2][0] + cpy[3] * _m[3][0];
		res[Y] = cpy[0] * _m[0][1] + cpy[1] * _m[1][1] + cpy[2] * _m[2][1] + cpy[3] * _m[3][1];
		res[Z] = cpy[0] * _m[0][2] + cpy[1] * _m[1][2] + cpy[2] * _m[2][2] + cpy[3] * _m[3][2];
		res[W] = cpy[0] * _m[0][3] + cpy[1] * _m[1][3] + cpy[2] * _m[2][3] + cpy[3] * _m[3][3];

		x = res[X];
		y = res[Y];
		z = res[Z];
		w = res[W];
		return res;
	}

	void rotateX(float _phi) {
		_phi = _phi * 3.1415 / 180;

		float matrix[4][4] = {
			1, 0, 0, 0,
			0, cos(_phi), -sin(_phi), 0,
			0, sin(_phi), cos(_phi),
			0, 0, 0, 0, 1 };
		multiply(matrix);
	}

	void rotateY(float _phi) {
		_phi = _phi * 3.1415 / 180;

		float matrix[4][4] = {
			cos(_phi), 0, -sin(_phi), 0,
			0, 1, 0, 0,
			sin(_phi), 0, cos(_phi), 0,
			0, 0, 0, 1 };
		multiply(matrix);
	}

	void rotateZ(float _phi) {
		_phi = _phi * 3.1415 / 180;

		float matrix[4][4] = {
			cos(_phi), -sin(_phi), 0, 0,
			sin(_phi), cos(_phi), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
		multiply(matrix);
	}

	float sx, sy, sz, sw;
public:

	float x, y, z, w;
	Dot(int _x, int _y, int _z) : x(_x), y(_y), z(_z)
	{
		w = 1;
	}
	Dot()
	{
		x = 0; y = 0; z = 0; w = 1;
	}


	void move(float _x, float _y, float _z)
	{
		float matrix[4][4] = {
			1,  0,  0,  0,
			0,  1,  0,  0,
			0,  0,  1,  0,
			_x, _y, _z, 1 };

		multiply(matrix);
	}

	void scale(float _x, float _y, float _z) {
		float matrix[4][4] = {
			_x, 0, 0, 0,
			0, _y, 0, 0,
			0, 0, _z, 0,
			0, 0, 0, 1 };

		multiply(matrix);
	}


	void rotate(float _phi, int _axis) {
		if (_axis == X)
			rotateX(_phi);
		if (_axis == Y)
			rotateY(_phi);
		if (_axis == Z)
			rotateZ(_phi);
	}

	void print()
	{
		printf("%f %f %f\n", x, y, z);

	}
};