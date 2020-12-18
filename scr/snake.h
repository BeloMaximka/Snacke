#pragma once
#include "includes.h"
void ClearMap(map& Map) {
	for (int y = 0; y < Map.Height; y++)
	{
		for (int x = 0; x < Map.Width; x++)
		{
			if (Map.Walls && (x == 0 || y == 0 || x == Map.Width - 1 || y == Map.Height - 1))
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
			// ��������� � ������ �������� ����
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
			// � ������� �������� ����
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
			// ��������� � ������ ������� ����
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
			// ��������� � ������� ������� ����
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
bool MoveSnake(drawtools& DrawTools, audiotools& Audio, map& Map, snake& Snake) {
	//--------------������ � �������--------------
	// ������������ ������� ����� ����� �������
	Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	// ������� ����� ������
	pos NewHeadPos = Snake.HeadPos;
	// ���������� ������� ����� ������
	// ���� ������ ������� �����
	if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_UP)
	{
		// ���� ������ ��������� �� ������� �����
		if (NewHeadPos.y - 1 < 0)
		{
			// ������������ ������� �� ������ ����� �����
			NewHeadPos.y = Map.Height - 1;
		}
		else
		{
			// ������� ������� ����� ������ �����
			NewHeadPos.y--;
		}
	}
	// ���� ������ ������� ����
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_DOWN)
	{
		// ���� ������ ��������� �� ������� �����
		if (NewHeadPos.y + 1 >= Map.Height)
		{
			// ������������ ������� �� ������ ����� �����
			NewHeadPos.y = 0;
		}
		else
		{
			// ������� ������� ����� ������ ����
			NewHeadPos.y++;
		}
	}
	// ���� ������ ������� �����
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_LEFT)
	{
		// ���� ������ ��������� �� ������� �����
		if (NewHeadPos.x - 1 < 0)
		{
			// ������������ ������� �� ������ ����� �����
			NewHeadPos.x = Map.Width - 1;
		}
		else
		{
			// ������� ������� ����� ������ �����
			NewHeadPos.x--;
		}
	}
	// ���� ������ ������� �����
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_SNAKE_RIGHT)
	{
		if (Snake.HeadPos.x + 1 >= Map.Width)
		{
			// ������������ ������� �� ������ ����� �����
			NewHeadPos.x = 0;
		}
		else
		{
			// ������� ������� ����� ������ ������
			NewHeadPos.x++;
		}
	}
	// ���� �� ���� �����
	if (NewHeadPos.y == Snake.TailPos.y && NewHeadPos.x == Snake.TailPos.x)
	{
		//--------------������ � �������--------------
	// ����������� ��������� ����� ������ �������
		DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY);
		// ���������� ����� �����
		if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP)
		{
			// ���� 
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
		// ��������� ������� ������ ������
		Snake.HeadPos = NewHeadPos;
		// ��������� ������� ������
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		return true;
	}
	// ���� �� ���� ������ ����������� (�.� �� ����� � �� ���)
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_EMPTY && Map.Tiles[NewHeadPos.y][NewHeadPos.x] != TILE_FOOD)
	{
		// �������� ���������� - ��������� false
		return false;
	}
	// ���� �� ���� ���
	else if (Map.Tiles[NewHeadPos.y][NewHeadPos.x] == TILE_FOOD)
	{
		// ����������� ���� �������� ������
		PlaySoundB(Audio, GSND_CHEW1 + rand() % 4, Audio.GameVolumePercent * 2);
		// ��������� ������� ������ ������
		Snake.HeadPos = NewHeadPos;
		// ��������� ������� ������
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
		// ����������� ���������� ���������
		Snake.Segments++;
		// ����������� ������� �������� ���
		Snake.FoodEaten++;
		// ����������� ����
		Snake.Score += FOOD_SCORE_REWARD;
		// ��������� ������� ��� �� ��������
		DrawInfoFoodCount(DrawTools, Snake.FoodEaten);
		// ��������� ���� �� ��������
		DrawInfoScore(DrawTools, Snake.Score);
		// ������ ��� ���� ���
		SpawnFood(DrawTools, Map);
		// ������������� ������� �����, ����� �� ����� �� ��������
		return true;
	}
	// ���� �� ���� �����
	else
	{
		// ��������� ������� ������ ������
		Snake.HeadPos = NewHeadPos;
		// ��������� ������� ������
		Map.Tiles[Snake.HeadPos.y][Snake.HeadPos.x] = Snake.Head;
	}
	//--------------������ � �������--------------
	// ����������� ��������� ����� ������ �������
	DrawTile(DrawTools, Snake.TailPos, TILE_EMPTY);
	// ���������� ����� �����
	if (Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] == TILE_SNAKE_UP)
	{
		// �������� ��������� ����� ������ �� �������
		Map.Tiles[Snake.TailPos.y][Snake.TailPos.x] = TILE_EMPTY;
		// ���� 
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
		// ������� ����� ������
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
		// ������� ����� ������
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
		// ������� ����� ������
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
void SpawnSnake(map& Map, snake& Snake, int DirTile, pos HeadPos, int Segments, int SnakeDelay) {
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
	Snake.Delay = SnakeDelay;;
}
bool SnakeFirstStep(drawtools& DrawTools, audiotools& Audio, map& Map, snake& Snake) {
	// ����� �� ������� �������
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	int Keycode;
	bool EndLoop = false;
	while (true)
	{
		if (WindowMaximized(DrawTools))
		{
			Sleep(WINDOW_MAXIMIZED_RENDER_DELAY);
			DrawMap(DrawTools, Map);
			DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
			DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
		}
		if (_kbhit())
		{
			Keycode = _getch();
			if (Keycode == 224) Keycode = _getch();
			if (Keycode == GMKEY_ENTER)
			{
				if (PauseMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score))
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
				if (!MoveSnake(DrawTools, Audio, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				EndLoop = true;;
			}
			else if (Keycode == GMKEY_DOWN && Snake.OldHead != TILE_SNAKE_UP)
			{
				Snake.Head = TILE_SNAKE_DOWN;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Audio, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				EndLoop = true;
			}
			else if (Keycode == GMKEY_LEFT && Snake.OldHead != TILE_SNAKE_RIGHT)
			{
				Snake.Head = TILE_SNAKE_LEFT;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Audio, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				EndLoop = true;
			}
			else if (Keycode == GMKEY_RIGHT && Snake.OldHead != TILE_SNAKE_LEFT)
			{
				Snake.Head = TILE_SNAKE_RIGHT;
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Audio, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					//dead
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				EndLoop = true;
			}
			while (_kbhit())
			{
				Keycode = _getch();
				if (Keycode == 224) Keycode = _getch();
				if (Keycode == GMKEY_ENTER)
				{
					if (PauseMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score))
					{
						return false;
					}
					DrawMap(DrawTools, Map);
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
				}
			}
			if (EndLoop)
			{
				break;
			}
		}
		Sleep(1);
	}
	return true;
}
void SnakeMainGame(drawtools& DrawTools, audiotools& Audio, map& Map, int SnakeDelay) {
	while (true)
	{
		ClearMap(Map); // ������� �����
		// Snake init
		snake Snake;
		int SnakeSpawnSegments = 3;
		pos SnakeSpawnPos = { SnakeSpawnSegments,Map.Height / 2 };
		SpawnSnake(Map, Snake, TILE_SNAKE_RIGHT, SnakeSpawnPos, SnakeSpawnSegments, SnakeDelay);
		DrawMap(DrawTools, Map);
		DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
		SpawnFood(DrawTools, Map);
		DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);		
		
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		if (!SnakeFirstStep(DrawTools, Audio, Map, Snake))
		{
			return;
		}
		Sleep(Snake.Delay);
		int Keycode; // ���������� ��� ������� ���� �������
		// Main loop		
		while (true)
		{
			if (WindowMaximized(DrawTools))
			{
				Sleep(WINDOW_MAXIMIZED_RENDER_DELAY);
				DrawMap(DrawTools, Map);
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
			}
			if (_kbhit())
			{
				if (_kbhit())
				{
					Keycode = _getch();
					if (Keycode == 224) Keycode = _getch();					
					if (Keycode == GMKEY_ENTER)
					{
						if (PauseMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score))
						{
							return;
						}
						DrawMap(DrawTools, Map);
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);						
						if (!SnakeFirstStep(DrawTools, Audio, Map, Snake))
						{
							return;
						}
						Sleep(Snake.Delay);
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
						if (!MoveSnake(DrawTools, Audio, Map, Snake))
						{
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}						
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						Sleep(Snake.Delay);
					}
				}				
				if (_kbhit())
				{
					Keycode = _getch();
					if (Keycode == 224) Keycode = _getch();
					// ������ �����, ����� �� ������ �������� ���������� �������� �������
					//FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
					while (_kbhit())
					{
						Keycode = _getch();
						if (Keycode == 224) Keycode = _getch();
						if (Keycode == GMKEY_ENTER)
						{
							if (PauseMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score))
							{
								return;
							}
							DrawMap(DrawTools, Map);
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							DrawInfoBar(DrawTools, Map, Snake.FoodEaten, Snake.Score);
							if (!SnakeFirstStep(DrawTools, Audio, Map, Snake))
							{
								return;
							}
							Sleep(Snake.Delay);
							continue;
						}
					}					
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
						if (!MoveSnake(DrawTools, Audio, Map, Snake))
						{
							DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
							break;
						}						
						DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
						Sleep(Snake.Delay);
					}
				}
			}
			else
			{
				Snake.OldHead = Snake.Head;
				if (!MoveSnake(DrawTools, Audio, Map, Snake))
				{
					DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
					break;
				}
				DrawSnake(DrawTools, Map, Snake, Map.Tiles[Snake.TailPos.y][Snake.TailPos.x]);
				Sleep(Snake.Delay);
			}
		}
		PlaySoundB(Audio, GSND_PUNCH, Audio.GameVolumePercent);
		Sleep(1500);
		if (!RetryMenu(DrawTools, Audio, Map, Snake.FoodEaten, Snake.Score))
		{
			return;
		}
	}
}

