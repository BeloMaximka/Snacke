#pragma once
#include "includes.h"
bool WindowMaximized(drawtools& DrawTools) {
	WINDOWPLACEMENT WinPlacement;
	GetWindowPlacement(DrawTools.Console.cHWND, &WinPlacement);
	if (DrawTools.WindowState == WINDOW_STATE_MINIMIZED && WinPlacement.showCmd == WINDOW_STATE_MAXIMIZED)
	{
		DrawTools.WindowState = WINDOW_STATE_MAXIMIZED;
		return true;
	}
	else
	{
		DrawTools.WindowState = WinPlacement.showCmd;
		return false;
	}
}
void RotateVector(dpos& Point, double Angle) {
	Angle *= (M_PI / 180);
	dpos NewPoint;
	NewPoint.x = Point.x * cos(Angle) - Point.y * sin(Angle);
	NewPoint.y = Point.x * sin(Angle) + Point.y * cos(Angle);
	Point = NewPoint;
}
void DrawStar(drawtools& DrawTools, dpos Pos, double SizeMod) {
	Pos = { Pos.x * DrawTools.TileSize, Pos.y * DrawTools.TileSize };
	int TileSize = DrawTools.TileSize;
	dpos StarOutEndDir = { 0,-1 };
	dpos StarInEndDir = { 0,1 };
	POINT PolyPoints[10];
	LONG PosX;
	LONG PosY;	
	double Angle =72;
	PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod;
	PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod;
	PolyPoints[0] = { PosX, PosY };	
	RotateVector(StarInEndDir, -2 * Angle);	
	PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod;
	PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod;
	PolyPoints[1] = { PosX, PosY };	
	RotateVector(StarOutEndDir, Angle);	
	for (int i = 2; i < 10; i++)
	{
		PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod;
		PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod;
		PolyPoints[i] = { PosX, PosY };		
		RotateVector(StarInEndDir, Angle);		
		i++;
		PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod;
		PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod;
		PolyPoints[i] = { PosX, PosY};		
		RotateVector(StarOutEndDir, Angle);		
	}
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKYELLOW]);
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKYELLOW]);
	Polygon(DrawTools.Console.cHDC, PolyPoints, 10);
}
void DrawApple(drawtools& DrawTools, dpos Pos) {
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	int& TileSize = DrawTools.TileSize;	
	int SizeMod = TileSize / 16;
	// �������� ���������� ���� ��� �������
	SelectObject(cHDC, Pens[GCLR_DARKRED]);
	// �������� ���������� ���� ��� ����������
	SelectObject(cHDC, Brushes[GCLR_DARKRED]);
	// ������ �������
	Rectangle(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16, Pos.y * TileSize + TileSize / 16 + SizeMod*2, Pos.x * TileSize + TileSize / 2 + TileSize / 16, Pos.y * TileSize + TileSize - SizeMod);
	// �������� ������� ���� ��� �������
	//SelectObject(cHDC, Pens[GCLR_RED]);
	// �������� ������� ���� ��� ����������
	//SelectObject(cHDC, Brushes[GCLR_RED]);
	// ������ ����� ������
	Ellipse(cHDC, Pos.x * TileSize + TileSize / 16 + SizeMod, Pos.y * TileSize + TileSize * 0.2 + SizeMod, Pos.x * TileSize + TileSize / 1.6 - SizeMod, Pos.y * TileSize + TileSize - SizeMod);
	// ������ ������ ������
	Ellipse(cHDC, Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod, Pos.y * TileSize + TileSize * 0.2 + SizeMod, Pos.x * TileSize + TileSize - TileSize / 1.6 + SizeMod, Pos.y * TileSize + TileSize - SizeMod);
	// �������� ������� ���� ��� �������
	//SelectObject(cHDC, Pens[GCLR_GREEN]);
	// �������� ������� ���� ��� ����������
	//SelectObject(cHDC, Brushes[GCLR_GREEN]);
	// ������ ������
	Ellipse(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16 + SizeMod/2, Pos.y * TileSize  + SizeMod*2, Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod*2, Pos.y * TileSize + TileSize / 6 + SizeMod);
	// �������� ������� ���� ��� �������
	//SelectObject(cHDC, Pens[GCLR_LIGHTRED]);
	// �������� ������� ���� ��� ����������
	//SelectObject(cHDC, Brushes[GCLR_LIGHTRED]);
	// ������ ����
	Ellipse(cHDC, Pos.x * TileSize + TileSize / 2.5, Pos.y * TileSize + TileSize / 1.8, Pos.x * TileSize + TileSize / 2.5 - TileSize / 6, Pos.y * TileSize + TileSize / 2 - TileSize / 5);
}
void DrawTile(drawtools& DrawTools, pos Pos, int TileID, bool NoApples = false) {
	Pos.y += INFO_BAR_SIZE;
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	int& TileSize = DrawTools.TileSize;
	if (TileID == TILE_WALL)
	{
		SelectObject(cHDC, Pens[GCLR_DARKBURLYWOOD]);
		SelectObject(cHDC, Brushes[GCLR_DARKBURLYWOOD]);
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize);
	}
	else if (TileID == TILE_FOOD && !NoApples)
	{
		HDC& cHDC = DrawTools.Console.cHDC;
		std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
		std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
		int& TileSize = DrawTools.TileSize;
		DrawTile(DrawTools, { Pos.x, Pos.y- INFO_BAR_SIZE }, TILE_EMPTY);
		// �������� ���������� ���� ��� �������
		SelectObject(cHDC, Pens[GCLR_BROWN]);
		// �������� ���������� ���� ��� ����������
		SelectObject(cHDC, Brushes[GCLR_BROWN]);
		// ������ �������
		Rectangle(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16, Pos.y * TileSize + TileSize / 16, Pos.x * TileSize + TileSize / 2 + TileSize / 16, Pos.y * TileSize + TileSize);
		// �������� ������� ���� ��� �������
		SelectObject(cHDC, Pens[GCLR_RED]);
		// �������� ������� ���� ��� ����������
		SelectObject(cHDC, Brushes[GCLR_RED]);
		// ������ ����� ������
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 16, Pos.y * TileSize + TileSize * 0.2, Pos.x * TileSize + TileSize / 1.6, Pos.y * TileSize + TileSize);
		// ������ ������ ������
		Ellipse(cHDC, Pos.x * TileSize + TileSize - TileSize / 16, Pos.y * TileSize + TileSize * 0.2, Pos.x * TileSize + TileSize - TileSize / 1.6, Pos.y * TileSize + TileSize);
		// �������� ������� ���� ��� �������
		SelectObject(cHDC, Pens[GCLR_GREEN]);
		// �������� ������� ���� ��� ����������
		SelectObject(cHDC, Brushes[GCLR_GREEN]);
		// ������ ������
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16, Pos.y * TileSize, Pos.x * TileSize + TileSize - TileSize / 16, Pos.y * TileSize + TileSize / 6);
		// �������� ������� ���� ��� �������
		SelectObject(cHDC, Pens[GCLR_LIGHTRED]);
		// �������� ������� ���� ��� ����������
		SelectObject(cHDC, Brushes[GCLR_LIGHTRED]);
		// ������ ����
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 2.5, Pos.y * TileSize + TileSize / 1.8, Pos.x * TileSize + TileSize / 2.5 - TileSize / 6, Pos.y * TileSize + TileSize / 2 - TileSize / 5);
	}
	else
	{
		if (Pos.x%2 == Pos.y%2)
		{
			SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]);
			SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]);
		}
		else
		{
			SelectObject(cHDC, Pens[GCLR_BURLYWOOD]);
			SelectObject(cHDC, Brushes[GCLR_BURLYWOOD]);
		}		
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize);
	}
}
void DrawSnakeBodyPart(drawtools DrawTools, pos Pos, int TileID, int SegmentsAmount, int SegmentNumber, bool Smashed = false) {	
	Pos.y += INFO_BAR_SIZE;
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	int& TileSize = DrawTools.TileSize;
	Pos.x = Pos.x * TileSize + TileSize / 2;
	Pos.y = Pos.y * TileSize + TileSize / 2;
	if (Smashed && SegmentNumber + 1 == SegmentsAmount)
	{
		SegmentNumber++;
		if (TileID == TILE_SNAKE_UP)
		{
			Pos.y -= TileSize / 2;
		}
		else if (TileID == TILE_SNAKE_DOWN)
		{
			Pos.y += TileSize / 2;
		}
		else if (TileID == TILE_SNAKE_LEFT)
		{
			Pos.x -= TileSize / 2;
		}
		else if (TileID == TILE_SNAKE_RIGHT)
		{
			Pos.x += TileSize / 2;
		}
	}

	int Width;
	Width = TileSize * (0.5 + ((double)SegmentNumber / SegmentsAmount) / 4);
	SelectObject(cHDC, Pens[GCLR_GREEN]);
	SelectObject(cHDC, Brushes[GCLR_GREEN]);
	POINT PolyPoints[4];
	if (TileID == TILE_SNAKE_UP)
	{
		PolyPoints[0].x = Pos.x - Width / 2;
		PolyPoints[0].y = (Pos.y * 2 - TileSize / 4) / 2;
		PolyPoints[1].x = Pos.x;
		PolyPoints[1].y = Pos.y + TileSize / 4;
		PolyPoints[2].x = Pos.x + Width / 2 - 1;
		PolyPoints[2].y = (Pos.y * 2 - TileSize / 4) / 2;
		PolyPoints[3].x = Pos.x;
		PolyPoints[3].y = Pos.y - TileSize / 2;
		if (SegmentNumber == SegmentsAmount)
		{
			if (Smashed)
			{
				int HalfHeadPosY = Pos.y / TileSize * TileSize;				
				//Ellipse(cHDC, Pos.x - Width / 2, HalfHeadPosY - TileSize/2, Pos.x + Width / 2, HalfHeadPosY + TileSize/2);
				Pie(cHDC, Pos.x - Width / 2, HalfHeadPosY - TileSize / 2, Pos.x + Width / 2, HalfHeadPosY + TileSize / 2, Pos.x - Width / 2, HalfHeadPosY, Pos.x + Width / 2, HalfHeadPosY);
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
				PolyPoints[0].x = Pos.x - Width / 2;
				PolyPoints[0].y = HalfHeadPosY;
				PolyPoints[1].x = Pos.x + Width / 2;
				PolyPoints[1].y = HalfHeadPosY;
				PolyPoints[2].x = Pos.x;
				PolyPoints[2].y = HalfHeadPosY + TileSize / 2;				
				Polygon(cHDC, PolyPoints, 3);
			}
			else
			{
				Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
				Polygon(cHDC, PolyPoints, 4);
				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]);
				SelectObject(cHDC, Brushes[GCLR_YELLOW]);
				Ellipse(cHDC, Pos.x - Width / 2 + TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x - Width / 2 + TileSize / 4, Pos.y - TileSize / 2 + TileSize / 3);
				Ellipse(cHDC, Pos.x + Width / 2 - TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x + Width / 2 - TileSize / 4, Pos.y - TileSize / 2 + TileSize / 3);
			}			
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y - TileSize / 4);
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
		else
		{
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
	}
	else if (TileID == TILE_SNAKE_DOWN)
	{
		PolyPoints[0].x = Pos.x - Width / 2;
		PolyPoints[0].y = (Pos.y * 2 + TileSize / 4) / 2;
		PolyPoints[1].x = Pos.x;
		PolyPoints[1].y = Pos.y + TileSize / 2 - 1;
		PolyPoints[2].x = Pos.x + Width / 2 - 1;
		PolyPoints[2].y = (Pos.y * 2 + TileSize / 4) / 2;
		PolyPoints[3].x = Pos.x;
		PolyPoints[3].y = Pos.y - TileSize / 4;
		if (SegmentNumber == SegmentsAmount)
		{
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
			// ������
			SelectObject(cHDC, Pens[GCLR_YELLOW]);
			SelectObject(cHDC, Brushes[GCLR_YELLOW]);
			Ellipse(cHDC, Pos.x - Width / 2 + TileSize / 16, Pos.y + TileSize / 2 - TileSize / 8, Pos.x - Width / 2 + TileSize / 4, Pos.y + TileSize / 2 - TileSize / 3);
			Ellipse(cHDC, Pos.x + Width / 2 - TileSize / 16, Pos.y + TileSize / 2 - TileSize / 8, Pos.x + Width / 2 - TileSize / 4, Pos.y + TileSize / 2 - TileSize / 3);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y, Pos.x + Width / 2, Pos.y + TileSize / 2);
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
		else
		{
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
	}
	else if (TileID == TILE_SNAKE_LEFT)
	{
		PolyPoints[0].x = Pos.x - TileSize / 2;
		PolyPoints[0].y = Pos.y;
		PolyPoints[1].x = (Pos.x * 2 - TileSize / 4) / 2;
		PolyPoints[1].y = Pos.y + Width / 2 - 1;
		PolyPoints[2].x = Pos.x + TileSize / 4 - 1;
		PolyPoints[2].y = Pos.y;
		PolyPoints[3].x = (Pos.x * 2 - TileSize / 4) / 2;
		PolyPoints[3].y = Pos.y - Width / 2;
		if (SegmentNumber == SegmentsAmount)
		{
			Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
			// ������
			SelectObject(cHDC, Pens[GCLR_YELLOW]);
			SelectObject(cHDC, Brushes[GCLR_YELLOW]);
			Ellipse(cHDC, Pos.x - TileSize / 2 + TileSize / 8, Pos.y - Width / 2 + TileSize / 16, Pos.x - TileSize / 2 + TileSize / 3, Pos.y - Width / 2 + TileSize / 4);
			Ellipse(cHDC, Pos.x - TileSize / 2 + TileSize / 8, Pos.y + Width / 2 - TileSize / 16, Pos.x - TileSize / 2 + TileSize / 3, Pos.y + Width / 2 - TileSize / 4);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x, Pos.y + Width / 2);
			Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
		else
		{
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}

	}
	else if (TileID == TILE_SNAKE_RIGHT)
	{
		PolyPoints[0].x = Pos.x - TileSize / 4;
		PolyPoints[0].y = Pos.y;
		PolyPoints[1].x = (Pos.x * 2 + TileSize / 4) / 2;
		PolyPoints[1].y = Pos.y + Width / 2 - 1;
		PolyPoints[2].x = Pos.x + TileSize / 2 - 1;
		PolyPoints[2].y = Pos.y;
		PolyPoints[3].x = (Pos.x * 2 + TileSize / 4) / 2;
		PolyPoints[3].y = Pos.y - Width / 2;
		if (SegmentNumber == SegmentsAmount)
		{
			Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
			// ������
			SelectObject(cHDC, Pens[GCLR_YELLOW]);
			SelectObject(cHDC, Brushes[GCLR_YELLOW]);
			Ellipse(cHDC, Pos.x + TileSize / 2 - TileSize / 8, Pos.y - Width / 2 + TileSize / 16, Pos.x + TileSize / 2 - TileSize / 3, Pos.y - Width / 2 + TileSize / 4);
			Ellipse(cHDC, Pos.x + TileSize / 2 - TileSize / 8, Pos.y + Width / 2 - TileSize / 16, Pos.x + TileSize / 2 - TileSize / 3, Pos.y + Width / 2 - TileSize / 4);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(cHDC, Pos.x, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
		else
		{
			Rectangle(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
		}
	}
}
void DrawSnake(drawtools& DrawTools, map& Map, snake& Snake, int TailTile,  bool Smashed = false) {

	int SegmentNumber = 0;
	int SegmentTile = TailTile;
	pos DrawPos = Snake.TailPos;	
	DrawTile(DrawTools, DrawPos, TILE_EMPTY);
	while (true)
	{
		SegmentNumber++;		
		DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber);
		if (Snake.Segments == SegmentNumber + 1)
		{
			DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber, Smashed);
		}
		if (DrawPos.x == Snake.HeadPos.x && DrawPos.y == Snake.HeadPos.y)
		{
			break;
		}
		if (SegmentTile == TILE_SNAKE_UP)
		{
			if (DrawPos.y - 1 < 0)
			{
				DrawPos.y = Map.Height - 1;

			}
			else
			{
				DrawPos.y--;
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY);
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_DOWN, Snake.Segments, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_DOWN)
		{
			if (DrawPos.y + 1 >= Map.Height)
			{
				DrawPos.y = 0;

			}
			else
			{
				DrawPos.y++;
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY);
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_UP, Snake.Segments, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_LEFT)
		{
			if (DrawPos.x - 1 < 0)
			{
				DrawPos.x = Map.Width - 1;

			}
			else
			{
				DrawPos.x--;
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY);
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_RIGHT, Snake.Segments, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_RIGHT)
		{
			if (DrawPos.x + 1 >= Map.Width)
			{
				DrawPos.x = 0;
			}
			else
			{
				DrawPos.x++;
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY);
			DrawSnakeBodyPart(DrawTools,DrawPos, TILE_SNAKE_LEFT, Snake.Segments, SegmentNumber);
		}
		SegmentTile = Map.Tiles[DrawPos.y][DrawPos.x];
	}
}
void DrawMap(drawtools& DrawTools, map& Map, bool NoApples = false) {
	for (int y = 0; y < Map.Height; y++)
	{
		for (int x = 0; x < Map.Width; x++)
		{
			DrawTile(DrawTools, { x, y }, Map.Tiles[y][x], NoApples);
		}
	}

}
void RenderText(drawtools& DrawTools, const char* Text, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	HWND cHWND = GetConsoleWindow(); // ���������� ����, ������������ ��������
	HDC cHDC = GetDC(GetConsoleWindow()); // ������ ���� HDC ������������ ����� 32-��������� ����� ����������� �����.	
	SetTextColor(cHDC, DrawTools.Palette.Colors[TxtClr]); // ���� ������
	SetBkMode(cHDC, TRANSPARENT); // ���������� ���� ����

	SelectObject(cHDC, Font); // ����� ������� � ����������� �����������
	if (Centered)
	{
		LOGFONT TempFont;
		GetObject(Font, sizeof(LOGFONT), &TempFont);
		Pos.y -= TempFont.lfHeight / 2;
		SetTextAlign(cHDC, TA_CENTER);
	}
	TextOutA(cHDC, Pos.x, Pos.y, Text, strlen(Text)); // ����� ������ �� �����

	ReleaseDC(cHWND, cHDC);
}
void DrawTextLines(drawtools& DrawTools, std::string* TextLines, int TextLinesCount, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	LOGFONT TempFont;
	GetObject(Font, sizeof(LOGFONT), &TempFont);
	if (Centered)
	{
		if (TextLinesCount % 2)
		{
			Pos.y -= TextLinesCount * TempFont.lfHeight;
		}
		else
		{

			Pos.y -= TempFont.lfHeight + (TextLinesCount - 1) * TempFont.lfHeight;
		}
	}
	for (int i = 0; i < TextLinesCount; i++)
	{
		RenderText(DrawTools, TextLines[i].c_str(), Pos, Font, TxtClr, Centered);
		Pos.y += TempFont.lfHeight * 2;
	}
}
void DrawInfoFoodCount(drawtools& DrawTools, int FoodCount) {
	char StrBuffer[6];
	sprintf_s(StrBuffer, 6, " %03i", FoodCount);
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]);
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]);
	Rectangle(DrawTools.Console.cHDC, 2 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 3.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize);
	RenderText(DrawTools, StrBuffer, { 2 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false);
}
void DrawInfoScore(drawtools& DrawTools, int Score) {
	char StrBuffer[8];
	sprintf_s(StrBuffer, 8, " %05i", Score);
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]);
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]);
	Rectangle(DrawTools.Console.cHDC, 5 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 8.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize);
	RenderText(DrawTools, StrBuffer, { 5 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false);
}
void DrawInfoBar(drawtools& DrawTools, map& Map, int FoodCount, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	SelectObject(cHDC, Pens[GCLR_DARKWOOD]);
	SelectObject(cHDC, Brushes[GCLR_DARKWOOD]);
	Rectangle(cHDC, 0, 0, Map.Width * DrawTools.TileSize, INFO_BAR_SIZE * DrawTools.TileSize);	
	SelectObject(cHDC, Pens[GCLR_SCARLET]);
	SelectObject(cHDC, Brushes[GCLR_SCARLET]);
	Ellipse(cHDC, DrawTools.TileSize, 0.5* DrawTools.TileSize, 2 * DrawTools.TileSize, 1.5 * DrawTools.TileSize);
	DrawApple(DrawTools, { 1, 0.5 });
	DrawInfoFoodCount(DrawTools, FoodCount);
	SelectObject(cHDC, Pens[GCLR_YELLOW]);
	SelectObject(cHDC, Brushes[GCLR_YELLOW]);
	Ellipse(cHDC, 4 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 5 * DrawTools.TileSize, 1.5 * DrawTools.TileSize);	
	DrawStar(DrawTools, { 4.5, 1 }, 0.4);
	DrawInfoScore(DrawTools, Score);	
}
void DrawMainMenuBackGround(drawtools& DrawTools, map& Map) {
	for (int y = 0; y < Map.Height + INFO_BAR_SIZE; y++) // ������ ���
	{
		for (int x = 0; x < Map.Width; x++) // ������������ ��������� ��� �� ���������
		{
			DrawTile(DrawTools, { x,y - INFO_BAR_SIZE }, TILE_EMPTY); // �������� ������� ��������� ������
		}
	}
}