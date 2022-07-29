#include "sprite.h"
#include "game.h"
#include <SDL.h>
#include <SDL_image.h>

SDL_Texture *load_image(char *filename, SDL_Renderer *renderer)
{
	int ret;
	SDL_Texture *texture;
	texture = IMG_LoadTexture(renderer, filename);
	return texture;
}

void draw_image(SDL_Texture *texture, int x, int y, SDL_Renderer *renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}