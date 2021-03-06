/*-------------------------------------------------------------------
	sound.h

	����� � ����.
-------------------------------------------------------------------*/

#pragma once // ����� ���� ����������� ����������� ������ ���� ���
#include "includes.h" // ��������� .h ���� � ���������

typedef std::vector<HSTREAM> sounds; // ���� ���� ������� ���������

// ��������� - ���������������� ���� ������
struct pos // ������������� �������
{
	int x; // ������� �� ��� �
	int y; // ������� �� ��� �
};
struct dpos // ������� �������
{
	double x;
	double y;
};
struct map // �����
{
	int** Tiles; // ��������� ������������ ������ ������
	int Height; // ������ �����
	int Width; // ������ �����
	bool Walls; // ����� ����
};
struct snake // ������
{
	pos HeadPos; // ������� ������
	pos TailPos; // ������� ������
	int Segments; // ���������� ���������
	int Head; // ����������� ������
	int OldHead; // ������ ����������� ������
	int FoodEaten; // ���������� �������� ���
	int FoodReward; // ������� �� ��������� ���
	int Score; // ����
	int Delay; // �������� ����� ������
};
struct console // ����������� �������
{
	HANDLE cHANDLE; // ���������� �������
	HWND cHWND; // ���������� ����
	HDC cHDC; // ���������� �������
};
struct palette // �����
{
	std::vector<HPEN> Pens; // �������
	std::vector<HBRUSH> Brushes; // �������
	std::vector<COLORREF> Colors; // ����� RGB
};
struct drawtools // �����������, ����������� ��� ������ � ��������
{
	palette Palette; // ������� ������
	console Console; // ����������� �������
	HFONT SmallFont; // ����� �����
	HFONT NormalFont; // ������� �����
	HFONT BigFont; // ������� �����
	HFONT TitleFont; // ����� ����������
	int TileSize; // ������ ������
	int WindowState; // ��������� ���� (�����������, ���������)
};
struct audiotools // �����, ���������
{
	sounds Sounds; // ������ ������
	double GameVolumePercent; // ��������� ����
	double MusicVolumePercent; // ��������� ������
};
struct saveddata // ������, ������� ����������� � ����
{
	double GameVolumePercent; // ��������� ����
	double MusicVolumePercent; // ��������� ������
	int SnakeDelay; // �������� ����� ������ ����
	bool Walls; // ����� ����
	bool FirstStart; // ������ ������ ����
};

// ������������
enum Sounds // ������������ ������
{
	GSND_MUSIC, // ������
	GSND_CHEW1, // ������ ����� �����
	GSND_CHEW2, // ������ ����
	GSND_CHEW3, // ������ ����
	GSND_CHEW4, // ��������� ����
	GSND_MENU_MOVEMENT, // ���� ����������� �� ����
	GSND_MENU_ENTER, // ���� ������� �������
	GSND_PUNCH // ���� ����� �� �����������
};
enum Keys // ���� ������
{
	GMKEY_LEFT = 75, // ����� �������
	GMKEY_RIGHT = 77, // ������ �������
	GMKEY_UP = 72, // ������� �����
	GMKEY_DOWN = 80, // �������� ����
	GMKEY_ENTER = 13, // Enter
	GMKEY_ESC = 27, // Esc
};
enum Tiles // ��������, ������, ��������
{
	TILE_EMPTY, // ������
	TILE_WALL, // �����
	TILE_SNAKE_UP, // ������� ����, ������������ �����
	TILE_SNAKE_DOWN, // ������� ����, ������������ ����
	TILE_SNAKE_LEFT, // ������� ����, ������������ �����
	TILE_SNAKE_RIGHT, // ������� ����, ������������ ������
	TILE_FOOD // ���
};
enum Directions // �����������
{
	DIR_LEFT = -1, // �����
	DIR_RIGHT = 1, // ������
	DIR_UP = -2, // �����
	DIR_DOWN = 2 // ����
};
enum Colors // �����
{
	GCLR_BLACK, // ������
	GCLR_WHITE, // �����
	GCLR_LIGHTBURLYWOOD, // ������� burlywood
	GCLR_BURLYWOOD, // Burlywood
	GCLR_DARKBURLYWOOD, // ������ burlywood
	GCLR_DARKWOOD, // ���� ������� ������
	GCLR_SCARLET, // ����
	GCLR_RED, // �������
	GCLR_DARKRED, // �����-�������
	GCLR_GREEN, // �������
	GCLR_DARKGREEN, // �����-�������
	GCLR_YELLOW, // ������
	GCLR_DARKYELLOW, // �����-������
	GCLR_BROWN, // ����������
	GCLR_LIGHTRED //������-�������
};
