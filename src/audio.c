#include "audio.h"
#include <stdlib.h>
#include <stdio.h>
#include "state.h"


Sound SFX_Hit;
Sound SFX_Enhance;
Sound SFX_Delve;
Sound SFX_Death;
Sound SFX_Ping;
Sound SFX_Nullify;

extern struct AppState appState;

void LoadAudio(void) {
    InitAudioDevice();

    SFX_Hit = LoadSound("assets/audio/sfx/hit.wav");
    SFX_Enhance = LoadSound("assets/audio/sfx/enhance.wav");
    SFX_Delve = LoadSound("assets/audio/sfx/delve.wav");
    SFX_Death = LoadSound("assets/audio/sfx/death.wav");
    SFX_Ping = LoadSound("assets/audio/sfx/ping.wav");
    SFX_Nullify = LoadSound("assets/audio/sfx/nullify.wav");
};

void UnloadAudio(void) {
    UnloadSound(SFX_Hit);
    UnloadSound(SFX_Enhance);
    UnloadSound(SFX_Delve);
    UnloadSound(SFX_Death);
    UnloadSound(SFX_Ping);
    UnloadSound(SFX_Nullify);
    
    CloseAudioDevice();

    return;
};

void PlaySFX(Sound sound) {
    SetSoundVolume(sound, ((float)appState.settings.sfxVolume)/10.);
    PlaySound(sound);
};