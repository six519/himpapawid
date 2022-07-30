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

void handle_game()
{
	while (SDL_PollEvent(&game_event))
	{
		switch (game_event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				handle_key(&game_event.key, 1, &game);
				break;
			case SDL_KEYUP:
				handle_key(&game_event.key, 0, &game);
				break;
			default:
				break;
		}
	}

	if (game.up)
	{
		player.y -= 4;
	}

	if (game.down)
	{
		player.y += 4;
	}

	if (game.right)
	{
		player.x += 4;
	}

	if (game.left)
	{
		player.x -= 4;
	}

	draw_image(bg, game.renderer);
	draw_image(player, game.renderer);

	if (game.up || game.down || game.left || game.right)
	{
		if (first_frame)
		{
			turbo1.x = player.x + 7;
			turbo1.y = player.y + player.h;
			draw_image(turbo1, game.renderer);
			turbo1.x += 26;
			draw_image(turbo1, game.renderer);
			turbo1.x -= 26;
			first_frame = 0;
		}
		else
		{
			first_frame = 1;
			turbo2.x = player.x + 7;
			turbo2.y = player.y + player.h;
			draw_image(turbo2, game.renderer);
			turbo2.x += 26;
			draw_image(turbo2, game.renderer);
			turbo2.x -= 26;
		}
	}

	if (bg.y == 0)
	{
		bg.y = 0 - GAME_HEIGHT;
	}
	bg.y = bg.y + 1;
}