#pragma once
#define TILESIZE 32
#define WINDOW_STATE_MAXIMIZED 1
#define WINDOW_STATE_MINIMIZED 2
#define WINDOW_MAXIMIZED_RENDER_DELAY 200
#define INFO_BAR_SIZE 2
#define FONT_NORMAL_THICKNESS 0
#define FOOD_SCORE_REWARD 100
#define SCORE_DIGITS 5
#define SNAKE_DEFAULT_DELAY 300
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>
#define M_PI 3.14159265358979323846
#include <cmath>
#include <thread>
#include "bass.h"

#include "structenum.h"
#include "files.h"
#include "render.h"
#include "cpptimer.h"
#include "sound.h"
#include "menus.h"
#include "snake.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"bass.lib")