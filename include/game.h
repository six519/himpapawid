#ifndef _GAME_H_
#define _GAME_H_
#include <SDL.h>

#define GAME_WIDTH 576
#define GAME_HEIGHT 720

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
} Game;

#endif