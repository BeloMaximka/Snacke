/*-------------------------------------------------------------------
	files.h

	Работа с файлом data.bin, в котором хранятся параметры игры
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами
#define FILENAME "data.bin" // Дефайн для обозначения названия файла

#define SNAKE_DEFAULT_DELAY 150 // Базовая скорость змейки

// Считывает данные с файла
bool ReadFileData(saveddata& Data, audiotools& Audio, int& SnakeDelay, map& Map) {
	FILE* File; // Для хранения файлового потока
	fopen_s(&File, FILENAME, "rb"); // Открываем бинарный файл для чтения
	if (File == nullptr) // Если открыть не удалось
	{
		return false; // Возвращаем false - операция не удалась
	}
	fseek(File, 0L, SEEK_END); // Идем в самый конец файла	
	if (ftell(File) != sizeof(saveddata)) // Если размер файла не соответствует нужному
	{
		fclose(File); // Закрываем файл
		return false; // Возвращаем false - файл непригодный для чтения
	}
	else // Если всё окей
	{
		fseek(File, 0L, SEEK_SET); // Идем обратно в начало файла 
		fread(&Data, sizeof(saveddata), 1, File); // Считываем данные с файла в переменную с данным
		Audio.MusicVolumePercent = Data.MusicVolumePercent; // Выставляем громкость музыки
		Audio.GameVolumePercent = Data.GameVolumePercent; // Выставляем громкость звука
		SnakeDelay = Data.SnakeDelay; // Выставляем задержку между шагами змейки
		Map.Walls = Data.Walls; // Выставляем режим со стенами ли без
		fclose(File); // Закрываем файл
		return true; // Операция удалась - возращаем true
	}
}

// Создает файл с базовыми параметрами
void CreateFileData(saveddata& Data, audiotools& Audio, int& SnakeDelay, map& Map) {
	Data.FirstStart = true; // Задаем первый запуск
	Audio.GameVolumePercent = 100; // Задаем громкость игры
	Data.GameVolumePercent = Audio.GameVolumePercent; // Записываем её в переменну с данными
	Audio.MusicVolumePercent = 50; // Задаем громкость музыки
	Data.MusicVolumePercent = Audio.MusicVolumePercent; // Записываем её в переменну с данными
	SnakeDelay = SNAKE_DEFAULT_DELAY; // Задаем задержку между шагами змейки
	Data.SnakeDelay = SnakeDelay; // Записываем её в переменну с данными
	Map.Walls = true; // Задаем режим со стенами
	Data.Walls = Map.Walls; // Записываем его в переменну с данными
	FILE* File; // Для хранения файлового потока
	fopen_s(&File, FILENAME, "wb"); // Открываем бинарный файл для записи
	fwrite(&Data, sizeof(saveddata), 1, File); // Записываем данные в файл
	fclose(File); // Закрываем файл
}

// Сохраняет файл с параметрами
void SaveFileData(saveddata& Data, audiotools& Audio, map& Map, int SnakeDelay) {
	FILE* File; // Для хранения файлового потока
	Data.GameVolumePercent = Audio.GameVolumePercent; // Записываем громкость игры в переменну с данными
	Data.MusicVolumePercent = Audio.MusicVolumePercent; // Записываем громкость музыки в переменну с данными
	Data.Walls = Map.Walls;  // Записываем стен в переменну с данными
	Data.SnakeDelay = SnakeDelay;  // Записываем задержку между шагами змейки в переменну с данными
	fopen_s(&File, FILENAME, "wb"); // Открываем бинарный файл для записи
	fwrite(&Data, sizeof(saveddata), 1, File); // Записываем данные в файл
	fclose(File); // Закрываем файл
}

// Сохраняет файл с параметрами, где обновились только настройки звука
void SaveFileData(saveddata& Data, audiotools& Audio) {
	FILE* File; // Для хранения файлового потока
	Data.GameVolumePercent = Audio.GameVolumePercent; // Записываем громкость игры в переменну с данными
	Data.MusicVolumePercent = Audio.MusicVolumePercent; // Записываем громкость музыки в переменну с данными
	fopen_s(&File, FILENAME, "wb"); // Открываем бинарный файл для записи
	fwrite(&Data, sizeof(saveddata), 1, File); // Записываем данные в файл
	fclose(File); // Закрываем файл
}