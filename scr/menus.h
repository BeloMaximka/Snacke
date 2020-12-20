#pragma once
#include "includes.h"

#define SNAKE_DELAY_CHANGE_STEP 25
#define SNAKE_DELAY_MIN 100
#define SNAKE_DELAY_MAX 400
#define SNAKE_DELAY_OFFSET 0
#define VOLUME_CHANGE_STEP 5

void SnakeMainGame(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay);
void MenuClearLineTile(drawtools& DrawTools, map& Map, int PosY) {
	PosY = PosY / DrawTools.TileSize - INFO_BAR_SIZE;
	for (int x = 1; x < Map.Width-1; x++)
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY);
	}
	PosY--;
	for (int x = 1; x < Map.Width - 1; x++)
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY);
	}
}
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
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
bool PauseMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int& SnakeDelay, saveddata& Data, int FoodEaten, int Score) {
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
	int StringsCount = 5; // Сколько будет опций выбора
	std::string Strings[] = { "Continue", "Exit to main menu","Game volume:", "Music volume: ",  "Exit to desktop" }; // Названия опций выбора	
	const int BufferSize = 32;
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
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
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
				if (Strings[SelectedButtonNum] == "Continue") // Если выбрали кнопку "Продолжить"
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
					return false;
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // Если "Выйти"
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
					return true;
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // Если "Выйти на рабочий стол"
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
					exit(0); // Закрываем приложение
				}
			}
			else if (Keycode == GMKEY_LEFT) // Если стрелочка влево
			{
				if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
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
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
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
				SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
				return false;
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
	return false;
}
void InfoMenu(drawtools& DrawTools, audiotools& Audio, map& Map) {
	HDC& cHDC = DrawTools.Console.cHDC; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // Передаем в новую переменную по ссылке для упрощения чтения кода
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int TileSize = DrawTools.TileSize; // Передаем в новую переменную по ссылке для упрощения чтения кода
	int BaseColor = GCLR_DARKWOOD; // Добавляем дополнительную переменную, чтобы можно было в одной строчке кода изменить цвета для обычных строчек текста	

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для обводки
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // Выбор цвета для заливки
	DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню

	RECT ClientRect, WindowRect; // Переменные для определения координат центра окна
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // Определяем ко-рды рабочей зоны
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // Определяем ко-рды окна		
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 2 };	 // Положение центра линий текста

	int StringsCount = 7; // Сколько будет строчек текста
	std::string Strings[] = { "It might help you...","", "Controls for all the stuff: arrow keys, Enter, Esc", "You can pause with Enter and Esc!", "The quicker you eat, the bigger the score reward!", " ","Got it!" }; // Строчки текста


	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Рисуем опции выбора		

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
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
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
}
void SettingsMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int& SnakeDelay, saveddata& Data) {
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
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // Положение слова "настройки"
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // Положение центра линий текста

	RenderText(DrawTools, "Settings", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Рисуем слово "настройки"
	int StringsCount = 5; // Сколько будет опций выбора
	std::string Strings[] = { "Back", "Mode: ", "Snake speed: ", "Game volume:", "Music volume: ", }; // Названия опций выбора
	const int BufferSize = 32;
	char Buffer[BufferSize]; // Буфер для различных именяющихся строчек
	sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay - SNAKE_DELAY_OFFSET); // Переводим число строку
	Strings[2] = Buffer; // Обновляем надпись
	sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
	Strings[3] = Buffer; // Обновляем надпись	
	sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // Переводим число в строку
	Strings[4] = Buffer; // Обновляем надпись	
	if (Map.Walls)
	{
		Strings[1] += "WALLS";
	}
	else
	{
		Strings[1] += "NO WALLS";
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
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
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					Map.Walls = Map.Walls ? false : true; // Переключаем режим стен
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // Переключаем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay + SNAKE_DELAY_CHANGE_STEP <= SNAKE_DELAY_MAX) // Если в строчке найдено ключевое слово
				{
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					SnakeDelay += SNAKE_DELAY_CHANGE_STEP; // Уменьшаяем задержку движения змейки
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay - SNAKE_DELAY_OFFSET); // Переводим число строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // Если в строчке найдено ключевое слово
				{
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
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук перемещения по меню					
					Map.Walls = Map.Walls ? false : true; // Переключаем режим стен
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // Переключаем надпись		
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay - SNAKE_DELAY_CHANGE_STEP >= SNAKE_DELAY_MIN) // Если в строчке найдено ключевое слово
				{
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					SnakeDelay -= SNAKE_DELAY_CHANGE_STEP; // Увеличиваем задержку движения змейки
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay - SNAKE_DELAY_OFFSET); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // Увеличиваем громкость игрового звука
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // Проигрываем звук изменения опции
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // Переводим число в строку
					Strings[SelectedButtonNum] = Buffer; // Обновляем надпись					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // Затираем старую надпись
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Рисуем выделенным текстом
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // Если в строчке найдено ключевое слово
				{
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
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // Проигрываем звук нажатия на опцию меню
				if (Strings[SelectedButtonNum] == "Play") // Если выбрали кнопку "Игать"
				{
					SnakeMainGame(DrawTools, Audio, Map, Data, SnakeDelay); // Запускаем змейку					
					// Вышли из змейки, рисуем меню заново					
					DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста					
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл
				}
				else if (Strings[SelectedButtonNum] == "Back") // Если "Выйти"
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
					return; // Возвращаемся обратно
				}
			}
			else if (Keycode == GMKEY_ESC)
			{
				SaveFileData(Data, Audio, Map, SnakeDelay); // записываем данные настроек в файл
				return;
			}
		}
		Sleep(1); // Ждём чуть-чуть, уменшить количество проверок на разворачивание
	}
}
void MainMenu(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay) {
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
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (true)
	{
		if (WindowMaximized(DrawTools)) // Обновляем кадр, если окно развернули (потмоу что при сворачивании картинка почему-то затирается)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // Ждем, пока окно достаточно не развернется
			// Далее, собсна, рисуем всё заново
			DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
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
					DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста					
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
				}
				else if (Strings[SelectedButtonNum] == "Settings") // Если "Выйти"
				{
					SettingsMenu(DrawTools, Audio, Map, SnakeDelay, Data);
					DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // Текста			
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // Обновляем
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // Тайтл					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // Рисуем меня снизу
				}
				else if (Strings[SelectedButtonNum] == "Help") // Если "Выйти"
				{
					InfoMenu(DrawTools, Audio, Map);
					DrawMainMenuBackGround(DrawTools, Map); // Обновляем шахматный фон меню
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