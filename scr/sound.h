/*-------------------------------------------------------------------
	sound.h

	Звуки и басы.
-------------------------------------------------------------------*/

#pragma once // Чтобы файл подключался линковщиком строго один раз
#include "includes.h" // Подключем .h файл с инклюдами

// Загружает звуки в память
void SoundsInit(audiotools& Audio) {
	// GSND_MUSIC
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/music.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_CHEW1
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew1.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_CHEW2
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew2.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_CHEW3
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew3.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_CHEW4
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew4.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_MENU_MOVEMENT
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/menmov.mp3", 0, 0, 0)); // Загружаем звук из файла
	// GSND_MENU_ENTER
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/menent.mp3", 0, 0, 0)); // Загружаем звук из файла
	//GSND_PUNCH
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/punch.mp3", 0, 0, 0)); // Загружаем звук из файла
}

// Проигрывает звук
void PlaySoundB(audiotools& Audio, int SoundID, double VolumePercent, bool Loop = false) {
	VolumePercent /= 100; // Делим процент на сто, очевидно зачем
	if (Loop) // Если звук нужно сделать зацикленым
	{
		BASS_ChannelFlags(Audio.Sounds[SoundID], BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP); // Зацикливаем звук
	}
	BASS_ChannelSetAttribute(Audio.Sounds[SoundID], BASS_ATTRIB_VOL, VolumePercent); // Выставляем громкость звука
	BASS_ChannelPlay(Audio.Sounds[SoundID], true); // Проигрываем звук
}