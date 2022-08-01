#include "game.h"

void generate_rocks()
{
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
}

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
	struct Object *miss;
	miss = malloc(sizeof(struct Object));
	memset(miss, 0, sizeof(struct Object));
	game.missile_tail->next = miss;
	game.missile_tail = miss;
	miss->x = player.x + 20;
	miss->y = player.y;
}

void reset_nep()
{
	nep.x = generate_random_number(-100, GAME_WIDTH - 80);
	nep.y = 0 - (nep.h + 100);
}

void generate_rock(int x, int y)
{
	struct Object *lv;
	lv = malloc(sizeof(struct Object));
	memset(lv, 0, sizeof(struct Object));
	game.rock_tail->next = lv;
	game.rock_tail = lv;

	SDL_Point p = get_image_size(rock);
	int rn = generate_random_number(5, p.x);

	lv->d = rn;
	lv->x = x;
	lv->y = y;
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
	struct Object *m, *prev, *l, *lprev;
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
		if (player.y > 0)
		{
			player.y -= PLAYER_SPEED;
		}
	}

	if (game.down)
	{
		if (player.y < (GAME_HEIGHT - player.h))
		{
			player.y += PLAYER_SPEED;
		}
	}

	if (game.right)
	{
		if (player.x < GAME_WIDTH - player.w)
		{
			player.x += PLAYER_SPEED;
		}
	}

	if (game.left)
	{
		if (player.x > 0)
		{
			player.x -= PLAYER_SPEED;
		}
	}

	draw_image(bg, game.renderer);
	if (bg.y == 0)
	{
		bg.y = 0 - GAME_HEIGHT;
	}
	bg.y += BG_SPEED;

	if (rock_can_spawn)
	{
		generate_rock(generate_random_number(0, GAME_WIDTH), -38);
		rock_can_spawn = 0;
	}

	if (missile_spawn_speed == MISSILE_SPAWN_SPEED)
	{
		missile_spawn_speed = 0;
		missile_can_spawn = 1;
	}

	if (rock_spawn_speed == ROCK_SPAWN_SPEED)
	{
		rock_spawn_speed = 0;
		rock_can_spawn = 1;
	}

	//draw_image(nep, game.renderer);
	draw_image_scale(nep, game.renderer, 188, 188);
	nep.y += NEP_SPEED;

	if (nep.y > GAME_HEIGHT)
	{
		reset_nep();
	}

	missile_spawn_speed += 1;
	rock_spawn_speed += 1;

	lprev = &game.rock_head;

	for (l = game.rock_head.next ; l != NULL ; l = l->next)
	{
		l->y += ROCK_SPEED;

		if (l->y > GAME_HEIGHT)
		{
			if (l == game.rock_tail)
			{
				game.rock_tail = lprev;
			}

			lprev->next = l->next;
			free(l);
			l = lprev;
		}

		lprev = l;
	}

	for (l = game.rock_head.next ; l != NULL ; l = l->next)
	{
		rock.x = l->x;
		rock.y = l->y;
		draw_image_scale(rock, game.renderer, l->d, l->d);
		//draw_image(rock, game.renderer);
	}


	prev = &game.missile_head;

	for (m = game.missile_head.next ; m != NULL ; m = m->next)
	{
		m->y -= MISSILE_SPEED;

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

	score_text = get_font_texture("Score:");
	char *str = malloc(10);
	sprintf(str, "%d", score);
	score_value_text = get_font_texture(str);
	lives_text = get_font_texture("Lives: ");
	draw_text(score_text, 10, 5);
	draw_text(score_value_text, 100, 5);
	draw_text(lives_text, 10, 30);
	free(str);

	rocket.x = 100;
	rocket.y = 43;
	for (int x=0; x < lives; x++)
	{
		draw_image(rocket, game.renderer);
		rocket.x += 20;
	}

}

SDL_Texture *get_font_texture(char *txt)
{
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	surface = TTF_RenderUTF8_Blended(font, txt, color);
	texture = SDL_CreateTextureFromSurface(game.renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}