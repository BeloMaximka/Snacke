#pragma once
#include "includes.h"
struct pos
{
	int x;
	int y;
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
	int TileSize;
};
enum Keys
{
	GMKEY_LEFT = 75,
	GMKEY_RIGHT = 77,
	GMKEY_UP = 72,
	GMKEY_DOWN = 80,
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
	GCLR_YELLOW,
	GCLR_RED,
	GCLR_GREEN,
	GCLR_DARKGREEN,
	GCLR_DARKYELLOW,
	GCLR_BROWN,
	GCLR_LIGHTRED
};
