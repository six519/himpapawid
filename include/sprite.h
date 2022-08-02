#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <SDL.h>

typedef struct
{
	SDL_Texture *texture;
	int x;
	int y;
	int w;
	int h;
} Sprite;

SDL_Texture *load_image(char *filename, SDL_Renderer *renderer);
void draw_image(Sprite sprite, SDL_Renderer *renderer);
void init_image(Sprite *sprite, SDL_Renderer *renderer, int x, int y, char *filename);
void draw_image_scale(Sprite sprite, SDL_Renderer *renderer, int w,  int h);
SDL_Point get_image_size(Sprite sprite);
void draw_text(SDL_Texture *texture, int x, int y);
int is_collided(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif