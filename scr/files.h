#pragma once
#include "includes.h"
#define FILENAME "data.bin"
bool ReadFileData(saveddata& Data, audiotools& Audio, int& SnakeDelay, map& Map) {
	FILE* File;
	fopen_s(&File, FILENAME, "rb");
	if (File == nullptr)
	{
		return false;
	}
	fseek(File, 0L, SEEK_END);
	ftell(File);
	if (ftell(File) != sizeof(saveddata))
	{
		fclose(File);
		return false;
	}
	else
	{
		fseek(File, 0L, SEEK_SET);
		fread(&Data, sizeof(saveddata), 1, File);
		Audio.MusicVolumePercent = Data.MusicVolumePercent;
		Audio.GameVolumePercent = Data.GameVolumePercent;
		SnakeDelay = Data.SnakeDelay;
		Map.Walls = Data.Walls;
		fclose(File);
		return true;
	}
}
void CreateFileData(saveddata& Data, audiotools& Audio, int& SnakeDelay, map& Map) {
	Data.FirstStart = true;
	Audio.GameVolumePercent = 100;
	Data.GameVolumePercent = Audio.GameVolumePercent;
	Audio.MusicVolumePercent = 100;
	Data.MusicVolumePercent = Audio.MusicVolumePercent;
	SnakeDelay = SNAKE_DEFAULT_DELAY;
	Data.SnakeDelay = SnakeDelay;
	Map.Walls = true;
	Data.Walls = Map.Walls;
	FILE* File;
	fopen_s(&File, FILENAME, "wb");
	fwrite(&Data, sizeof(saveddata), 1, File);
	fclose(File);
}
void SaveFileData(saveddata& Data, audiotools& Audio, map& Map, int SnakeDelay) {
	FILE* File;
	Data.GameVolumePercent = Audio.GameVolumePercent;
	Data.MusicVolumePercent = Audio.MusicVolumePercent;
	Data.Walls = Map.Walls;
	Data.SnakeDelay = SnakeDelay;
	fopen_s(&File, FILENAME, "wb");
	fwrite(&Data, sizeof(saveddata), 1, File);
	fclose(File);
}