#include "includes.h"
using namespace std;
// Глобальные переменные
#define TILESIZE 32

void MapInit(map& Map, int MapHeight, int MapWidth) {
	Map.Height = MapHeight;
	Map.Width = MapWidth;
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
}
void InitPalette(palette& Palette) {
	//GCLR_BLACK
	Palette.Colors.push_back(RGB(0, 0, 0));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BLACK]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BLACK]));
	//GCLR_WHITE
	Palette.Colors.push_back(RGB(255, 255, 255));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_WHITE]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_WHITE]));
	//GCLR_YELLOW
	Palette.Colors.push_back(RGB(214, 191, 142));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_YELLOW]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_YELLOW]));
	//GCLR_RED
	Palette.Colors.push_back(RGB(194, 42, 4));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_RED]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_RED]));
	//GCLR_GREEN
	Palette.Colors.push_back(RGB(76, 115, 21));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_GREEN]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_GREEN]));
	//GCLR_DARKGREEN
	Palette.Colors.push_back(RGB(42, 64, 11));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKGREEN]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKGREEN]));
	//GCLR_DARKYELLOW
	Palette.Colors.push_back(RGB(211, 205, 0));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKYELLOW]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKYELLOW]));
	//GCLR_BROWN
	Palette.Colors.push_back(RGB(89, 48, 26));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BROWN]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BROWN]));
	//GCLR_LIGHTRED
	Palette.Colors.push_back(RGB(189, 93, 74));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_LIGHTRED]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_LIGHTRED]));
}
bool SpawnFood(drawtools& DrawTools, map& Map) {
	pos FoodPos;
	for (int i = 0; i < Map.Height * Map.Width; i++)
	{
		FoodPos.y = rand() % Map.Height;
		FoodPos.x = rand() % Map.Width;
		if (Map.Tiles[FoodPos.y][FoodPos.x] == TILE_EMPTY)
		{
			Map.Tiles[FoodPos.y][FoodPos.x] = TILE_FOOD;
			DrawTile(DrawTools, FoodPos, TILE_FOOD);
			return true;
		}
	}
	if (rand() % 2)
	{
		if (rand() % 2)
		{
			// двигаемся с левого верхнего угла
			for (int y = 0; y < Map.Height; y++)
			{
				for (int x = 0; x < Map.Width; x++)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY)
					{
						Map.Tiles[y][x] = TILE_FOOD;
						DrawTile(DrawTools, { x, y }, TILE_FOOD);
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
						DrawTile(DrawTools, { x, y }, TILE_FOOD);
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
						DrawTile(DrawTools, { x, y }, TILE_FOOD);
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
						DrawTile(DrawTools, { x, y }, TILE_FOOD);
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool MoveSnake(drawtools& DrawTools, map& Map, snake& Snake) {
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
		if (Snake.HeadPos.x - 1 < 0)
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
		SpawnFood(DrawTools, Map);
		Snake.Segments++;
		return true;
	}
	else
	{
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	}

	DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY);
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
void GameInit(drawtools& DrawTools) {
	// Дескриптов консоли
	DrawTools.Console.cHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	// Дескриптов окна
	DrawTools.Console.cHWND = GetConsoleWindow();
	// Дескриптов дисплея
	DrawTools.Console.cHDC = GetDC(DrawTools.Console.cHWND);
	// Размер плиток
	DrawTools.TileSize = TILESIZE;
	// Рандом
	srand(time(0));
	rand();
	//Инициализация цветов
	InitPalette(DrawTools.Palette);
	// Махинации с консолью
	SetConsoleTitle(TEXT("SNACKE!"));

	// Отключаем ненужные флаги
	DWORD ConMode;
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(ConsoleInput, &ConMode);
	SetConsoleMode(ConsoleInput, ConMode & ENABLE_EXTENDED_FLAGS);

	// Задаем нужный шрифт
	CONSOLE_FONT_INFOEX CFI;
	CFI.cbSize = sizeof CFI;
	CFI.nFont = 0;
	CFI.dwFontSize.X = 12;
	CFI.dwFontSize.Y = 16;
	CFI.FontFamily = FF_DONTCARE;
	CFI.FontWeight = FW_NORMAL;
	wcscpy_s(CFI.FaceName, L"Terminal");
	SetCurrentConsoleFontEx(DrawTools.Console.cHANDLE, FALSE, &CFI);
	// Задаем размеры буфера консоли
	system("mode con cols=115 lines=52");

	// Центрируем консоль
	RECT rectClient, rectWindow;
	GetClientRect(DrawTools.Console.cHWND, &rectClient);
	GetWindowRect(DrawTools.Console.cHWND, &rectWindow);
	int posx, posy;
	posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2;
	posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2;
	MoveWindow(DrawTools.Console.cHWND, posx, posy, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, TRUE);
	SetWindowLong(DrawTools.Console.cHWND, GWL_STYLE, GetWindowLong(DrawTools.Console.cHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Прячем курсор
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = false;
	Cursor.dwSize = 100;
	SetConsoleCursorInfo(DrawTools.Console.cHANDLE, &Cursor);
}
int main()
{
	drawtools DrawTools;
	GameInit(DrawTools);
	// Map init
	map Map;
	MapInit(Map, 15, 15);

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
		SpawnFood(DrawTools, Map);
		//Timer init
		int UpdateDelayMiliseconds = 500;
		DrawMap(DrawTools, Map);
		DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
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
						if (!MoveSnake(DrawTools, Map, Snake))
						{
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
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
						if (!MoveSnake(DrawTools, Map, Snake))
						{
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
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
				if (!MoveSnake(DrawTools, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					break;
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
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
