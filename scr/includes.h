/*-------------------------------------------------------------------
	includes.h

	В этот файл подключаются все остальные
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз

// Дефайны, которые используются в нескольких файлах
#define WINDOW_MAXIMIZED_RENDER_DELAY 200 // Задержка перед рендером изображение (используется при разворачивании окна)
#define INFO_BAR_SIZE 2 // Высота инфобара в плитках
#define M_PI 3.14159265358979323846 // Число пи

// Стандартные библиотеки
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <cmath>

// Библиотека BASS
#include "bass.h"

// Собственные .h файлы
#include "structenum.h"
#include "files.h"
#include "render.h"
#include "sound.h"
#include "menus.h"
#include "snake.h"

// Подключение lib файла
#pragma comment(lib,"bass.lib")