#ifndef _SOUND_H_
#define _SOUND_H_
#include <SDL_mixer.h>
#define SND_CHANNEL 4

Mix_Music *music;
void init_game_music(char *filename);

#endif