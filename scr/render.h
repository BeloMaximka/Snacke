/*-------------------------------------------------------------------
	render.h

	Всё, что касается отрисовки чего-либо
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами

#define WINDOW_STATE_MAXIMIZED 1 // Состоние развернутого окна
#define WINDOW_STATE_MINIMIZED 2 // Состояние свернутого окна

// Проверяет, развернулось ли окно
bool WindowMaximized(drawtools& DrawTools) {
	WINDOWPLACEMENT WinPlacement; // Некоторые свойства окна
	GetWindowPlacement(DrawTools.Console.cHWND, &WinPlacement); // Получаем свойства окна
	// Если состояние изменилось со свернутого на развернутое
	if (DrawTools.WindowState == WINDOW_STATE_MINIMIZED && WinPlacement.showCmd == WINDOW_STATE_MAXIMIZED)
	{
		DrawTools.WindowState = WinPlacement.showCmd; // Обновляем состояние
		return true; // Окно развернулось - возвращаем true
	}
	else // Если нет
	{
		DrawTools.WindowState = WinPlacement.showCmd; // Обновляем состояние
		return false; // Окно не развернулось - возвращаем false
	}
}

// Поворачивает геометрический вектор на угол в градусах
void RotateVector(dpos& Point, double Angle) {
	Angle *= (M_PI / 180); // Переводим углы в радианную меру
	dpos NewPoint; // Переменная для новой позиции вектора
	NewPoint.x = Point.x * cos(Angle) - Point.y * sin(Angle); // Преобразование координаты x по геометрической формуле
	NewPoint.y = Point.x * sin(Angle) + Point.y * cos(Angle); // Преобразование координаты у по геометрической формуле
	Point = NewPoint; // Присваиваем новую позицию
}

// Рисует звезду заданного размера
void DrawStar(drawtools& DrawTools, dpos Pos, double SizeMod) {
	int TileSize = DrawTools.TileSize; // Перезаписываем значение в другую переменную для удобства чтения

	Pos = { Pos.x * TileSize, Pos.y * TileSize }; // Конвертируем позицию в позицию окна	
	dpos StarOutEndDir = { 0,-1 }; // Начальное направление выпуклых концов звезды
	dpos StarInEndDir = { 0,1 }; // Начальное направление впуклых концов звезды
	POINT PolyPoints[10]; // Точки, которые будут определять позиции концов звезды
	LONG PosX; // Позиция точек звезды по оси X
	LONG PosY; // Позиция точек звезды по оси Y
	double Angle = 72; // Угол поворота вектора до след. точки (360/5 = 72)

	PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod; // Задаем первую внешнюю точку по оси X
	PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod; // Задаем первую внешнюю точку по оси Y
	PolyPoints[0] = { PosX, PosY }; // Копируем точку в массив
	RotateVector(StarInEndDir, -2 * Angle); // Поворачиваем вектор направления внутренней точки против часовой два раза
	PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod; // Задаем первую внутреннюю точку по оси X
	PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod; // Задаем первую внутреннюю точку по оси Y
	PolyPoints[1] = { PosX, PosY }; // Копируем точку в массив
	RotateVector(StarOutEndDir, Angle); // Поворачиваем вектор направления внешней точки по часовой

	for (int i = 2; i < 10; i++) // Повторяем действия выше для остальных точек
	{
		PosX = Pos.x + StarOutEndDir.x * TileSize * SizeMod; // Задаем внешнюю точку по оси X
		PosY = Pos.y + StarOutEndDir.y * TileSize * SizeMod; // Задаем внешнюю точку по оси Y
		PolyPoints[i] = { PosX, PosY }; // Копируем точку в массив
		RotateVector(StarInEndDir, Angle); // Поворачиваем вектор направления внутренней точки по часовой
		i++; // Точку скопировали, увеличиваем управляющую переменную
		PosX = Pos.x + StarInEndDir.x * 0.5 * TileSize * SizeMod; // Задаем внутреннюю точку по оси X
		PosY = Pos.y + StarInEndDir.y * 0.5 * TileSize * SizeMod; // Задаем внутреннюю точку по оси Y
		PolyPoints[i] = { PosX, PosY }; // Копируем точку в массив
		RotateVector(StarOutEndDir, Angle); // Поворачиваем вектор направления внешней точки по часовой
	}

	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKYELLOW]); // Выбираем темно-желтый цвет для обводки
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKYELLOW]); // Выбираем темно-желтый цвет для заполнения
	Polygon(DrawTools.Console.cHDC, PolyPoints, 10); // Рисуем саму звезду по точкам
}

// Рисует яблоко для инфобара
void DrawApple(drawtools& DrawTools, dpos Pos) {
	HDC& cHDC = DrawTools.Console.cHDC; // Создаем ссылку для переменной для удобства
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Создаем ссылку для переменной для удобства
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Создаем ссылку для переменной для удобства
	int TileSize = DrawTools.TileSize; // Копируем значение в другую переменную для удобства
	int SizeMod = TileSize / 16; // Корректировка размеров частей яблока
	pos LeftTopPos; // Левая верхняя точка фигуры
	pos RightBottomPos; // Правая нижняя точка фигуры
	SelectObject(cHDC, Pens[GCLR_DARKRED]); // Выбираем темно-красный цвет для обводки	
	SelectObject(cHDC, Brushes[GCLR_DARKRED]); // Выбираем темно-красный цвет для заполнения
	// Палочка
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // Позиция х левой верхней точки
	LeftTopPos.y = Pos.y * TileSize + TileSize / 16 + SizeMod * 2; // Позиция y левой верхней точки
	RightBottomPos.x = Pos.x * TileSize + TileSize / 2 + TileSize / 16; // Позиция х правой нижней точки 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // Позиция y правой нижней точки
	Rectangle(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем палочку
	// Левая долька
	LeftTopPos.x = Pos.x * TileSize + TileSize / 16 + SizeMod; // Позиция х левой верхней точки
	LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2 + SizeMod; // Позиция y левой верхней точки
	RightBottomPos.x = Pos.x * TileSize + TileSize / 1.6 - SizeMod; // Позиция х правой нижней точки 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // Позиция y правой нижней точки
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем левую дольку	
	// Правая долька
	LeftTopPos.x = Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod; // Позиция х левой верхней точки
	LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2 + SizeMod; // Позиция y левой верхней точки
	RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 1.6 + SizeMod; // Позиция х правой нижней точки 
	RightBottomPos.y = Pos.y * TileSize + TileSize - SizeMod; // Позиция y правой нижней точки
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правую дольку		
	// Листок
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16 + SizeMod / 2; // Позиция х левой верхней точки
	LeftTopPos.y = Pos.y * TileSize + SizeMod * 2; // Позиция y левой верхней точки
	RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 16 - SizeMod * 2; // Позиция х правой нижней точки
	RightBottomPos.y = Pos.y * TileSize + TileSize / 6 + SizeMod; // Позиция y правой нижней точки
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем листок	
	// Блик
	LeftTopPos.x = Pos.x * TileSize + TileSize / 2.5; // Позиция х левой верхней точки
	LeftTopPos.y = Pos.y * TileSize + TileSize / 1.8; // Позиция y левой верхней точки
	RightBottomPos.x = Pos.x * TileSize + TileSize / 2.5 - TileSize / 6; // Позиция х правой нижней точки 
	RightBottomPos.y = Pos.y * TileSize + TileSize / 2 - TileSize / 5; // Позиция y правой нижней точки
	Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем блик
}

// Рисует плитку но заданным координатам
void DrawTile(drawtools& DrawTools, pos Pos, int TileID, bool NoApples = false) {
	Pos.y += INFO_BAR_SIZE; // Смещаем коорды вниз, ибо сверху инфобар
	HDC& cHDC = DrawTools.Console.cHDC; // Создаем ссылку для переменной для удобства
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Создаем ссылку для переменной для удобства
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Создаем ссылку для переменной для удобства
	int TileSize = DrawTools.TileSize; // Копируем значение в другую переменную для удобства	
	if (TileID == TILE_WALL) // Если стена
	{
		SelectObject(cHDC, Pens[GCLR_DARKBURLYWOOD]); // Выбираем цвет обводки
		SelectObject(cHDC, Brushes[GCLR_DARKBURLYWOOD]); // Выбираем цвет заполнения
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize); // Рисуем квадрат
	}
	else if (TileID == TILE_FOOD && !NoApples)
	{
		pos LeftTopPos; // Левая верхняя точка фигуры
		pos RightBottomPos; // Правая нижняя точка фигуры
		DrawTile(DrawTools, { Pos.x, Pos.y - INFO_BAR_SIZE }, TILE_EMPTY); // Сначала затираем клетку фоном

		// Палочка
		SelectObject(cHDC, Pens[GCLR_BROWN]); // Выбираем коричневый цвет для обводки		
		SelectObject(cHDC, Brushes[GCLR_BROWN]); // Выбираем коричневый цвет для заполнения
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // Позиция х левой верхней точки
		LeftTopPos.y = Pos.y * TileSize + TileSize / 16; // Позиция y левой верхней точки
		RightBottomPos.x = Pos.x * TileSize + TileSize / 2 + TileSize / 16; // Позиция х правой нижней точки 
		RightBottomPos.y = Pos.y * TileSize + TileSize; // Позиция y правой нижней точки
		Rectangle(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем палочку		

		SelectObject(cHDC, Pens[GCLR_RED]); // Выбираем красный цвет для обводки		
		SelectObject(cHDC, Brushes[GCLR_RED]); // Выбираем красный цвет для заполнения
		// Левая долька
		LeftTopPos.x = Pos.x * TileSize + TileSize / 16; // Позиция х левой верхней точки
		LeftTopPos.y = Pos.y * TileSize + TileSize * 0.2; // Позиция y левой верхней точки
		RightBottomPos.x = Pos.x * TileSize + TileSize / 1.6; // Позиция х правой нижней точки 		
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем левую дольку		
		// Правая долька
		LeftTopPos.x = Pos.x * TileSize + TileSize - TileSize / 16; // Позиция х левой верхней точки		
		RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 1.6; // Позиция х правой нижней точки 		
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правую дольку

		// Листок
		SelectObject(cHDC, Pens[GCLR_GREEN]); // Выбираем зеленый цвет для обводки		
		SelectObject(cHDC, Brushes[GCLR_GREEN]); // Выбираем зеленый цвет для заполнения		
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2 - TileSize / 16; // Позиция х левой верхней точки
		LeftTopPos.y = Pos.y * TileSize; // Позиция y левой верхней точки
		RightBottomPos.x = Pos.x * TileSize + TileSize - TileSize / 16; // Позиция х правой нижней точки 
		RightBottomPos.y = Pos.y * TileSize + TileSize / 6; // Позиция y правой нижней точки
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем листок		

		// Блик		
		SelectObject(cHDC, Pens[GCLR_LIGHTRED]); // Выбираем красный цвет для обводки		
		SelectObject(cHDC, Brushes[GCLR_LIGHTRED]); // Выбираем красный цвет для заполнения
		LeftTopPos.x = Pos.x * TileSize + TileSize / 2.5; // Позиция х левой верхней точки
		LeftTopPos.y = Pos.y * TileSize + TileSize / 1.8; // Позиция y левой верхней точки
		RightBottomPos.x = Pos.x * TileSize + TileSize / 2.5 - TileSize / 6; // Позиция х правой нижней точки 
		RightBottomPos.y = Pos.y * TileSize + TileSize / 2 - TileSize / 5; // Позиция y правой нижней точки
		Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем блик		
	}
	else
	{
		if (Pos.x % 2 == Pos.y % 2) // Если обе четные или нечетные
		{
			SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбираем цвет для заполнения
		}
		else // Если не
		{
			SelectObject(cHDC, Pens[GCLR_BURLYWOOD]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_BURLYWOOD]); // Выбираем цвет для заполнения
		}
		Rectangle(cHDC, Pos.x * TileSize, Pos.y * TileSize, Pos.x * TileSize + TileSize, Pos.y * TileSize + TileSize); // Рисуем квадрат
	}
}

// Рисует сегмент змейки
void DrawSnakeBodyPart(drawtools DrawTools, pos Pos, int TileID, int SegmentsAmount, int SegmentNumber, bool Smashed = false) {
	Pos.y += INFO_BAR_SIZE; // Смещаем коорды вниз, ибо сверху инфобар
	HDC& cHDC = DrawTools.Console.cHDC; // Создаем ссылку для переменной для удобства
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Создаем ссылку для переменной для удобства
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Создаем ссылку для переменной для удобства
	int TileSize = DrawTools.TileSize; // Копируем значение в другую переменную для удобства	
	Pos.x = Pos.x * TileSize + TileSize / 2; // Конвертируем x координаты в координаты окна
	Pos.y = Pos.y * TileSize + TileSize / 2; // Конвертируем y координаты в координаты окна

	if (Smashed && SegmentNumber + 1 == SegmentsAmount) // Если врезался и сегмент головы
	{
		SegmentNumber++; // Увеличиваем номер сегмента на один (особенности рендера)
		if (TileID == TILE_SNAKE_UP) // Если смотрит вверх
		{
			Pos.y -= TileSize / 2; // Смещаем наверх
		}
		else if (TileID == TILE_SNAKE_DOWN) // Если смотрит вниз
		{
			Pos.y += TileSize / 2; // Смещаем вниз
		}
		else if (TileID == TILE_SNAKE_LEFT) // Если смотрит влево
		{
			Pos.x -= TileSize / 2; // Смещаем влево
		}
		else if (TileID == TILE_SNAKE_RIGHT) // Если смотрти вправо
		{
			Pos.x += TileSize / 2; // Смещаем вправо
		}
	}

	int Width; // Ширина сегмента
	Width = TileSize * (0.5 + ((double)SegmentNumber / SegmentsAmount) / 4); // Вычисляем ширину сегмента. Чем дальше семгент - тем уже
	SelectObject(cHDC, Pens[GCLR_GREEN]); // Выбираем цвет для обводки
	SelectObject(cHDC, Brushes[GCLR_GREEN]); // Выбираем цвет для заполнения
	POINT PolyPoints[4]; // Точки для отрисовки ромба
	pos LeftTopPos; // Левая верхняя точка фигуры
	pos RightBottomPos; // Правая нижняя точка фигуры

	if (TileID == TILE_SNAKE_UP) // Если смотрит вверх
	{
		// Ромбовидный узор
		PolyPoints[0].x = Pos.x - Width / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[0].y = (Pos.y * 2 - TileSize / 4) / 2; // Устанавливаем координаты y точки ромба
		PolyPoints[1].x = Pos.x; // Устанавливаем координаты х точки ромба
		PolyPoints[1].y = Pos.y + TileSize / 4; // Устанавливаем координаты y точки ромба
		PolyPoints[2].x = Pos.x + Width / 2 - 1; // Устанавливаем координаты х точки ромба
		PolyPoints[2].y = (Pos.y * 2 - TileSize / 4) / 2; // Устанавливаем координаты y точки ромба
		PolyPoints[3].x = Pos.x; // Устанавливаем координаты х точки ромба
		PolyPoints[3].y = Pos.y - TileSize / 2; // Устанавливаем координаты y точки ромба
		if (SegmentNumber == SegmentsAmount) // Если голова
		{
			if (Smashed) // Если врезалась
			{
				// Голова
				int HalfHeadPosY = Pos.y / TileSize * TileSize; // Координата у половины головы
				LeftTopPos.x = Pos.x - Width / 2; // Позиция х левой верхней точки
				LeftTopPos.y = HalfHeadPosY - TileSize / 2; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2; // Позиция х правой нижней точки 
				RightBottomPos.y = HalfHeadPosY + TileSize / 2; // Позиция y правой нижней точки
				// Рисуем половину головы
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfHeadPosY, RightBottomPos.x, HalfHeadPosY);

				// Узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				PolyPoints[0].x = Pos.x - Width / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[0].y = HalfHeadPosY; // Устанавливаем координаты y точки треугольника
				PolyPoints[1].x = Pos.x + Width / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[1].y = HalfHeadPosY; // Устанавливаем координаты y точки треугольника
				PolyPoints[2].x = Pos.x; // Устанавливаем координаты х точки треугольника
				PolyPoints[2].y = HalfHeadPosY + TileSize / 2; // Устанавливаем координаты y точки треугольника
				Polygon(cHDC, PolyPoints, 3); // Рисуем треугольник

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				Pos.y += TileSize / 4 + TileSize / 24; // Смещение позиции вниз
				int HalfEyePosY = Pos.y + 11 * TileSize / 48 - TileSize / 2; // Координата у половины глаз
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // Позиция y правой нижней точки
				// Рисуем левый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfEyePosY, RightBottomPos.x, HalfEyePosY);
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				// Рисуем правый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfEyePosY, LeftTopPos.x, HalfEyePosY);

			}
			else // Если не врезались
			{
				// Рисуем голову
				Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);

				// Ромбовидный узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // Позиция y правой нижней точки
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем левый глазик
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правый глазик
			}
		}
		else if (SegmentNumber == 1) // Если хвост
		{
			// Рисуем квадратную половину хвоста
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y - TileSize / 4);
			// Рисуем круглую половину хвоста
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
		else // Если промежотчный сегмент
		{
			// Рисуем сегмент
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 2, Pos.x + Width / 2, Pos.y + TileSize / 4);
			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
	}
	else if (TileID == TILE_SNAKE_DOWN) // Если смотрит вниз
	{
		// Координаты ромбовидного узора
		PolyPoints[0].x = Pos.x - Width / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[0].y = (Pos.y * 2 + TileSize / 4) / 2; // Устанавливаем координаты y точки ромба
		PolyPoints[1].x = Pos.x; // Устанавливаем координаты х точки ромба
		PolyPoints[1].y = Pos.y + TileSize / 2 - 1; // Устанавливаем координаты y точки ромба
		PolyPoints[2].x = Pos.x + Width / 2 - 1; // Устанавливаем координаты х точки ромба
		PolyPoints[2].y = (Pos.y * 2 + TileSize / 4) / 2; // Устанавливаем координаты y точки ромба
		PolyPoints[3].x = Pos.x; // Устанавливаем координаты х точки ромба
		PolyPoints[3].y = Pos.y - TileSize / 4; // Устанавливаем координаты y точки ромба
		if (SegmentNumber == SegmentsAmount) // Если голова
		{
			if (Smashed) // Если врезалась
			{
				// Голова
				int HalfHeadPosY = Pos.y / TileSize * TileSize; // Координата у половины головы
				LeftTopPos.x = Pos.x - Width / 2; // Позиция х левой верхней точки
				LeftTopPos.y = HalfHeadPosY - TileSize / 2; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2; // Позиция х правой нижней точки 
				RightBottomPos.y = HalfHeadPosY + TileSize / 2; // Позиция y правой нижней точки
				// Рисуем половину головы
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfHeadPosY, LeftTopPos.x, HalfHeadPosY);

				// Узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				PolyPoints[0].x = Pos.x - Width / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[0].y = HalfHeadPosY; // Устанавливаем координаты y точки треугольника
				PolyPoints[1].x = Pos.x + Width / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[1].y = HalfHeadPosY; // Устанавливаем координаты y точки треугольника
				PolyPoints[2].x = Pos.x; // Устанавливаем координаты х точки треугольника
				PolyPoints[2].y = HalfHeadPosY - TileSize / 2; // Устанавливаем координаты y точки треугольника
				Polygon(cHDC, PolyPoints, 3); // Рисуем треугольник

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				Pos.y += TileSize / 4 + TileSize / 24; // Смещение позиции вниз
				int HalfEyePosY = Pos.y + 11 * TileSize / 48 - TileSize / 2; // Координата у половины глаз
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - TileSize / 2 + TileSize / 8; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - TileSize / 2 + TileSize / 3; // Позиция y правой нижней точки
				// Рисуем левый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, RightBottomPos.x, HalfEyePosY, LeftTopPos.x, HalfEyePosY);
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				// Рисуем правый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, LeftTopPos.x, HalfEyePosY, RightBottomPos.x, HalfEyePosY);
			}
			else // Если не врезалась
			{
				// Рисуем голову
				Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);

				// Ромбовидный узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				LeftTopPos.x = Pos.x - Width / 2 + TileSize / 16; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y + TileSize / 2 - TileSize / 8; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - Width / 2 + TileSize / 4; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y + TileSize / 2 - TileSize / 3; // Позиция y правой нижней точки
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисем левый глазик
				LeftTopPos.x = Pos.x + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.x = Pos.x + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правый глазик
			}

		}
		else if (SegmentNumber == 1) // Если хвост
		{
			// Рисуем квадратную половину хвоста
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// Рисуем круглую половину хвоста
			Ellipse(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
		else // Если промежотчный сегмент
		{
			// Рисуем сегмент
			Rectangle(cHDC, Pos.x - Width / 2, Pos.y - TileSize / 4, Pos.x + Width / 2, Pos.y + TileSize / 2);
			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
	}
	else if (TileID == TILE_SNAKE_LEFT) // Если смотрит влево
	{
		// Координаты ромбовидного узора
		PolyPoints[0].x = Pos.x - TileSize / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[0].y = Pos.y; // Устанавливаем координаты y точки ромба
		PolyPoints[1].x = (Pos.x * 2 - TileSize / 4) / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[1].y = Pos.y + Width / 2 - 1; // Устанавливаем координаты y точки ромба
		PolyPoints[2].x = Pos.x + TileSize / 4 - 1; // Устанавливаем координаты х точки ромба 
		PolyPoints[2].y = Pos.y; // Устанавливаем координаты y точки ромба
		PolyPoints[3].x = (Pos.x * 2 - TileSize / 4) / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[3].y = Pos.y - Width / 2; // Устанавливаем координаты y точки ромба

		if (SegmentNumber == SegmentsAmount) // Если голова
		{
			if (Smashed) // Если врезалась
			{
				// Голова
				int HalfHeadPosX = Pos.x / TileSize * TileSize; // Координата x половины головы		
				LeftTopPos.x = HalfHeadPosX - TileSize / 2; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2; // Позиция y левой верхней точки
				RightBottomPos.x = HalfHeadPosX + TileSize / 2; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y + Width / 2; // Позиция y правой нижней точки
				// Рисуем половину головы				
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfHeadPosX, RightBottomPos.y, HalfHeadPosX, LeftTopPos.y);

				// Узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				PolyPoints[0].x = HalfHeadPosX; // Устанавливаем координаты х точки треугольника
				PolyPoints[0].y = Pos.y - Width / 2; // Устанавливаем координаты y точки треугольника
				PolyPoints[1].x = HalfHeadPosX; // Устанавливаем координаты х точки треугольника
				PolyPoints[1].y = Pos.y + Width / 2; // Устанавливаем координаты y точки треугольника
				PolyPoints[2].x = HalfHeadPosX + TileSize / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[2].y = Pos.y; // Устанавливаем координаты y точки треугольника
				Polygon(cHDC, PolyPoints, 3); // Рисуем треугольник

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				Pos.x += TileSize / 4 + TileSize / 24; // Смещение позиции вправо				
				int HalfEyePosX = Pos.x + 11 * TileSize / 48 - TileSize / 2; // Координата у половины глаз
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // Позиция y правой нижней точки
				// Рисуем левый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, RightBottomPos.y, HalfEyePosX, LeftTopPos.y);
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				// Рисуем правый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, LeftTopPos.y, HalfEyePosX, RightBottomPos.y);
			}
			else // Если не врезалась
			{
				// Рисуем голову
				Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

				// Ромбовидный узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // Позиция y правой нижней точки
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисем левый глазик
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правый глазик
			}
		}
		else if (SegmentNumber == 1) // Если хвост
		{
			// Рисуем квадратную половину хвоста
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x, Pos.y + Width / 2);
			// Рисуем круглую половину хвоста
			Ellipse(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
		else // Если промежотчный сегмент
		{
			// Рисуем сегмент
			Rectangle(cHDC, Pos.x - TileSize / 2, Pos.y - Width / 2, Pos.x + TileSize / 4, Pos.y + Width / 2);

			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}

	}
	else if (TileID == TILE_SNAKE_RIGHT) // Если смотрит вправо
	{
		// Координаты ромбовидного узора
		PolyPoints[0].x = Pos.x - TileSize / 4; // Устанавливаем координаты х точки ромба
		PolyPoints[0].y = Pos.y; // Устанавливаем координаты y точки ромба
		PolyPoints[1].x = (Pos.x * 2 + TileSize / 4) / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[1].y = Pos.y + Width / 2 - 1; // Устанавливаем координаты y точки ромба
		PolyPoints[2].x = Pos.x + TileSize / 2 - 1; // Устанавливаем координаты х точки ромба
		PolyPoints[2].y = Pos.y; // Устанавливаем координаты y точки ромба
		PolyPoints[3].x = (Pos.x * 2 + TileSize / 4) / 2; // Устанавливаем координаты х точки ромба
		PolyPoints[3].y = Pos.y - Width / 2; // Устанавливаем координаты y точки ромба

		if (SegmentNumber == SegmentsAmount) // Если голова
		{
			if (Smashed) // Если врезалась
			{
				// Голова
				int HalfHeadPosX = Pos.x / TileSize * TileSize; // Координата x половины головы		
				LeftTopPos.x = HalfHeadPosX - TileSize / 2; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2; // Позиция y левой верхней точки
				RightBottomPos.x = HalfHeadPosX + TileSize / 2; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y + Width / 2; // Позиция y правой нижней точки
				// Рисуем половину головы				
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfHeadPosX, LeftTopPos.y, HalfHeadPosX, RightBottomPos.y);

				// Узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				PolyPoints[0].x = HalfHeadPosX; // Устанавливаем координаты х точки треугольника
				PolyPoints[0].y = Pos.y - Width / 2; // Устанавливаем координаты y точки треугольника
				PolyPoints[1].x = HalfHeadPosX; // Устанавливаем координаты х точки треугольника
				PolyPoints[1].y = Pos.y + Width / 2; // Устанавливаем координаты y точки треугольника
				PolyPoints[2].x = HalfHeadPosX - TileSize / 2; // Устанавливаем координаты х точки треугольника
				PolyPoints[2].y = Pos.y; // Устанавливаем координаты y точки треугольника
				Polygon(cHDC, PolyPoints, 3); // Рисуем треугольник

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				Pos.x += TileSize / 4 + TileSize / 24; // Смещение позиции вправо				
				int HalfEyePosX = Pos.x + 11 * TileSize / 48 - TileSize / 2; // Координата у половины глаз
				LeftTopPos.x = Pos.x - TileSize / 2 + TileSize / 8; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x - TileSize / 2 + TileSize / 3; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // Позиция y правой нижней точки
				// Рисуем левый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, LeftTopPos.y, HalfEyePosX, RightBottomPos.y);
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				// Рисуем правый глазик
				Chord(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y, HalfEyePosX, RightBottomPos.y, HalfEyePosX, LeftTopPos.y);
			}
			else // Если не врезалась
			{
				// Рисуем голову
				Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

				// Ромбовидный узор
				SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
				Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор

				// Глазки
				SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
				SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
				LeftTopPos.x = Pos.x + TileSize / 2 - TileSize / 8; // Позиция х левой верхней точки
				LeftTopPos.y = Pos.y - Width / 2 + TileSize / 16; // Позиция y левой верхней точки
				RightBottomPos.x = Pos.x + TileSize / 2 - TileSize / 3; // Позиция х правой нижней точки 
				RightBottomPos.y = Pos.y - Width / 2 + TileSize / 4; // Позиция y правой нижней точки
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисем левый глазик
				LeftTopPos.y = Pos.y + Width / 2 - TileSize / 16; // Позиция х левой верхней точки
				RightBottomPos.y = Pos.y + Width / 2 - TileSize / 4; // Позиция х правой нижней точки 
				Ellipse(cHDC, LeftTopPos.x, LeftTopPos.y, RightBottomPos.x, RightBottomPos.y); // Рисуем правый глазик
			}
		}
		else if (SegmentNumber == 1) // Если хвост
		{
			// Рисуем квадратную половину хвоста
			Rectangle(cHDC, Pos.x, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);
			// Рисуем круглую половину хвоста
			Ellipse(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
		else // Если промежуточный сегмент
		{
			// Рисуем сегмент
			Rectangle(cHDC, Pos.x - TileSize / 4, Pos.y - Width / 2, Pos.x + TileSize / 2, Pos.y + Width / 2);

			// Ромбовидный узор
			SelectObject(cHDC, Pens[GCLR_DARKGREEN]); // Выбираем цвет для обводки
			SelectObject(cHDC, Brushes[GCLR_DARKGREEN]); // Выбираем цвет для заполнения
			Polygon(cHDC, PolyPoints, 4); // Рисуем ромбовидный узор
		}
	}
}

// Рисует змейки целиком
void DrawSnake(drawtools& DrawTools, map& Map, snake& Snake, int TailTile, bool Smashed = false) {
	int SegmentNumber = 0; // Номер начального сегмента
	int SegmentTile = TailTile; // ID начального сегмента, начинаем с хвоста
	pos DrawPos = Snake.TailPos; // Позиция начального сегмента
	DrawTile(DrawTools, DrawPos, TILE_EMPTY); // Затираем позицию фоном
	while (true) // Движемся по сегментам и отрисовываем их
	{
		SegmentNumber++; // Увеличиваем номер сегмента на 1
		DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber); // Рисуем сегмент
		if (SegmentNumber + 1 == Snake.Segments) // Проверка для разбившейся змейки
		{
			DrawSnakeBodyPart(DrawTools, DrawPos, SegmentTile, Snake.Segments, SegmentNumber, Smashed); // Дорисовываем скомканую голову, если Smashed == true
		}
		if (DrawPos.x == Snake.HeadPos.x && DrawPos.y == Snake.HeadPos.y) // Если дошли до головы
		{
			break; // Останавливаемся
		}
		if (SegmentTile == TILE_SNAKE_UP) // Если смотрит вверх
		{
			if (DrawPos.y - 1 < 0) // Проверка на границу карты
			{
				DrawPos.y = Map.Height - 1; // Перескакиваем на другой конец

			}
			else // Если не на границе
			{
				DrawPos.y--; // Сдвигаемся вверх
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // Затираем фоном
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_DOWN, Snake.Segments, SegmentNumber); // Рисуем обратный сегмент
		}
		else if (SegmentTile == TILE_SNAKE_DOWN) // Если смотрит вниз
		{
			if (DrawPos.y + 1 >= Map.Height) // Проверка на границу карты
			{
				DrawPos.y = 0; // Перескакиваем на другой конец

			}
			else // Если не на границе
			{
				DrawPos.y++; // Сдвигаемся вниз
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // Затираем фоном
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_UP, Snake.Segments, SegmentNumber); // Рисуем обратный сегмент
		}
		else if (SegmentTile == TILE_SNAKE_LEFT) // Если смотрит влево
		{
			if (DrawPos.x - 1 < 0) // Проверка на границу карты
			{
				DrawPos.x = Map.Width - 1; // Перескакиваем на другой конец

			}
			else // Если не на границе
			{
				DrawPos.x--; // Сдвигаемся влево
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // Затираем фоном
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_RIGHT, Snake.Segments, SegmentNumber); // Рисуем обратный сегмент
		}
		else if (SegmentTile == TILE_SNAKE_RIGHT) // Если смотрти вправо
		{
			if (DrawPos.x + 1 >= Map.Width) // Проверка на границу карты
			{
				DrawPos.x = 0; // Перескакиваем на другой конец
			}
			else // Если не на границе
			{
				DrawPos.x++; // Сдвигаемся вправо
			}
			DrawTile(DrawTools, DrawPos, TILE_EMPTY); // Затираем фоном
			DrawSnakeBodyPart(DrawTools, DrawPos, TILE_SNAKE_LEFT, Snake.Segments, SegmentNumber); // Рисуем обратный сегмент
		}
		SegmentTile = Map.Tiles[DrawPos.y][DrawPos.x]; // Получаем ID следующего сегмента
	}
}

// Рисует карту
void DrawMap(drawtools& DrawTools, map& Map, bool NoApples = false) {
	for (int y = 0; y < Map.Height; y++) // На все позиции по вертикали
	{
		for (int x = 0; x < Map.Width; x++) // И по горизонтали
		{
			DrawTile(DrawTools, { x, y }, Map.Tiles[y][x], NoApples); // Рисуем плитки
		}
	}
}

// Отображает текст
void RenderText(drawtools& DrawTools, const char* Text, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	HWND cHWND = GetConsoleWindow(); // дескриптор окна, используемый консолью
	HDC cHDC = GetDC(GetConsoleWindow()); // данные типа HDC представляют собой 32-разрядное целое беззнаковое число.	
	SetTextColor(cHDC, DrawTools.Palette.Colors[TxtClr]); // цвет текста
	SetBkMode(cHDC, TRANSPARENT); // прозрачный цвет фона
	SelectObject(cHDC, Font); // выбор объекта с настройками отображения
	if (Centered) // Если текст нужно центрировать
	{
		LOGFONT TempFont; // Хранения свойств фона
		GetObject(Font, sizeof(LOGFONT), &TempFont); // Получаем свойства фона
		Pos.y -= TempFont.lfHeight / 2; // Выравниваем по вертикали
		SetTextAlign(cHDC, TA_CENTER); // Выравниваем по горизонтали
	}
	TextOutA(cHDC, Pos.x, Pos.y, Text, strlen(Text)); // Вывод текста на экран
	ReleaseDC(cHWND, cHDC); // Освобождаем дескриптор
}

// Рисует линии от центра с равным промежутком между собой
void DrawTextLines(drawtools& DrawTools, std::string* TextLines, int TextLinesCount, pos Pos, HFONT Font, int TxtClr, bool Centered) {
	LOGFONT TempFont; // Для хранения свойств фона
	GetObject(Font, sizeof(LOGFONT), &TempFont); // Получаем свойства фона
	if (Centered) // Если нужно центрировать
	{
		if (TextLinesCount % 2) // Если нечетное количество строк
		{
			Pos.y -= TextLinesCount * TempFont.lfHeight; // Смещаем позицию к самой верхней строке
		}
		else // Если четное количество строк
		{

			Pos.y -= TempFont.lfHeight + (TextLinesCount - 1) * TempFont.lfHeight; // Смещаем позицию к самой верхней строке
		}
	}
	for (int i = 0; i < TextLinesCount; i++) // Поочередно рисуем строки
	{
		RenderText(DrawTools, TextLines[i].c_str(), Pos, Font, TxtClr, Centered); // Рисуем строку
		Pos.y += TempFont.lfHeight * 2; // Смещаемся вниз
	}
}

// Рисует счетчик еды
void DrawInfoFoodCount(drawtools& DrawTools, int FoodCount) {
	char StrBuffer[6]; // Буфер для строки счетчика
	sprintf_s(StrBuffer, 6, " %03i", FoodCount); // Конвертируем число в строку
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]); // Выбираем цвет для обводки
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]); // Выбираем цвет для заполнения
	Rectangle(DrawTools.Console.cHDC, 2 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 3.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // Затираем предыдущий текст
	RenderText(DrawTools, StrBuffer, { 2 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false); // Рисуем текст
}

// Рисует счет
void DrawInfoScore(drawtools& DrawTools, int Score) {
	char StrBuffer[8]; // Буфер для строки счета
	sprintf_s(StrBuffer, 8, " %05i", Score); // Конвертируем число в строку
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Pens[GCLR_DARKWOOD]); // Выбираем цвет для обводки
	SelectObject(DrawTools.Console.cHDC, DrawTools.Palette.Brushes[GCLR_DARKWOOD]); // Выбираем цвет для заполнения
	Rectangle(DrawTools.Console.cHDC, 5 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 8.4 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // Затираем предыдущий текст
	RenderText(DrawTools, StrBuffer, { 5 * DrawTools.TileSize, (int)(0.5 * DrawTools.TileSize) }, DrawTools.NormalFont, GCLR_BURLYWOOD, false); // Рисуем текст
}

// Рисует инфобар
void DrawInfoBar(drawtools& DrawTools, map& Map, int FoodCount, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // Создаем ссылку для переменной для удобства
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Создаем ссылку для переменной для удобства
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Создаем ссылку для переменной для удобства

	// Фон
	SelectObject(cHDC, Pens[GCLR_DARKWOOD]); // Выбираем цвет для обводки
	SelectObject(cHDC, Brushes[GCLR_DARKWOOD]); // Выбираем цвет для заполнения
	Rectangle(cHDC, 0, 0, Map.Width * DrawTools.TileSize, INFO_BAR_SIZE * DrawTools.TileSize); // Рисуем фон инфобара

	// Яблоки
	SelectObject(cHDC, Pens[GCLR_SCARLET]); // Выбираем цвет для обводки
	SelectObject(cHDC, Brushes[GCLR_SCARLET]); // Выбираем цвет для заполнения
	Ellipse(cHDC, DrawTools.TileSize, 0.5 * DrawTools.TileSize, 2 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // Рисуем круг иконки
	DrawApple(DrawTools, { 1, 0.5 }); // Рисуем яблоко
	DrawInfoFoodCount(DrawTools, FoodCount); // Рисуем счетчик

	// Cчет
	SelectObject(cHDC, Pens[GCLR_YELLOW]); // Выбираем цвет для обводки
	SelectObject(cHDC, Brushes[GCLR_YELLOW]); // Выбираем цвет для заполнения
	Ellipse(cHDC, 4 * DrawTools.TileSize, 0.5 * DrawTools.TileSize, 5 * DrawTools.TileSize, 1.5 * DrawTools.TileSize); // Рисуем круг счета
	DrawStar(DrawTools, { 4.5, 1 }, 0.4); // Рисуем звезду
	DrawInfoScore(DrawTools, Score); // Рисуем счет
}

// Затирает всю область фоном
void DrawMenuBackground(drawtools& DrawTools, map& Map) {
	for (int y = 0; y < Map.Height + INFO_BAR_SIZE; y++) // Рисуем фон
	{
		for (int x = 0; x < Map.Width; x++) // Отрисовываем шахматный фон по клеточкам
		{
			DrawTile(DrawTools, { x,y - INFO_BAR_SIZE }, TILE_EMPTY); // Вызываем функцию отрисовки ячейки
		}
	}
}