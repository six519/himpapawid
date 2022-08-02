#include "game.h"

void generate_rocks()
{
	for (int x=0; x < 12; x++)
	{
		generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	}
}

void handle_game_over()
{
	while (SDL_PollEvent(&game_event))
	{
		switch (game_event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}	

	draw_image(bg, game.renderer);
	if (bg.y == 0)
	{
		bg.y = 0 - GAME_HEIGHT;
	}
	bg.y += BG_SPEED;

	score_text = get_font_texture("Score:");
	char *str = malloc(10);
	sprintf(str, "%d", score);
	score_value_text = get_font_texture(str);
	draw_text(score_text, 10, 5);
	draw_text(score_value_text, 100, 5);
	free(str);

	draw_image(game_over, game.renderer);
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
	miss->x = player.x + 16;
	miss->y = player.y;
}

void generate_bullet(int x, int y, int down_only, int go_right)
{
	struct Object *lv;
	lv = malloc(sizeof(struct Object));
	memset(lv, 0, sizeof(struct Object));
	game.bullet_tail->next = lv;
	game.bullet_tail = lv;
	SDL_Point p = get_image_size(enemy_bullet);
	lv->x = x + 5;
	lv->y = y;
	lv->down_only = down_only;
	lv->go_right = go_right;
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

void generate_alien(int x, int y)
{
	struct Object *lv;
	lv = malloc(sizeof(struct Object));
	memset(lv, 0, sizeof(struct Object));
	game.alien_tail->next = lv;
	game.alien_tail = lv;
	SDL_Point p = get_image_size(alien_1);
	lv->sp = 1;
	lv->x = x;
	lv->y = y;
}

void generate_explosion(int x, int y)
{
	struct Object *lv;
	lv = malloc(sizeof(struct Object));
	memset(lv, 0, sizeof(struct Object));
	game.explosion_tail->next = lv;
	game.explosion_tail = lv;
	SDL_Point p = get_image_size(explosion_1);
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
	struct Object *m, *prev, *l, *lprev, *al, *aprev, *el, *eprev, *bl, *blprev;
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
			Mix_PlayChannel(-1, shot, 0);
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

	if (alien_can_spawn)
	{
		generate_alien(generate_random_number(0, GAME_WIDTH), -16);
		alien_can_spawn = 0;
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

	if (alien_spawn_speed == ALIEN_SPAWN_SPEED)
	{
		alien_spawn_speed = 0;
		alien_can_spawn = 1;
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
	alien_spawn_speed += 1;

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

	aprev = &game.alien_head;

	for (al = game.alien_head.next ; al != NULL ; al = al->next)
	{
		al->y += ALIEN_SPEED;

		if (al->bsp == BULLET_SPAWN_SPEED)
		{
			if (al->sp)
			{
		
				al->sp = 0;
				int down_only = 0;
				int checker = generate_random_number(0, 200);

				if (checker > 100)
				{
					down_only = 1;
				}

				int go_right = 0;

				if(player.x > al->x)
				{
					go_right = 1;
				}

				generate_bullet(al->x, al->y, down_only, go_right);
				Mix_PlayChannel(-1, plasma, 0);
			}
		} 
		else 
		{
			al->bsp += 1;
		}

		if (al->y > GAME_HEIGHT)
		{
			if (al == game.alien_tail)
			{
				game.alien_tail = aprev;
			}

			aprev->next = al->next;
			free(al);
			al = aprev;
		}

		//check collision to player
		if (is_collided(al->x, al->y, alien_1.w, alien_1.h, player.x, player.y, player.w, player.h))
		{
			generate_explosion(al->x, al->y);
			if (al == game.alien_tail)
			{
				game.alien_tail = aprev;
			}

			aprev->next = al->next;
			free(al);
			al = aprev;
			Mix_PlayChannel(-1, explode, 0);
			lives -= 1;

			if (lives == 0)
			{
				game_state = 2;
			}
		}

		//check collision to missile
		prev = &game.missile_head;
		for (m = game.missile_head.next ; m != NULL ; m = m->next)
		{
			if (is_collided(al->x, al->y, alien_1.w, alien_1.h, m->x, m->y, missile.w, missile.h))
			{
				generate_explosion(al->x, al->y);
				if (al == game.alien_tail)
				{
					game.alien_tail = aprev;
				}

				aprev->next = al->next;
				free(al);
				al = aprev;
				score += 5;
				Mix_PlayChannel(-1, explode, 0);

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

		aprev = al;
	}

	for (al = game.alien_head.next ; al != NULL ; al = al->next)
	{
		alien_1.x = al->x;
		alien_1.y = al->y;
		draw_image(alien_1, game.renderer);
	}

	blprev = &game.bullet_head;

	for (bl = game.bullet_head.next ; bl != NULL ; bl = bl->next)
	{
		
		bl->y += BULLET_SPEED;

		if (!bl->down_only)
		{
			if (bl->go_right)
			{
				bl->x += 1;
			} else {
				bl->x -= 1;
			}
		}

		if (bl->y > GAME_HEIGHT || bl->x > GAME_WIDTH || bl->x < 0)
		{
			if (bl == game.bullet_tail)
			{
				game.bullet_tail = blprev;
			}

			blprev->next = bl->next;
			free(bl);
			bl = blprev;
		}

		//check collision to player
		if (is_collided(bl->x, bl->y, enemy_bullet.w, enemy_bullet.h, player.x, player.y, player.w, player.h))
		{
			generate_explosion(bl->x, bl->y);
			if (bl == game.bullet_tail)
			{
				game.bullet_tail = blprev;
			}

			blprev->next = bl->next;
			free(bl);
			bl = blprev;
			Mix_PlayChannel(-1, explode, 0);
			lives -= 1;

			if (lives == 0)
			{
				game_state = 2;
			}
		}

		blprev = bl;
	}

	for (bl = game.bullet_head.next ; bl != NULL ; bl = bl->next)
	{
		enemy_bullet.x = bl->x;
		enemy_bullet.y = bl->y;
		draw_image(enemy_bullet, game.renderer);
	}

	eprev = &game.explosion_head;

	for (al = game.explosion_head.next ; al != NULL ; al = al->next)
	{
		al->cf += 1;
		if (al->cf == (5 * 3) + 1)
		{
			if (al == game.explosion_tail)
			{
				game.explosion_tail = eprev;
			}

			eprev->next = al->next;
			free(al);
			al = eprev;
		}

		eprev = al;
	}

	for (al = game.explosion_head.next ; al != NULL ; al = al->next)
	{
		switch (al->cf)
		{
		case 2 * 3:
			explosion_2.x = al->x;
			explosion_2.y = al->y;
			//draw_image(explosion_2, game.renderer);
			draw_image_scale(explosion_2, game.renderer, 32, 32);
			break;
		case 3 * 3:
			explosion_3.x = al->x;
			explosion_3.y = al->y;
			//draw_image(explosion_3, game.renderer);
			draw_image_scale(explosion_3, game.renderer, 32, 32);
			break;
		case 4 * 3:
			explosion_4.x = al->x;
			explosion_4.y = al->y;
			//draw_image(explosion_4, game.renderer);
			draw_image_scale(explosion_4, game.renderer, 32, 32);
			break;
		case 5 * 3:
			explosion_5.x = al->x;
			explosion_5.y = al->y;
			//draw_image(explosion_5, game.renderer);
			draw_image_scale(explosion_5, game.renderer, 32, 32);
			break;
		default:
			explosion_1.x = al->x;
			explosion_1.y = al->y;
			//draw_image(explosion_1, game.renderer);
			draw_image_scale(explosion_1, game.renderer, 32, 32);
			break;
		}
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