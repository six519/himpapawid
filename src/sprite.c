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

void draw_text(SDL_Texture *texture, int x, int y)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(game.renderer, texture, NULL, &dest);
}

void draw_image_scale(Sprite sprite, SDL_Renderer *renderer, int w,  int h)
{
	SDL_Rect dest;
	dest.x = sprite.x;
	dest.y = sprite.y;
	dest.w = w;
	dest.h = h;

	SDL_RenderCopy(renderer, sprite.texture, NULL, &dest);
}

SDL_Point get_image_size(Sprite sprite)
{
	SDL_Point size;
	SDL_QueryTexture(sprite.texture, NULL, NULL, &size.x, &size.y);
	return size;
}

void init_image(Sprite *sprite, SDL_Renderer *renderer, int x, int y, char *filename)
{
	memset(sprite, 0, sizeof(Sprite));
	sprite->x = x;
	sprite->y = y;
	sprite->texture = load_image(filename, renderer);

	SDL_Point pnt = get_image_size(*sprite);
	sprite->w = pnt.x;
	sprite->h = pnt.y;
}