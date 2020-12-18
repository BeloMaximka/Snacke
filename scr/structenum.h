#pragma once
#include "includes.h"

typedef std::vector<HSTREAM> sounds;
struct pos
{
	int x;
	int y;
};
struct dpos
{
	double x;
	double y;
};
struct map
{
	int** Tiles;
	int Height;
	int Width;
};
struct snake
{
	pos HeadPos;
	pos TailPos;
	int Segments;
	int Head;
	int OldHead;
	int FoodEaten;
	int Score;
};
struct console
{
	HANDLE cHANDLE;	
	HWND cHWND;
	HDC cHDC;
};
struct palette
{
	std::vector<HPEN> Pens;
	std::vector<HBRUSH> Brushes;
	std::vector<COLORREF> Colors;
};
struct drawtools
{	
	palette Palette;
	console Console;
	HFONT SmallFont;
	HFONT NormalFont;
	HFONT BigFont;
	HFONT TitleFont;
	int TileSize;
	int WindowState;
};
struct audiotools
{
	sounds Sounds;
	double GameVolumePercent;
	double MusicVolumePercent;
};

enum Sounds
{
	GSND_MUSIC,
	GSND_CHEW1,
	GSND_CHEW2,
	GSND_CHEW3,
	GSND_CHEW4,
	GSND_MENU_MOVEMENT
};
enum Keys
{
	GMKEY_LEFT = 75,
	GMKEY_RIGHT = 77,
	GMKEY_UP = 72,
	GMKEY_DOWN = 80,
	GMKEY_ENTER = 13,
};
enum Tiles
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_SNAKE_UP,
	TILE_SNAKE_DOWN,
	TILE_SNAKE_LEFT,
	TILE_SNAKE_RIGHT,
	TILE_FOOD
};
enum Directions
{
	DIR_LEFT = -1,
	DIR_RIGHT = 1,
	DIR_UP = -2,
	DIR_DOWN = 2
};
enum Colors
{
	GCLR_BLACK,
	GCLR_WHITE,
	GCLR_LIGHTBURLYWOOD,
	GCLR_BURLYWOOD,
	GCLR_DARKBURLYWOOD,
	GCLR_DARKWOOD,
	GCLR_SCARLET,
	GCLR_RED,
	GCLR_DARKRED,
	GCLR_GREEN,
	GCLR_DARKGREEN,
	GCLR_YELLOW,
	GCLR_DARKYELLOW,
	GCLR_BROWN,
	GCLR_LIGHTRED
};
