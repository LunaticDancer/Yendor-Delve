#ifndef audio_h
#define audio_h
#include "raylib.h"
#include "state.h"

extern Sound SFX_Hit;
extern Sound SFX_Enhance;
extern Sound SFX_Delve;
extern Sound SFX_Death;
extern Sound SFX_Ping;
extern Sound SFX_Nullify;

void LoadAudio(void);
void UnloadAudio(void);
void PlaySFX(Sound);

#endif