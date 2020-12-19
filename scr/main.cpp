#include "includes.h"
using namespace std;

void MapInit(map& Map, int MapHeight, int MapWidth) {
	Map.Height = MapHeight;
	Map.Width = MapWidth;
	Map.Tiles = new int* [Map.Height];
	Map.Walls = false;
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
	_itoa_s(1.0 * DrawTools.TileSize / CFI.dwFontSize.Y * (MapHeight + INFO_BAR_SIZE), ItoaBuffer, 10);
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

	BASS_Init(-1, 44100, 0, 0, nullptr); // Инициализируем BASS	
	Sleep(200); // Даем окну возможность развернуться
}

int main()
{
	int SnakeDelay;

	// Map init
	map Map;
	MapInit(Map, 15, 18);
	//
	drawtools DrawTools;
	GameInit(DrawTools, Map.Height, Map.Width);

	// Sound init
	audiotools Audio;
	SoundsInit(Audio);

	saveddata Data;
	if (!ReadFileData(Data, Audio, SnakeDelay, Map))
	{
		CreateFileData(Data, Audio, SnakeDelay, Map);
	}
	if (Data.FirstStart)
	{
		InfoMenu(DrawTools, Audio, Map);
		Data.FirstStart = false;
		SaveFileData(Data, Audio, Map, SnakeDelay);
	}
	// Set background music
	PlaySoundB(Audio, GSND_MUSIC, Audio.MusicVolumePercent, true);
	MainMenu(DrawTools, Audio, Map, Data, SnakeDelay);
	system("pause");

}
