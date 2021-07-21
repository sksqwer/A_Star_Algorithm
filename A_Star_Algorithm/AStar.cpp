#include "AStar.h"

bool operator!=(const POINT a, const POINT b)
{
	if (a.x == b.x && a.y == b.y)
		return false;
	else
		return true;
}

bool operator==(const POINT a, const POINT b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	else
		return false;
}


Astar::Astar(int a, int b)
{
	row = a;
	col = b;
	map = new int*[a];
	for (int i = 0; i < a; i++)
		map[i] = new int[b];

	Astarmap = new Astar_Node*[a];
	for (int i = 0; i < a; i++)
		Astarmap[i] = new Astar_Node[b];


	for (int i = 0; i < a; i++)
		for (int j = 0; j < b; j++)
		{
			Astarmap[i][j].row = i;
			Astarmap[i][j].col = j;
		}

	make_map();
//	c.CreateBitmap();
}

void Astar::shutdown()
{
	for (int i = 0; i < row; i++)
		delete[] map[i];
	delete[] map;

	for (int i = 0; i < row; i++)
		delete[] Astarmap[i];
	delete[] Astarmap;

}

void Astar::reset_Astarmap()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			Astarmap[i][j].g = 0;
			Astarmap[i][j].h = -1;
			Astarmap[i][j].f = -1;
			Astarmap[i][j].past = { 0, 0 };			
			if (map[i][j] != 1 && map[i][j] != 0)
				map[i][j] = 1;
		}
	srow = 0; scol = 0;
	drow = 0; dcol = 0;
	minh = MAX;
	mini = -1; minj = -1;


}

void Astar::make_map()
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			if (rand() % 10 < 2)
				map[i][j] = 0;
			else
				map[i][j] = 1;
		}
}

void Astar::Show_map(HWND hWnd, HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap, hOldBitmap;
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(255, 0, 0);
	lb.lbHatch = 0;
	unsigned int *p = new unsigned int[width * 21 * width * 21];
	for (int i = 0; i < width * 21 * width * 21; i++) *(p + i) = 0xFFFFFFFF;
	//hBitmap = CreateCompatibleBitmap(hdc, width * 21, width * 21);
	hBitmap = CreateBitmap(width * 21, width * 21, 1, 32, p);
	delete[] p;
	hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);


	// LINE 
	HPEN hPen, oldPen;
	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(memDC, hPen);
	for (int i = 0; i <= 20; i++)
	{
		MoveToEx(memDC, 0, i * width, NULL);
		LineTo(memDC, 20 * width, i * width);
	}
	for (int i = 0; i <= 20; i++)
	{
		MoveToEx(memDC, i * width, 0, NULL);
		LineTo(memDC, i * width, 20 * width);
	}

	//BLOCK
	HBRUSH hBrush, oldBrush;
	HBRUSH blueBrush = CreateSolidBrush(RGB(160, 180, 255));
	HBRUSH greenBrush = CreateSolidBrush(RGB(196, 255, 14));
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH purpleBrush = CreateSolidBrush(RGB(100, 0, 150));
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (map[i][j] == 0)
			{
				Rectangle(memDC, i * width, j * width, (i + 1) * width, (j + 1)* width);
			}
			else if (map[i][j] == 3 || map[i][j] == 4)
			{
				SelectObject(memDC, blueBrush);
				Rectangle(memDC, i * width, j * width, (i + 1) * width, (j + 1)* width);
				SelectObject(memDC, hBrush);
			}
			else if (map[i][j] == 5)
			{
				SelectObject(memDC, greenBrush);
				Rectangle(memDC, i * width, j * width, (i + 1) * width, (j + 1)* width);
				SelectObject(memDC, hBrush);
			}
			else if (map[i][j] == 6)
			{
				SelectObject(memDC, redBrush);
				Rectangle(memDC, i * width, j * width, (i + 1) * width, (j + 1)* width);
				SelectObject(memDC, hBrush);
			}
			else if (map[i][j] == 7)
			{
				SelectObject(memDC, purpleBrush);
				Rectangle(memDC, i * width, j * width, (i + 1) * width, (j + 1)* width);
				SelectObject(memDC, hBrush);
			}
		}
	}

	//text print
	HFONT hFont, oldFont;
	hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
	RECT rt;
	int Textwidth = 4;
	for (int i = 0; i < 20; i++)
	{
		rt = { width * i + Textwidth, Textwidth, width * (i + 1) - Textwidth, width - Textwidth };
		for (int j = 0; j < 20; j++)
		{
			
			if (map[i][j] == 3)
			{
				oldFont = (HFONT)SelectObject(memDC, hFont);
				SetBkMode(memDC, TRANSPARENT);
				DrawText(memDC, _T("S"), _tcslen(_T("S")), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				SetBkMode(memDC, OPAQUE);
				SelectObject(memDC, oldFont);
			}
			else if (map[i][j] == 4)
			{
				oldFont = (HFONT)SelectObject(memDC, hFont);
				SetBkMode(memDC, TRANSPARENT);
				DrawText(memDC, _T("D"), _tcslen(_T("D")), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				SetBkMode(memDC, OPAQUE);
				SelectObject(memDC, oldFont);

			}
			else if (map[i][j] != 0)
			{
				TCHAR strg[10];
				_stprintf_s(strg, 10, _T("%d"), Astarmap[i][j].g);
				TCHAR strh[10];
				_stprintf_s(strh, 10, _T("%d"), Astarmap[i][j].h);
				TCHAR strf[10];
				_stprintf_s(strf, 10, _T("%d"), Astarmap[i][j].f);

				DrawText(memDC, strg, _tcslen(strg), &rt, DT_LEFT | DT_TOP | DT_SINGLELINE);
				DrawText(memDC, strh, _tcslen(strh), &rt, DT_RIGHT | DT_TOP | DT_SINGLELINE);
				DrawText(memDC, strf, _tcslen(strf), &rt, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
			}
			rt.top += width;
			rt.bottom += width;
		}
	}

	

	//
	BitBlt(hdc, 0, 0, 21 * width, 21 * width, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, hOldBitmap);
	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush);
	DeleteObject(hBrush);
	DeleteObject(blueBrush);
	DeleteObject(greenBrush);
	DeleteObject(redBrush);
	DeleteObject(purpleBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);
	DeleteObject(hPen);
	DeleteObject(hBitmap);
	DeleteDC(memDC);
}

bool Astar::set_map(int i, int j, int n)
{
	if (map[i][j] != 0 && n != 0)
	{
		if (n == 3)
		{
			srow = i;
			scol = j;
			map[i][j] = n;
		}
		else if (n == 4)
		{
			if (map[i][j] == 3) return false;
			drow = i;
			dcol = j;
			map[i][j] = n;
		}
		return true;
	}
	else
	{
		if (n == 0)
		{
			if (map[i][j] == 0)
				map[i][j] = 1;
			else
			{
				Astarmap[i][j].g = 0;
				Astarmap[i][j].h = -1;
				Astarmap[i][j].f = -1;
				Astarmap[i][j].past = { 0, 0 };
				map[i][j] = 0;
			}
		}
		else
			return false;

	}
}

bool Astar::Astar_algorithm(int a, int b)
{
	static std::priority_queue<Astar_Node, std::vector<Astar_Node>, compare> pq;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (a + i < 0 || a + i >= row || b + j < 0 || b + j >= col ||
				((i == 0) && (j == 0)) || map[a+i][b+j] == 0 || map[a + i][b + j] == 6 ||
				((a + i == srow) && (b+j == scol)))
				continue;

			if (a + i == drow && b + j == dcol)
			{
				Astarmap[a + i][b + j].past = { a, b };
				while (!pq.empty())
					pq.pop();
				return true;
			}
			int temph = set_h(a + i, b + j);
			int tempg = (abs(i) + abs(j) > 1) ? 14 + Astarmap[a][b].g : 10 + Astarmap[a][b].g;
			int tempf = temph + tempg;
			if(map[a+i][b+j] == 1)
				map[a + i][b + j] = 5;
			if(temph < minh)
			{
				minh = temph;
				mini = a + i;
				minj = b + j;
			}
			if (Astarmap[a + i][b + j].f == -1 || Astarmap[a + i][b + j].f > tempf)
			{
				Astarmap[a + i][b + j].h = temph;
				Astarmap[a + i][b + j].g = tempg;
				Astarmap[a + i][b + j].f = tempf;
				Astarmap[a + i][b + j].past = { a, b };
				pq.push(Astarmap[a + i][b + j]);
			}

		}
	}

	while(!pq.empty())
	{
		int mina = pq.top().row;
		int minb = pq.top().col;
		
		map[mina][minb] = 6;

		pq.pop();
		if (Astar_algorithm(mina, minb))
		{
			while (!pq.empty())
				pq.pop();
			return true;
		}
	}

	return false;

}


void Astar::set_path(bool ch)
{
	if (ch)
	{

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				if (map[i][j] == 6) map[i][j] = 5;
		POINT temp = Astarmap[drow][dcol].past;
		POINT Start = { srow, scol };
		while (1)
		{
			if (temp == Start) break;

			if (Astarmap[temp.x][temp.y].past.x == 0 && Astarmap[temp.x][temp.y].past.y == 0)
			{
				break;
			}

			map[temp.x][temp.y] = 6;
			temp = Astarmap[temp.x][temp.y].past;


		}
	}
	else
	{
		POINT temp = { mini, minj };
		POINT Start = { srow, scol };
		while (1)
		{
			if (temp == Start) break;
			if (minh == MAX) break;
			map[temp.x][temp.y] = 7;
			temp = Astarmap[temp.x][temp.y].past;


		}
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
			{
				if (map[i][j] == 6)
				{
					Astarmap[i][j].f = -1;
					Astarmap[i][j].g = 0;
					Astarmap[i][j].h = -1;
					map[i][j] = 1;

				}
			}
	}
}

int Astar::set_h(int i, int j)
{
	int res1 = abs(drow - i) * 10 + abs(dcol - j) * 4;
	int res2 = abs(drow - i) * 4 + abs(dcol - j) * 10;

	if (res1 < res2)
		return res2;
	else
		return res1;
}

void cha::CreateBitmap()
{
	Hbcha = (HBITMAP)LoadImage(NULL, TEXT("Bazzi.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(Hbcha, sizeof(BITMAP), &bcha);
}

void cha::DrawBitmap(HWND hWnd, HDC hdc, int sx, int sy, int curframey)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, Hbcha);

	TransparentBlt(hdc, sx, sy, bx, by, hMemDC,
		curframex * bx, curframey * by,
		bx, by, RGB(255, 0, 255));
	//TransparentBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

	curframex++;
	if (curframex % RUN_FRAME_MAX == 0)
		curframex = 0;

	SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hMemDC);
}
