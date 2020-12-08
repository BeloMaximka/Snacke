#include "includes.h"
using namespace std;
struct pos
{
	int x;
	int y;	
};
struct map
{
	int** Tiles;
	int Height;
	int Width;
};
struct snake
{
	pos HeadPos;
	pos TailPos;
	int Segments;
	int Head;
	int OldHead;	
};
// Глобальные переменные
map Map = { nullptr,15,15 };
//int MapHeight = 15;
//int MapWidth = 15;
//int** Map;
int TileSize = 32;
// Дескриптор вывода данных
HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
// Дескриптов окна
HWND ConsoleWindow = GetConsoleWindow();
// Дескриптов дисплея
HDC ConsoleDisplay = GetDC(ConsoleWindow);
//----------------------

enum Keys
{
	GMKEY_LEFT = 75,
	GMKEY_RIGHT = 77,
	GMKEY_UP = 72,
	GMKEY_DOWN = 80,
};
enum Tiles
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_SNAKE_UP,
	TILE_SNAKE_DOWN,
	TILE_SNAKE_LEFT,
	TILE_SNAKE_RIGHT,
	TILE_FOOD
};
enum Directions
{
	DIR_LEFT = -1,
	DIR_RIGHT = 1,
	DIR_UP = -2,
	DIR_DOWN = 2
};
const int ColorsAmount = 9;
HPEN Pens[ColorsAmount];
HBRUSH Brushes[ColorsAmount];
enum Colors
{
	GCLR_BLACK,
	GCLR_WHITE,	
	GCLR_YELLOW,
	GCLR_RED,
	GCLR_GREEN,
	GCLR_DARKGREEN,
	GCLR_DARKYELLOW,
	GCLR_BROWN,
	GCLR_LIGHTRED
};
void InitColors() {
	//GCLR_BLACK
	Pens[0] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	Brushes[0] = CreateSolidBrush(RGB(0, 0, 0));
	//GCLR_WHITE
	Pens[1] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	Brushes[1] = CreateSolidBrush(RGB(255, 255, 255));
	//GCLR_YELLOW
	Pens[2] = CreatePen(PS_SOLID, 1, RGB(214, 191, 142));
	Brushes[2] = CreateSolidBrush(RGB(214, 191, 142));
	//GCLR_RED
	Pens[3] = CreatePen(PS_SOLID, 1, RGB(194, 42, 4));
	Brushes[3] = CreateSolidBrush(RGB(194, 42, 4));
	//GCLR_GREEN
	Pens[4] = CreatePen(PS_SOLID, 1, RGB(76, 115, 21));	
	Brushes[4] = CreateSolidBrush(RGB(76, 115, 21));
	//GCLR_DARKGREEN
	Pens[5] = CreatePen(PS_SOLID, 1, RGB(42, 64, 11));
	Brushes[5] = CreateSolidBrush(RGB(42, 64, 11));
	//GCLR_DARKYELLOW
	Pens[6] = CreatePen(PS_SOLID, 1, RGB(211, 205, 0));
	Brushes[6] = CreateSolidBrush(RGB(211, 205, 0));
	//GCLR_BROWN
	Pens[7] = CreatePen(PS_SOLID, 1, RGB(89, 48, 26));
	Brushes[7] = CreateSolidBrush(RGB(89, 48, 26));
	//GCLR_LIGHTRED
	Pens[8] = CreatePen(PS_SOLID, 1, RGB(189, 93, 74));
	Brushes[8] = CreateSolidBrush(RGB(189, 93, 74));
}
void DrawTile(int PosX, int PosY, int TileID) {
	if (TileID == TILE_WALL)
	{
		SelectObject(ConsoleDisplay, Pens[GCLR_WHITE]);
		SelectObject(ConsoleDisplay, Brushes[GCLR_WHITE]);
		Rectangle(ConsoleDisplay, PosX * TileSize, PosY * TileSize, PosX * TileSize + TileSize, PosY * TileSize + TileSize);
	}
	if (TileID == TILE_FOOD)
	{		
		// Выбираем коричневый цвет для обводки
		SelectObject(ConsoleDisplay, Pens[GCLR_BROWN]);
		// Выбираем коричневый цвет для заполнения
		SelectObject(ConsoleDisplay, Brushes[GCLR_BROWN]);
		// Рисуем палочку
		Rectangle(ConsoleDisplay, PosX * TileSize + TileSize / 2 - TileSize / 16, PosY * TileSize + TileSize / 16, PosX * TileSize + TileSize / 2 + TileSize / 16, PosY * TileSize + TileSize);
		// Выбираем красный цвет для обводки
		SelectObject(ConsoleDisplay, Pens[GCLR_RED]);
		// Выбираем красный цвет для заполнения
		SelectObject(ConsoleDisplay, Brushes[GCLR_RED]);
		// Рисуем левую дольку
		Ellipse(ConsoleDisplay, PosX * TileSize + TileSize/16, PosY * TileSize + TileSize*0.2, PosX * TileSize + TileSize/1.6, PosY * TileSize + TileSize);
		// Рисуем правую дольку
		Ellipse(ConsoleDisplay, PosX * TileSize +TileSize - TileSize / 16, PosY * TileSize + TileSize *0.2, PosX * TileSize +TileSize- TileSize / 1.6, PosY * TileSize + TileSize);
		// Выбираем зеленый цвет для обводки
		SelectObject(ConsoleDisplay, Pens[GCLR_GREEN]);
		// Выбираем зеленый цвет для заполнения
		SelectObject(ConsoleDisplay, Brushes[GCLR_GREEN]);
		// Рисуем листок
		Ellipse(ConsoleDisplay, PosX * TileSize + TileSize/2 - TileSize / 16, PosY * TileSize, PosX * TileSize + TileSize  -TileSize / 16, PosY * TileSize + TileSize/6);
		// Выбираем красный цвет для обводки
		SelectObject(ConsoleDisplay, Pens[GCLR_LIGHTRED]);
		// Выбираем красный цвет для заполнения
		SelectObject(ConsoleDisplay, Brushes[GCLR_LIGHTRED]);
		// Рисуем блик
		Ellipse(ConsoleDisplay, PosX * TileSize + TileSize / 2.5, PosY * TileSize + TileSize / 1.8, PosX * TileSize + TileSize / 2.5 - TileSize / 6, PosY * TileSize + TileSize / 2 - TileSize / 5);
		
	}
	else
	{
		
		SelectObject(ConsoleDisplay, Pens[GCLR_YELLOW]);
		SelectObject(ConsoleDisplay, Brushes[GCLR_YELLOW]);
		Rectangle(ConsoleDisplay, PosX * TileSize, PosY * TileSize, PosX * TileSize + TileSize, PosY * TileSize + TileSize);
	}
	
}
bool SpawnFood() {
	int FoodPosX;
	int FoodPosY;
	for (int i = 0; i < Map.Height*Map.Width; i++)
	{
		FoodPosY = rand() % Map.Height;
		FoodPosX = rand() % Map.Width;
		if (Map.Tiles[FoodPosY][FoodPosX] == TILE_EMPTY)
		{
			Map.Tiles[FoodPosY][FoodPosX] = TILE_FOOD;
			DrawTile(FoodPosX, FoodPosY, TILE_FOOD);
			return true;
		}
	}
	if (rand()%2)
	{
		if (rand()%2)
		{
			// двигаемся с левого верхнего угла
			for (int y = 0; y < Map.Height; y++)
			{
				for (int x = 0; x < Map.Width; x++)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY)
					{
						Map.Tiles[y][x] = TILE_FOOD;
						DrawTile(x, y, TILE_FOOD);
						return true;
					}
				}
			}
		}
		else
		{
			// я правого верхнего угла
			for (int y = 0; y < Map.Height; y++)
			{
				for (int x = Map.Width = 1; x > 0; x--)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY)
					{
						Map.Tiles[y][x] = TILE_FOOD;
						DrawTile(x, y, TILE_FOOD);
						return true;
					}
				}
			}
		}
	}
	else
	{
		if (rand() % 2)
		{
			// двигаемся с левого нижнего угла
			for (int y = Map.Height - 1; y > 0; y--)
			{
				for (int x = 0; x < Map.Width; x++)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY)
					{
						Map.Tiles[y][x] = TILE_FOOD;
						DrawTile(x, y, TILE_FOOD);
						return true;
					}
				}
			}
		}
		else
		{
			// двигаемся с правого нижнего угла
			for (int y = Map.Height - 1; y > 0; y--)
			{
				for (int x = Map.Width = 1; x > 0; x--)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY)
					{
						Map.Tiles[y][x] = TILE_FOOD;
						DrawTile(x, y, TILE_FOOD);
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool MoveSnake(snake& Snake) {	
	// Поворачиваем сегмент перед головой
	Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] == TILE_SNAKE_UP)
	{
		if (Snake.HeadPos.y - 1 < 0)
		{			
			if (Map.Tiles[Map.Height - 1][Snake.HeadPos.x] == TILE_EMPTY || Map.Tiles[Map.Height - 1][Snake.HeadPos.x] == TILE_FOOD)
			{
				Snake.HeadPos.y = Map.Height - 1;
			}
			else
			{
				return false;
			}
			
		}
		else if (Map.Tiles[Snake.HeadPos.y - 1][Snake.HeadPos.x] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y - 1][Snake.HeadPos.x] == TILE_FOOD)
		{			
			Snake.HeadPos.y--;
		}
		else
		{
			return false;
		}
	}
	else if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] == TILE_SNAKE_DOWN)
	{
		if (Snake.HeadPos.y + 1 >= Map.Height)
		{
			if (Map.Tiles[0][Snake.HeadPos.x] == TILE_EMPTY || Map.Tiles[0][Snake.HeadPos.x] == TILE_FOOD)
			{
				Snake.HeadPos.y = 0;
			}
			else
			{
				return false;
			}			
		}
		else if (Map.Tiles[Snake.HeadPos.y + 1][Snake.HeadPos.x] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y + 1][Snake.HeadPos.x] == TILE_FOOD)
		{		
			Snake.HeadPos.y++;
		}
		else
		{
			return false;
		}
	}
	else if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] == TILE_SNAKE_LEFT)
	{
		if (Snake.HeadPos.x - 1 < 0 )
		{
			if (Map.Tiles[Snake.HeadPos.y][Map.Width - 1] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y][Map.Width - 1] == TILE_FOOD)
			{
				Snake.HeadPos.x = Map.Width - 1;
			}
			else
			{
				return false;
			}			
		}
		else if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x - 1] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x - 1] == TILE_FOOD)
		{			
			Snake.HeadPos.x--;
		}
		else
		{
			return false;
		}
	}
	else if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] == TILE_SNAKE_RIGHT)
	{
		if (Snake.HeadPos.x + 1 >= Map.Width)
		{
			if (Map.Tiles[Snake.HeadPos.y][0] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y][0] == TILE_FOOD)
			{
				Snake.HeadPos.x = 0;
			}
			else
			{
				return false;
			}			
		}
		else if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x + 1] == TILE_EMPTY || Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x + 1] == TILE_FOOD)
		{		
			Snake.HeadPos.x++;
		}
		else
		{
			return false;
		}
	}
	if (Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] == TILE_FOOD)
	{
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		SpawnFood();
		Snake.Segments++;
		return true;
	}
	else
	{
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	}	
	
	DrawTile(Snake.TailPos.x, Snake.TailPos.y, TILE_EMPTY);
	// Определяем новый конец
	if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP)
	{		
		// Удаляем конец хвоста
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		if (Snake.TailPos.y - 1 < 0)
		{
			Snake.TailPos.y = Map.Height - 1;
		}
		else
		{
			Snake.TailPos.y--;
		}

	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_DOWN)
	{
		// Удаляем конец хвоста
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		if (Snake.TailPos.y + 1 >= Map.Height)
		{
			Snake.TailPos.y = 0;
		}
		else
		{
			Snake.TailPos.y++;
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_LEFT)
	{
		// Удаляем конец хвоста
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		if (Snake.TailPos.x - 1 < 0)
		{
			Snake.TailPos.x = Map.Width - 1;
		}
		else
		{
			Snake.TailPos.x--;
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_RIGHT)
	{
		// Удаляем конец хвоста
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		if (Snake.TailPos.x + 1 >= Map.Width)
		{
			Snake.TailPos.x = 0;
		}
		else
		{
			Snake.TailPos.x++;
		}
	}
	
	return true;
}
void DrawSnakeBodyPart(pos Pos, int TileID, int SegmentsAmount, int SegmentNumber) {
	Pos.x = Pos.x*TileSize + TileSize / 2;
	Pos.y = Pos.y *TileSize + TileSize / 2;
	int Width;
	Width = TileSize * (0.5 + ((double)SegmentNumber / SegmentsAmount) / 4);
	SelectObject(ConsoleDisplay, Pens[GCLR_GREEN]);
	SelectObject(ConsoleDisplay, Brushes[GCLR_GREEN]);
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
		if (SegmentNumber==SegmentsAmount)
		{
			Ellipse(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
			// глазки
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKYELLOW]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKYELLOW]);
			Ellipse(ConsoleDisplay, Pos.x - Width / 2 + TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x - Width / 2 + TileSize/4, Pos.y - TileSize / 2 + TileSize/3);
			Ellipse(ConsoleDisplay, Pos.x + Width / 2 - TileSize / 16, Pos.y - TileSize / 2 + TileSize / 8, Pos.x + Width / 2 - TileSize / 4, Pos.y - TileSize / 2 + TileSize / 3);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y - TileSize / 4);
			Ellipse(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
		}
		else
		{
			Rectangle(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
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
			Ellipse(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
			// глазки
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKYELLOW]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKYELLOW]);
			Ellipse(ConsoleDisplay, Pos.x - Width / 2 + TileSize / 16, Pos.y + TileSize / 2 - TileSize / 8, Pos.x - Width / 2 + TileSize / 4, Pos.y + TileSize / 2 - TileSize / 3);
			Ellipse(ConsoleDisplay, Pos.x + Width / 2 - TileSize / 16, Pos.y + TileSize / 2 - TileSize / 8, Pos.x + Width / 2 - TileSize / 4, Pos.y + TileSize / 2 - TileSize / 3);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(ConsoleDisplay, Pos.x - Width / 2, Pos.y, Pos.x + Width / 2, Pos.y + TileSize / 2);
			Ellipse(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
		}
		else
		{
			Rectangle(ConsoleDisplay, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
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
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
			// глазки
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKYELLOW]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKYELLOW]);			
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 2 + TileSize / 8, Pos.y - Width / 2 + TileSize / 16, Pos.x - TileSize / 2 + TileSize / 3, Pos.y - Width / 2 + TileSize / 4);
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 2 + TileSize / 8, Pos.y + Width / 2 - TileSize / 16, Pos.x - TileSize / 2 + TileSize / 3, Pos.y + Width / 2 - TileSize / 4);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(ConsoleDisplay, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x, Pos.y + Width / 2);
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
		}
		else
		{
			Rectangle(ConsoleDisplay, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
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
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
			// глазки
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKYELLOW]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKYELLOW]);
			Ellipse(ConsoleDisplay, Pos.x + TileSize / 2 - TileSize / 8, Pos.y - Width / 2 + TileSize / 16, Pos.x + TileSize / 2 - TileSize / 3, Pos.y - Width / 2 + TileSize / 4);
			Ellipse(ConsoleDisplay, Pos.x + TileSize / 2 - TileSize / 8, Pos.y + Width / 2 - TileSize / 16, Pos.x + TileSize / 2 - TileSize / 3, Pos.y + Width / 2 - TileSize / 4);
		}
		else if (SegmentNumber == 1)
		{
			Rectangle(ConsoleDisplay, Pos.x, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			Ellipse(ConsoleDisplay, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
		}
		else
		{
			Rectangle(ConsoleDisplay, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			SelectObject(ConsoleDisplay, Pens[GCLR_DARKGREEN]);
			SelectObject(ConsoleDisplay, Brushes[GCLR_DARKGREEN]);
			Polygon(ConsoleDisplay, PolyPoints, 4);
		}		
	}
}
void DrawSnake(snake& Snake, int TailTile) {
	int SegmentNumber = 0;
	int SegmentTile = TailTile;
	pos DrawPos = Snake.TailPos;
	DrawTile(DrawPos.x, DrawPos.y, TILE_EMPTY);
	while (true)
	{
		SegmentNumber++;		
		DrawSnakeBodyPart(DrawPos, SegmentTile, Snake.Segments, SegmentNumber);
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
			DrawTile(DrawPos.x, DrawPos.y, TILE_EMPTY);
			DrawSnakeBodyPart(DrawPos, TILE_SNAKE_DOWN, Snake.Segments, SegmentNumber);
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
			DrawTile(DrawPos.x, DrawPos.y, TILE_EMPTY);
			DrawSnakeBodyPart(DrawPos, TILE_SNAKE_UP, Snake.Segments, SegmentNumber);
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
			DrawTile(DrawPos.x, DrawPos.y, TILE_EMPTY);
			DrawSnakeBodyPart(DrawPos, TILE_SNAKE_RIGHT, Snake.Segments, SegmentNumber);
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
			DrawTile(DrawPos.x, DrawPos.y, TILE_EMPTY);
			DrawSnakeBodyPart(DrawPos, TILE_SNAKE_LEFT, Snake.Segments, SegmentNumber);
		}
		SegmentTile = Map.Tiles[DrawPos.y][DrawPos.x];
	}
}
void DrawMap() {
	for (int y = 0; y < Map.Height; y++)
	{
		for (int x = 0; x < Map.Width; x++)
		{
			DrawTile(x, y, Map.Tiles[y][x]);
		}		
	}		

}
void GameInit() {

	// Рандом
	srand(time(0));
	rand();
	//Инициализация цветов
	InitColors();
	// Махинации с консолью
	SetConsoleTitle(TEXT("AMAZE!"));

	// Отключаем ненужные флаги
	DWORD ConMode;
	HANDLE Console = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(Console, &ConMode);
	SetConsoleMode(Console, ConMode & ENABLE_EXTENDED_FLAGS);

	// Задаем нужный шрифт
	CONSOLE_FONT_INFOEX CFI;
	CFI.cbSize = sizeof CFI;
	CFI.nFont = 0;
	CFI.dwFontSize.X = 12;
	CFI.dwFontSize.Y = 16;
	CFI.FontFamily = FF_DONTCARE;
	CFI.FontWeight = FW_NORMAL;
	wcscpy_s(CFI.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(ConsoleOutput, FALSE, &CFI);
	// Задаем размеры буфера консоли
	system("mode con cols=115 lines=52");

	// Центрируем консоль
	RECT rectClient, rectWindow;
	GetClientRect(ConsoleWindow, &rectClient);
	GetWindowRect(ConsoleWindow, &rectWindow);
	int posx, posy;
	posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
	MoveWindow(ConsoleWindow, posx, posy, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, TRUE);
	SetWindowLong(ConsoleWindow, GWL_STYLE, GetWindowLong(ConsoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Прячем курсор
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = false;
	Cursor.dwSize = 100;
	SetConsoleCursorInfo(ConsoleOutput, &Cursor);
}
int main()
{
	GameInit();
	// Map init

	Map.Tiles = new int* [Map.Height];
	for (int y = 0; y < Map.Height; y++)
	{
		Map.Tiles[y] = new int[Map.Width];
		for (int x = 0; x < Map.Width; x++)
		{
			if (x == 0 || y == 0 || x == Map.Width - 1 || y == Map.Height - 1)
			{
				Map.Tiles[y][x] = TILE_WALL;
			}
			else
			{
				Map.Tiles[y][x] = TILE_EMPTY;
			}
			// это пременно
			Map.Tiles[y][x] = TILE_EMPTY;
		}
	}
	while (true)
	{
		// Snake init		
		snake Snake;
		Snake.Segments = 5;
		Snake.Head = TILE_SNAKE_RIGHT;
		Snake.OldHead = Snake.Head;
		Snake.HeadPos = { 4,1 };
		// Временно
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x - 1] = Snake.Head;
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x - 2] = Snake.Head;
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x - 3] = Snake.Head;
		Snake.TailPos.y = Snake.HeadPos.y;
		Snake.TailPos.x = Snake.HeadPos.x - 4;
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = Snake.Head;
		SpawnFood();
		//Timer init
		int UpdateDelayMiliseconds = 100;				
		DrawMap();
		DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
		// Main loop
		while (true)
		{
			if (_kbhit())
			{
				if (_kbhit())
				{
					int Keycode = _getch();
					if (Keycode == 224) Keycode = _getch();
					//FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
					if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN)
					{
						Snake.Head = TILE_SNAKE_UP;
					}
					else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP)
					{
						Snake.Head = TILE_SNAKE_DOWN;
					}
					else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT)
					{
						Snake.Head = TILE_SNAKE_LEFT;
					}
					else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT)
					{
						Snake.Head = TILE_SNAKE_RIGHT;
					}
					if (Snake.Head != Snake.OldHead)
					{
						Snake.OldHead = Snake.Head;
						if (!MoveSnake(Snake))
						{
							DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}
						DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						Sleep(UpdateDelayMiliseconds);
					}
				}
				if (_kbhit())
				{
					int Keycode = _getch();
					if (Keycode == 224) Keycode = _getch();
					// чистим буфер, чтобы не задать огромное количетсво действий напереёд
					FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
					if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN)
					{
						Snake.Head = TILE_SNAKE_UP;
					}
					else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP)
					{
						Snake.Head = TILE_SNAKE_DOWN;
					}
					else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT)
					{
						Snake.Head = TILE_SNAKE_LEFT;
					}
					else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT)
					{
						Snake.Head = TILE_SNAKE_RIGHT;
					}
					if (Snake.Head != Snake.OldHead)
					{
						Snake.OldHead = Snake.Head;
						if (!MoveSnake(Snake))
						{
							DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}
						DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						Sleep(UpdateDelayMiliseconds);
					}
				}
			}
			//else if (CheckTimerEnd(&UpdateTimer))
			//{
			//	// Перезапускаем таймер
			//	StartTimer(&UpdateTimer);
			//	OldSnakeHead = SnakeHead;
			//	MoveSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, SnakeHead);
			//	DrawMap();
			//}
			else
			{
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(Snake))
				{
					DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					break;
				}
				DrawSnake(Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				Sleep(UpdateDelayMiliseconds);
			}
		}
		Beep(500, 700);
		Beep(300, 700);
		Beep(200, 700);
		for (int y = 0; y < Map.Height; y++)
		{
			Map.Tiles[y] = new int[Map.Width];
			for (int x = 0; x < Map.Width; x++)
			{				
				Map.Tiles[y][x] = TILE_EMPTY;
			}
		}		
	}
	
	system("pause");

}
