#pragma once

#include "../libwinbgi/src/graphics.h"
#include <stdio.h>
#include <queue>
#include <iostream>

#include "Dot.h"
#include "mPolygon.h"
#include "prisma.h"

#define SCLR 15
#define M_LENGTH 32
#define R_DEG 32


class okno
{
public:
	int Xstart, Ystart, size;

	okno() {};
	okno(float x, float y, float s) : Xstart(x), Ystart(y), size(s) {};

};

class allDots
{
public:
	Dot** dots;
	int dot_num;

	allDots()
	{
		dots = nullptr;
		dot_num = 0;
	}

	allDots(int _number, Dot** _points)
	{
		dots = _points;
		dot_num = _number;
	}

};

class allPols
{
public:
	int pol_num;
	mPolygon** planes;

	allPols()
	{
		pol_num = 0;
		planes = nullptr;
	}

	allPols(int _number, mPolygon** _planes)
	{
		planes = _planes;
		pol_num = _number;
	}

	void update_data_each()
	{
		for (int i = 0; i < pol_num; i++)			
			planes[i]->update_data();
	}

};



class Controller
{
	allDots* all_objs_dots; // все вершины в сцене
	allPols* all_objs_pols; // все многоугольники в сцене
	Prism p1;
	Prism p2;
	Dot* d1;
	Dot* d2;

	int frame_size;

	void gabarit(int _i, okno* _w, bool* _isNotVisible)
	{
		int Xleft = _w->Xstart;
		int Xright = _w->Xstart + _w->size - 1;
		int Ydown = _w->Ystart;
		int Yup = _w->Ystart + _w->size - 1;

		*_isNotVisible = true;

		if (all_objs_pols->planes[_i]->Xmin > Xright) {
			*_isNotVisible = false;
		}
		if (all_objs_pols->planes[_i]->Xmax < Xleft) {
			*_isNotVisible = false;
		}

		if (all_objs_pols->planes[_i]->Ymin > Yup) {
			*_isNotVisible = false;
		}
		if (all_objs_pols->planes[_i]->Ymax < Ydown) {
			*_isNotVisible = false;
		}

	}

	void color(okno* _w, int _color)
	{
		for (int x = _w->Xstart; x < _w->Xstart + _w->size; x++)
		{	
			setcolor(_color);
			line(x, _w->Ystart, x, _w->Ystart + _w->size);
		}
	}

	void isPointVisible(Dot* p, Dot* p1, Dot* p2, int* pointSeen)
	{
		int rab1 = (p->x - p1->x) * (p2->y - p1->y);
		int rab2 = (p->y - p1->y) * (p2->x - p1->x);
		int rab3 = rab1 - rab2;

		if (rab3 > 0)
			*pointSeen = 1;
		if (rab3 == 0)
			*pointSeen = 0;

		if (rab3 < 0)
			*pointSeen = -1;

	}

	void getZ(int i, okno* _w, int* z)
	{
		Dot* center = new Dot;

		center->x = _w->Xstart + _w->size / 2;
		center->y = _w->Ystart + _w->size / 2;

		if (all_objs_pols->planes[i]->C == 0)
		{
			for (int j = 1; j < all_objs_pols->pol_num; j++)
				*z = max(all_objs_dots->dots[j]->z, all_objs_dots->dots[j - 1]->z);
		}
		else
		{
			float A = all_objs_pols->planes[i]->A;
			float B = all_objs_pols->planes[i]->B;
			float C = all_objs_pols->planes[i]->C;
			float D = all_objs_pols->planes[i]->D;
			*z = -(A * center->x + B * center->y + D) / C;
		}
	}


	
	void pokritie(okno* _w, int* number)
	{
		int Zmax = -1000000;
		int num = 0;

		Dot* okno_center = new Dot;

		okno_center->x = _w->Xstart + _w->size / 2;
		okno_center->y = _w->Ystart + _w->size / 2;
		

		for (int i = 0; i < all_objs_pols->pol_num; i++)
		{
			int idx = all_objs_pols->planes[i]->first_vertex_idx;
			int isSeen = 0;
			

			for (int j = 0; j < all_objs_pols->planes[i]->point_number - 1; j++)
			{
				d1->x = all_objs_dots->dots[idx]->x;
				d1->y = all_objs_dots->dots[idx]->y;
				d2->x = all_objs_dots->dots[idx + 1]->x;
				d2->y = all_objs_dots->dots[idx + 1]->y;
				isPointVisible(okno_center, d1, d2, &isSeen);
				if (isSeen < 0)
					break; 
				idx++;
			}

			if (isSeen >= 0)
			{ 
				d1->x = all_objs_dots->dots[idx]->x;
				d1->y = all_objs_dots->dots[idx]->y;
				int start_idx = all_objs_pols->planes[i]->first_vertex_idx;
				d2->x = all_objs_dots->dots[start_idx]->x;
				d2->y = all_objs_dots->dots[start_idx]->y;

				isPointVisible(okno_center, d1, d2, &isSeen);

				if (isSeen >= 0)
				{
					int z = 0;
					getZ(i, _w, &z);
					if (z >= Zmax)
					{
						Zmax = z;
						*number = i;
					}
				}
			}

		}
	}

	void draw()
	{
		std::vector<okno*> stack;
		okno* frame = new okno(0, 0, frame_size);

		stack.push_back(frame);

		bool notEmpty;
		while (stack.size() != 0)
		{
			okno* poped_okno = stack.back();

			stack.pop_back();
			
			notEmpty = false;
			int idx = 0;
			while (idx < all_objs_pols->pol_num && notEmpty == false)
			{
				gabarit(idx, poped_okno, &notEmpty);
				idx++;
			}

			if (notEmpty)
			{
				if (poped_okno->size > 1)
				{
					int s = poped_okno->size / 2;
					int Xst = poped_okno->Xstart;
					int Yst = poped_okno->Ystart;

					okno* newOkno1 = new okno(Xst + s, Yst + s, s);
					okno* newOkno2 = new okno(Xst, Yst + s, s);
					okno* newOkno3 = new okno(Xst + s, Yst, s);
					okno* newOkno4 = new okno(Xst, Yst, s);

					stack.push_back(newOkno1);
					stack.push_back(newOkno2);
					stack.push_back(newOkno3);
					stack.push_back(newOkno4);
				}
				else
				{
					int num = -1;
					pokritie(poped_okno, &num);
					if (num >= 0)
						color(poped_okno, all_objs_pols->planes[num]->color);
					else
						color(poped_okno, BCKG_COLOR);					
				}
			}
			else
				color(poped_okno, BCKG_COLOR);
			
		}

	}
	
	void update_objs_and_draw()
	{
		p1.cast_shadow();
		p2.cast_shadow();
		p1.proj();
		p2.proj();

		all_objs_pols->update_data_each();
		draw();

		p1.unproj();
		p2.unproj();
	}

	void preset_obj()
	{
		p1.scale(0.5, 0.5, 0.5);
		p1.move(0, 64, 0);
		p2.rotate(10, 1);
		p2.scale(0.5, 0.5, 0.5);
		p2.move(128, 64, 0);
	}

public:

	Controller()
	{
		frame_size = 256;
		d1 = new Dot;
		d2 = new Dot;
	}

	void create_data_call_menu()
	{

		Dot* A1 = new Dot(0, 180, 100);
		Dot* B1 = new Dot(100, 180, 100);
		Dot* C1 = new Dot(100, 180, 0);
		Dot* D1 = new Dot(0, 180, 0);
		Dot* E1 = new Dot(100, 0, 100);
		Dot* F1 = new Dot(200, 0, 100);
		Dot* G1 = new Dot(200, 0, 0);
		Dot* H1 = new Dot(100, 0, 0);

		mPolygon* A1B1F1E1 = new mPolygon(A1, B1, F1, E1, 0, 9);
		mPolygon* B1C1G1F1 = new mPolygon(B1, C1, G1, F1, 4, 10);
		mPolygon* C1D1H1G1 = new mPolygon(C1, D1, H1, G1, 8, 11);
		mPolygon* D1A1E1H1 = new mPolygon(D1, A1, E1, H1, 12, 12);
		mPolygon* E1F1G1H1 = new mPolygon(E1, F1, G1, H1, 16, 13);
		mPolygon* D1C1B1A1 = new mPolygon(D1, C1, B1, A1, 20, 14);


		Dot* s_A1 = new Dot;
		Dot* s_B1 = new Dot;
		Dot* s_C1 = new Dot;
		Dot* s_D1 = new Dot;
		Dot* s_E1 = new Dot;
		Dot* s_F1 = new Dot;
		Dot* s_G1 = new Dot;
		Dot* s_H1 = new Dot;

		mPolygon* s_A1B1F1E1 = new mPolygon(s_A1, s_B1, s_F1, s_E1, 24, SCLR);
		mPolygon* s_B1C1G1F1 = new mPolygon(s_B1, s_C1, s_G1, s_F1, 28, SCLR);
		mPolygon* s_C1D1H1G1 = new mPolygon(s_C1, s_D1, s_H1, s_G1, 32, SCLR);
		mPolygon* s_D1A1E1H1 = new mPolygon(s_D1, s_A1, s_E1, s_H1, 36, SCLR);
		mPolygon* s_E1F1G1H1 = new mPolygon(s_E1, s_F1, s_G1, s_H1, 40, SCLR);
		mPolygon* s_D1C1B1A1 = new mPolygon(s_D1, s_C1, s_B1, s_A1, 44, SCLR);

		Dot* p1_points[8] = { A1, B1, C1, D1, E1, F1, G1, H1 };
		mPolygon* p1_planes[6] = { A1B1F1E1, B1C1G1F1, C1D1H1G1, D1A1E1H1, E1F1G1H1,  D1C1B1A1 };
		mPolygon* s_p1_planes[6] = { s_A1B1F1E1, s_B1C1G1F1, s_C1D1H1G1, s_D1A1E1H1, s_E1F1G1H1,  s_D1C1B1A1 };

		Dot* s_p1_points[8] = { s_A1,  s_B1,  s_C1,  s_D1,  s_E1,  s_F1,  s_G1,  s_H1 };



		Dot* A3 = new Dot(0, 100, 30);
		Dot* B3 = new Dot(30, 100, 60);
		Dot* C3 = new Dot(60, 100, 50);
		Dot* D3 = new Dot(60, 100, 30);
		Dot* E3 = new Dot(30, 100, 0);

		Dot* A4 = new Dot(0, 0, 30);
		Dot* B4 = new Dot(30, 0, 60);
		Dot* C4 = new Dot(60, 0, 50);
		Dot* D4 = new Dot(60, 0, 30);
		Dot* E4 = new Dot(30, 0, 0);

		mPolygon* A3B3C3D3E3 = new mPolygon(E3, D3, C3, B3, A3, 48, 4);
		mPolygon* A4B4C4D4E4 = new mPolygon(A4, B4, C4, D4, E4, 53, 5);
		mPolygon* A3B3B4A4 = new mPolygon  (A3, B3, B4, A4, 58, 11);
		mPolygon* B3C3C4B4 = new mPolygon(B3, C3, C4, B4, 62, 12);
		mPolygon* A3D3B4A4 = new mPolygon(C3, D3, D4, C4, 66, 13);
		mPolygon* D3E3E4D4 = new mPolygon(D3, E3, E4, D4, 70, 14);
		mPolygon* E3A3A4E4 = new mPolygon(E3, A3, A4, E4, 74, 15);


		Dot* s_A3 = new Dot();
		Dot* s_B3 = new Dot();
		Dot* s_C3 = new Dot();
		Dot* s_D3 = new Dot();
		Dot* s_E3 = new Dot();
		Dot* s_A4 = new Dot();
		Dot* s_B4 = new Dot();
		Dot* s_C4 = new Dot();
		Dot* s_D4 = new Dot();
		Dot* s_E4 = new Dot();

		mPolygon* s_A3B3C3D3E3 = new mPolygon(s_E3, s_D3, s_C3, s_B3, s_A3, 78, SCLR);
		mPolygon* s_A4B4C4D4E4 = new mPolygon(s_A4, s_B4, s_C4, s_D4, s_E4, 83, SCLR);
		mPolygon* s_A3B3B4A4 = new mPolygon(s_A3, s_B3, s_B4, s_A4, 88, SCLR);
		mPolygon* s_B3C3C4B4 = new mPolygon(s_B3, s_C3, s_C4, s_B4, 92, SCLR);
		mPolygon* s_A3D3B4A4 = new mPolygon(s_C3, s_D3, s_D4, s_C4, 96, SCLR);

		mPolygon* s_D3E3E4D4 = new mPolygon(s_D3, s_E3, s_E4, s_D4, 100, SCLR);
		mPolygon* s_E3A3A4E4 = new mPolygon(s_E3, s_A3, s_A4, s_E4, 104, SCLR);
		
		
		Dot* p2_points[10] = { A3, B3, C3, D3, E3, A4, B4, C4, D4, E4 };
		mPolygon* p2_planes[7] = { A3B3C3D3E3, A4B4C4D4E4, A3B3B4A4, B3C3C4B4, A3D3B4A4,  D3E3E4D4,E3A3A4E4 };

		Dot* s_p2_points[10] = { s_A3, s_B3, s_C3, s_D3, s_E3, s_A4, s_B4, s_C4, s_D4, s_E4 };
		mPolygon* s_p2_planes[7] = { s_A3B3C3D3E3, s_A4B4C4D4E4, s_A3B3B4A4, s_B3C3C4B4, s_A3D3B4A4, s_D3E3E4D4, s_E3A3A4E4 };

		

		Dot* points[108] = {
		A1, B1, F1, E1,		B1, C1, G1, F1,		C1, D1, H1, G1,		D1, A1, E1, H1,		E1, F1, G1, H1,		D1, C1, B1, A1,

		s_A1,  s_B1,  s_F1,  s_E1, 		s_B1,  s_C1,  s_G1,  s_F1, 		s_C1,  s_D1,  s_H1,  s_G1, 		s_D1,  s_A1,  s_E1,  s_H1, 		s_E1,  s_F1,  s_G1,  s_H1, 		s_D1,  s_C1,  s_B1,  s_A1, 

		E3, D3, C3, B3, A3, 		A4, B4, C4, D4, E4, 		A3, B3, B4, A4,  		B3, C3, C4, B4,  		C3, D3, D4, C4,  		D3, E3, E4, D4, 		E3, A3, A4, E4,  

		s_E3, s_D3, s_C3, s_B3, s_A3, 		s_A4, s_B4, s_C4, s_D4, s_E4, 		s_A3, s_B3, s_B4, s_A4,		s_B3, s_C3, s_C4, s_B4,		s_C3, s_D3, s_D4, s_C4,		s_D3, s_E3, s_E4, s_D4,		s_E3, s_A3, s_A4, s_E4 		};


		mPolygon* planes[26] = {
			A1B1F1E1, B1C1G1F1, C1D1H1G1, D1A1E1H1, E1F1G1H1,  D1C1B1A1,
			s_A1B1F1E1, s_B1C1G1F1, s_C1D1H1G1, s_D1A1E1H1, s_E1F1G1H1,  s_D1C1B1A1,
			A3B3C3D3E3, A4B4C4D4E4, A3B3B4A4, B3C3C4B4, A3D3B4A4,  D3E3E4D4, E3A3A4E4,
			s_A3B3C3D3E3, s_A4B4C4D4E4, s_A3B3B4A4, s_B3C3C4B4, s_A3D3B4A4, s_D3E3E4D4, s_E3A3A4E4
		};
		

		all_objs_dots = new allDots(108, points);
		all_objs_pols = new allPols(26, planes);
		
		p1 = *new Prism(p1_points, s_p1_points, p1_planes, s_p1_planes, 6, 8 );
		p2 = *new Prism(p2_points, s_p2_points, p2_planes, s_p2_planes, 7, 10);
		
		preset_obj();
		
		menu();
	}

	void menu()
	{
		initwindow(frame_size, frame_size);
			
		char c;
		while (true)
		{
			update_objs_and_draw();
			c = getch();
			switch (c)
			{
			case '0':
				closegraph();
				return;

			case '1':
				p1.scale(2, 2, 2);	
				break;
			case '2':
				p1.scale(0.5, 0.5, 0.5);
				break;
			case 'd':
				p1.move(M_LENGTH, 0, 0);
				break;
			case 'a':
				p1.move(-M_LENGTH, 0, 0);
				break;

			case 'w':
				p1.move(0, -M_LENGTH, 0);
				break;
			case 's':
				p1.move(0, M_LENGTH, 0);
				break;
			case 'q':
				p1.move(0, 0, M_LENGTH);
				break;
			case 'e':
				p1.move(0, 0, -M_LENGTH);
				break;

			case 'r':
				p1.rotate(R_DEG, X);
				break;
			case 'f':
				p1.rotate(-R_DEG, X);
				break;
			case 'c':
				p1.rotate(R_DEG, Y);
				break;
			case 'v':
				p1.rotate(-R_DEG, Y);
				break;
			case 'z':
				p1.rotate(R_DEG, Z);
				break;
			case 'x':
				p1.rotate(-R_DEG, Z);
				break;


			case '6':
				p2.scale(2, 2, 2);
				break;
			case '7':
				p2.scale(0.5, 0.5, 0.5);
				break;
			case 'k':
				p2.move(M_LENGTH, 0, 0);
				break;
			case 'h':
				p2.move(-M_LENGTH, 0, 0);
				break;
			case 'u':
				p2.move(0, -M_LENGTH, 0);
				break;
			case 'j':
				p2.move(0, M_LENGTH, 0);
				break;
			case 'y':
				p2.move(0, 0, M_LENGTH);
				break;
			case 'i':
				p2.move(0, 0, -M_LENGTH);
				break;
			case 't':
				p2.rotate(R_DEG, X);
				break;
			case 'g':
				p2.rotate(-R_DEG, X);
				break;
			case 'b':
				p2.rotate(R_DEG, Y);
				break;
			case 'n':
				p2.rotate(-R_DEG, Y);
				break;
			case 'o':
				p2.rotate(R_DEG, Z);
				break;
			case 'l':
				p2.rotate(-R_DEG, Z);
				break;

			default:
				break;
			}
		}	
		closegraph();
	}
};