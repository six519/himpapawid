#include "game.h"

void handle_key(SDL_KeyboardEvent *event, int value, Game *game)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			game->up = value;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			game->down = value;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			game->left = value;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			game->right = value;
		}
	}
}