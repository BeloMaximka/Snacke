#pragma once
#include "includes.h"
void SoundsInit(sounds& Sounds) {
	// GSND_CHEW1
	Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew1.mp3", 0, 0, 0));
	int Test = BASS_ErrorGetCode();
	// GSND_CHEW2
	Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew2.mp3", 0, 0, 0));
	// GSND_CHEW3
	Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew3.mp3", 0, 0, 0));
	// GSND_CHEW4
	Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew4.mp3", 0, 0, 0));
	// GSND_MENU_MOVEMENT
	Sounds.push_back(BASS_StreamCreateFile(false, "./sounds/chew2.mp3", 0, 0, 0));	
}
void PlaySoundB(sounds& Sounds, int SoundID, double VolumePercent) {
	VolumePercent /= 100;
	BASS_ChannelSetAttribute(Sounds[SoundID], BASS_ATTRIB_VOL, VolumePercent);
	BASS_ChannelPlay(Sounds[SoundID], true);
}