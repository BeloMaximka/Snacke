/*-------------------------------------------------------------------
	includes.h

	� ���� ���� ������������ ��� ���������
-------------------------------------------------------------------*/

#pragma once // ����� ���� ����������� ����������� ������ ���� ���

// �������, ������� ������������ � ���������� ������
#define WINDOW_MAXIMIZED_RENDER_DELAY 200 // �������� ����� �������� ����������� (������������ ��� �������������� ����)
#define INFO_BAR_SIZE 2 // ������ �������� � �������
#define M_PI 3.14159265358979323846 // ����� ��

// ����������� ����������
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <cmath>

// ���������� BASS
#include "bass.h"

// ����������� .h �����
#include "structenum.h"
#include "files.h"
#include "render.h"
#include "sound.h"
#include "menus.h"
#include "snake.h"

// ����������� lib �����
#pragma comment(lib,"bass.lib")