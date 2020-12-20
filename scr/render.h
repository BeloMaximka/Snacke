/*-------------------------------------------------------------------
	render.h

	��, ��� �������� ��������� ����-����
-------------------------------------------------------------------*/

#pragma once // ����� ���� ����������� ����������� ������ ���� ���
#include "includes.h" // ��������� .h ���� � ���������

#define WINDOW_STATE_MAXIMIZED 1 // �������� ������������ ����
#define WINDOW_STATE_MINIMIZED 2 // ��������� ���������� ����

// ���������, ������������ �� ����
bool WindowMaximized(drawtools& DrawTools) {
	WINDOWPLACEMENT WinPlacement; // ��������� �������� ����
	GetWindowPlacement(DrawTools.Console.cHWND, &WinPlacement); // �������� �������� ����
	// ���� ��������� ���������� �� ���������� �� �����������
	if (DrawTools.WindowState == WINDOW_STATE_MINIMIZED && WinPlacement.showCmd == WINDOW_STATE_MAXIMIZED)
	{
		DrawTools.WindowState = WinPlacement.showCmd; // ��������� ���������
		return true; // ���� ������������ - ���������� true
	}
	else // ���� ���
	{
		DrawTools.WindowState = WinPlacement.showCmd; // ��������� ���������
		return false; // ���� �� ������������ - ���������� false
	}
}

// ������������ �������������� ������ �� ���� � ��������
void RotateVector(dpos& Point, double Angle) {
	Angle *= (M_PI / 180); // ��������� ���� � ��������� ����
	dpos NewPoint; // ���������� ��� ����� ������� �������
	NewPoint.x = Point.x * cos(Angle) - Point.y * sin(Angle); // �������������� ���������� x �� �������������� �������
	NewPoint.y = Point.x * sin(Angle) + Point.y * cos(Angle); // �������������� ���������� � �� �������������� �������
	Point = NewPoint; // ����������� ����� �������
}

// ������ ������ ��������� �������
void DrawStar(drawtools& DrawTools, dpos Pos, double SizeMod) {
	int TileSize = DrawTools.TileSize; // �������������� �������� � ������ ���������� ��� �������� ������

	Pos = { Pos.x * TileSize, Pos.y * TileSize }; // ������������ ������� � ������� ����	
	dpos StarOutEndDir = { 0,-1 }; // ��������� ����������� �������� ������ ������
	dpos StarInEndDir = { 0,1 }; // ��������� ����������� ������� ������ ������
	POINT PolyPoints[10]; // �����, ������� ����� ���������� ������� ������ ������
	LONG PosX; // ������� ����� ������ �� ��� X
	LONG PosY; // ������� ����� ������ �� ��� Y
	double Angle = 72; // ���� �������� ������� �� ����. ����� (360/5 = 72)

	PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod; // ������ ������ ������� ����� �� ��� X
	PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod; // ������ ������ ������� ����� �� ��� Y
	PolyPoints[0] = { PosX, PosY }; // �������� ����� � ������
	RotateVector(StarInEndDir, -2 * Angle); // ������������ ������ ����������� ���������� ����� ������ ������� ��� ����
	PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod; // ������ ������ ���������� ����� �� ��� X
	PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod; // ������ ������ ���������� ����� �� ��� Y
	PolyPoints[1] = { PosX, PosY }; // �������� ����� � ������
	RotateVector(StarOutEndDir, Angle); // ������������ ������ ����������� ������� ����� �� �������

	for (int i = 2; i < 10; i++) // ��������� �������� ���� ��� ��������� �����
	{
		PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod; // ������ ������� ����� �� ��� X
		PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod; // ������ ������� ����� �� ��� Y
		PolyPoints[i] = { PosX, PosY }; // �������� ����� � ������
		RotateVector(StarInEndDir, Angle); // ������������ ������ ����������� ���������� ����� �� �������
		i++; // ����� �����������, ����������� ����������� ����������
		PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod; // ������ ���������� ����� �� ��� X
		PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod; // ������ ���������� ����� �� ��� Y
		PolyPoints[i] = { PosX, PosY }; // �������� ����� � ������
		RotateVector(StarOutEndDir, Angle); // ������������ ������ ����������� ������� ����� �� �������
	}

	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKYELLOW]); // �������� �����-������ ���� ��� �������
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKYELLOW]); // �������� �����-������ ���� ��� ����������
	Polygon(DrawTools.Console.cHDC, PolyPoints, 10); // ������ ���� ������ �� ������
}

// ������ ������ ��� ��������
void DrawApple(drawtools& DrawTools, dpos Pos) {
	HDC& cHDC = DrawTools.Console.cHDC; // ������� ������ ��� ���������� ��� ��������
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // ������� ������ ��� ���������� ��� ��������
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // ������� ������ ��� ���������� ��� ��������
	int TileSize = DrawTools.TileSize; // �������� �������� � ������ ���������� ��� ��������
	int SizeMod = TileSize / 16; // ������������� �������� ������ ������
	pos LeftTopPos; // ����� ������� ����� ������
	pos RightBottomPos; // ������ ������ ����� ������
	SelectObject(cHDC, Pens[GCLR_DARKRED]); // �������� �����-������� ���� ��� �������	
	SelectObject(cHDC, Brushes[GCLR_DARKRED]); // �������� �����-������� ���� ��� ����������
	// �������
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // ������� � ����� ������� �����
	LeftTopPos.y = Pos.y * TileSize + TileSize / 16 + SizeMod * 2; // ������� y ����� ������� �����
	RightBottomPos.x = Pos.x * TileSize + TileSize / 2 + TileSize / 16; // ������� � ������ ������ ����� 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // ������� y ������ ������ �����
	Rectangle(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ �������
	// ����� ������
	LeftTopPos.x = Pos.x * TileSize + TileSize / 16 + SizeMod; // ������� � ����� ������� �����
	LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2 + SizeMod; // ������� y ����� ������� �����
	RightBottomPos.x = Pos.x * TileSize + TileSize / 1.6 - SizeMod; // ������� � ������ ������ ����� 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // ������� y ������ ������ �����
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ����� ������	
	// ������ ������
	LeftTopPos.x = Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod; // ������� � ����� ������� �����
	LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2 + SizeMod; // ������� y ����� ������� �����
	RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 1.6 + SizeMod; // ������� � ������ ������ ����� 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // ������� y ������ ������ �����
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������		
	// ������
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16 + SizeMod / 2; // ������� � ����� ������� �����
	LeftTopPos.y = Pos.y * TileSize + SizeMod * 2; // ������� y ����� ������� �����
	RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod * 2; // ������� � ������ ������ �����
	RightBottomPos.y = Pos.y * TileSize + TileSize / 6 + SizeMod; // ������� y ������ ������ �����
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������	
	// ����
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2.5; // ������� � ����� ������� �����
	LeftTopPos.y = Pos.y * TileSize + TileSize / 1.8; // ������� y ����� ������� �����
	RightBottomPos.x = Pos.x * TileSize + TileSize / 2.5 - TileSize / 6; // ������� � ������ ������ ����� 
	RightBottomPos.y = Pos.y * TileSize + TileSize / 2 - TileSize / 5; // ������� y ������ ������ �����
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ����
}

// ������ ������ �� �������� �����������
void DrawTile(drawtools& DrawTools, pos Pos, int TileID, bool NoApples = false) {
	Pos.y += INFO_BAR_SIZE; // ������� ������ ����, ��� ������ �������
	HDC& cHDC = DrawTools.Console.cHDC; // ������� ������ ��� ���������� ��� ��������
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // ������� ������ ��� ���������� ��� ��������
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // ������� ������ ��� ���������� ��� ��������
	int TileSize = DrawTools.TileSize; // �������� �������� � ������ ���������� ��� ��������	
	if (TileID == TILE_WALL) // ���� �����
	{
		SelectObject(cHDC, Pens[GCLR_DARKBURLYWOOD]); // �������� ���� �������
		SelectObject(cHDC, Brushes[GCLR_DARKBURLYWOOD]); // �������� ���� ����������
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize); // ������ �������
	}
	else if (TileID == TILE_FOOD && !NoApples)
	{
		pos LeftTopPos; // ����� ������� ����� ������
		pos RightBottomPos; // ������ ������ ����� ������
		DrawTile(DrawTools, { Pos.x, Pos.y - INFO_BAR_SIZE }, TILE_EMPTY); // ������� �������� ������ �����

		// �������
		SelectObject(cHDC, Pens[GCLR_BROWN]); // �������� ���������� ���� ��� �������		
		SelectObject(cHDC, Brushes[GCLR_BROWN]); // �������� ���������� ���� ��� ����������
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // ������� � ����� ������� �����
		LeftTopPos.y = Pos.y * TileSize + TileSize / 16; // ������� y ����� ������� �����
		RightBottomPos.x = Pos.x * TileSize + TileSize / 2 + TileSize / 16; // ������� � ������ ������ ����� 
		RightBottomPos.y = Pos.y * TileSize + TileSize; // ������� y ������ ������ �����
		Rectangle(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ �������		

		SelectObject(cHDC, Pens[GCLR_RED]); // �������� ������� ���� ��� �������		
		SelectObject(cHDC, Brushes[GCLR_RED]); // �������� ������� ���� ��� ����������
		// ����� ������
		LeftTopPos.x = Pos.x * TileSize + TileSize / 16; // ������� � ����� ������� �����
		LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2; // ������� y ����� ������� �����
		RightBottomPos.x = Pos.x * TileSize + TileSize / 1.6; // ������� � ������ ������ ����� 		
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ����� ������		
		// ������ ������
		LeftTopPos.x = Pos.x * TileSize + TileSize - TileSize / 16; // ������� � ����� ������� �����		
		RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 1.6; // ������� � ������ ������ ����� 		
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������

		// ������
		SelectObject(cHDC, Pens[GCLR_GREEN]); // �������� ������� ���� ��� �������		
		SelectObject(cHDC, Brushes[GCLR_GREEN]); // �������� ������� ���� ��� ����������		
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // ������� � ����� ������� �����
		LeftTopPos.y = Pos.y * TileSize; // ������� y ����� ������� �����
		RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 16; // ������� � ������ ������ ����� 
		RightBottomPos.y = Pos.y * TileSize + TileSize / 6; // ������� y ������ ������ �����
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������		

		// ����		
		SelectObject(cHDC, Pens[GCLR_LIGHTRED]); // �������� ������� ���� ��� �������		
		SelectObject(cHDC, Brushes[GCLR_LIGHTRED]); // �������� ������� ���� ��� ����������
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2.5; // ������� � ����� ������� �����
		LeftTopPos.y = Pos.y * TileSize + TileSize / 1.8; // ������� y ����� ������� �����
		RightBottomPos.x = Pos.x * TileSize + TileSize / 2.5 - TileSize / 6; // ������� � ������ ������ ����� 
		RightBottomPos.y = Pos.y * TileSize + TileSize / 2 - TileSize / 5; // ������� y ������ ������ �����
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ����		
	}
	else
	{
		if (Pos.x % 2 == Pos.y % 2) // ���� ��� ������ ��� ��������
		{
			SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // �������� ���� ��� ����������
		}
		else // ���� ��
		{
			SelectObject(cHDC, Pens[GCLR_BURLYWOOD]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_BURLYWOOD]); // �������� ���� ��� ����������
		}
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize); // ������ �������
	}
}

// ������ ������� ������
void DrawSnakeBodyPart(drawtools DrawTools, pos Pos, int TileID, int SegmentsAmount, int SegmentNumber, bool Smashed = false) {
	Pos.y += INFO_BAR_SIZE; // ������� ������ ����, ��� ������ �������
	HDC& cHDC = DrawTools.Console.cHDC; // ������� ������ ��� ���������� ��� ��������
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // ������� ������ ��� ���������� ��� ��������
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // ������� ������ ��� ���������� ��� ��������
	int TileSize = DrawTools.TileSize; // �������� �������� � ������ ���������� ��� ��������	
	Pos.x = Pos.x * TileSize + TileSize / 2; // ������������ x ���������� � ���������� ����
	Pos.y = Pos.y * TileSize + TileSize / 2; // ������������ y ���������� � ���������� ����

	if (Smashed && SegmentNumber + 1 == SegmentsAmount) // ���� �������� � ������� ������
	{
		SegmentNumber++; // ����������� ����� �������� �� ���� (����������� �������)
		if (TileID == TILE_SNAKE_UP) // ���� ������� �����
		{
			Pos.y -= TileSize / 2; // ������� ������
		}
		else if (TileID == TILE_SNAKE_DOWN) // ���� ������� ����
		{
			Pos.y += TileSize / 2; // ������� ����
		}
		else if (TileID == TILE_SNAKE_LEFT) // ���� ������� �����
		{
			Pos.x -= TileSize / 2; // ������� �����
		}
		else if (TileID == TILE_SNAKE_RIGHT) // ���� ������� ������
		{
			Pos.x += TileSize / 2; // ������� ������
		}
	}

	int Width; // ������ ��������
	Width = TileSize * (0.5 + ((double)SegmentNumber / SegmentsAmount) / 4); // ��������� ������ ��������. ��� ������ ������� - ��� ���
	SelectObject(cHDC, Pens[GCLR_GREEN]); // �������� ���� ��� �������
	SelectObject(cHDC, Brushes[GCLR_GREEN]); // �������� ���� ��� ����������
	POINT PolyPoints[4]; // ����� ��� ��������� �����
	pos LeftTopPos; // ����� ������� ����� ������
	pos RightBottomPos; // ������ ������ ����� ������

	if (TileID == TILE_SNAKE_UP) // ���� ������� �����
	{
		// ����������� ����
		PolyPoints[0].x = Pos.x - Width / 2; // ������������� ���������� � ����� �����
		PolyPoints[0].y = (Pos.y * 2 - TileSize / 4) / 2; // ������������� ���������� y ����� �����
		PolyPoints[1].x = Pos.x; // ������������� ���������� � ����� �����
		PolyPoints[1].y = Pos.y + TileSize / 4; // ������������� ���������� y ����� �����
		PolyPoints[2].x = Pos.x + Width / 2 - 1; // ������������� ���������� � ����� �����
		PolyPoints[2].y = (Pos.y * 2 - TileSize / 4) / 2; // ������������� ���������� y ����� �����
		PolyPoints[3].x = Pos.x; // ������������� ���������� � ����� �����
		PolyPoints[3].y = Pos.y - TileSize / 2; // ������������� ���������� y ����� �����
		if (SegmentNumber == SegmentsAmount) // ���� ������
		{
			if (Smashed) // ���� ���������
			{
				// ������
				int HalfHeadPosY = Pos.y / TileSize * TileSize; // ���������� � �������� ������
				LeftTopPos.x = Pos.x - Width / 2; // ������� � ����� ������� �����
				LeftTopPos.y = HalfHeadPosY - TileSize / 2; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2; // ������� � ������ ������ ����� 
				RightBottomPos.y = HalfHeadPosY + TileSize / 2; // ������� y ������ ������ �����
				// ������ �������� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfHeadPosY, RightBottomPos.x, HalfHeadPosY);

				// ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				PolyPoints[0].x = Pos.x - Width / 2; // ������������� ���������� � ����� ������������
				PolyPoints[0].y = HalfHeadPosY; // ������������� ���������� y ����� ������������
				PolyPoints[1].x = Pos.x + Width / 2; // ������������� ���������� � ����� ������������
				PolyPoints[1].y = HalfHeadPosY; // ������������� ���������� y ����� ������������
				PolyPoints[2].x = Pos.x; // ������������� ���������� � ����� ������������
				PolyPoints[2].y = HalfHeadPosY + TileSize / 2; // ������������� ���������� y ����� ������������
				Polygon(cHDC, PolyPoints, 3); // ������ �����������

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				Pos.y += TileSize / 4 + TileSize / 24; // �������� ������� ����
				int HalfEyePosY = Pos.y + 11 * TileSize / 48 - TileSize / 2; // ���������� � �������� ����
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // ������� y ������ ������ �����
				// ������ ����� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfEyePosY, RightBottomPos.x, HalfEyePosY);
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				// ������ ������ ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfEyePosY, LeftTopPos.x, HalfEyePosY);

			}
			else // ���� �� ���������
			{
				// ������ ������
				Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);

				// ����������� ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // ������� y ������ ������ �����
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ����� ������
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������
			}
		}
		else if (SegmentNumber == 1) // ���� �����
		{
			// ������ ���������� �������� ������
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y - TileSize / 4);
			// ������ ������� �������� ������
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
		else // ���� ������������ �������
		{
			// ������ �������
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
	}
	else if (TileID == TILE_SNAKE_DOWN) // ���� ������� ����
	{
		// ���������� ������������ �����
		PolyPoints[0].x = Pos.x - Width / 2; // ������������� ���������� � ����� �����
		PolyPoints[0].y = (Pos.y * 2 + TileSize / 4) / 2; // ������������� ���������� y ����� �����
		PolyPoints[1].x = Pos.x; // ������������� ���������� � ����� �����
		PolyPoints[1].y = Pos.y + TileSize / 2 - 1; // ������������� ���������� y ����� �����
		PolyPoints[2].x = Pos.x + Width / 2 - 1; // ������������� ���������� � ����� �����
		PolyPoints[2].y = (Pos.y * 2 + TileSize / 4) / 2; // ������������� ���������� y ����� �����
		PolyPoints[3].x = Pos.x; // ������������� ���������� � ����� �����
		PolyPoints[3].y = Pos.y - TileSize / 4; // ������������� ���������� y ����� �����
		if (SegmentNumber == SegmentsAmount) // ���� ������
		{
			if (Smashed) // ���� ���������
			{
				// ������
				int HalfHeadPosY = Pos.y / TileSize * TileSize; // ���������� � �������� ������
				LeftTopPos.x = Pos.x - Width / 2; // ������� � ����� ������� �����
				LeftTopPos.y = HalfHeadPosY - TileSize / 2; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2; // ������� � ������ ������ ����� 
				RightBottomPos.y = HalfHeadPosY + TileSize / 2; // ������� y ������ ������ �����
				// ������ �������� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfHeadPosY, LeftTopPos.x, HalfHeadPosY);

				// ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				PolyPoints[0].x = Pos.x - Width / 2; // ������������� ���������� � ����� ������������
				PolyPoints[0].y = HalfHeadPosY; // ������������� ���������� y ����� ������������
				PolyPoints[1].x = Pos.x + Width / 2; // ������������� ���������� � ����� ������������
				PolyPoints[1].y = HalfHeadPosY; // ������������� ���������� y ����� ������������
				PolyPoints[2].x = Pos.x; // ������������� ���������� � ����� ������������
				PolyPoints[2].y = HalfHeadPosY - TileSize / 2; // ������������� ���������� y ����� ������������
				Polygon(cHDC, PolyPoints, 3); // ������ �����������

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				Pos.y += TileSize / 4 + TileSize / 24; // �������� ������� ����
				int HalfEyePosY = Pos.y + 11 * TileSize / 48 - TileSize / 2; // ���������� � �������� ����
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // ������� y ������ ������ �����
				// ������ ����� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfEyePosY, LeftTopPos.x, HalfEyePosY);
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				// ������ ������ ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfEyePosY, RightBottomPos.x, HalfEyePosY);
			}
			else // ���� �� ���������
			{
				// ������ ������
				Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);

				// ����������� ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y + TileSize / 2 - TileSize / 8; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y + TileSize / 2 - TileSize / 3; // ������� y ������ ������ �����
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ����� ����� ������
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������
			}

		}
		else if (SegmentNumber == 1) // ���� �����
		{
			// ������ ���������� �������� ������
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// ������ ������� �������� ������
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
		else // ���� ������������ �������
		{
			// ������ �������
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
	}
	else if (TileID == TILE_SNAKE_LEFT) // ���� ������� �����
	{
		// ���������� ������������ �����
		PolyPoints[0].x = Pos.x - TileSize / 2; // ������������� ���������� � ����� �����
		PolyPoints[0].y = Pos.y; // ������������� ���������� y ����� �����
		PolyPoints[1].x = (Pos.x * 2 - TileSize / 4) / 2; // ������������� ���������� � ����� �����
		PolyPoints[1].y = Pos.y + Width / 2 - 1; // ������������� ���������� y ����� �����
		PolyPoints[2].x = Pos.x + TileSize / 4 - 1; // ������������� ���������� � ����� ����� 
		PolyPoints[2].y = Pos.y; // ������������� ���������� y ����� �����
		PolyPoints[3].x = (Pos.x * 2 - TileSize / 4) / 2; // ������������� ���������� � ����� �����
		PolyPoints[3].y = Pos.y - Width / 2; // ������������� ���������� y ����� �����

		if (SegmentNumber == SegmentsAmount) // ���� ������
		{
			if (Smashed) // ���� ���������
			{
				// ������
				int HalfHeadPosX = Pos.x / TileSize * TileSize; // ���������� x �������� ������		
				LeftTopPos.x = HalfHeadPosX - TileSize / 2; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2; // ������� y ����� ������� �����
				RightBottomPos.x = HalfHeadPosX + TileSize / 2; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y + Width / 2; // ������� y ������ ������ �����
				// ������ �������� ������				
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfHeadPosX, RightBottomPos.y, HalfHeadPosX, LeftTopPos.y);

				// ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				PolyPoints[0].x = HalfHeadPosX; // ������������� ���������� � ����� ������������
				PolyPoints[0].y = Pos.y - Width / 2; // ������������� ���������� y ����� ������������
				PolyPoints[1].x = HalfHeadPosX; // ������������� ���������� � ����� ������������
				PolyPoints[1].y = Pos.y + Width / 2; // ������������� ���������� y ����� ������������
				PolyPoints[2].x = HalfHeadPosX + TileSize / 2; // ������������� ���������� � ����� ������������
				PolyPoints[2].y = Pos.y; // ������������� ���������� y ����� ������������
				Polygon(cHDC, PolyPoints, 3); // ������ �����������

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				Pos.x += TileSize / 4 + TileSize / 24; // �������� ������� ������				
				int HalfEyePosX = Pos.x + 11 * TileSize / 48 - TileSize / 2; // ���������� � �������� ����
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // ������� y ������ ������ �����
				// ������ ����� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, RightBottomPos.y, HalfEyePosX, LeftTopPos.y);
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				// ������ ������ ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, LeftTopPos.y, HalfEyePosX, RightBottomPos.y);
			}
			else // ���� �� ���������
			{
				// ������ ������
				Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

				// ����������� ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // ������� y ������ ������ �����
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ����� ����� ������
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������
			}
		}
		else if (SegmentNumber == 1) // ���� �����
		{
			// ������ ���������� �������� ������
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x, Pos.y + Width / 2);
			// ������ ������� �������� ������
			Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
		else // ���� ������������ �������
		{
			// ������ �������
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}

	}
	else if (TileID == TILE_SNAKE_RIGHT) // ���� ������� ������
	{
		// ���������� ������������ �����
		PolyPoints[0].x = Pos.x - TileSize / 4; // ������������� ���������� � ����� �����
		PolyPoints[0].y = Pos.y; // ������������� ���������� y ����� �����
		PolyPoints[1].x = (Pos.x * 2 + TileSize / 4) / 2; // ������������� ���������� � ����� �����
		PolyPoints[1].y = Pos.y + Width / 2 - 1; // ������������� ���������� y ����� �����
		PolyPoints[2].x = Pos.x + TileSize / 2 - 1; // ������������� ���������� � ����� �����
		PolyPoints[2].y = Pos.y; // ������������� ���������� y ����� �����
		PolyPoints[3].x = (Pos.x * 2 + TileSize / 4) / 2; // ������������� ���������� � ����� �����
		PolyPoints[3].y = Pos.y - Width / 2; // ������������� ���������� y ����� �����

		if (SegmentNumber == SegmentsAmount) // ���� ������
		{
			if (Smashed) // ���� ���������
			{
				// ������
				int HalfHeadPosX = Pos.x / TileSize * TileSize; // ���������� x �������� ������		
				LeftTopPos.x = HalfHeadPosX - TileSize / 2; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2; // ������� y ����� ������� �����
				RightBottomPos.x = HalfHeadPosX + TileSize / 2; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y + Width / 2; // ������� y ������ ������ �����
				// ������ �������� ������				
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfHeadPosX, LeftTopPos.y, HalfHeadPosX, RightBottomPos.y);

				// ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				PolyPoints[0].x = HalfHeadPosX; // ������������� ���������� � ����� ������������
				PolyPoints[0].y = Pos.y - Width / 2; // ������������� ���������� y ����� ������������
				PolyPoints[1].x = HalfHeadPosX; // ������������� ���������� � ����� ������������
				PolyPoints[1].y = Pos.y + Width / 2; // ������������� ���������� y ����� ������������
				PolyPoints[2].x = HalfHeadPosX - TileSize / 2; // ������������� ���������� � ����� ������������
				PolyPoints[2].y = Pos.y; // ������������� ���������� y ����� ������������
				Polygon(cHDC, PolyPoints, 3); // ������ �����������

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				Pos.x += TileSize / 4 + TileSize / 24; // �������� ������� ������				
				int HalfEyePosX = Pos.x + 11 * TileSize / 48 - TileSize / 2; // ���������� � �������� ����
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // ������� y ������ ������ �����
				// ������ ����� ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, LeftTopPos.y, HalfEyePosX, RightBottomPos.y);
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				// ������ ������ ������
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, RightBottomPos.y, HalfEyePosX, LeftTopPos.y);
			}
			else // ���� �� ���������
			{
				// ������ ������
				Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

				// ����������� ����
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
				Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����

				// ������
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
				LeftTopPos.x = Pos.x + TileSize / 2 - TileSize / 8; // ������� � ����� ������� �����
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // ������� y ����� ������� �����
				RightBottomPos.x = Pos.x + TileSize / 2 - TileSize / 3; // ������� � ������ ������ ����� 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // ������� y ������ ������ �����
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ����� ����� ������
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // ������� � ����� ������� �����
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // ������� � ������ ������ ����� 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // ������ ������ ������
			}
		}
		else if (SegmentNumber == 1) // ���� �����
		{
			// ������ ���������� �������� ������
			Rectangle(cHDC, Pos.x, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			// ������ ������� �������� ������
			Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
		else // ���� ������������� �������
		{
			// ������ �������
			Rectangle(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

			// ����������� ����
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // �������� ���� ��� �������
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // �������� ���� ��� ����������
			Polygon(cHDC, PolyPoints, 4); // ������ ����������� ����
		}
	}
}

// ������ ������ �������
void DrawSnake(drawtools& DrawTools, map& Map, snake& Snake, int TailTile, bool Smashed = false) {
	int SegmentNumber = 0; // ����� ���������� ��������
	int SegmentTile = TailTile; // ID ���������� ��������, �������� � ������
	pos DrawPos = Snake.TailPos; // ������� ���������� ��������
	DrawTile(DrawTools, DrawPos, TILE_EMPTY); // �������� ������� �����
	while (true) // �������� �� ��������� � ������������ ��
	{
		SegmentNumber++; // ����������� ����� �������� �� 1
		DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber); // ������ �������
		if (SegmentNumber + 1 == Snake.Segments) // �������� ��� ����������� ������
		{
			DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber, Smashed); // ������������ ��������� ������, ���� Smashed == true
		}
		if (DrawPos.x == Snake.HeadPos.x && DrawPos.y == Snake.HeadPos.y) // ���� ����� �� ������
		{
			break; // ���������������
		}
		if (SegmentTile == TILE_SNAKE_UP) // ���� ������� �����
		{
			if (DrawPos.y - 1 < 0) // �������� �� ������� �����
			{
				DrawPos.y = Map.Height - 1; // ������������� �� ������ �����

			}
			else // ���� �� �� �������
			{
				DrawPos.y--; // ���������� �����
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // �������� �����
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_DOWN, Snake.Segments, SegmentNumber); // ������ �������� �������
		}
		else if (SegmentTile == TILE_SNAKE_DOWN) // ���� ������� ����
		{
			if (DrawPos.y + 1 >= Map.Height) // �������� �� ������� �����
			{
				DrawPos.y = 0; // ������������� �� ������ �����

			}
			else // ���� �� �� �������
			{
				DrawPos.y++; // ���������� ����
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // �������� �����
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_UP, Snake.Segments, SegmentNumber); // ������ �������� �������
		}
		else if (SegmentTile == TILE_SNAKE_LEFT) // ���� ������� �����
		{
			if (DrawPos.x - 1 < 0) // �������� �� ������� �����
			{
				DrawPos.x = Map.Width - 1; // ������������� �� ������ �����

			}
			else // ���� �� �� �������
			{
				DrawPos.x--; // ���������� �����
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // �������� �����
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_RIGHT, Snake.Segments, SegmentNumber); // ������ �������� �������
		}
		else if (SegmentTile == TILE_SNAKE_RIGHT) // ���� ������� ������
		{
			if (DrawPos.x + 1 >= Map.Width) // �������� �� ������� �����
			{
				DrawPos.x = 0; // ������������� �� ������ �����
			}
			else // ���� �� �� �������
			{
				DrawPos.x++; // ���������� ������
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // �������� �����
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_LEFT, Snake.Segments, SegmentNumber); // ������ �������� �������
		}
		SegmentTile = Map.Tiles[DrawPos.y][DrawPos.x]; // �������� ID ���������� ��������
	}
}

// ������ �����
void DrawMap(drawtools& DrawTools, map& Map, bool NoApples = false) {
	for (int y = 0; y < Map.Height; y++) // �� ��� ������� �� ���������
	{
		for (int x = 0; x < Map.Width; x++) // � �� �����������
		{
			DrawTile(DrawTools, { x, y }, Map.Tiles[y][x], NoApples); // ������ ������
		}
	}
}

// ���������� �����
void RenderText(drawtools& DrawTools, const char* Text, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	HWND cHWND = GetConsoleWindow(); // ���������� ����, ������������ ��������
	HDC cHDC = GetDC(GetConsoleWindow()); // ������ ���� HDC ������������ ����� 32-��������� ����� ����������� �����.	
	SetTextColor(cHDC, DrawTools.Palette.Colors[TxtClr]); // ���� ������
	SetBkMode(cHDC, TRANSPARENT); // ���������� ���� ����
	SelectObject(cHDC, Font); // ����� ������� � ����������� �����������
	if (Centered) // ���� ����� ����� ������������
	{
		LOGFONT TempFont; // �������� ������� ����
		GetObject(Font, sizeof(LOGFONT), &TempFont); // �������� �������� ����
		Pos.y -= TempFont.lfHeight / 2; // ����������� �� ���������
		SetTextAlign(cHDC, TA_CENTER); // ����������� �� �����������
	}
	TextOutA(cHDC, Pos.x, Pos.y, Text, strlen(Text)); // ����� ������ �� �����
	ReleaseDC(cHWND, cHDC); // ����������� ����������
}

// ������ ����� �� ������ � ������ ����������� ����� �����
void DrawTextLines(drawtools& DrawTools, std::string* TextLines, int TextLinesCount, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	LOGFONT TempFont; // ��� �������� ������� ����
	GetObject(Font, sizeof(LOGFONT), &TempFont); // �������� �������� ����
	if (Centered) // ���� ����� ������������
	{
		if (TextLinesCount % 2) // ���� �������� ���������� �����
		{
			Pos.y -= TextLinesCount * TempFont.lfHeight; // ������� ������� � ����� ������� ������
		}
		else // ���� ������ ���������� �����
		{

			Pos.y -= TempFont.lfHeight + (TextLinesCount - 1) * TempFont.lfHeight; // ������� ������� � ����� ������� ������
		}
	}
	for (int i = 0; i < TextLinesCount; i++) // ���������� ������ ������
	{
		RenderText(DrawTools, TextLines[i].c_str(), Pos, Font, TxtClr, Centered); // ������ ������
		Pos.y += TempFont.lfHeight * 2; // ��������� ����
	}
}

// ������ ������� ���
void DrawInfoFoodCount(drawtools& DrawTools, int FoodCount) {
	char StrBuffer[6]; // ����� ��� ������ ��������
	sprintf_s(StrBuffer, 6, " %03i", FoodCount); // ������������ ����� � ������
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]); // �������� ���� ��� �������
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]); // �������� ���� ��� ����������
	Rectangle(DrawTools.Console.cHDC, 2 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 3.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // �������� ���������� �����
	RenderText(DrawTools, StrBuffer, { 2 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false); // ������ �����
}

// ������ ����
void DrawInfoScore(drawtools& DrawTools, int Score) {
	char StrBuffer[8]; // ����� ��� ������ �����
	sprintf_s(StrBuffer, 8, " %05i", Score); // ������������ ����� � ������
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]); // �������� ���� ��� �������
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]); // �������� ���� ��� ����������
	Rectangle(DrawTools.Console.cHDC, 5 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 8.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // �������� ���������� �����
	RenderText(DrawTools, StrBuffer, { 5 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false); // ������ �����
}

// ������ �������
void DrawInfoBar(drawtools& DrawTools, map& Map, int FoodCount, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // ������� ������ ��� ���������� ��� ��������
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // ������� ������ ��� ���������� ��� ��������
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // ������� ������ ��� ���������� ��� ��������

	// ���
	SelectObject(cHDC, Pens[GCLR_DARKWOOD]); // �������� ���� ��� �������
	SelectObject(cHDC, Brushes[GCLR_DARKWOOD]); // �������� ���� ��� ����������
	Rectangle(cHDC, 0, 0, Map.Width * DrawTools.TileSize, INFO_BAR_SIZE * DrawTools.TileSize); // ������ ��� ��������

	// ������
	SelectObject(cHDC, Pens[GCLR_SCARLET]); // �������� ���� ��� �������
	SelectObject(cHDC, Brushes[GCLR_SCARLET]); // �������� ���� ��� ����������
	Ellipse(cHDC, DrawTools.TileSize, 0.5 * DrawTools.TileSize, 2 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // ������ ���� ������
	DrawApple(DrawTools, { 1, 0.5 }); // ������ ������
	DrawInfoFoodCount(DrawTools, FoodCount); // ������ �������

	// C���
	SelectObject(cHDC, Pens[GCLR_YELLOW]); // �������� ���� ��� �������
	SelectObject(cHDC, Brushes[GCLR_YELLOW]); // �������� ���� ��� ����������
	Ellipse(cHDC, 4 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 5 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // ������ ���� �����
	DrawStar(DrawTools, { 4.5, 1 }, 0.4); // ������ ������
	DrawInfoScore(DrawTools, Score); // ������ ����
}

// �������� ��� ������� �����
void DrawMenuBackground(drawtools& DrawTools, map& Map) {
	for (int y = 0; y < Map.Height + INFO_BAR_SIZE; y++) // ������ ���
	{
		for (int x = 0; x < Map.Width; x++) // ������������ ��������� ��� �� ���������
		{
			DrawTile(DrawTools, { x,y - INFO_BAR_SIZE }, TILE_EMPTY); // �������� ������� ��������� ������
		}
	}
}