#include "includes.h"
using namespace std;

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
void ClearMap(map& Map, bool Walls = false) {
	for (int y = 0; y < Map.Height; y++)
	{
		for (int x = 0; x < Map.Width; x++)
		{
			if (Walls && (x == 0 || y == 0 || x == Map.Width - 1 || y == Map.Height - 1))
			{
				Map.Tiles[y][x] = TILE_WALL;
			}
			else
			{
				Map.Tiles[y][x] = TILE_EMPTY;
			}
		}
	}
}
void MapInit(map& Map, int MapHeight, int MapWidth) {
	Map.Height = MapHeight;
	Map.Width = MapWidth;
	Map.Tiles = new int* [Map.Height];
	for (int y = 0; y < Map.Height; y++)
	{
		Map.Tiles[y] = new int[Map.Width];
	}
}
// Настройка логического шрифта
void InitFont(HFONT& HFont, int FontSize, int Thickness) {
	LOGFONTA Font; // Создаем шрифт, вручную задаем ему параметры	
	Font.lfHeight = FontSize; // Высота	
	//Font.lfWidth = FontSize/2; // Ширина	
	Font.lfEscapement = 0; // Наклон строки текста
	Font.lfOrientation = 0; // Поворот букв 
	Font.lfWeight = Thickness; // Толщина, 0 - базовое значение
	Font.lfItalic = false; // Курсив
	Font.lfUnderline = false; // Нижнее подчёркивание
	Font.lfStrikeOut = false; // Зачеркнутость
	Font.lfCharSet = RUSSIAN_CHARSET; // Набор символов
	Font.lfOutPrecision = OUT_DEFAULT_PRECIS; // Точность вывода
	Font.lfClipPrecision = CLIP_DEFAULT_PRECIS; // Точность отсечения
	Font.lfQuality = ANTIALIASED_QUALITY; // Качество
	Font.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE; // Не так важно, что это
	Font.lfFaceName[LF_FACESIZE]; // Название шрифта
	strcpy_s(Font.lfFaceName, "Impact"); // Копируем название
	HFont = CreateFontIndirectA(&Font);
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
	//GCLR_LIGHTBURLYWOOD
	Palette.Colors.push_back(RGB(214, 191, 142));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_LIGHTBURLYWOOD]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_LIGHTBURLYWOOD]));
	//GCLR_BURLYWOOD
	Palette.Colors.push_back(RGB(201, 180, 133));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BURLYWOOD]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BURLYWOOD]));
	//GCLR_DARKBURLYWOOD
	Palette.Colors.push_back(RGB(150, 134, 99));
	//Palette.Colors.push_back(RGB(112, 100, 74));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKBURLYWOOD]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKBURLYWOOD]));
	//GCLR_DARKWOOD		
	Palette.Colors.push_back(RGB(125, 112, 82));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKWOOD]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKWOOD]));
	//GCLR_SCARLET
	Palette.Colors.push_back(RGB(212, 46, 4));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_SCARLET]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_SCARLET]));
	//GCLR_RED
	Palette.Colors.push_back(RGB(194, 42, 4));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_RED]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_RED]));
	//GCLR_DARKRED
	Palette.Colors.push_back(RGB(148, 32, 3));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKRED]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKRED]));
	//GCLR_GREEN
	Palette.Colors.push_back(RGB(76, 115, 21));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_GREEN]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_GREEN]));
	//GCLR_DARKGREEN
	Palette.Colors.push_back(RGB(42, 64, 11));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKGREEN]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKGREEN]));
	//GCLR_YELLOW
	Palette.Colors.push_back(RGB(211, 205, 0));
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_YELLOW]));
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_YELLOW]));
	//GCLR_DARKYELLOW
	Palette.Colors.push_back(RGB(161, 155, 0));
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
	//--------------РАБОТА С ГОЛОВОЙ--------------
	// Поворачиваем сегмент перед новой головой
	Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	// Позиция новой головы
	pos NewHeadPos = Snake.HeadPos;
	// Определяем позицию новой головы
	// Если голова смотрит вверх
	if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_UP)
	{
		// Если змейка находится на границе карты
		if (NewHeadPos.y - 1 < 0)
		{
			// Перекидываем позицию на другой конец карты
			NewHeadPos.y = Map.Height - 1;
		}
		else
		{
			// Смещаем позицию новой головы вверх
			NewHeadPos.y--;
		}
	}
	// Если голова смотрит вниз
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_DOWN)
	{
		// Если змейка находится на границе карты
		if (NewHeadPos.y + 1 >= Map.Height)
		{
			// Перекидываем позицию на другой конец карты
			NewHeadPos.y = 0;
		}
		else
		{
			// Смещаем позицию новой головы вниз
			NewHeadPos.y++;
		}
	}
	// Если голова смотрит влево
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_LEFT)
	{
		// Если змейка находится на границе карты
		if (NewHeadPos.x - 1 < 0)
		{
			// Перекидываем позицию на другой конец карты
			NewHeadPos.x = Map.Width - 1;
		}
		else
		{
			// Смещаем позицию новой головы влево
			NewHeadPos.x--;
		}
	}
	// Если голова смотрит влево
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_RIGHT)
	{
		if (Snake.HeadPos.x + 1 >= Map.Width)
		{
			// Перекидываем позицию на другой конец карты
			NewHeadPos.x = 0;
		}
		else
		{
			// Смещаем позицию новой головы вправо
			NewHeadPos.x++;
		}
	}
	// Если на пути хвост
	if (NewHeadPos.y == Snake.TailPos.y && NewHeadPos.x == Snake.TailPos.x)
	{
		//--------------РАБОТА С ХВОСТОМ--------------
	// Закрашиваем удаляемый хвост пустой клеткой
		DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY);
		// Определяем новый конец
		if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP)
		{
			// Если 
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
			if (Snake.TailPos.x + 1 >= Map.Width)
			{
				Snake.TailPos.x = 0;
			}
			else
			{
				Snake.TailPos.x++;
			}
		}
		// Обновляем позицию головы змейки
		Snake.HeadPos = NewHeadPos;
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		return true;
	}
	// Если на пути змейки препятствие (т.е не пусто и не еда)
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_EMPTY && Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_FOOD)
	{
		// Движение невозможно - возращаем false
		return false;
	}
	// Если на пути еда
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_FOOD)
	{
		// Обновляем позицию головы змейки
		Snake.HeadPos = NewHeadPos;
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		// Увеличиваем количество сегментов
		Snake.Segments++;
		// Увеличиваем счетчик съеденой еды
		Snake.FoodEaten++;
		// Увеличиваем счет
		Snake.Score += FOOD_SCORE_REWARD;
		// Обновляем счетчик еды на инфобаре
		DrawInfoFoodCount(DrawTools, Snake.FoodEaten);
		// Обновляем счет на инфобаре
		DrawInfoScore(DrawTools, Snake.Score);
		// Создаём ещё одну еду
		SpawnFood(DrawTools, Map);
		// Останавливаем функцию здесь, чтобы не хвост не удалился
		return true;
	}
	// Если на пути пусто
	else
	{
		// Обновляем позицию головы змейки
		Snake.HeadPos = NewHeadPos;
		// Добавляем сегмент головы
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	}
	//--------------РАБОТА С ХВОСТОМ--------------
	// Закрашиваем удаляемый хвост пустой клеткой
	DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY);
	// Определяем новый конец
	if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP)
	{
		// Заменяем удаляемый конец хвоста на пустоту
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		// Если 
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
void GameInit(drawtools& DrawTools, int MapHeight, int MapWidth) {
	// Дескриптов консоли
	DrawTools.Console.cHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	// Дескриптов окна
	DrawTools.Console.cHWND = GetConsoleWindow();
	// Дескриптов дисплея
	DrawTools.Console.cHDC = GetDC(DrawTools.Console.cHWND);
	// Размер плиток
	DrawTools.TileSize = TILESIZE;
	// Состояние окна (свернутое, развернутое)
	WINDOWPLACEMENT WinPlacement;
	GetWindowPlacement(DrawTools.Console.cHWND, &WinPlacement);
	DrawTools.WindowState = WinPlacement.showCmd;
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
	CFI.dwFontSize.X = 8;
	CFI.dwFontSize.Y = 16;
	CFI.FontFamily = FF_DONTCARE;
	CFI.FontWeight = FW_NORMAL;
	wcscpy_s(CFI.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(DrawTools.Console.cHANDLE, FALSE, &CFI);
	// Задаем размеры буфера консоли
	std::string ConBufferStr = "mode con cols=";
	char ItoaBuffer[5];
	_itoa_s(2.0 * DrawTools.TileSize / CFI.dwFontSize.Y * MapWidth, ItoaBuffer, 10);
	ConBufferStr += ItoaBuffer;
	ConBufferStr += " lines=";
	_itoa_s(1.0 * DrawTools.TileSize / CFI.dwFontSize.Y * (MapWidth + INFO_BAR_SIZE), ItoaBuffer, 10);
	ConBufferStr += ItoaBuffer;
	system(ConBufferStr.c_str());
	// Центрируем консоль
	RECT ClientRect, WindowRect;
	GetClientRect(DrawTools.Console.cHWND, &ClientRect);
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect);
	pos WindowPos;
	WindowPos.x = GetSystemMetrics(SM_CXSCREEN) / 2 - (WindowRect.right - WindowRect.left) / 2;
	WindowPos.y = GetSystemMetrics(SM_CYSCREEN) / 2 - (WindowRect.bottom - WindowRect.top) / 2;
	MoveWindow(DrawTools.Console.cHWND, WindowPos.x, WindowPos.y, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);
	SetWindowLong(DrawTools.Console.cHWND, GWL_STYLE, GetWindowLong(DrawTools.Console.cHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	//Инициализация маленького шрифта
	InitFont(DrawTools.SmallFont, DrawTools.TileSize / 1.3, FONT_NORMAL_THICKNESS);
	//Инициализация обычного шрифта
	InitFont(DrawTools.NormalFont, DrawTools.TileSize, FONT_NORMAL_THICKNESS);
	//Инициализация большого шрифта
	InitFont(DrawTools.BigFont, (WindowRect.bottom - WindowRect.top) / 8, FONT_NORMAL_THICKNESS);
	//Инициализация заголовочного шрифта
	InitFont(DrawTools.TitleFont, (WindowRect.bottom - WindowRect.top) / 4, FONT_NORMAL_THICKNESS);
	// Прячем курсор
	CONSOLE_CURSOR_INFO Cursor;
	Cursor.bVisible = false;
	Cursor.dwSize = 100;
	SetConsoleCursorInfo(DrawTools.Console.cHANDLE, &Cursor);
	Sleep(200); // Даем окну возможность развернуться
}
void SpawnSnake(map& Map, snake& Snake, int DirTile, pos HeadPos, int Segments) {
	pos PlacePos = HeadPos;
	pos PlaceDir = { 0,0 };
	if (DirTile == TILE_SNAKE_UP)
	{
		PlaceDir.y = 1;
	}
	else if (DirTile == TILE_SNAKE_DOWN)
	{
		PlaceDir.y = -1;
	}
	else if (DirTile == TILE_SNAKE_LEFT)
	{
		PlaceDir.x = 1;
	}
	else if (DirTile == TILE_SNAKE_RIGHT)
	{
		PlaceDir.x = -1;
	}
	if (PlaceDir.y != 0 && Segments >= Map.Height)
	{
		Segments = Map.Height - 1;
	}
	else if (Segments >= Map.Width)
	{
		Segments = Map.Width - 1;
	}
	for (int i = 0; i < Segments - 1; i++)
	{
		Map.Tiles[PlacePos.y][PlacePos.x] = DirTile;
		if (PlacePos.x + PlaceDir.x < 0)
		{
			PlacePos.x = Map.Width - 1;
		}
		else if (PlacePos.x + PlaceDir.x >= Map.Width)
		{
			PlacePos.x = 0;
		}
		else
		{
			PlacePos.x += PlaceDir.x;
		}
		if (PlacePos.y + PlaceDir.y < 0)
		{
			PlacePos.y = Map.Height - 1;
		}
		else if (PlacePos.y + PlaceDir.y >= Map.Height)
		{
			PlacePos.y = 0;
		}
		else
		{
			PlacePos.y += PlaceDir.y;
		}
	}
	Map.Tiles[PlacePos.y][PlacePos.x] = DirTile;
	Snake.Head = DirTile;
	Snake.HeadPos = HeadPos;
	Snake.OldHead = DirTile;
	Snake.Segments = Segments;
	Snake.TailPos = PlacePos;
	Snake.FoodEaten = 0;
	Snake.Score = 0;
}
bool PauseMenu(drawtools& DrawTools, map& Map, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)	

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение названия игры	

	DrawMap(DrawTools, Map, true); // Отрисовываем карту, инфобар перерисовывать не надо
	RenderText(DrawTools, "GAME PAUSED", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Рисуем паузу	
	int StringsCount = 3; // Сколько будет опций выбора
	string Strings[] = { "Continue", "Exit to main menu", "Exit to desktop" }; // Названия опций выбора	
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 16) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора

	int SelectedButtonNum = 0; // Выбранная опция выбора по умолчанию
	pos ActiveButtonPos = TextLinesCenterPos; // Позиция выбранной опции. Начинаем с центра, будем смещать
	if (StringsCount % 2) // В зависимости от четности будут разные формулы смещения
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // Смещаем позицию к последней
	}
	else // Если четная
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // Смещаем позицию к последней
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // Смещаем к фактической позиции	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем текст
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(200); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново			
			DrawMap(DrawTools, Map, true); // Карта
			DrawInfoBar(DrawTools, Map, FoodEaten, Score); // Инфобар
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "GAME PAUSED", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Тайтл
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				//Rectangle(cHDC, 0, ActiveButtonPos.y, ActiveButtonPos.x + Map.Width / 2 * TileSize, ActiveButtonPos.y + TileSize); // Затираем текст выбранной позиции
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				if (Strings[SelectedButtonNum] == "Continue") // Если выбрали кнопку "Продолжить"
				{
					return false;
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // Если "Выйти"
				{
					return true;
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // Если "Выйти на рабочий стол"
				{
					exit(0); // Закрываем приложение
				}
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
	return false;
}
bool SnakeFirstStep(drawtools& DrawTools, map& Map, snake& Snake) {
	// Пазуа до первого нажатия
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools))
		{
			Sleep(200);
			DrawMap(DrawTools, Map);
			DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
			DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
		}
		if (_kbhit())
		{
			int Keycode = _getch();
			if (Keycode == 224) Keycode = _getch();
			if (Keycode == GMKEY_ENTER)
			{
				if (PauseMenu(DrawTools, Map, Snake.FoodEaten, Snake.Score))
				{
					return false;
				}
				DrawMap(DrawTools, Map);
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
			}
			else if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN)
			{
				Snake.Head = TILE_SNAKE_UP;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				break;
			}
			else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP)
			{
				Snake.Head = TILE_SNAKE_DOWN;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				break;
			}
			else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT)
			{
				Snake.Head = TILE_SNAKE_LEFT;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				break;
			}
			else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT)
			{
				Snake.Head = TILE_SNAKE_RIGHT;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				break;
			}
		}
		Sleep(1);
	}
	return true;
}

bool RetryMenu(drawtools& DrawTools, map& Map, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)	

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение названия игры	

	DrawMap(DrawTools, Map, true); // Отрисовываем карту, инфобар перерисовывать не надо
	RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Рисуем game over
	LOGFONT TempFont;
	GetObject(DrawTools.NormalFont, sizeof(LOGFONT), &TempFont);
	MainTitlePos.y += TempFont.lfHeight * 2;
	std::string ScoreStr = "Your score: ";
	char StrBuffer[SCORE_DIGITS + 1];
	_itoa_s(Score, StrBuffer, SCORE_DIGITS + 1, 10);
	ScoreStr += StrBuffer;
	RenderText(DrawTools, ScoreStr.c_str(), MainTitlePos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем название игры
	int StringsCount = 3; // Сколько будет опций выбора
	string Strings[] = { "Retry", "Exit to main menu", "Exit to desktop" }; // Названия опций выбора	
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 16) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора

	int SelectedButtonNum = 0; // Выбранная опция выбора по умолчанию
	pos ActiveButtonPos = TextLinesCenterPos; // Позиция выбранной опции. Начинаем с центра, будем смещать
	if (StringsCount % 2) // В зависимости от четности будут разные формулы смещения
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // Смещаем позицию к последней
	}
	else // Если четная
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // Смещаем позицию к последней
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // Смещаем к фактической позиции	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем текст
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(200); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново			
			DrawMap(DrawTools, Map, true); // Карта
			DrawInfoBar(DrawTools, Map, FoodEaten, Score); // Инфобар
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Тайтл
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				//Rectangle(cHDC, 0, ActiveButtonPos.y, ActiveButtonPos.x + Map.Width / 2 * TileSize, ActiveButtonPos.y + TileSize); // Затираем текст выбранной позиции
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				if (Strings[SelectedButtonNum] == "Retry") // Если выбрали кнопку ратрая
				{
					return true;
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // Если "Выйти"
				{
					return false;
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // Если "Выйти на рабочий стол"
				{
					exit(0); // Закрываем приложение
				}
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
	return false;
}
void SnakeMainGame(drawtools& DrawTools, map& Map) {
	while (true)
	{
		ClearMap(Map); // Очищаем карту
		// Snake init		
		snake Snake;
		int SnakeSpawnSegments = 3;
		pos SnakeSpawnPos = { SnakeSpawnSegments,Map.Height / 2 };
		SpawnSnake(Map, Snake, TILE_SNAKE_RIGHT, SnakeSpawnPos, SnakeSpawnSegments);
		DrawMap(DrawTools, Map);
		DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
		SpawnFood(DrawTools, Map);
		DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
		//Timer init
		int UpdateDelayMiliseconds = 100;
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		if (!SnakeFirstStep(DrawTools, Map, Snake))
		{
			return;
		}
		Sleep(UpdateDelayMiliseconds);
		// Main loop		
		while (true)
		{
			if (WindowMaximized(DrawTools))
			{
				Sleep(200);
				DrawMap(DrawTools, Map);
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
			}
			if (_kbhit())
			{
				if (_kbhit())
				{
					int Keycode = _getch();
					if (Keycode == 224) Keycode = _getch();
					//FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
					if (Keycode == GMKEY_ENTER)
					{
						if (PauseMenu(DrawTools, Map, Snake.FoodEaten, Snake.Score))
						{
							return;
						}
						DrawMap(DrawTools, Map);
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
						//Sleep(1000);
						if (!SnakeFirstStep(DrawTools, Map, Snake))
						{
							return;
						}
						Sleep(UpdateDelayMiliseconds);
					}
					else if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN)
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
		Beep(500, 400);
		Beep(300, 200);
		Beep(200, 500);
		if (!RetryMenu(DrawTools, Map, Snake.FoodEaten, Snake.Score))
		{
			return;
		}
	}
}
void MainMenu(drawtools& DrawTools, map& Map) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для обводки
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для заливки
	DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение названия игры
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста

	RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Рисуем название игры
	int StringsCount = 3; // Сколько будет опций выбора
	string Strings[] = { "Play", "Settings", "Exit" }; // Названия опций выбора	
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора	
	RenderText(DrawTools, "Controls: Arrow keys, Enter", { MainTitlePos.x,TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем управление сверху
	RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу

	int SelectedButtonNum = 0; // Выбранная опция выбора по умолчанию
	pos ActiveButtonPos = TextLinesCenterPos; // Позиция выбранной опции. Начинаем с центра, будем смещать
	if (StringsCount % 2) // В зависимости от четности будут разные формулы смещения
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // Смещаем позицию к последней
	}
	else // Если четная
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // Смещаем позицию к последней
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // Смещаем к фактической позиции	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем текст
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(200); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				if (Strings[SelectedButtonNum] == "Play") // Если выбрали кнопку "Игать"
				{
					SnakeMainGame(DrawTools, Map); // Запускаем змейку					
					// Вышли из змейки, рисуем меню заново					
					DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста					
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл
				}
				else if (Strings[SelectedButtonNum] == "Exit") // Если "Выйти"
				{
					exit(0); // Закрываем приложение
				}
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
}
int main()
{
	// Map init
	map Map;
	MapInit(Map, 15, 15);
	//
	drawtools DrawTools;
	GameInit(DrawTools, Map.Height, Map.Width);
	//
	MainMenu(DrawTools, Map);
	system("pause");
	SnakeMainGame(DrawTools, Map);
}
