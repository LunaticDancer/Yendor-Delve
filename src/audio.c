#include "audio.h"
#include <stdlib.h>
#include <stdio.h>
#include "state.h"


extern struct AppState appState;
Sound SFX_Sounds[SFX_SIZE];

void LoadAudio(void) {
    InitAudioDevice();

    SFX_Sounds[SFX_HIT] = LoadSound("assets/audio/sfx/hit.wav");
    SFX_Sounds[SFX_ENHANCE] = LoadSound("assets/audio/sfx/enhance.wav");
    SFX_Sounds[SFX_DELVE] = LoadSound("assets/audio/sfx/delve.wav");
    SFX_Sounds[SFX_DEATH] = LoadSound("assets/audio/sfx/death.wav");
    SFX_Sounds[SFX_PING] = LoadSound("assets/audio/sfx/ping.wav");
    SFX_Sounds[SFX_NULLIFY] = LoadSound("assets/audio/sfx/nullify.wav");
};

void UnloadAudio(void) {
    for (int i = 0; i < SFX_SIZE; i++) {
        UnloadSound(SFX_Sounds[i]);
    }
    CloseAudioDevice();

    return;
};

void PlaySFX(SFX_ID sound) {
    SetSoundVolume(SFX_Sounds[sound], ((float)appState.settings.sfxVolume)/10.);
    StopSound(SFX_Sounds[sound]);
    PlaySound(SFX_Sounds[sound]);
};