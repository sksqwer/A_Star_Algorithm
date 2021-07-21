#pragma once
#ifndef _ASTAR_H_
#define _ASTAR_H_

#include "framework.h"


bool operator!=(const POINT a, const POINT b);
bool operator==(const POINT a, const POINT b);


class Astar_Node
{
public:
	int row = 0;
	int col = 0;
	int g = 0;
	int h = -1;
	int f = -1;
	POINT past = { 0,0 };
};

struct compare {
	bool operator()(const Astar_Node& a, const Astar_Node& b)
	{
		return a.f > b.f;
	}
};

class cha
{
public:
	HBITMAP Hbcha;
	BITMAP bcha;
	int bx = 48, by = 64;
	int RUN_FRAME_MAX = 6, RUN_FRAME_MIN = 0, curframex = 0;
	void CreateBitmap();
	void DrawBitmap(HWND hWnd, HDC hdc, int sx, int sy, int curframey);
};

class Astar
{
private:
	enum {MAX = 987654321};
	
	int row, col;
	int srow = 0, scol = 0;
	int drow = 0, dcol = 0;
	int minh = MAX, mini, minj;
	int **map;
	Astar_Node **Astarmap;
//	character c;
public:
	const int width = 55;
	Astar(int a, int b);
	void shutdown();
	void reset_Astarmap();
	void make_map();
	void Show_map(HWND hWnd, HDC hdc);
	bool set_map(int i, int j, int n);
	bool Astar_algorithm(int a, int b);
	bool Astar_algorithm() { return Astar_algorithm(srow, scol); }
	void set_path(bool ch);
	int set_h(int i, int j);


};


#endif //!_ASTAR_H_