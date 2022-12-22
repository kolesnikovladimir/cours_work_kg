#pragma once
#include <stdio.h>
#include "Dot.h"
#include "mPolygon.h"

class Prism
{
	Dot* center;

	void upd_mid()
	{
		center->x = 0;
		for (int i = 0; i < dot_num; i++)
			center->x += dots[i]->x;

		center->x /= (float)dot_num;

		center->y = 0;
		for (int i = 0; i < dot_num; i++)
			center->y += dots[i]->y;

		center->y /= (float)dot_num;

		center->z = 0;
		for (int i = 0; i < dot_num; i++)
			center->z += dots[i]->z;

		center->z /= (float)dot_num;

		center->w = 1;
	}


public:
	int side_num;
	int dot_num;
	Dot** dots;
	mPolygon** sides;

	Dot** dots2;
	mPolygon** sh_sides;

	Prism()
	{
	}

	Prism(Dot** _v, Dot** _sh_v, mPolygon** _s, mPolygon** _sh_s, int _side_num, int _vertex_num)
	{
		dots = _v;
		sides = _s;

		dots2 = _sh_v;
		sh_sides = _sh_s;
		center = new Dot;
		side_num = _side_num;
		dot_num = _vertex_num;
	}


	void move(double _x, double _y, double _z)
	{
		for (int i = 0; i < dot_num; i++)
			dots[i]->move(_x, _y, _z);
	}

	void scale(double _x, double _y, double _z)
	{
		upd_mid();
		move(-center->x, -center->y, -center->z);
		for (int i = 0; i < dot_num; i++)
			dots[i]->scale(_x, _y, _z);
		move(center->x, center->y, center->z);
	}

	void rotate(double _phi, int axis)
	{
		upd_mid();
		move(-center->x, -center->y, -center->z);
		for (int i = 0; i < dot_num; i++)
			dots[i]->rotate( _phi,  axis);
		move(center->x, center->y, center->z);
	}
	void cast_shadow()
	{
		for (int i = 0; i < dot_num; i++)
		{
			dots2[i]->x = dots[i]->x+16;
			dots2[i]->y = 200;
			dots2[i]->z = dots[i]->z+16;
			dots2[i]->rotate(30, X);
		}
	}

	void proj()
	{
		for (int i = 0; i < dot_num; i++)
			dots[i]->rotate(30, X);	
	}

	void unproj()
	{
		for (int i = 0; i < dot_num; i++)
			dots[i]->rotate(-30, X);		
	}
};