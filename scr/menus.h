/*-------------------------------------------------------------------
	menus.h

	�������, �������, �������
-------------------------------------------------------------------*/

#pragma once // ����� ���� ����������� ����������� ������ ���� ���
#include "includes.h" // ��������� .h ���� � ���������

#define SCORE_DIGITS 5 // ���������� ����� � �����
#define SNAKE_DELAY_CHANGE_STEP 25 // ��� ��������� �������� ������ � ����������
#define SNAKE_DELAY_MIN 100 // ����������� �������� ������
#define SNAKE_DELAY_MAX 400 // ������������� �������� ������
#define VOLUME_CHANGE_STEP 5 // ��� ��������� ��������� � ����������

// �������� �� ����� snake.h, ����� ��������� �� �������
void SnakeMainGame(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay);

// �������� ������ ������ �����
void MenuClearLineTile(drawtools& DrawTools, map& Map, int PosY) {	
	PosY = (PosY-DrawTools.TileSize/2) / DrawTools.TileSize - INFO_BAR_SIZE; // ������������ ���������� ���� � ���������� �����
	for (int x = 1; x < Map.Width - 1; x++) // �������� �� �������� �����
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY); // ���������� ������ ���
	}
	PosY++; // ���������� ���� �� ����
	for (int x = 1; x < Map.Width - 1; x++) // �������� �� ����� ����
	{
		DrawTile(DrawTools, { x, PosY }, TILE_EMPTY); // ���������� ������ ���
	}
}

// ���� ����� ���������
bool RetryMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int TileSize = DrawTools.TileSize; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int BaseColor = GCLR_DARKBURLYWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ������� ������� ������
	int SelectedButtonColor = GCLR_DARKWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ���������� ������� ������ (� ��������� ����, ������)	

	RECT ClientRect, WindowRect; // ���������� ��� ����������� ��������� ������ ����
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // ���������� ��-��� ������� ����
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // ���������� ��-��� ����	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // ��������� game over

	DrawMap(DrawTools, Map, true); // ������������ �����, ������� �������������� �� ����
	RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // ������ game over
	LOGFONT TempFont; // ��������� �������� ��� �������� �������� �� ������
	GetObject(DrawTools.NormalFont, sizeof(LOGFONT), &TempFont); // �������� ���������� � ������ NormalFont
	pos ScorePos = MainTitlePos; // ������� ������ �����
	ScorePos.y += TempFont.lfHeight * 2; // ��������� ���� �� ��� ������ NormalFont
	std::string ScoreStr = "Your score: "; // ������ "Your score "
	char StrBuffer[SCORE_DIGITS + 1]; // ����� ��� �������� ����� ������ � ��������� ����
	_itoa_s(Score, StrBuffer, SCORE_DIGITS + 1, 10); // ��������� ���� �� ����� � ������
	ScoreStr += StrBuffer; // ��������� � "Your score: "
	RenderText(DrawTools, ScoreStr.c_str(), ScorePos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ����
	int StringsCount = 3; // ������� ����� ����� ������
	std::string Strings[] = { "Retry", "Exit to main menu", "Exit to desktop" }; // �������� ����� ������	
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 16) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // ��������� ������ ����� ������
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������ ����� ������

	int SelectedButtonNum = 0; // ��������� ����� ������ �� ���������
	pos ActiveButtonPos = TextLinesCenterPos; // ������� ��������� �����. �������� � ������, ����� �������
	if (StringsCount % 2) // � ����������� �� �������� ����� ������ ������� ��������
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // ������� ������� � ���������
	}
	else // ���� ������
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // ������� ������� � ���������
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // ������� � ����������� �������	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ��������� �����
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // ������� ����� �� ���������� ������
	while (true)
	{
		if (WindowMaximized(DrawTools)) // ��������� ����, ���� ���� ���������� (������ ��� ��� ������������ �������� ������-�� ����������)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // ����, ���� ���� ���������� �� �����������
			// �����, ������, ������ �� ������			
			DrawMap(DrawTools, Map, true); // �����
			DrawInfoBar(DrawTools, Map, FoodEaten, Score); // �������
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
			RenderText(DrawTools, "GAME OVER", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // �����
			RenderText(DrawTools, ScoreStr.c_str(), ScorePos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ����
		}
		if (_kbhit()) // ���� ������ �����-������ ������
		{
			int Keycode = _getch(); // ���������� ��� ������� ������� � ����������
			if (Keycode == 224) Keycode = _getch(); // ����������� �� �����������: ��� ��� ����� ��� ����. ����� ������, ������
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // ���� ��������� ����� (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum--; // ������ ����� ��������� �����
				ActiveButtonPos.y -= TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // ���� ��������� ����  (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				//Rectangle(cHDC, 0, ActiveButtonPos.y, ActiveButtonPos.x + Map.Width / 2 * TileSize, ActiveButtonPos.y + TileSize); // �������� ����� ��������� �������
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum++; // ������ ����� ��������� �����
				ActiveButtonPos.y += TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_ENTER) // ���� �����
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
				if (Strings[SelectedButtonNum] == "Retry") // ���� ������� ������ ������
				{
					return true; // true - ����������
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // ���� "�����"
				{
					return false; // false - � ������� ����
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // ���� "����� �� ������� ����"
				{
					exit(0); // ��������� ����������
				}
			}
		}
		Sleep(1); // ��� ����-����, ����� �������� ���������� �������� �� ��������������
	}
	return false; // false - � ������� ����
}

// ���� �����
bool PauseMenu(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int FoodEaten, int Score) {
	HDC& cHDC = DrawTools.Console.cHDC; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int TileSize = DrawTools.TileSize; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int BaseColor = GCLR_DARKBURLYWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ������� ������� ������
	int SelectedButtonColor = GCLR_DARKWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ���������� ������� ������ (� ��������� ����, ������)	

	RECT ClientRect, WindowRect; // ���������� ��� ����������� ��������� ������ ����
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // ���������� ��-��� ������� ����
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // ���������� ��-��� ����	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // ��������� �����

	DrawMap(DrawTools, Map, true); // ������������ �����, ������� �������������� �� ����
	RenderText(DrawTools, "GAME PAUSED", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // ������ �����	
	int StringsCount = 5; // ������� ����� ����� ������
	std::string Strings[] = { "Continue", "Exit to main menu","Game volume:", "Music volume: ",  "Exit to desktop" }; // �������� ����� ������	
	const int BufferSize = 32; // ������ ������
	char Buffer[BufferSize]; // ����� ��� ��������� ����������� �������	
	sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
	Strings[2] = Buffer; // ��������� �������	
	sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
	Strings[3] = Buffer; // ��������� �������	
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 16) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // ��������� ������ ����� ������
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������ ����� ������

	int SelectedButtonNum = 0; // ��������� ����� ������ �� ���������
	pos ActiveButtonPos = TextLinesCenterPos; // ������� ��������� �����. �������� � ������, ����� �������
	if (StringsCount % 2) // � ����������� �� �������� ����� ������ ������� ��������
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // ������� ������� � ���������
	}
	else // ���� ������
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // ������� ������� � ���������
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // ������� � ����������� �������	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ��������� �����
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // ������� ����� �� ���������� ������
	bool VolumeChanged = false; // ���� �� ��������� ���������
	while (true)
	{
		if (WindowMaximized(DrawTools)) // ��������� ����, ���� ���� ���������� (������ ��� ��� ������������ �������� ������-�� ����������)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // ����, ���� ���� ���������� �� �����������
			// �����, ������, ������ �� ������			
			DrawMap(DrawTools, Map, true); // �����
			DrawInfoBar(DrawTools, Map, FoodEaten, Score); // �������
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
			RenderText(DrawTools, "GAME PAUSED", MainTitlePos, DrawTools.BigFont, SelectedButtonColor, true); // �����
		}
		if (_kbhit()) // ���� ������ �����-������ ������
		{
			int Keycode = _getch(); // ���������� ��� ������� ������� � ����������
			if (Keycode == 224) Keycode = _getch(); // ����������� �� �����������: ��� ��� ����� ��� ����. ����� ������, ������
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // ���� ��������� ����� (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum--; // ������ ����� ��������� �����
				ActiveButtonPos.y -= TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // ���� ��������� ����  (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				//Rectangle(cHDC, 0, ActiveButtonPos.y, ActiveButtonPos.x + Map.Width / 2 * TileSize, ActiveButtonPos.y + TileSize); // �������� ����� ��������� �������
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum++; // ������ ����� ��������� �����
				ActiveButtonPos.y += TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_ENTER) // ���� �����
			{
				if (Strings[SelectedButtonNum] == "Continue") // ���� ������� ������ "����������"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
					if (VolumeChanged) // ���� ���� ��������� ���������
					{
						SaveFileData(Data, Audio); // ���������� ������ �������� � ����
					}
					return false; // false - ���������� ����
				}
				else if (Strings[SelectedButtonNum] == "Exit to main menu") // ���� "�����"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
					if (VolumeChanged) // ���� ���� ��������� ���������
					{
						SaveFileData(Data, Audio); // ���������� ������ �������� � ����
					}
					return true; // true - ����������� ����
				}
				else if (Strings[SelectedButtonNum] == "Exit to desktop") // ���� "����� �� ������� ����"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
					if (VolumeChanged) // ���� ���� ��������� ���������
					{
						SaveFileData(Data, Audio); // ���������� ������ �������� � ����
					}
					exit(0); // ��������� ����������
				}
			}
			else if (Keycode == GMKEY_LEFT) // ���� ��������� �����
			{
				if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // ���� � ������� ������� �������� �����
				{
					VolumeChanged = true; // ��������� ��������� ����
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // ���� � ������� ������� �������� �����
				{
					VolumeChanged = true; // ��������� ��������� ����
					Audio.MusicVolumePercent -= VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // ��������� ��������� ������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
			}
			else if (Keycode == GMKEY_RIGHT) // ���� ��������� �����
			{
				if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent + VOLUME_CHANGE_STEP <= 100) // ���� � ������� ������� �������� �����
				{
					VolumeChanged = true; // ��������� ��������� ����
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // ���� � ������� ������� �������� �����
				{
					VolumeChanged = true; // ��������� ��������� ����
					Audio.MusicVolumePercent += VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // ��������� ��������� ������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����					
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
			}
			else if (Keycode == GMKEY_ESC)
			{
				if (VolumeChanged) // ���� ���� ��������� ���������
				{
					SaveFileData(Data, Audio); // ���������� ������ �������� � ����
				}
				return false; // false - ���������� ����
			}
		}
		Sleep(1); // ��� ����-����, �������� ���������� �������� �� ��������������
	}
	return false; // false - ���������� ����
}

// ���� � �����������
void InfoMenu(drawtools& DrawTools, audiotools& Audio, map& Map) {
	HDC& cHDC = DrawTools.Console.cHDC; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int TileSize = DrawTools.TileSize; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int BaseColor = GCLR_DARKWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ������� ������� ������	

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����

	RECT ClientRect, WindowRect; // ���������� ��� ����������� ��������� ������ ����
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // ���������� ��-��� ������� ����
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // ���������� ��-��� ����		
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 2 };	 // ��������� ������ ����� ������

	int StringsCount = 7; // ������� ����� ������� ������
	std::string Strings[] = { "It might help you...","", "Controls for all the stuff: arrow keys, Enter, Esc", "You can pause with Enter and Esc!", "The quicker you eat, the bigger the score reward!", " ","Got it!" }; // ������� ������


	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������ ����� ������		

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // ������� ����� �� ���������� ������
	while (true)
	{
		if (WindowMaximized(DrawTools)) // ��������� ����, ���� ���� ���������� (������ ��� ��� ������������ �������� ������-�� ����������)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // ����, ���� ���� ���������� �� �����������
			// �����, ������, ������ �� ������
			DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������						
		}
		if (_kbhit()) // ���� ������ �����-������ ������
		{
			int Keycode = _getch(); // ���������� ��� ������� ������� � ����������
			if (Keycode == 224) Keycode = _getch(); // ����������� �� �����������: ��� ��� ����� ��� ����. ����� ������, ������

			else if (Keycode == GMKEY_ENTER) // ���� �����
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
				return; // ������������ �������				
			}
			else if (Keycode == GMKEY_ESC) // ���� �����
			{

				return; // ������������ �������				
			}
		}
		Sleep(1); // ��� ����-����, ����� ��������� ���������� �������� �� ��������������
	}
}

// ���� ��������
void SettingsMenu(drawtools& DrawTools, audiotools& Audio, map& Map, int& SnakeDelay, saveddata& Data) {
	HDC& cHDC = DrawTools.Console.cHDC; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int TileSize = DrawTools.TileSize; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int BaseColor = GCLR_DARKBURLYWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ������� ������� ������
	int SelectedButtonColor = GCLR_DARKWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ���������� ������� ������ (� ��������� ����, ������)

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����

	RECT ClientRect, WindowRect; // ���������� ��� ����������� ��������� ������ ����
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // ���������� ��-��� ������� ����
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // ���������� ��-��� ����
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // ��������� ����� "���������"
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // ��������� ������ ����� ������

	RenderText(DrawTools, "Settings", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // ������ ����� "���������"
	int StringsCount = 5; // ������� ����� ����� ������
	std::string Strings[] = { "Back", "Mode: ", "Snake speed: ", "Game volume:", "Music volume: ", }; // �������� ����� ������
	const int BufferSize = 32;
	char Buffer[BufferSize]; // ����� ��� ��������� ����������� �������
	sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // ��������� ����� ������
	Strings[2] = Buffer; // ��������� �������
	sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
	Strings[3] = Buffer; // ��������� �������	
	sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
	Strings[4] = Buffer; // ��������� �������	
	if (Map.Walls) // ���� ����� �� ������
	{
		Strings[1] += "WALLS"; // ��������� ����� � ������ ������ ����
	}
	else // ���� ����� ��� ����
	{
		Strings[1] += "NO WALLS"; // ��������� ����� � ������ ������ ����
	}

	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������ ����� ������		

	int SelectedButtonNum = 0; // ��������� ����� ������ �� ���������
	pos ActiveButtonPos = TextLinesCenterPos; // ������� ��������� �����. �������� � ������, ����� �������
	if (StringsCount % 2) // � ����������� �� �������� ����� ������ ������� ��������
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // ������� ������� � ���������
	}
	else // ���� ������
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // ������� ������� � ���������
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // ������� � ����������� �������	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ��������� �����

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // ������� ����� �� ���������� ������
	bool AnyChanges = false; // ���������� ��� �������� �����-���� ��������� ��������
	while (true)
	{
		if (WindowMaximized(DrawTools)) // ��������� ����, ���� ���� ���������� (������ ��� ��� ������������ �������� ������-�� ����������)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // ����, ���� ���� ���������� �� �����������
			// �����, ������, ������ �� ������
			DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
			RenderText(DrawTools, "Settings", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // �����
		}
		if (_kbhit()) // ���� ������ �����-������ ������
		{
			int Keycode = _getch(); // ���������� ��� ������� ������� � ����������
			if (Keycode == 224) Keycode = _getch(); // ����������� �� �����������: ��� ��� ����� ��� ����. ����� ������, ������
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // ���� ��������� ����� (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum--; // ������ ����� ��������� �����
				ActiveButtonPos.y -= TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // ���� ��������� ����  (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum++; // ������ ����� ��������� �����
				ActiveButtonPos.y += TileSize * 2; // �������  ������� ��������� ����� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_LEFT) // ���� ��������� �����
			{
				if (Strings[SelectedButtonNum].find("Mode: ", 0) != std::string::npos) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					Map.Walls = Map.Walls ? false : true; // ����������� ����� ����
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // ����������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay + SNAKE_DELAY_CHANGE_STEP <= SNAKE_DELAY_MAX) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					SnakeDelay += SNAKE_DELAY_CHANGE_STEP; // ���������� �������� �������� ������
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // ��������� ����� ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent - VOLUME_CHANGE_STEP >= 0) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					Audio.GameVolumePercent -= VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent - VOLUME_CHANGE_STEP >= 0) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					Audio.MusicVolumePercent -= VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // ��������� ��������� ������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
			}
			else if (Keycode == GMKEY_RIGHT) // ���� ��������� �����
			{
				if (Strings[SelectedButtonNum].find("Mode: ", 0) != std::string::npos) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����					
					Map.Walls = Map.Walls ? false : true; // ����������� ����� ����
					Strings[SelectedButtonNum] = Map.Walls ? "Mode: WALLS" : "Mode: NO WALLS"; // ����������� �������		
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Snake speed: ", 0) != std::string::npos && SnakeDelay - SNAKE_DELAY_CHANGE_STEP >= SNAKE_DELAY_MIN) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					SnakeDelay -= SNAKE_DELAY_CHANGE_STEP; // ����������� �������� �������� ������
					sprintf_s(Buffer, "Snake speed: %i", SNAKE_DELAY_MAX + SNAKE_DELAY_MIN - SnakeDelay); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Game volume: ", 0) != std::string::npos && Audio.GameVolumePercent + VOLUME_CHANGE_STEP <= 100) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					Audio.GameVolumePercent += VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����
					sprintf_s(Buffer, "Game volume: %i", (int)Audio.GameVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
				else if (Strings[SelectedButtonNum].find("Music volume: ", 0) != std::string::npos && Audio.MusicVolumePercent + VOLUME_CHANGE_STEP <= 100) // ���� � ������� ������� �������� �����
				{
					AnyChanges = true; // ������� ��������� � ����������
					Audio.MusicVolumePercent += VOLUME_CHANGE_STEP; // ����������� ��������� �������� �����
					BASS_ChannelSetAttribute(Audio.Sounds[GSND_MUSIC], BASS_ATTRIB_VOL, Audio.MusicVolumePercent / 100); // ��������� ��������� ������
					PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ��������� �����					
					sprintf_s(Buffer, "Music volume: %i", (int)Audio.MusicVolumePercent); // ��������� ����� � ������
					Strings[SelectedButtonNum] = Buffer; // ��������� �������					
					MenuClearLineTile(DrawTools, Map, ActiveButtonPos.y); // �������� ������ �������
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
				}
			}
			else if (Keycode == GMKEY_ENTER) // ���� �����
			{
				if (Strings[SelectedButtonNum] == "Back") // ���� "�����"
				{
					PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
					if (AnyChanges) // ���� ���� �����-���� ��������� � ����������
					{
						SaveFileData(Data, Audio, Map, SnakeDelay); // ���������� ������ �������� � ����
					}					
					return; // ������������ �������
				}
			}
			else if (Keycode == GMKEY_ESC)
			{
				if (AnyChanges) // ���� ���� �����-���� ��������� � ����������
				{
					SaveFileData(Data, Audio, Map, SnakeDelay); // ���������� ������ �������� � ����
				}			
				return; // ������������ �������
			}
		}
		Sleep(1); // ��� ����-����, �������� ���������� �������� �� ��������������
	}
}

// ������� ����
void MainMenu(drawtools& DrawTools, audiotools& Audio, map& Map, saveddata& Data, int SnakeDelay) {
	HDC& cHDC = DrawTools.Console.cHDC; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HPEN>& Pens = DrawTools.Palette.Pens; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	std::vector<HBRUSH>& Brushes = DrawTools.Palette.Brushes; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int TileSize = DrawTools.TileSize; // �������� � ����� ���������� �� ������ ��� ��������� ������ ����
	int BaseColor = GCLR_DARKBURLYWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ������� ������� ������
	int SelectedButtonColor = GCLR_DARKWOOD; // ��������� �������������� ����������, ����� ����� ���� � ����� ������� ���� �������� ����� ��� ���������� ������� ������ (� ��������� ����, ������)

	SelectObject(cHDC, Pens[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	SelectObject(cHDC, Brushes[GCLR_LIGHTBURLYWOOD]); // ����� ����� ��� �������
	DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����

	RECT ClientRect, WindowRect; // ���������� ��� ����������� ��������� ������ ����
	GetClientRect(DrawTools.Console.cHWND, &ClientRect); // ���������� ��-��� ������� ����
	GetWindowRect(DrawTools.Console.cHWND, &WindowRect); // ���������� ��-��� ����	
	pos MainTitlePos = { (WindowRect.right - WindowRect.left) / 2 , (WindowRect.bottom - WindowRect.top) / 4 }; // ��������� �������� ����
	pos TextLinesCenterPos = { (WindowRect.right - WindowRect.left) / 2,((MainTitlePos.y + (WindowRect.bottom - WindowRect.top) / 8) + (WindowRect.bottom - WindowRect.top)) / 2 };	 // ��������� ������ ����� ������

	RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // ������ �������� ����
	int StringsCount = 4; // ������� ����� ����� ������
	std::string Strings[] = { "Play", "Settings", "Help", "Exit" }; // �������� ����� ������	
	DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������ ����� ������		
	RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // ������ ���� �����

	int SelectedButtonNum = 0; // ��������� ����� ������ �� ���������
	pos ActiveButtonPos = TextLinesCenterPos; // ������� ��������� �����. �������� � ������, ����� �������
	if (StringsCount % 2) // � ����������� �� �������� ����� ������ ������� ��������
	{
		ActiveButtonPos.y -= StringsCount * TileSize; // ������� ������� � ���������
	}
	else // ���� ������
	{
		ActiveButtonPos.y -= TileSize + (StringsCount - 1) * TileSize; // ������� ������� � ���������
	}
	ActiveButtonPos.y += SelectedButtonNum * TileSize * 2;	 // ������� � ����������� �������	
	RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ��������� �����

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));  // ������� ����� �� ���������� ������
	while (true)
	{
		if (WindowMaximized(DrawTools)) // ��������� ����, ���� ���� ���������� (������ ��� ��� ������������ �������� ������-�� ����������)
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY); // ����, ���� ���� ���������� �� �����������
			// �����, ������, ������ �� ������
			DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
			DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
			RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
			RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // �����
			RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // ������ ���� �����
		}
		if (_kbhit()) // ���� ������ �����-������ ������
		{
			int Keycode = _getch(); // ���������� ��� ������� ������� � ����������
			if (Keycode == 224) Keycode = _getch(); // ����������� �� �����������: ��� ��� ����� ��� ����. ����� ������, ������			
			if (Keycode == GMKEY_UP && SelectedButtonNum > 0) // ���� ��������� ����� (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum--; // ������ ����� ��������� �����
				ActiveButtonPos.y -= TileSize * 2; // �������  ������� ��������� ����� �� ����				
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_DOWN && SelectedButtonNum + 1 < StringsCount) // ���� ��������� ����  (� �������� �� ����� �� ������)
			{
				PlaySoundB(Audio, GSND_MENU_MOVEMENT, Audio.GameVolumePercent); // ����������� ���� ����������� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, BaseColor, true); // ������ ������� ������
				SelectedButtonNum++; // ������ ����� ��������� �����
				ActiveButtonPos.y += TileSize * 2; // �������  ������� ��������� ����� �� ����
				RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ������ ���������� �������
			}
			else if (Keycode == GMKEY_ENTER) // ���� �����
			{
				PlaySoundB(Audio, GSND_MENU_ENTER, Audio.GameVolumePercent); // ����������� ���� ������� �� ����� ����
				if (Strings[SelectedButtonNum] == "Play") // ���� ������� ������ "�����"
				{
					SnakeMainGame(DrawTools, Audio, Map, Data, SnakeDelay); // ��������� ������					
					// ����� �� ������, ������ ���� ������					
					DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������					
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // �����					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // ������ ���� �����
				}
				else if (Strings[SelectedButtonNum] == "Settings") // ���� "�����"
				{
					SettingsMenu(DrawTools, Audio, Map, SnakeDelay, Data);
					DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // �����					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // ������ ���� �����
				}
				else if (Strings[SelectedButtonNum] == "Help") // ���� "�����"
				{
					InfoMenu(DrawTools, Audio, Map);
					DrawMenuBackground(DrawTools, Map); // ��������� ��������� ��� ����
					DrawTextLines(DrawTools, Strings, StringsCount, TextLinesCenterPos, DrawTools.NormalFont, BaseColor, true); // ������			
					RenderText(DrawTools, Strings[SelectedButtonNum].c_str(), ActiveButtonPos, DrawTools.NormalFont, SelectedButtonColor, true); // ���������
					RenderText(DrawTools, "SNACKE!", MainTitlePos, DrawTools.TitleFont, SelectedButtonColor, true); // �����					
					RenderText(DrawTools, "Made by BeloMaximka", { MainTitlePos.x,(Map.Height + INFO_BAR_SIZE) * TileSize - TileSize / 2 }, DrawTools.SmallFont, SelectedButtonColor, true); // ������ ���� �����
				}

				else if (Strings[SelectedButtonNum] == "Exit") // ���� "�����"
				{
					exit(0); // ��������� ����������
				}
			}
		}
		Sleep(1); // ��� ����-����, �������� ���������� �������� �� ��������������
	}
}