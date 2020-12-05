#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "cpptimer.h"
using namespace std;
#pragma comment(lib,"msimg32.lib")
// Глобальные переменные
int MapHeight = 50;
int MapWidth = 50;
int** Map;
int TileSize = 16;
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
const int ColorsAmount = 5;
HPEN Pens[ColorsAmount];
HBRUSH Brushes[ColorsAmount];
enum Colors
{
	GCLR_BLACK,
	GCLR_WHITE,	
	GCLR_YELLOW,
	GCLR_RED,
	GCLR_GREEN
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
	Pens[3] = CreatePen(PS_SOLID, 1, RGB(227, 46, 46));
	Brushes[3] = CreateSolidBrush(RGB(227, 46, 46));
	//GCLR_GREEN
	Pens[4] = CreatePen(PS_SOLID, 1, RGB(76, 115, 21));	
	Brushes[4] = CreateSolidBrush(RGB(76, 115, 21));
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
		SelectObject(ConsoleDisplay, Pens[GCLR_RED]);
		SelectObject(ConsoleDisplay, Brushes[GCLR_RED]);
		Ellipse(ConsoleDisplay, PosX * TileSize, PosY * TileSize, PosX * TileSize + TileSize, PosY * TileSize + TileSize);
	}
	else
	{
		//SelectObject(ConsoleDisplay, Pens[GCLR_BLACK]);
		SelectObject(ConsoleDisplay, Pens[GCLR_YELLOW]);
		SelectObject(ConsoleDisplay, Brushes[GCLR_YELLOW]);
		Rectangle(ConsoleDisplay, PosX * TileSize, PosY * TileSize, PosX * TileSize + TileSize, PosY * TileSize + TileSize);
	}
	
}
void SpawnFood() {
	int FoodPosX;
	int FoodPosY;
	while (true)
	{
		FoodPosY = rand() % MapHeight;
		FoodPosX = rand() % MapWidth;
		if (Map[FoodPosY][FoodPosX] == TILE_EMPTY)
		{
			Map[FoodPosY][FoodPosX] = TILE_FOOD;
			DrawTile(FoodPosX, FoodPosY, TILE_FOOD);
			break;
		}
	}
}
bool MoveSnake(int& HeadPosX, int& HeadPosY, int& TailPosX, int& TailPosY, int NewHead, int& SegmentsCount) {
	bool FoodEaten = false;
	// Поворачиваем сегмент перед головой
	Map[HeadPosY][HeadPosX] = NewHead;
	if (Map[HeadPosY][HeadPosX] == TILE_SNAKE_UP)
	{
		if (HeadPosY - 1 < 0)
		{
			HeadPosY = MapHeight - 1;
		}
		else if (Map[HeadPosY - 1][HeadPosX] == TILE_EMPTY || Map[HeadPosY - 1][HeadPosX] == TILE_FOOD)
		{
			if (Map[HeadPosY - 1][HeadPosX] == TILE_FOOD)
			{
				FoodEaten = true;
			}
			
			HeadPosY--;
		}
		else
		{
			return false;
		}
	}
	else if (Map[HeadPosY][HeadPosX] == TILE_SNAKE_DOWN)
	{
		if (HeadPosY + 1 >= MapHeight)
		{
			HeadPosY = 0;
		}
		else if (Map[HeadPosY + 1][HeadPosX] == TILE_EMPTY || Map[HeadPosY + 1][HeadPosX] == TILE_FOOD)
		{
			if (Map[HeadPosY + 1][HeadPosX] == TILE_FOOD)
			{
				FoodEaten = true;
			}			
			HeadPosY++;
		}
		else
		{
			return false;
		}
	}
	else if (Map[HeadPosY][HeadPosX] == TILE_SNAKE_LEFT)
	{
		if (HeadPosX - 1 < 0)
		{
			HeadPosX = MapWidth - 1;
		}
		else if (Map[HeadPosY][HeadPosX - 1] == TILE_EMPTY || Map[HeadPosY][HeadPosX - 1] == TILE_FOOD)
		{
			if (Map[HeadPosY][HeadPosX - 1] == TILE_FOOD)
			{
				FoodEaten = true;
			}			
			HeadPosX--;
		}
		else
		{
			return false;
		}
	}
	else if (Map[HeadPosY][HeadPosX] == TILE_SNAKE_RIGHT)
	{
		if (HeadPosX + 1 >= MapWidth)
		{
			HeadPosX = 0;
		}
		else if (Map[HeadPosY][HeadPosX + 1] == TILE_EMPTY || Map[HeadPosY][HeadPosX + 1] == TILE_FOOD)
		{
			if (Map[HeadPosY][HeadPosX + 1] == TILE_FOOD)
			{
				FoodEaten = true;
			}			
			HeadPosX++;
		}
		else
		{
			return false;
		}
	}
	// Добавляем сегмент головы
	Map[HeadPosY][HeadPosX] = NewHead;	
	if (FoodEaten)
	{
		SpawnFood();
		SegmentsCount++;
		return true;
	}
	DrawTile(TailPosX, TailPosY, TILE_EMPTY);
	// Определяем новый конец
	if (Map[TailPosY][TailPosX] == TILE_SNAKE_UP)
	{		
		// Удаляем конец хвоста
		Map[TailPosY][TailPosX] = TILE_EMPTY;
		if (TailPosY - 1 < 0)
		{
			TailPosY = MapHeight - 1;
		}
		else
		{
			TailPosY--;
		}

	}
	else if (Map[TailPosY][TailPosX] == TILE_SNAKE_DOWN)
	{
		// Удаляем конец хвоста
		Map[TailPosY][TailPosX] = TILE_EMPTY;
		if (TailPosY + 1 >= MapHeight)
		{
			TailPosY = 0;
		}
		else
		{
			TailPosY++;
		}
	}
	else if (Map[TailPosY][TailPosX] == TILE_SNAKE_LEFT)
	{
		// Удаляем конец хвоста
		Map[TailPosY][TailPosX] = TILE_EMPTY;
		if (TailPosX - 1 < 0)
		{
			TailPosX = MapWidth - 1;
		}
		else
		{
			TailPosX--;
		}
	}
	else if (Map[TailPosY][TailPosX] == TILE_SNAKE_RIGHT)
	{
		// Удаляем конец хвоста
		Map[TailPosY][TailPosX] = TILE_EMPTY;
		if (TailPosX + 1 >= MapWidth)
		{
			TailPosX = 0;
		}
		else
		{
			TailPosX++;
		}
	}
	
	return true;
}
void DrawSnakeBodyPart(int PosX, int PosY, int TileID, int SegmentsAmount, int SegmentNumber) {
	PosX = PosX*TileSize + TileSize / 2;
	PosY = PosY*TileSize + TileSize / 2;
	int Width = TileSize * (0.5 + ((double)SegmentNumber / SegmentsAmount) / 2);
	SelectObject(ConsoleDisplay, Pens[GCLR_GREEN]);
	SelectObject(ConsoleDisplay, Brushes[GCLR_GREEN]);
	if (TileID == TILE_SNAKE_UP)
	{
		Rectangle(ConsoleDisplay, PosX - Width / 2, PosY - TileSize / 2, PosX + Width / 2, PosY + TileSize / 4);
	}
	else if (TileID == TILE_SNAKE_DOWN)
	{
		Rectangle(ConsoleDisplay, PosX - Width / 2, PosY - TileSize / 4, PosX + Width / 2, PosY + TileSize / 2);
	}
	else if (TileID == TILE_SNAKE_LEFT)
	{
		Rectangle(ConsoleDisplay, PosX - TileSize / 2, PosY - Width / 2, PosX + TileSize / 4, PosY + Width / 2);
	}
	else if (TileID == TILE_SNAKE_RIGHT)
	{
		Rectangle(ConsoleDisplay, PosX - TileSize / 4, PosY - Width / 2, PosX + TileSize / 2, PosY + Width / 2);
	}
}
void DrawSnake(int HeadPosX, int HeadPosY, int TailPosX, int TailPosY, int TailTile, int SegmentsAmount) {
	int SegmentNumber = 0;
	int SegmentTile = TailTile;
	DrawTile(TailPosX, TailPosY, TILE_EMPTY);
	while (true)
	{
		SegmentNumber++;		
		DrawSnakeBodyPart(TailPosX, TailPosY, SegmentTile, SegmentsAmount, SegmentNumber);
		if (TailPosX == HeadPosX && TailPosY == HeadPosY)
		{
			break;
		}
		if (SegmentTile == TILE_SNAKE_UP)
		{			
			if (TailPosY - 1 < 0)
			{
				TailPosY = MapHeight - 1;

			}
			else
			{
				TailPosY--;
			}
			DrawTile(TailPosX, TailPosY, TILE_EMPTY);
			DrawSnakeBodyPart(TailPosX, TailPosY, TILE_SNAKE_DOWN, SegmentsAmount, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_DOWN)
		{			
			if (TailPosY + 1 >= MapHeight)
			{
				TailPosY = 0;

			}
			else
			{
				TailPosY++;
			}
			DrawTile(TailPosX, TailPosY, TILE_EMPTY);
			DrawSnakeBodyPart(TailPosX, TailPosY, TILE_SNAKE_UP, SegmentsAmount, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_LEFT)
		{			
			if (TailPosX - 1 < 0)
			{
				TailPosX = MapWidth - 1;

			}
			else
			{
				TailPosX--;
			}
			DrawTile(TailPosX, TailPosY, TILE_EMPTY);
			DrawSnakeBodyPart(TailPosX, TailPosY, TILE_SNAKE_RIGHT, SegmentsAmount, SegmentNumber);
		}
		else if (SegmentTile == TILE_SNAKE_RIGHT)
		{			
			if (TailPosX + 1 >= MapWidth)
			{
				TailPosX = 0;

			}
			else
			{
				TailPosX++;
			}
			DrawTile(TailPosX, TailPosY, TILE_EMPTY);
			DrawSnakeBodyPart(TailPosX, TailPosY, TILE_SNAKE_LEFT, SegmentsAmount, SegmentNumber);			
		}
		SegmentTile = Map[TailPosY][TailPosX];
	}
}
void DrawMap() {
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			DrawTile(x, y, Map[y][x]);
		}		
	}		

}
void GameInit() {

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

	Map = new int* [MapHeight];
	for (int y = 0; y < MapHeight; y++)
	{
		Map[y] = new int[MapWidth];
		for (int x = 0; x < MapWidth; x++)
		{
			if (x == 0 || y == 0 || x == MapWidth - 1 || y == MapHeight - 1)
			{
				Map[y][x] = TILE_WALL;
			}
			else
			{
				Map[y][x] = TILE_EMPTY;
			}
			Map[y][x] = TILE_EMPTY;
		}
	}
	// Snake init
	int SnakeSegmentsCount = 8;
	int SnakeHead = TILE_SNAKE_RIGHT;
	int OldSnakeHead = SnakeHead;
	int HeadPosX = MapWidth / 2;
	int HeadPosY = MapHeight / 2;
	Map[HeadPosY][HeadPosX] = SnakeHead;
	Map[HeadPosY][HeadPosX - 1] = SnakeHead;	
	Map[HeadPosY][HeadPosX - 2] = SnakeHead;	
	Map[HeadPosY][HeadPosX - 3] = SnakeHead;
	Map[HeadPosY][HeadPosX - 4] = SnakeHead;
	Map[HeadPosY][HeadPosX - 5] = SnakeHead;
	Map[HeadPosY][HeadPosX - 6] = SnakeHead;
	int TailPosY = HeadPosY;
	int TailPosX = HeadPosX - 7;
	Map[TailPosY][TailPosX] = SnakeHead;
	SpawnFood();
	//Timer init
	int UpdateDelayMiliseconds = 30;
	timer UpdateTimer;
	UpdateTimer.DurationMiliseconds = UpdateDelayMiliseconds;
	StartTimer(&UpdateTimer);
	DrawMap();
	DrawSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, Map[TailPosY][TailPosX], SnakeSegmentsCount);
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
				if (Keycode == GMKEY_UP && OldSnakeHead != TILE_SNAKE_DOWN)
				{
					SnakeHead = TILE_SNAKE_UP;
				}
				else if (Keycode == GMKEY_DOWN && OldSnakeHead != TILE_SNAKE_UP)
				{
					SnakeHead = TILE_SNAKE_DOWN;
				}
				else if (Keycode == GMKEY_LEFT && OldSnakeHead != TILE_SNAKE_RIGHT)
				{
					SnakeHead = TILE_SNAKE_LEFT;
				}
				else if (Keycode == GMKEY_RIGHT && OldSnakeHead != TILE_SNAKE_LEFT)
				{
					SnakeHead = TILE_SNAKE_RIGHT;
				}
				if (SnakeHead != OldSnakeHead)
				{
					OldSnakeHead = SnakeHead;
					MoveSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, SnakeHead, SnakeSegmentsCount);
					DrawSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, Map[TailPosY][TailPosX], SnakeSegmentsCount);
					Sleep(UpdateDelayMiliseconds);
				}
			}
			if (_kbhit())
			{
				int Keycode = _getch();
				if (Keycode == 224) Keycode = _getch();
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				if (Keycode == GMKEY_UP && OldSnakeHead != TILE_SNAKE_DOWN)
				{
					SnakeHead = TILE_SNAKE_UP;
				}
				else if (Keycode == GMKEY_DOWN && OldSnakeHead != TILE_SNAKE_UP)
				{
					SnakeHead = TILE_SNAKE_DOWN;
				}
				else if (Keycode == GMKEY_LEFT && OldSnakeHead != TILE_SNAKE_RIGHT)
				{
					SnakeHead = TILE_SNAKE_LEFT;
				}
				else if (Keycode == GMKEY_RIGHT && OldSnakeHead != TILE_SNAKE_LEFT)
				{
					SnakeHead = TILE_SNAKE_RIGHT;
				}
				if (SnakeHead != OldSnakeHead)
				{
					OldSnakeHead = SnakeHead;
					MoveSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, SnakeHead, SnakeSegmentsCount);
					DrawSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, Map[TailPosY][TailPosX], SnakeSegmentsCount);
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
			OldSnakeHead = SnakeHead;
			MoveSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, SnakeHead, SnakeSegmentsCount);
			DrawSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, Map[TailPosY][TailPosX], SnakeSegmentsCount);
			Sleep(UpdateDelayMiliseconds);
		}


	}
	DrawMap();
	system("pause");

}
