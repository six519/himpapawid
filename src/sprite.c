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

void draw_image(Sprite sprite, SDL_Renderer *renderer)
{
	SDL_Rect dest;
	dest.x = sprite.x;
	dest.y = sprite.y;
	SDL_QueryTexture(sprite.texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, sprite.texture, NULL, &dest);
}

void init_image(Sprite *sprite, SDL_Renderer *renderer, int x, int y, char *filename)
{
	memset(sprite, 0, sizeof(Sprite));
	sprite->x = x;
	sprite->y = y;
	sprite->texture = load_image(filename, renderer);

}