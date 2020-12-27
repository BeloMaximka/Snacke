/*-------------------------------------------------------------------
	menus.h

	Менюшки, менюшки, менюшки
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами

#define SCORE_DIGITS 5 // Количество нулей в счете
#define SNAKE_DELAY_CHANGE_STEP 25 // Шаг изменения задержки змейки в настройках
#define SNAKE_DELAY_MIN 100 // Минимальная задержка змейки
#define SNAKE_DELAY_MAX 400 // ММаксимальная задержка змейки
#define VOLUME_CHANGE_STEP 5 // Шаг изменения громкости в настройках

// Прототип из файла snake.h, чтобы линковщик не ругался
void SnakeMainGame(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay);

// Затирает строку текста фоном
void MenuClearLineTile(drawtools& DrawTools, map& Map, int PosY) {	
	PosY = (PosY-DrawTools.TileSize/2) / DrawTools.TileSize - INFO_BAR_SIZE; // Конвертируем координаты окна в координаты карты
	for (int x = 1; x < Map.Width - 1; x++) // Затираем на указаной линии
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY); // Поплиточно рисуем фон
	}
	PosY++; // Спускаемся вниз на один
	for (int x = 1; x < Map.Width - 1; x++) // Затираем на линии выше
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY); // Поплиточно рисуем фон
	}
}

// Меню после поражения
bool RetryMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)	

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение game over

	DrawMap(DrawTools, Map, true); // Отрисовываем карту, инфобар перерисовывать не надо
	RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Рисуем game over
	LOGFONT TempFont; // Временная пеменная дла расчетов смещения от тайтла
	GetObject(DrawTools.NormalFont, sizeof(LOGFONT), &TempFont); // Получаем информацию о шрифте NormalFont
	pos ScorePos = MainTitlePos; // Позиция строки счета
	ScorePos.y += TempFont.lfHeight * 2; // Смещаемся вниз на две высоты NormalFont
	std::string ScoreStr = "Your score: "; // Строка "Your score "
	char StrBuffer[SCORE_DIGITS + 1]; // Буфер для хранения счета игрока в текстовом виде
	_itoa_s(Score, StrBuffer, SCORE_DIGITS + 1, 10); // Переводим счет из числа в строку
	ScoreStr += StrBuffer; // Добавляем к "Your score: "
	RenderText(DrawTools, ScoreStr.c_str(), ScorePos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем счет
	int StringsCount = 3; // Сколько будет опций выбора
	std::string Strings[] = { "Retry", "Exit to main menu", "Exit to desktop" }; // Названия опций выбора	
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер от предыдущих вводов
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново			
			DrawMap(DrawTools, Map, true); // Карта
			DrawInfoBar(DrawTools, Map, FoodEaten, Score); // Инфобар
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Тайтл
			RenderText(DrawTools, ScoreStr.c_str(), ScorePos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем счет
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				//Rectangle(cHDC, 0, ActiveButtonPos.y, ActiveButtonPos.x + Map.Width / 2 * TileSize, ActiveButtonPos.y + TileSize); // Затираем текст выбранной позиции
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
				if (Strings[SelectedButtonNum] == "Retry") // Если выбрали кнопку ретрая
				{
					return true; // true - перезапуск
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // Если "Выйти"
				{
					return false; // false - в главное меню
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // Если "Выйти на рабочий стол"
				{
					exit(0); // Закрываем приложение
				}
			}
		}
		Sleep(1); // Ждём чуть-чуть, чтобы уменшить количество проверок на разворачивание
	}
	return false; // false - в главное меню
}

// Меню паузы
bool PauseMenu(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)	

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение паузы

	DrawMap(DrawTools, Map, true); // Отрисовываем карту, инфобар перерисовывать не надо
	RenderText(DrawTools, "GAME PAUSED", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // Рисуем паузу	
	int StringsCount = 5; // Сколько будет опций выбора
	std::string Strings[] = { "Continue", "Exit to main menu","Game volume:", "Music volume: ",  "Exit to desktop" }; // Названия опций выбора	
	const int BufferSize = 32; // Размер буфера
	char Buffer[BufferSize]; // Буфер для различных именяющихся строчек	
	sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
	Strings[2] = Buffer; // Обновляем надпись	
	sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
	Strings[3] = Buffer; // Обновляем надпись	
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер от предыдущих вводов
	bool VolumeChanged = false; // Были ли изменения громкости
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потому что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
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
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
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
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
					if (VolumeChanged) // Если были изменения громкости
					{
						SaveFileData(Data, Audio); // записываем данные настроек в файл
					}
					return false; // false - прололжаем игру
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // Если "Выйти"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
					if (VolumeChanged) // Если были изменения громкости
					{
						SaveFileData(Data, Audio); // записываем данные настроек в файл
					}
					return true; // true - заканчиваем игру
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // Если "Выйти на рабочий стол"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
					if (VolumeChanged) // Если были изменения громкости
					{
						SaveFileData(Data, Audio); // записываем данные настроек в файл
					}
					exit(0); // Закрываем приложение
				}
			}
			else if (Keycode == GMKEY_LEFT) // Если стрелочка влево
			{
				if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					VolumeChanged = true; // Изменения громкости были
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					VolumeChanged = true; // Изменения громкости были
					Audio.MusicVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // Обновляем громкость музыки
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
			}
			else if (Keycode == GMKEY_RIGHT) // Если стрелочка влево
			{
				if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
					VolumeChanged = true; // Изменения громкости были
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
					VolumeChanged = true; // Изменения громкости были
					Audio.MusicVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // Обновляем громкость музыки
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции					
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
			}
			else if (Keycode == GMKEY_ESC)
			{
				if (VolumeChanged) // Если были изменения громкости
				{
					SaveFileData(Data, Audio); // записываем данные настроек в файл
				}
				return false; // false - продолжаем игру
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
	return false; // false - продолжаем игру
}

// Меню с информацией
void InfoMenu(drawtools& DrawTools, audiotools& Audio, map& Map) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста	

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для обводки
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для заливки
	DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна		
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 2 };	 // Положение центра линий текста

	int StringsCount = 7; // Сколько будет строчек текста
	std::string Strings[] = { "It might help you...","", "Controls for all the stuff: arrow keys, Enter, Esc", "You can pause with Enter and Esc!", "The quicker you eat, the bigger the score reward!", " ","Got it!" }; // Строчки текста


	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора		

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер от предыдущих вводов
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста						
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный

			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
				return; // Возвращаемся обратно				
			}
			else if (Keycode == GMKEY_ESC) // Если энтер
			{

				return; // Возвращаемся обратно				
			}
		}
		Sleep(1); // Ждём чуть-чуть, чтобы уменьшить количество проверок на разворачивание
	}
}

// Меню настроек
void SettingsMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int& SnakeDelay, saveddata& Data) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для обводки
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для заливки
	DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение слова "настройки"
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста

	RenderText(DrawTools, "Settings", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Рисуем слово "настройки"
	int StringsCount = 5; // Сколько будет опций выбора
	std::string Strings[] = { "Back", "Mode: ", "Snake speed: ", "Game volume:", "Music volume: ", }; // Названия опций выбора
	const int BufferSize = 32;
	char Buffer[BufferSize]; // Буфер для различных именяющихся строчек
	sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // Переводим число строку
	Strings[2] = Buffer; // Обновляем надпись
	sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
	Strings[3] = Buffer; // Обновляем надпись	
	sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
	Strings[4] = Buffer; // Обновляем надпись	
	if (Map.Walls) // Если режим со стенам
	{
		Strings[1] += "WALLS"; // Добавляем слово к строке режима игры
	}
	else // Если режим без стен
	{
		Strings[1] += "NO WALLS"; // Добавляем слово к строке режима игры
	}

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

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Очищаем буфер от предыдущих вводов
	bool AnyChanges = false; // Переменная для фиксация каких-либо изменений настроек
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "Settings", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_LEFT) // Если стрелочка влево
			{
				if (Strings[SelectedButtonNum].find("Mode: ", 0) != std::string::npos) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					Map.Walls = Map.Walls ? false : true; // Переключаем режим стен
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // Переключаем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay + SNAKE_DELAY_CHANGE_STEP <= SNAKE_DELAY_MAX) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					SnakeDelay += SNAKE_DELAY_CHANGE_STEP; // Уменьшаяем задержку движения змейки
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // Переводим число строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					Audio.MusicVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // Обновляем громкость музыки
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
			}
			else if (Keycode == GMKEY_RIGHT) // Если стрелочка влево
			{
				if (Strings[SelectedButtonNum].find("Mode: ", 0) != std::string::npos) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню					
					Map.Walls = Map.Walls ? false : true; // Переключаем режим стен
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // Переключаем надпись		
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay - SNAKE_DELAY_CHANGE_STEP >= SNAKE_DELAY_MIN) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					SnakeDelay -= SNAKE_DELAY_CHANGE_STEP; // Увеличиваем задержку движения змейки
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
					AnyChanges = true; // Сделали изменения в настройках
					Audio.MusicVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // Обновляем громкость музыки
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции					
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				if (Strings[SelectedButtonNum] == "Back") // Если "Выйти"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
					if (AnyChanges) // Если были какие-либо изменения в настройках
					{
						SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
					}					
					return; // Возвращаемся обратно
				}
			}
			else if (Keycode == GMKEY_ESC)
			{
				if (AnyChanges) // Если были какие-либо изменения в настройках
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
				}			
				return; // Возвращаемся обратно
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
}

// Главное меню
void MainMenu(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKBURLYWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста
	int SelectedButtonColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для выбранныех строчек текста (и заголовка тоже, кстати)

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для обводки
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для заливки
	DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение названия игры
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста

	RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Рисуем название игры
	int StringsCount = 4; // Сколько будет опций выбора
	std::string Strings[] = { "Play", "Settings", "Help", "Exit" }; // Названия опций выбора	
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора		
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

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));  // Очищаем буфер от предыдущих вводов
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
			RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл
			RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
		}
		if (_kbhit()) // Если нажата какая-нибудь кнопка
		{
			int Keycode = _getch(); // Записываем код нажатой клавиши в переменную
			if (Keycode == 224) Keycode = _getch(); // Особенность со стрелочками: она даёт сразу два кода. Берем второй, нужный			
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // Если стрелочка вверх (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum--; // Меняем номер выбранной опции
				ActiveButtonPos.y -= TileSize * 2; // Смещаем  позицию выбранной опции на окне				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // Если стрелочка вниз  (и проверка на выход из границ)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // Рисуем обычным цветом
				SelectedButtonNum++; // Меняем номер выбранной опции
				ActiveButtonPos.y += TileSize * 2; // Смещаем  позицию выбранной опции на окне
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
			}
			else if (Keycode == GMKEY_ENTER) // Если энтер
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
				if (Strings[SelectedButtonNum] == "Play") // Если выбрали кнопку "Игать"
				{
					SnakeMainGame(DrawTools, Audio, Map, Data, SnakeDelay); // Запускаем змейку					
					// Вышли из змейки, рисуем меню заново					
					DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста					
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
				}
				else if (Strings[SelectedButtonNum] == "Settings") // Если "Выйти"
				{
					SettingsMenu(DrawTools, Audio, Map, SnakeDelay, Data);
					DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
				}
				else if (Strings[SelectedButtonNum] == "Help") // Если "Выйти"
				{
					InfoMenu(DrawTools, Audio, Map);
					DrawMenuBackground(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
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