#include "game.h"

void handle_title()
{
	while (SDL_PollEvent(&game_event))
	{
		switch (game_event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
					if (game_event.key.repeat == 0)
					{
						if (game_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
						{
							game_state = 1;
						}	
					}
				break;
			default:
				break;
		}
	}	

	if (title.x < ((GAME_WIDTH / 2) - (title.w / 2)) && loaded)
	{
		title.x += 4;
	}

	draw_image(bg_title, game.renderer);
	draw_image(title, game.renderer);
}

void generate_missile()
{
	struct Missile *miss;
	miss = malloc(sizeof(struct Missile));
	memset(miss, 0, sizeof(struct Missile));
	game.missile_tail->next = miss;
	game.missile_tail = miss;
	miss->x = player.x + 20;
	miss->y = player.y;
}

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

		if (game_event.key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			game->firing = value;
		}
	}
}

void handle_game()
{
	struct Missile *m, *prev;
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

	if (game.firing)
	{
		if (missile_can_spawn)
		{
			missile_can_spawn = 0;
			generate_missile();
			Mix_PlayChannel(0, shot, 0);
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

	if (missile_spawn_speed == MISSILE_SPAWN_SPEED)
	{
		missile_spawn_speed = 0;
		missile_can_spawn = 1;
	}

	missile_spawn_speed += 1;

	prev = &game.missile_head;

	for (m = game.missile_head.next ; m != NULL ; m = m->next)
	{
		m->y -= 8;

		if (m->y < (0 - missile.h))
		{
			if (m == game.missile_tail)
			{
				game.missile_tail = prev;
			}

			prev->next = m->next;
			free(m);
			m = prev;
		}

		prev = m;
	}

	for (m = game.missile_head.next ; m != NULL ; m = m->next)
	{
		missile.x = m->x;
		missile.y = m->y;
		draw_image(missile, game.renderer);
	}

}