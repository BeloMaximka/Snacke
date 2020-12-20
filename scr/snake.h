/*-------------------------------------------------------------------
	snake.h

	Всё, что касается непосредственно игры
-------------------------------------------------------------------*/
#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами

#define FOOD_SCORE_REWARD 10 // Минимальная награда за поедание яблок

// Очищает карту
void ClearMap(map& Map) {
	for (int y = 0; y < Map.Height; y++) // Движемся по высоте
	{
		for (int x = 0; x < Map.Width; x++) // По ширине
		{
			if (Map.Walls && (x == 0 || y == 0 || x == Map.Width - 1 || y == Map.Height - 1)) // Если режим стен
			{
				Map.Tiles[y][x] = TILE_WALL; // Ставим стены
			}
			else
			{
				Map.Tiles[y][x] = TILE_EMPTY; // Ставим пустоту
			}
		}
	}
}

// Создает еду в случайно точке карты
bool SpawnFood(drawtools& DrawTools, map& Map, snake& Snake) {
	pos FoodPos; // Позиция еды
	for (int i = 0; i < Map.Height * Map.Width; i++) // h*w раз пытается создать яблоко
	{
		FoodPos.y = rand() % Map.Height; // Выбираем случайную позицию по высоте
		FoodPos.x = rand() % Map.Width; // Выбираем случайную позицию по ширине
		if (Map.Tiles[FoodPos.y][FoodPos.x] == TILE_EMPTY) // Если на клетке пусто
		{
			Map.Tiles[FoodPos.y][FoodPos.x] = TILE_FOOD; // Создаем там еду
			DrawTile(DrawTools, FoodPos, TILE_FOOD); // Рисуем там еду
			// Вычисляем награду за еду в зависимости от расстояния
			Snake.FoodReward = FOOD_SCORE_REWARD + 2 * sqrt(pow(Snake.HeadPos.x - FoodPos.x, 2) + pow(Snake.HeadPos.y - FoodPos.y, 2));
			return true; // Создание удалось - возращаем false
		}
	}
	// Если ни разу не повезло создать, используем псевдо-псевдо случайный метод генерации
	if (rand() % 2) // Выбираем вертикальное направление движения. Это сверху
	{
		if (rand() % 2) // Горизонтальное направление движение. Это слева
		{
			// Двигаемся с левого верхнего угла
			for (int y = 0; y < Map.Height; y++)
			{
				for (int x = 0; x < Map.Width; x++)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY) // Если на клетке пусто
					{
						Map.Tiles[y][x] = TILE_FOOD; // Создаем там еду
						DrawTile(DrawTools, { x, y }, TILE_FOOD); // Рисуем там еду
						// Вычисляем награду за еду в зависимости от расстояния
						Snake.FoodReward = FOOD_SCORE_REWARD + 2 * sqrt(pow(Snake.HeadPos.x - x, 2) + pow(Snake.HeadPos.y - y, 2));
						return true; // Создание удалось - возращаем false
					}
				}
			}
		}
		else // Спаава
		{
			// Двигаемся с правого верхнего угла
			for (int y = 0; y < Map.Height; y++)
			{
				for (int x = Map.Width = 1; x > 0; x--)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY) // Если на клетке пусто
					{
						Map.Tiles[y][x] = TILE_FOOD; // Создаем там еду
						DrawTile(DrawTools, { x, y }, TILE_FOOD); // Рисуем там еду
						// Вычисляем награду за еду в зависимости от расстояния
						Snake.FoodReward = FOOD_SCORE_REWARD + 2 * sqrt(pow(Snake.HeadPos.x - x, 2) + pow(Snake.HeadPos.y - y, 2));
						return true; // Создание удалось - возращаем false
					}
				}
			}
		}
	}
	else // Снизу
	{
		if (rand() % 2)
		{
			// Двигаемся с левого нижнего угла
			for (int y = Map.Height - 1; y > 0; y--)
			{
				for (int x = 0; x < Map.Width; x++)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY) // Если на клетке пусто
					{
						Map.Tiles[y][x] = TILE_FOOD; // Создаем там еду
						DrawTile(DrawTools, { x, y }, TILE_FOOD); // Рисуем там еду
						// Вычисляем награду за еду в зависимости от расстояния
						Snake.FoodReward = FOOD_SCORE_REWARD + 2 * sqrt(pow(Snake.HeadPos.x - x, 2) + pow(Snake.HeadPos.y - y, 2));
						return true; // Создание удалось - возращаем false
					}
				}
			}
		}
		else
		{
			// Двигаемся с правого нижнего угла
			for (int y = Map.Height - 1; y > 0; y--)
			{
				for (int x = Map.Width = 1; x > 0; x--)
				{
					if (Map.Tiles[y][x] == TILE_EMPTY) // Если на клетке пусто
					{
						Map.Tiles[y][x] = TILE_FOOD; // Создаем там еду
						DrawTile(DrawTools, { x, y }, TILE_FOOD); // Рисуем там еду
						// Вычисляем награду за еду в зависимости от расстояния
						Snake.FoodReward = FOOD_SCORE_REWARD + 2 * sqrt(pow(Snake.HeadPos.x - x, 2) + pow(Snake.HeadPos.y - y, 2));
						return true; // Создание удалось - возращаем false
					}
				}
			}
		}
	}
	return false; // Если совсем не получилось создать - значит свободных клеток не осталось. Возращаем false
}

// Удаляет конец хвоста без изменения количества сегментов (нужно для врезания)
void RemoveTailPart(drawtools DrawTools, map& Map, snake& Snake) {
	DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY); // Закрашиваем удаляемый хвост пустой клеткой
	// Определяем новый конец
	if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP) // Если смотрит вверх
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.y - 1 < 0) // Проверка на границу
		{
			Snake.TailPos.y = Map.Height - 1; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.y--; // Смещаемся вверх
		}

	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_DOWN) // Если смотрит вниз
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.y + 1 >= Map.Height) // Проверка на границу
		{
			Snake.TailPos.y = 0; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.y++; // Смещаемся вниз
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_LEFT) // Если смотрит влево
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.x - 1 < 0) // Проверка на границу
		{
			Snake.TailPos.x = Map.Width - 1; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.x--; // Смещаемся влево
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_RIGHT) // Если смотрит вправо
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.x + 1 >= Map.Width) // Проверка на границу
		{
			Snake.TailPos.x = 0; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.x++; // Смещаемся вправо
		}
	}
}

// Передвигает змейку вперед
bool MoveSnake(drawtools& DrawTools, audiotools& Audio, map& Map, snake& Snake) {
	//--------------РАБОТА С ГОЛОВОЙ--------------	
	Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head; // Поворачиваем сегмент перед новой головой	
	pos NewHeadPos = Snake.HeadPos; // Позиция новой головы
	// Определяем позицию новой головы	
	if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_UP) // Если голова смотрит вверх
	{
		if (NewHeadPos.y - 1 < 0) // Если змейка находится на границе карты
		{
			NewHeadPos.y = Map.Height - 1; // Перекидываем позицию на другой конец карты
		}
		else // Если не на границе
		{
			NewHeadPos.y--; // Смещаем позицию новой головы вверх
		}
	}
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_DOWN) // Если голова смотрит вниз
	{
		if (NewHeadPos.y + 1 >= Map.Height) // Если змейка находится на границе карты
		{
			NewHeadPos.y = 0; // Перекидываем позицию на другой конец карты
		}
		else // Если не на границе
		{
			NewHeadPos.y++; // Смещаем позицию новой головы вниз
		}
	}
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_LEFT) // Если голова смотрит влево
	{
		if (NewHeadPos.x - 1 < 0) // Если змейка находится на границе карты
		{
			NewHeadPos.x = Map.Width - 1; // Перекидываем позицию на другой конец карты
		}
		else // Если не на границе
		{
			NewHeadPos.x--; // Смещаем позицию новой головы влево
		}
	}
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_RIGHT) // Если голова смотрит влево
	{
		if (Snake.HeadPos.x + 1 >= Map.Width) // Если змейка находится на границе карты
		{
			NewHeadPos.x = 0; // Перекидываем позицию на другой конец карты
		}
		else // Если не на границе
		{
			NewHeadPos.x++; // Смещаем позицию новой головы вправо
		}
	}
	if (NewHeadPos.y == Snake.TailPos.y && NewHeadPos.x == Snake.TailPos.x) // Если на пути хвост
	{
		if (Snake.FoodReward > FOOD_SCORE_REWARD) // Если награда за еду больше минимального значения
		{
			Snake.FoodReward--; // Уменьшаем еду на один
		}
		//--------------РАБОТА С ХВОСТОМ--------------		
		DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY); // Закрашиваем удаляемый хвост пустой клеткой
		// Определяем новый конец
		if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP) // Если голова смотрит вверх
		{
			if (Snake.TailPos.y - 1 < 0) // Если хвост находится на границе карты
			{
				Snake.TailPos.y = Map.Height - 1; // Перекидываем позицию на другой конец карты
			}
			else // Если не на границе
			{
				Snake.TailPos.y--; // Смещаем позицию нового хвоста вверх
			}

		}
		else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_DOWN) // Если голова смотрит внз
		{
			if (Snake.TailPos.y + 1 >= Map.Height) // Если хвост находится на границе карты
			{
				Snake.TailPos.y = 0; // Перекидываем позицию на другой конец карты
			}
			else // Если не на границе
			{
				Snake.TailPos.y++; // Смещаем позицию нового хвоста вниз
			}
		}
		else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_LEFT) // Если голова смотрит влево
		{
			if (Snake.TailPos.x - 1 < 0) // Если хвост находится на границе карты
			{
				Snake.TailPos.x = Map.Width - 1; // Перекидываем позицию на другой конец карты
			}
			else // Если не на границе
			{
				Snake.TailPos.x--; // Смещаем позицию нового хвоста влево
			}
		}
		else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_RIGHT) // Если голова смотрит вправо
		{
			if (Snake.TailPos.x + 1 >= Map.Width) // Если хвост находится на границе карты
			{
				Snake.TailPos.x = 0; // Перекидываем позицию на другой конец карты
			}
			else // Если не на границе
			{
				Snake.TailPos.x++; // Смещаем позицию нового хвоста вправо
			}
		}
		Snake.HeadPos = NewHeadPos; // Обновляем позицию головы змейки		
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head; // Добавляем сегмент головы
		return true; // Движение удалось - возвращем true
	}
	// Если на пути змейки препятствие (т.е не пусто и не еда)
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_EMPTY && Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_FOOD)
	{
		return false; // Движение невозможно - возращаем false
	}
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_FOOD) // Если на пути еда
	{
		PlaySoundB(Audio, GSND_CHEW1 + rand() % 4, Audio.GameVolumePercent * 2); // Проигрываем звук поедания яблока		
		Snake.HeadPos = NewHeadPos; // Обновляем позицию головы змейки		
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head; // Добавляем сегмент головы		
		Snake.Segments++; // Увеличиваем количество сегментов		
		Snake.FoodEaten++; // Увеличиваем счетчик съеденой еды		
		Snake.Score += Snake.FoodReward; // Увеличиваем счет		
		DrawInfoFoodCount(DrawTools, Snake.FoodEaten); // Обновляем счетчик еды на инфобаре		
		DrawInfoScore(DrawTools, Snake.Score); // Обновляем счет на инфобаре		
		SpawnFood(DrawTools, Map, Snake); // Создаём ещё одну еду		
		return true; // Останавливаем функцию здесь, чтобы не хвост не удалился
	}
	else // Если на пути пусто
	{
		if (Snake.FoodReward > FOOD_SCORE_REWARD) // Если награда за еду больше минимального значения
		{
			Snake.FoodReward--; // Уменьшаем еду на один
		}
		Snake.HeadPos = NewHeadPos; // Обновляем позицию головы змейки		
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head; // Добавляем сегмент головы
	}
	//--------------РАБОТА С ХВОСТОМ--------------	
	DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY); // Закрашиваем удаляемый хвост пустой клеткой
	// Определяем новый конец
	if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP) // Если смотрит вверх
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;  // Удаляем конец хвоста
		if (Snake.TailPos.y - 1 < 0) // Проверка на границу
		{
			Snake.TailPos.y = Map.Height - 1; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.y--; // Смещаемся вверх
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_DOWN) // Если смотрит вниз
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.y + 1 >= Map.Height) // Проверка на границу
		{
			Snake.TailPos.y = 0; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.y++; // Смещаемся вверх
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_LEFT) // Если смотрит влево
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.x - 1 < 0) // Проверка на границу
		{
			Snake.TailPos.x = Map.Width - 1; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.x--; // Смещаемся вверх
		}
	}
	else if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_RIGHT) // Если смотрит вправо
	{
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY; // Удаляем конец хвоста
		if (Snake.TailPos.x + 1 >= Map.Width) // Проверка на границу
		{
			Snake.TailPos.x = 0; // Перескакиваем на другой конец
		}
		else // Если не на границе
		{
			Snake.TailPos.x++; // Смещаемся вверх
		}
	}
	return true; // Движение удалось - возращаем true
}

// Создает змейку
void SpawnSnake(map& Map, snake& Snake, int DirTile, pos HeadPos, int Segments, int SnakeDelay) {
	pos PlacePos = HeadPos; // Начальная позици, начинаем с головы
	pos PlaceDir = { 0,0 }; // Направление смещения
	if (DirTile == TILE_SNAKE_UP) // Если смотрит вверх
	{
		PlaceDir.y = 1; // Направление вниз
	}
	else if (DirTile == TILE_SNAKE_DOWN) // Если смотрит вниз
	{
		PlaceDir.y = -1; // Направление вверх
	}
	else if (DirTile == TILE_SNAKE_LEFT) // Если смотрит влево
	{
		PlaceDir.x = 1; // Направление вправо
	}
	else if (DirTile == TILE_SNAKE_RIGHT) // Если смотрит вправо
	{
		PlaceDir.x = -1; // Направление влево
	}
	if (PlaceDir.y != 0 && Segments >= Map.Height) // Проверка на границу
	{
		Segments = Map.Height - 1; // Перескакиваем на другой конец
	}
	else if (Segments >= Map.Width) // Проверка на границу
	{
		Segments = Map.Width - 1; // Перескакиваем на другой конец
	}
	for (int i = 0; i < Segments - 1; i++) // Начинам размещать змейку
	{
		Map.Tiles[PlacePos.y][PlacePos.x] = DirTile; // Размещаем на карте сегмент
		if (PlacePos.x + PlaceDir.x < 0) // Проверка на одну границу
		{
			PlacePos.x = Map.Width - 1; // Перескакиваем
		}
		else if (PlacePos.x + PlaceDir.x >= Map.Width) // Проверка на другую границу
		{
			PlacePos.x = 0; // Перескакиваем
		}
		else // Если не на границе
		{
			PlacePos.x += PlaceDir.x; // Смещаем позицию
		}
		if (PlacePos.y + PlaceDir.y < 0) // Проверка на одну границу
		{
			PlacePos.y = Map.Height - 1; // Перескакиваем
		}
		else if (PlacePos.y + PlaceDir.y >= Map.Height) // Проверка на другую границу
		{
			PlacePos.y = 0; // Перескакиваем
		}
		else // Если не на границе
		{
			PlacePos.y += PlaceDir.y; // Смещаем позицию
		}
	}
	Map.Tiles[PlacePos.y][PlacePos.x] = DirTile; // Размещаем последний сегмент
	Snake.Head = DirTile; // Задаем направление головы
	Snake.HeadPos = HeadPos; // Задаем позицию головы
	Snake.OldHead = DirTile; // Задаем старое направление головы. В данном случае это то же направление
	Snake.Segments = Segments; // Задаем количество сегментов змейки
	Snake.TailPos = PlacePos; // Задаем позицию хвоста
	Snake.FoodEaten = 0; // Количество съеденой еды
	Snake.Score = 0; // Счет
	Snake.Delay = SnakeDelay; // Задержка между шагами
}

// Первый шаг змейки. Пауза до первого нажатия
bool SnakeFirstStep(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, snake& Snake) {
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер ввода от предыдущих
	int Keycode; // Код нажатой клавиши
	bool EndLoop = false; // Окончание цикла
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потому что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, чтобы окно успело развернуться
			// Отрисовываем:
			DrawMap(DrawTools, Map); // Фон
			DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейка
			DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
		}
		if (_kbhit()) // Если была нажат клавиша
		{
			Keycode = _getch(); // Получаем код нажатой клавиши
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_ENTER || Keycode == GMKEY_ESC) // Если Enter или Esc
			{
				if (PauseMenu(DrawTools, Audio, Map, Data, Snake.FoodEaten, Snake.Score)) // Ставим паузу
				{
					return false; // Возвращаем false, ибо PauseMenu вернула true
				}
				// Заново отрисовываем:
				DrawMap(DrawTools, Map); // Фон
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
				DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
			}
			else if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN) // Если вверх и голова не смотрит вниз
			{
				Snake.Head = TILE_SNAKE_UP; // Поворачиваем голову
				Snake.OldHead = Snake.Head; // Обновляем позицию старой головы
				if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Если змейка не сдвинулась
				{
					// Данное действие исключено, но оставил для будущего
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку					
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				EndLoop = true; // Заканичваем цикл
			}
			else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP) // Если вниз и голова не смотрит вверх
			{
				Snake.Head = TILE_SNAKE_DOWN; // Поворачиваем голову
				Snake.OldHead = Snake.Head; // Обновляем позицию старой головы
				if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Если змейка не сдвинулась
				{
					// Данное действие исключено, но оставил для будущего
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				EndLoop = true; // Заканичваем цикл
			}
			else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT) // Если влево и голова не смотрит вправо
			{
				Snake.Head = TILE_SNAKE_LEFT; // Поворачиваем голову
				Snake.OldHead = Snake.Head; // Обновляем позицию старой головы
				if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Если змейка не сдвинулась
				{
					// Данное действие исключено, но оставил для будущего
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				EndLoop = true; // Заканичваем цикл
			}
			else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT) // Если влево и голова не смотрит вправо
			{
				Snake.Head = TILE_SNAKE_RIGHT; // Поворачиваем голову
				Snake.OldHead = Snake.Head; // Обновляем позицию старой головы
				if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Если змейка не сдвинулась
				{
					// Данное действие исключено, но оставил для будущего
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку					
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				EndLoop = true; // Заканичваем цикл
			}
			while (_kbhit()) // Проверка на нажатие паузы
			{
				Keycode = _getch(); // Получаем код клавиши
				if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
				if (Keycode == GMKEY_ENTER || Keycode == GMKEY_ESC) // Если Enter или Esc
				{
					if (PauseMenu(DrawTools, Audio, Map, Data, Snake.FoodEaten, Snake.Score)) // Ставим паузу
					{
						return false; // Возвращаем false, ибо PauseMenu вернула true
					}
					// Заново отрисовываем:
					DrawMap(DrawTools, Map); // Фон
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
					DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
				}
			}
			if (EndLoop) // Если конец цикла
			{
				break; // Остановочка
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
	return true; // Успешно сделали первый шаг - возвращаем true
}

// Сама функция игры, main loop
void SnakeMainGame(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay) {
	while (true)
	{
		ClearMap(Map); // Очищаем карту		
		snake Snake; // Змея
		int SnakeSpawnSegments = 3; // Количество сегментов змеки
		pos SnakeSpawnPos = { SnakeSpawnSegments,Map.Height / 2 }; // Начальная позиция змеи
		SpawnSnake(Map, Snake, TILE_SNAKE_RIGHT, SnakeSpawnPos, SnakeSpawnSegments, SnakeDelay); // Создаем змею

		DrawMap(DrawTools, Map); // Рисуем карту
		DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змею
		SpawnFood(DrawTools, Map, Snake); // Создаем еду
		DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Рисуем инфобар

		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер ввода от предыдущих

		if (!SnakeFirstStep(DrawTools, Audio, Map, Data, Snake)) // Первый шаг змеи
		{
			return; // Возвращаемся в главное меню
		}
		Sleep(Snake.Delay); // Движение было сделано - ждем до следующего шага

		int Keycode; // Переменная для храения кода символа		
		while (true)
		{
			if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потому что при сворачивании картинка почему-то затирается)
			{
				Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, чтобы окно успело развернуться
				// Отрисовываем:
				DrawMap(DrawTools, Map); // Фон
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
				DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
			}
			if (_kbhit()) // Если было нажатие
			{
				if (_kbhit()) // Проверка первого нажатия
				{
					Keycode = _getch(); // Получаем код символа
					if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный

					if (Keycode == GMKEY_ENTER || Keycode == GMKEY_ESC) // Если Enter или Esc
					{
						if (PauseMenu(DrawTools, Audio, Map, Data, Snake.FoodEaten, Snake.Score)) // Ставим паузу
						{
							return; // Возвращаемся в главное меню
						}
						// Заново отрисовываем:
						DrawMap(DrawTools, Map); // Фон
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
						DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
						if (!SnakeFirstStep(DrawTools, Audio, Map, Data, Snake)) // Запускам паузу до шага змеи
						{
							return; // Возвращаемся в главное меню
						}
						Sleep(Snake.Delay); // Ждем до следующего шага
					}
					else if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN) // Если вверх и голова не смотрит вниз
					{
						Snake.Head = TILE_SNAKE_UP; // Поворачиваем голову вверх
					}
					else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP) // Если вниз и голова не смотрит вверх
					{
						Snake.Head = TILE_SNAKE_DOWN; // Поворачиваем голову вниз
					}
					else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT) // Если влево и голова не смотрит вправо
					{
						Snake.Head = TILE_SNAKE_LEFT; // Поворачиваем голову влево
					}
					else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT) // Если вправо и голова не смотрит влево
					{
						Snake.Head = TILE_SNAKE_RIGHT; // Поворичваем голову вправо
					}
					if (Snake.Head != Snake.OldHead) // Если новая позиция головы не совпадает со старой позицией
					{
						Snake.OldHead = Snake.Head; // Обновляем старую позицию головы
						if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Двигаем змейку
						{
							break; // Змейка не двинулась, а значит врезаалсь - завершаем веселье
						}
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
						Sleep(Snake.Delay); // Ждем до следующего шага
					}
				}
				if (_kbhit()) // Проверка первого нажатия подряд 
				{
					Keycode = _getch(); // Получаем код символа
					if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный


					if (Keycode == GMKEY_ENTER || Keycode == GMKEY_ESC) // Если Enter или Esc
					{
						if (PauseMenu(DrawTools, Audio, Map, Data, Snake.FoodEaten, Snake.Score)) // Ставим паузу
						{
							return; // Возвращаемся в главное меню
						}
						// Заново отрисовываем:
						DrawMap(DrawTools, Map); // Фон
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
						DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
						if (!SnakeFirstStep(DrawTools, Audio, Map, Data, Snake)) // Запускам паузу до шага змеи
						{
							return; // Возвращаемся в главное меню
						}
						Sleep(Snake.Delay); // Ждем до следующего шага
					}
					else if (Keycode == GMKEY_UP && Snake.OldHead != TILE_SNAKE_DOWN) // Если вверх и голова не смотрит вниз
					{
						Snake.Head = TILE_SNAKE_UP; // Поворачиваем голову вверх
					}
					else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP) // Если вниз и голова не смотрит вверх
					{
						Snake.Head = TILE_SNAKE_DOWN; // Поворачиваем голову вниз
					}
					else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT) // Если влево и голова не смотрит вправо
					{
						Snake.Head = TILE_SNAKE_LEFT; // Поворачиваем голову влево
					}
					else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT) // Если вправо и голова не смотрит влево
					{
						Snake.Head = TILE_SNAKE_RIGHT; // Поворичваем голову вправо
					}
					if (Snake.Head != Snake.OldHead) // Если новая позиция головы не совпадает со старой позицией
					{
						Snake.OldHead = Snake.Head; // Обновляем старую позицию головы
						if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Двигаем змейку
						{
							break; // Змейка не двинулась, а значит врезаалсь - завершаем веселье
						}
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
						Sleep(Snake.Delay); // Ждем до следующего шага
					}
					while (_kbhit()) // Проверка остального буфера на наличие паузы
					{
						Keycode = _getch(); // Получаем код символа
						if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
						if (Keycode == GMKEY_ENTER || Keycode == GMKEY_ESC) // Если Enter или Esc
						{
							if (PauseMenu(DrawTools, Audio, Map, Data, Snake.FoodEaten, Snake.Score)) // Ставим паузу
							{
								return; // Возвращаемся в главное меню
							}
							// Заново отрисовываем:
							DrawMap(DrawTools, Map); // Фон
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Змейку
							DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score); // Инфобар
							if (!SnakeFirstStep(DrawTools, Audio, Map, Data, Snake)) // Запускам паузу до шага змеи
							{
								return; // Возвращаемся в главное меню
							}
							Sleep(Snake.Delay); // Ждем до следующего шага
						}
					}
				}
			}
			else  // Если ничего не было нажато
			{
				Snake.OldHead = Snake.Head; // Обновляем старую позицию головы
				if (!MoveSnake(DrawTools, Audio, Map, Snake)) // Двигаем змейку
				{
					break; // Змейка не двинулась, а значит врезаалсь - завершаем веселье
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]); // Рисуем змейку
				Sleep(Snake.Delay); // Ждем до следующего шага
			}
		}
		// Когда врезались, попадаем сюда
		PlaySoundB(Audio, GSND_PUNCH, Audio.GameVolumePercent); // Проигрываем звук удара
		RemoveTailPart(DrawTools, Map, Snake); // Удаляем хвост
		DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x], true); // Рисуем смятую змейку
		Sleep(1500); // Ждем 1.5 секунды
		if (!RetryMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score)) // Открываем меню ретрая
		{
			return; // Если не выбрали заново, возвращаемся в главное меню
		}
	}
}