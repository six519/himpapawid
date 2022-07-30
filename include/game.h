#ifndef _GAME_H_
#define _GAME_H_
#include <SDL.h>
#include <SDL_mixer.h>
#include "sprite.h"

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

Game game;
Sprite player;
Sprite bg;
Sprite turbo1;
Sprite turbo2;
Mix_Music *music;
SDL_Event game_event;
int first_frame;

void handle_key(SDL_KeyboardEvent *event, int value, Game *game);
void handle_game();

#endif