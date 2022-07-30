#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>

typedef struct
{
	SDL_Texture *texture;
	int x;
	int y;
} Sprite;

SDL_Texture *load_image(char *filename, SDL_Renderer *renderer);
void draw_image(Sprite sprite, SDL_Renderer *renderer);
void init_image(Sprite *sprite, SDL_Renderer *renderer, int x, int y, char *filename);

#endif