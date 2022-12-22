#pragma once

#include "stdio.h"
#include "Dot.h"

#define BCKG_COLOR 0
#define X 0
#define Y 1
#define Z 2

class mPolygon
{

public:
	Dot** v;
	int first_vertex_idx;
	int point_number;

	int color;
	float A, B, C, D;
	float Xmin, Xmax, Ymin, Ymax;

	mPolygon()
	{

	}

	mPolygon(Dot* _A, Dot* _B, Dot* _C,int _firts_vertex_idx, int _color)
	{
		point_number = 3;
		v = new Dot * [point_number];
		v[0] = _A;
		v[1] = _B;
		v[2] = _C;
		
		first_vertex_idx = _firts_vertex_idx;
		color = _color;
		update_data();
	}
	mPolygon(Dot* _A, Dot* _B, Dot* _C, Dot* _D, int _firts_vertex_idx, int _color)
	{
		point_number = 4;
		v = new Dot * [point_number];
		v[0] = _A;
		v[1] = _B;
		v[2] = _C;
		v[3] = _D;
		first_vertex_idx = _firts_vertex_idx;
		color = _color;
		update_data();
	}
	mPolygon(Dot* _A, Dot* _B, Dot* _C, Dot* _D, Dot* _E, int _firts_vertex_idx, int _color)
	{
		point_number = 5;
		v = new Dot * [point_number];
		v[0] = _A;
		v[1] = _B;
		v[2] = _C;
		v[3] = _D;
		v[4] = _E;
		first_vertex_idx = _firts_vertex_idx;
		color = _color;
		update_data();
	}

	void update_data()
	{

		int i = 0;
		float M[3] = { v[i]->x, v[i]->y, v[i]->z };
		int ai = i - 1;
		if (ai < 0)
			ai = 2;
		int bi = i + 1;
		if (bi > 2)
			bi = 0;
		float a[3] = { v[ai]->x - v[i]->x, v[ai]->y - v[i]->y, v[ai]->z - v[i]->z };
		float b[3] = { v[bi]->x - v[i]->x, v[bi]->y - v[i]->y, v[bi]->z - v[i]->z };

		A = a[Y] * b[Z] - a[Z] * b[Y];
		B = a[X] * b[Z] - a[Z] * b[X];
		B = -B;
		C = a[X] * b[Y] - a[Y] * b[X];

		D = M[X] * A + M[Y] * B + M[Z] * C;
		D = -D;
		Dot* cam = new Dot(0, 0, 0);

		float val = A * cam->x + B * cam->y + C * cam->z + D;
		if (val <= 0)
		{
			A = -A;
			B = -B;
			C = -C;
			D = -D;
		}


		Xmin = 256;
		Ymin = 256;
		Xmax = -1;
		Ymax = -1;
		for (int i = 0; i < point_number; i++)
		{
			if (v[i]->x < Xmin)
				Xmin = v[i]->x;

			if (v[i]->y < Ymin)
				Ymin = v[i]->y;

			if (v[i]->x >= Xmax)
				Xmax = v[i]->x;

			if (v[i]->y >= Ymax)
				Ymax = v[i]->y;
		}
	}

};