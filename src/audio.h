#ifndef audio_h
#define audio_h
#include "raylib.h"
#include "state.h"

typedef enum SFX_ID {
    SFX_HIT,
    SFX_ENHANCE,
    SFX_DELVE,
    SFX_DEATH,
    SFX_PING,
    SFX_NULLIFY,

    SFX_SIZE,
} SFX_ID;

extern Sound SFX_Sounds[SFX_SIZE];

void LoadAudio(void);
void UnloadAudio(void);
void PlaySFX(SFX_ID);

#endif