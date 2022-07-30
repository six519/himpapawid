#ifndef _GAME_H_
#define _GAME_H_
#include <SDL.h>

#define GAME_WIDTH 576
#define GAME_HEIGHT 720

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	int up;
	int down;
	int left;
	int right;
} Game;

void handle_key(SDL_KeyboardEvent *event, int value, Game *game);

#endif