#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;
#pragma comment(lib,"msimg32.lib")
// Глобальные переменные
int MapHeight = 50;
int MapWidth = 50;
int** Map;
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
	TILE_SNAKE_RIGHT
};
enum Directions
{
	DIR_LEFT = -1,
	DIR_RIGHT = 1,
	DIR_UP = -2,
	DIR_DOWN = 2
};
bool MoveSnake(int& HeadPosX, int& HeadPosY, int& TailPosX, int& TailPosY, int NewHead) {

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
	// Добавляем новый сегмент головы
	Map[HeadPosY][HeadPosX] = NewHead;
	if (Map[HeadPosY][HeadPosX] == TILE_SNAKE_UP)
	{
		if (HeadPosY - 1 < 0)
		{
			HeadPosY = MapHeight - 1;
		}
		else if (Map[HeadPosY - 1][HeadPosX] == TILE_EMPTY)
		{
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
		else if (Map[HeadPosY + 1][HeadPosX] == TILE_EMPTY)
		{
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
		else if (Map[HeadPosY][HeadPosX - 1] == TILE_EMPTY)
		{
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
		else if (Map[HeadPosY][HeadPosX + 1] == TILE_EMPTY)
		{
			HeadPosX++;
		}
		else
		{
			return false;
		}
	}
	Map[HeadPosY][HeadPosX] = NewHead;
}
void DrawMap() {
	int TileSize = 15;
	
	//HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleCursorPosition(Console, { 0,0 });

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(ConsoleDisplay, pen);
	SelectObject(ConsoleDisplay, brush);
	//Rectangle(ConsoleDisplay, 0, 0, MapWidth * TileSize + TileSize, MapHeight * TileSize + TileSize);
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			if (Map[y][x] == TILE_WALL)
			{
				pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
				brush = CreateSolidBrush(RGB(255, 255, 255));
			}
			else if (Map[y][x] == TILE_SNAKE_UP)
			{
				pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
				brush = CreateSolidBrush(RGB(0, 255, 255));
			}
			else if (Map[y][x] == TILE_SNAKE_DOWN)
			{
				pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
				brush = CreateSolidBrush(RGB(0, 255, 255));
			}
			else if (Map[y][x] == TILE_SNAKE_LEFT)
			{
				pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
				brush = CreateSolidBrush(RGB(0, 255, 255));
			}
			else if (Map[y][x] == TILE_SNAKE_RIGHT)
			{
				pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
				brush = CreateSolidBrush(RGB(0, 255, 255));				
			}
			else
			{
				pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
				brush = CreateSolidBrush(RGB(100, 100, 100));
			}
			SelectObject(ConsoleDisplay, pen);
			SelectObject(ConsoleDisplay, brush);
			Rectangle(ConsoleDisplay, x * TileSize, y * TileSize, x * TileSize + TileSize, y * TileSize + TileSize);
			DeleteObject(pen);
			DeleteObject(brush);
			//Sleep(10);
		}
		//cout << endl;
	}
	
	
}
void GameInit() {
	
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
	int SnakeHead = TILE_SNAKE_RIGHT;
	int OldSnakeHead = SnakeHead;
	int HeadPosX = MapWidth / 2;
	int HeadPosY = MapHeight / 2;
	Map[HeadPosY][HeadPosX] = SnakeHead;
	Map[HeadPosY][HeadPosX - 1] = SnakeHead;
	int TailPosY = HeadPosY;
	int TailPosX = HeadPosX - 2;
	Map[TailPosY][TailPosX] = SnakeHead;

	// Main loop
	while (true)
	{
		while (_kbhit())
		{
			int Keycode = _getch();
			if (Keycode == 224) Keycode = _getch();
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
		}
		OldSnakeHead = SnakeHead;
		MoveSnake(HeadPosX, HeadPosY, TailPosX, TailPosY, SnakeHead);
		DrawMap();
		//Sleep(100);
	}
	DrawMap();
	system("pause");

}
