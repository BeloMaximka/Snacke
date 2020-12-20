/*-------------------------------------------------------------------
	sound.h

	Звуки и басы.
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами

typedef std::vector<HSTREAM> sounds; // Даем типу удобный псевдоним

// Структуры - пользовательские типы данных
struct pos // Целочисленная позиция
{
	int x; // Позиция по оси х
	int y; // Позиция по оси у
};
struct dpos // Дробная позиция
{
	double x;
	double y;
};
struct map // Карта
{
	int** Tiles; // Двумерный динамический массив клеток
	int Height; // Высота карты
	int Width; // Ширина карты
	bool Walls; // Режим стен
};
struct snake // Змейка
{
	pos HeadPos; // Позиция головы
	pos TailPos; // Позиция хвоста
	int Segments; // Количество сегментов
	int Head; // Направление головы
	int OldHead; // Старое направление головы
	int FoodEaten; // Количество съеденой еды
	int FoodReward; // Награда за следующую еду
	int Score; // Счет
	int Delay; // Задержка между шагами
};
struct console // Дескрипторы консоли
{
	HANDLE cHANDLE; // Дескриптор консоли
	HWND cHWND; // Дескриптор окна
	HDC cHDC; // Дескриптор дисплея
};
struct palette // Цвета
{
	std::vector<HPEN> Pens; // Заливки
	std::vector<HBRUSH> Brushes; // Контуры
	std::vector<COLORREF> Colors; // Цвета RGB
};
struct drawtools // Инструменты, необходимые для работы с рендером
{
	palette Palette; // Палитра цветов
	console Console; // Дескрипторы консоли
	HFONT SmallFont; // Малый шрифт
	HFONT NormalFont; // Обычный шрифт
	HFONT BigFont; // Большой шрифт
	HFONT TitleFont; // Шрифт заголовков
	int TileSize; // Размер плитки
	int WindowState; // Состояние окна (развернутое, свернутое)
};
struct audiotools // Звуки, громкости
{
	sounds Sounds; // Вектор звуков
	double GameVolumePercent; // Громкость игры
	double MusicVolumePercent; // Громкость музыки
};
struct saveddata // Данные, которые сохраняются в файл
{
	double GameVolumePercent; // Громкость игры
	double MusicVolumePercent; // Громкость музыки
	int SnakeDelay; // Задержка между шагами змеи
	bool Walls; // Режим стен
	bool FirstStart; // Первый запуск игры
};

// Перечисления
enum Sounds // Перечисление звуков
{
	GSND_MUSIC, // Музыка
	GSND_CHEW1, // Первый сэмпл хрума
	GSND_CHEW2, // Второй хрум
	GSND_CHEW3, // Третий хрум
	GSND_CHEW4, // Четвертый хрум
	GSND_MENU_MOVEMENT, // Звук перемещения по меню
	GSND_MENU_ENTER, // Звук нажатия Ентером
	GSND_PUNCH // Звук удара об препятствие
};
enum Keys // Коды клавиш
{
	GMKEY_LEFT = 75, // Левая стрелка
	GMKEY_RIGHT = 77, // Правая стрелка
	GMKEY_UP = 72, // Стрелка вверх
	GMKEY_DOWN = 80, // Стрелвка вниз
	GMKEY_ENTER = 13, // Enter
	GMKEY_ESC = 27, // Esc
};
enum Tiles // Плиточки, ячейки, клеточки
{
	TILE_EMPTY, // Пустая
	TILE_WALL, // Стена
	TILE_SNAKE_UP, // Сегмент змеи, направленный вверх
	TILE_SNAKE_DOWN, // Сегмент змеи, направленный вниз
	TILE_SNAKE_LEFT, // Сегмент змеи, направленный влево
	TILE_SNAKE_RIGHT, // Сегмент змеи, направленный вправо
	TILE_FOOD // Еда
};
enum Directions // Направления
{
	DIR_LEFT = -1, // Влево
	DIR_RIGHT = 1, // Вправо
	DIR_UP = -2, // Вверх
	DIR_DOWN = 2 // Вниз
};
enum Colors // Цвета
{
	GCLR_BLACK, // Черный
	GCLR_WHITE, // Белый
	GCLR_LIGHTBURLYWOOD, // Светлый burlywood
	GCLR_BURLYWOOD, // Burlywood
	GCLR_DARKBURLYWOOD, // Темный burlywood
	GCLR_DARKWOOD, // Цвет темноог дерева
	GCLR_SCARLET, // Алый
	GCLR_RED, // Красный
	GCLR_DARKRED, // Темно-красный
	GCLR_GREEN, // Зеленый
	GCLR_DARKGREEN, // Темно-зеленый
	GCLR_YELLOW, // Желтый
	GCLR_DARKYELLOW, // Темно-желтый
	GCLR_BROWN, // Коричневый
	GCLR_LIGHTRED //Светло-красный
};
