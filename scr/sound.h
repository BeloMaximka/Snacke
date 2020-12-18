#pragma once
#include "includes.h"
void SoundsInit(audiotools& Audio) {
	Audio.GameVolumePercent = 100;
	Audio.MusicVolumePercent = 100;
	// GSND_MUSIC
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/music.mp3", 0, 0, 0));
	// GSND_CHEW1
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew1.mp3", 0, 0, 0));
	// GSND_CHEW2
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew2.mp3", 0, 0, 0));
	// GSND_CHEW3
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew3.mp3", 0, 0, 0));
	// GSND_CHEW4
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew4.mp3", 0, 0, 0));
	// GSND_MENU_MOVEMENT
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/menmov.mp3", 0, 0, 0));
	// GSND_MENU_ENTER
	Audio.Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/menmov.mp3", 0, 0, 0));
}
void PlaySoundB(audiotools& Audio, int SoundID, double VolumePercent, bool Loop = false) {
	VolumePercent /= 100;
	if (Loop)
	{
		BASS_ChannelFlags(Audio.Sounds[SoundID], BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	}
	BASS_ChannelSetAttribute(Audio.Sounds[SoundID], BASS_ATTRIB_VOL, VolumePercent);
	BASS_ChannelPlay(Audio.Sounds[SoundID], true);
}