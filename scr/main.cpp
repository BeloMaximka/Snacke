/*-------------------------------------------------------------------
	main.cpp

	Технические моменты и непосредственно точка начала приложения
-------------------------------------------------------------------*/

#include "includes.h" // Подключем .h файл с инклюдами

#define FONT_NORMAL_THICKNESS 0 // Обычная толщина шрифта
#define TILESIZE 32 // Размер плитки

// Выделяет память под карту
void MapInit(map& Map, int MapHeight, int MapWidth) {
	Map.Height = MapHeight; // Задаем высоту карты	
	Map.Width = MapWidth; // Задаем ширину карты	
	Map.Tiles = new int* [Map.Height]; // Поскольку у нас двумерный динамический массив, выделяем память под указатели	
	for (int y = 0; y < Map.Height; y++) // Выделяем память на каждую линию карты
	{
		Map.Tiles[y] = new int[Map.Width]; // Выделяем память с помощью оператора new
	}
}

// Создание шрифта в памяти
void InitFont(HFONT& HFont, int FontSize, int Thickness) {
	LOGFONTA Font; // Создаем шрифт, вручную задаем ему параметры	
	Font.lfHeight = FontSize; // Высота
	Font.lfWidth = 0; // Ширина. Если 0, то определяет по высоте
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
	HFont = CreateFontIndirectA(&Font); // Передаем переменной полученный шрифт
}

// Инициализация цветов в векторе цветов
void InitPalette(palette& Palette) {
	//GCLR_BLACK
	Palette.Colors.push_back(RGB(0, 0, 0)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BLACK])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BLACK])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_WHITE
	Palette.Colors.push_back(RGB(255, 255, 255)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_WHITE])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_WHITE])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_LIGHTBURLYWOOD
	Palette.Colors.push_back(RGB(214, 191, 142)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_LIGHTBURLYWOOD])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_LIGHTBURLYWOOD])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_BURLYWOOD
	Palette.Colors.push_back(RGB(201, 180, 133)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BURLYWOOD])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BURLYWOOD])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_DARKBURLYWOOD
	Palette.Colors.push_back(RGB(150, 134, 99)); // Добавляем в конец вектора элемент COLORREF	
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKBURLYWOOD])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKBURLYWOOD])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_DARKWOOD		
	Palette.Colors.push_back(RGB(125, 112, 82)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKWOOD])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKWOOD])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_SCARLET
	Palette.Colors.push_back(RGB(212, 46, 4)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_SCARLET])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_SCARLET])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_RED
	Palette.Colors.push_back(RGB(194, 42, 4)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_RED])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_RED])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_DARKRED
	Palette.Colors.push_back(RGB(148, 32, 3)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKRED])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKRED])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_GREEN
	Palette.Colors.push_back(RGB(76, 115, 21)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_GREEN])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_GREEN])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_DARKGREEN
	Palette.Colors.push_back(RGB(42, 64, 11)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKGREEN])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKGREEN])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_YELLOW
	Palette.Colors.push_back(RGB(211, 205, 0)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_YELLOW])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_YELLOW])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_DARKYELLOW
	Palette.Colors.push_back(RGB(161, 155, 0)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_DARKYELLOW])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_DARKYELLOW])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_BROWN
	Palette.Colors.push_back(RGB(89, 48, 26)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_BROWN])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_BROWN])); // Добавляем в конец вектора элемент HBRUSH
	//GCLR_LIGHTRED
	Palette.Colors.push_back(RGB(189, 93, 74)); // Добавляем в конец вектора элемент COLORREF
	Palette.Pens.push_back(CreatePen(PS_SOLID, 1, Palette.Colors[GCLR_LIGHTRED])); // Добавляем в конец вектора элемент HPEN
	Palette.Brushes.push_back(CreateSolidBrush(Palette.Colors[GCLR_LIGHTRED])); // Добавляем в конец вектора элемент HBRUSH
}

// Инициализация всех необходимых компонентов приложения
void GameInit(drawtools& DrawTools, int MapHeight, int MapWidth) {
	DrawTools.Console.cHANDLE = GetStdHandle(STD_OUTPUT_HANDLE); // Получаем дескриптор консоли	
	DrawTools.Console.cHWND = GetConsoleWindow(); // Получаем дескриптор окна	
	DrawTools.Console.cHDC = GetDC(DrawTools.Console.cHWND); // Получаем дескриптор дисплея	
	DrawTools.TileSize = TILESIZE; // Размер плиток	

	// Получаем значени для переменной, отвечающей за сворачивание-разворачивание окна
	WINDOWPLACEMENT WinPlacement; // Некоторые свойства окна
	GetWindowPlacement(DrawTools.Console.cHWND, &WinPlacement); // Получаем свойства окна
	DrawTools.WindowState = WinPlacement.showCmd; // Состояние окна(свернутое, развернутое)	

	// Рандом
	srand(time(0)); // Кидаем в рандом seed текущего времени
	rand(); // Запускаем рандом один раз. Первый раз более предсказуемый

	InitPalette(DrawTools.Palette); //Инициализация цветов
	// Махинации с консолью
	SetConsoleTitle(TEXT("SNACKE!")); // Выставляем название окна

	// Отключаем ненужные флаги
	DWORD ConMode; // Для хранения флагов консоли
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE); // Дескриптор ввода
	GetConsoleMode(ConsoleInput, &ConMode); // Получаем флаги
	SetConsoleMode(ConsoleInput, ConMode & ENABLE_EXTENDED_FLAGS); // Отключаем ненужные флаги (выделение и т.д.)

	// Задаем нужный шрифт консоли. Все это нужно для правильного отображения плиток
	CONSOLE_FONT_INFOEX CFI; // Для хранения шрифта
	CFI.cbSize = sizeof(CONSOLE_FONT_INFOEX); // Размер этой структуры в байтах
	CFI.nFont = 0; // Индекс шрифта
	CFI.dwFontSize.X = 8; // Размер шрифта по ширине
	CFI.dwFontSize.Y = 16; // Размер шрифта по высоте
	CFI.FontFamily = FF_DONTCARE; // Семейство шрифтов
	CFI.FontWeight = FW_NORMAL; // Толщина шрифта
	wcscpy_s(CFI.FaceName, L"Consolas"); // Шрифт Consolas
	SetCurrentConsoleFontEx(DrawTools.Console.cHANDLE, FALSE, &CFI); // Задаем шрифт

	// Задаем размеры буфера консоли
	std::string ConBufferStr = "mode con cols="; // Разбиваем строчку на куски, чтобы можно было вставить свои значения
	char ItoaBuffer[5]; // Буфер для текстового представления значений высоты и ширины
	_itoa_s(2.0 * DrawTools.TileSize / CFI.dwFontSize.Y * MapWidth, ItoaBuffer, 10); // Переводим размеры окна в плитах в размер буфера (2.0 * ибо ширина 8)
	ConBufferStr += ItoaBuffer; // Добавляем буфер в строку
	ConBufferStr += " lines="; // Добавляем в строку параметр линий
	_itoa_s(1.0 * DrawTools.TileSize / CFI.dwFontSize.Y * (MapHeight + INFO_BAR_SIZE), ItoaBuffer, 10); // Переводим размеры окна в плитах в размер буфера (2.0 * ибо высота 16)
	ConBufferStr += ItoaBuffer; // Добавляем буфер в строку
	system(ConBufferStr.c_str()); // Вводим в консоль команду выставления буфера окна

	// Центрируем консоль
	RECT ClientRect; // Размеры рабочей облайсти
	RECT WindowRect; // Размеры окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Получаем размеры рабочей области
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Получаем размеры окна
	pos WindowPos; // Позиция окна
	WindowPos.x = GetSystemMetrics(SM_CXSCREEN) / 2 - (WindowRect.right - WindowRect.left) / 2; // Находим позицию центра экрана по ширине
	WindowPos.y = GetSystemMetrics(SM_CYSCREEN) / 2 - (WindowRect.bottom - WindowRect.top) / 2;// Находим позицию центра экрана по высоте
	MoveWindow(DrawTools.Console.cHWND, WindowPos.x, WindowPos.y, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE); // Перемещаем окно в центр
	// Убираем окну возможность масштабирования и развернуться на полный экран
	SetWindowLong(DrawTools.Console.cHWND, GWL_STYLE, GetWindowLong(DrawTools.Console.cHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Инициализация шрифтов
	InitFont(DrawTools.SmallFont, DrawTools.TileSize / 1.3, FONT_NORMAL_THICKNESS); //Инициализация маленького шрифта	
	InitFont(DrawTools.NormalFont, DrawTools.TileSize, FONT_NORMAL_THICKNESS); //Инициализация обычного шрифта	
	InitFont(DrawTools.BigFont, (WindowRect.bottom - WindowRect.top) / 8, FONT_NORMAL_THICKNESS); //Инициализация большого шрифта 	
	InitFont(DrawTools.TitleFont, (WindowRect.bottom - WindowRect.top) / 4, FONT_NORMAL_THICKNESS); //Инициализация заголовочного шрифта

	// Прячем каретка
	CONSOLE_CURSOR_INFO Cursor; // Для хранения информации о каретка
	Cursor.bVisible = false; // Делаем невидимую
	Cursor.dwSize = 100; // Задаем размеры на полную. Это важно для десятки
	SetConsoleCursorInfo(DrawTools.Console.cHANDLE, &Cursor); // Задаем информацию о каретке

	BASS_Init(-1, 44100, 0, 0, nullptr); // Инициализируем BASS
	Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Даем окну возможность развернуться перед тем, как рендерить что-либо
}

// Тут всё начинается
int main()
{
	int SnakeDelay; // Для хранения задержки между шагами змейки

	map Map; // Карта, по которой бегает змейка
	MapInit(Map, 15, 18); // Выделяем память под карту и задаем ей размеры

	drawtools DrawTools; // Переменные, небходимые для работы с выводом
	GameInit(DrawTools, Map.Height, Map.Width); // Инициализируем основные компоненты игры

	audiotools Audio; // Переменные, необходимые для работы со звуком
	SoundsInit(Audio); // Инициализируем audiotools

	saveddata Data; // Данные, которые будут сохранятся в бинарник

	if (!ReadFileData(Data, Audio, SnakeDelay, Map)) // Пытаемся прочитать данные с файла
	{
		CreateFileData(Data, Audio, SnakeDelay, Map); // Если не получилось, создаем новый файл с базовыми настройками
	}

	if (Data.FirstStart) // Если пользователь в первый раз запустил приложение
	{
		InfoMenu(DrawTools, Audio, Map); // Показываем инфо меню
		Data.FirstStart = false; // Отмечаем, что первый запуск уже был
		SaveFileData(Data, Audio, Map, SnakeDelay); // Сохраняем данные о первом запуске
	}

	PlaySoundB(Audio, GSND_MUSIC, Audio.MusicVolumePercent, true); // Запускаем фоновую музыку
	MainMenu(DrawTools, Audio, Map, Data, SnakeDelay); // Запускаем меню
}
