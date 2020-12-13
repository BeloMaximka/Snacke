#pragma once
#include "includes.h"
void DrawTile(drawtools& DrawTools, pos Pos, int TileID) {
	Pos.y += INFO_BAR_SIZE;
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	int& TileSize = DrawTools.TileSize;
	if (TileID == TILE_WALL)
	{
		SelectObject(cHDC, Pens[GCLR_WHITE]);
		SelectObject(cHDC, Brushes[GCLR_WHITE]);
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize);
	}
	if (TileID == TILE_FOOD)
	{
		DrawTile(DrawTools, Pos, TILE_EMPTY);
		// Выбираем коричневый цвет для обводки
		SelectObject(cHDC, Pens[GCLR_BROWN]);
		// Выбираем коричневый цвет для заполнения
		SelectObject(cHDC, Brushes[GCLR_BROWN]);
		// Рисуем палочку
		Rectangle(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16, Pos.y * TileSize + TileSize / 16, Pos.x * TileSize + TileSize / 2 + TileSize / 16, Pos.y * TileSize + TileSize);
		// Выбираем красный цвет для обводки
		SelectObject(cHDC, Pens[GCLR_RED]);
		// Выбираем красный цвет для заполнения
		SelectObject(cHDC, Brushes[GCLR_RED]);
		// Рисуем левую дольку
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 16, Pos.y * TileSize + TileSize * 0.2, Pos.x * TileSize + TileSize / 1.6, Pos.y * TileSize + TileSize);
		// Рисуем правую дольку
		Ellipse(cHDC, Pos.x * TileSize + TileSize - TileSize / 16, Pos.y * TileSize + TileSize * 0.2, Pos.x * TileSize + TileSize - TileSize / 1.6, Pos.y * TileSize + TileSize);
		// Выбираем зеленый цвет для обводки
		SelectObject(cHDC, Pens[GCLR_GREEN]);
		// Выбираем зеленый цвет для заполнения
		SelectObject(cHDC, Brushes[GCLR_GREEN]);
		// Рисуем листок
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 2 - TileSize / 16, Pos.y * TileSize, Pos.x * TileSize + TileSize - TileSize / 16, Pos.y * TileSize + TileSize / 6);
		// Выбираем красный цвет для обводки
		SelectObject(cHDC, Pens[GCLR_LIGHTRED]);
		// Выбираем красный цвет для заполнения
		SelectObject(cHDC, Brushes[GCLR_LIGHTRED]);
		// Рисуем блик
		Ellipse(cHDC, Pos.x * TileSize + TileSize / 2.5, Pos.y * TileSize + TileSize / 1.8, Pos.x * TileSize + TileSize / 2.5 - TileSize / 6, Pos.y * TileSize + TileSize / 2 - TileSize / 5);
	}
	else
	{
		SelectObject(cHDC, Pens[GCLR_YELLOW]);
		SelectObject(cHDC, Brushes[GCLR_YELLOW]);
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize);
	}
}
void DrawSnakeBodyPart(drawtools DrawTools, pos Pos, int TileID, int SegmentsAmount, int SegmentNumber) {	
	Pos.y += INFO_BAR_SIZE;
	HDC& cHDC = DrawTools.Console.cHDC;
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens;
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes;
	int& TileSize = DrawTools.TileSize;	
	Pos.x = Pos.x * TileSize + TileSize / 2;	
	Pos.y = Pos.y * TileSize + TileSize / 2;
	
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
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]);
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]);
			Polygon(cHDC, PolyPoints, 4);
			// глазки
			SelectObject(cHDC, Pens[GCLR_DARKYELLOW]);
			SelectObject(cHDC, Brushes[GCLR_DARKYELLOW]);
			Ellipse(cHDC, Pos.x - Width / 2 + TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x - Width / 2 + TileSize / 4, Pos.y - TileSize / 2 + TileSize / 3);
			Ellipse(cHDC, Pos.x + Width / 2 - TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x + Width / 2 - TileSize / 4, Pos.y - TileSize / 2 + TileSize / 3);
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
			// глазки
			SelectObject(cHDC, Pens[GCLR_DARKYELLOW]);
			SelectObject(cHDC, Brushes[GCLR_DARKYELLOW]);
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
			// глазки
			SelectObject(cHDC, Pens[GCLR_DARKYELLOW]);
			SelectObject(cHDC, Brushes[GCLR_DARKYELLOW]);
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
			// глазки
			SelectObject(cHDC, Pens[GCLR_DARKYELLOW]);
			SelectObject(cHDC, Brushes[GCLR_DARKYELLOW]);
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
void DrawSnake(drawtools& DrawTools, map& Map, snake& Snake, int TailTile) {

	int SegmentNumber = 0;
	int SegmentTile = TailTile;
	pos DrawPos = Snake.TailPos;	
	DrawTile(DrawTools, DrawPos, TILE_EMPTY);
	while (true)
	{
		SegmentNumber++;
		DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber);
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
void DrawMap(drawtools& DrawTools, map& Map) {
	for (int y = 0; y < Map.Height; y++)
	{
		for (int x = 0; x < Map.Width; x++)
		{
			DrawTile(DrawTools, { x, y }, Map.Tiles[y][x]);
		}
	}

}