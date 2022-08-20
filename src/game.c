#include "game.h"

void draw_bg()
{
	draw_image(bg, game.renderer);
	bg.y = (bg.y == 0)? 0 - GAME_HEIGHT : bg.y + BG_SPEED;
}

void generate_rocks()
{
	for (int x=0; x < 12; x++)
	{
		generate_rock(generate_random_number(0, GAME_WIDTH), generate_random_number(0, GAME_HEIGHT));
	}
}

void draw_common(Object *head, Sprite *sprite, int is_scaled)
{
	Object *m;
	for (m = head->next ; m != NULL ; m = m->next)
	{
		sprite->x = m->x;
		sprite->y = m->y;

		if (is_scaled)
		{
			draw_image_scale(*sprite, game.renderer, m->d, m->d);
		}
		else
		{
			draw_image(*sprite, game.renderer);
		}
	}
}

void clear_object(Object *head, Object **tail)
{
	Object *m, *prev;
	prev = head;
	for (m = head->next ; m != NULL ; m = m->next)
	{
		if (m == *tail)
		{
			*tail = prev;
		}

		prev->next = m->next;
		free(m);
		m = prev;
		prev = m;
	}
}

void back_to_title()
{
	game_state = 0;
	score = 0;
	lives = 3;
	title.x = ((GAME_WIDTH / 2) - (title.w / 2)) - title.w;
	game.up = 0;
	game.down = 0;
	game.left = 0;
	game.right = 0;
#ifdef __ANDROID__
	game.firing = 1;
#else
	game.firing = 0;
#endif
	player.x = (GAME_WIDTH / 2) - (player.w / 2);
	player.y = GAME_HEIGHT - (player.h + 20);

	clear_object(&game.missile_head, &game.missile_tail);
	clear_object(&game.rock_head, &game.rock_tail);
	clear_object(&game.alien_head, &game.alien_tail);
	clear_object(&game.explosion_head, &game.explosion_tail);
	clear_object(&game.bullet_head, &game.bullet_tail);
	clear_object(&game.alien2_head, &game.alien2_tail);
	clear_object(&game.explosion2_head, &game.explosion2_tail);
	generate_rocks();
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
			case SDL_KEYDOWN:
					if (game_event.key.repeat == 0 && game_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
					{
						back_to_title();
					}
				break;
#ifdef __ANDROID__
			case SDL_FINGERDOWN:
					back_to_title();
				break;
#endif
			default:
				break;
		}
	}	

	draw_bg();

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
					if (game_event.key.repeat == 0 && game_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
					{
						game_state = 1;
					}
				break;
#ifdef __ANDROID__
			case SDL_FINGERDOWN:
					game_state = 1;
				break;
#endif
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
	Object *miss;
	miss = malloc(sizeof(Object));
	memset(miss, 0, sizeof(Object));
	game.missile_tail->next = miss;
	game.missile_tail = miss;
	miss->x = player.x + 16;
	miss->y = player.y;
}

void generate_bullet(int x, int y, int down_only, int go_right)
{
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
	game.bullet_tail->next = lv;
	game.bullet_tail = lv;
	SDL_Point p = get_image_size(enemy_bullet);
	lv->x = x + 13;
	lv->y = y + 20;
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
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
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
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
	game.alien_tail->next = lv;
	game.alien_tail = lv;
	SDL_Point p = get_image_size(alien_1);
	lv->sp = 1;
	lv->x = x;
	lv->y = y;
}

void generate_alien2(int x, int y)
{
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
	game.alien2_tail->next = lv;
	game.alien2_tail = lv;
	SDL_Point p = get_image_size(alien_2);
	lv->lives = 3;
	lv->change_direction = 0;
	//lv->change_direction_speed = 100;
	lv->down = 1;
	lv->x = x;
	lv->y = y;
}

void generate_explosion(int x, int y)
{
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
	game.explosion_tail->next = lv;
	game.explosion_tail = lv;
	SDL_Point p = get_image_size(explosion_1);
	lv->x = x;
	lv->y = y;
}

void generate_explosion2(int x, int y)
{
	Object *lv;
	lv = malloc(sizeof(Object));
	memset(lv, 0, sizeof(Object));
	game.explosion2_tail->next = lv;
	game.explosion2_tail = lv;
	SDL_Point p = get_image_size(ex1);
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

void set_speed(int *can_spawn, int *spawn_speed, int speed)
{
	if (*spawn_speed == speed)
	{
		*spawn_speed = 0;
		*can_spawn = 1;
	}
}

void handle_game()
{
	Object *m, *prev, *l, *lprev, *al, *aprev, *el, *eprev, *bl, *blprev, *al2, *al2prev, *e2prev;
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
#ifdef __ANDROID__
			case SDL_FINGERDOWN:
				touch_location.x = game_event.tfinger.x * screen_rect.w;
				touch_location.y = game_event.tfinger.y * screen_rect.h;
				finger_x = touch_location.x;
				finger_y = touch_location.y;			
				break;
			case SDL_FINGERUP:
				touch_location.x = game_event.tfinger.x * screen_rect.w;
				touch_location.y = game_event.tfinger.y * screen_rect.h;
				finger_x = 0;
				finger_y = 0;
				game.up = 0;
				game.down = 0;
				game.left = 0;
				game.right = 0;
				break;
			case SDL_FINGERMOTION:
				touch_location.x = game_event.tfinger.x * screen_rect.w;
				touch_location.y = game_event.tfinger.y * screen_rect.h;
				
				LOGI("Start X: %d", finger_x);
				LOGI("Start Y: %d", finger_y);
				LOGI("Current X: %d", touch_location.x);
				LOGI("Current Y: %d", touch_location.y);

				if (finger_y > touch_location.y + 50 && !game.up)
				{
					game.up = 1;
					game.down = 0;
					game.left = 0;
					game.right = 0;
					finger_y = touch_location.y;
				}

				if (finger_y < touch_location.y - 50 && !game.down)
				{
					game.down = 1;
					game.up = 0;
					game.left = 0;
					game.right = 0;
					finger_y = touch_location.y;
				}

				if (finger_x > touch_location.x + 50 && !game.left)
				{
					game.left = 1;
					game.right = 0;
					game.up = 0;
					game.down = 0;
					finger_x = touch_location.x;
				}

				if (finger_x < touch_location.x - 50 && !game.right)
				{
					game.right = 1;
					game.left = 0;
					game.up = 0;
					game.down = 0;
					finger_x = touch_location.x;
				}

				break;
#endif
			default:
				break;
		}
	}

	if (game.firing && missile_can_spawn)
	{
		missile_can_spawn = 0;
		generate_missile();
		Mix_PlayChannel(-1, shot, 0);
	}

	if (game.up && player.y > 0)
	{
		player.y -= PLAYER_SPEED;
	}

	if (game.down && player.y < (GAME_HEIGHT - player.h))
	{
		player.y += PLAYER_SPEED;
	}

	if (game.right && player.x < GAME_WIDTH - player.w)
	{
		player.x += PLAYER_SPEED;
	}

	if (game.left && player.x > 0)
	{
		player.x -= PLAYER_SPEED;
	}

	draw_bg();

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

	if (alien2_can_spawn)
	{
		generate_alien2(generate_random_number(0, GAME_WIDTH - alien_2.w), 0);
		alien2_can_spawn = 0;
	}

	set_speed(&missile_can_spawn, &missile_spawn_speed, MISSILE_SPAWN_SPEED);
	set_speed(&rock_can_spawn, &rock_spawn_speed, ROCK_SPAWN_SPEED);
	set_speed(&alien_can_spawn, &alien_spawn_speed, ALIEN_SPAWN_SPEED);
	set_speed(&alien2_can_spawn, &alien2_spawn_speed, ALIEN2_SPAWN_SPEED);

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
	alien2_spawn_speed += 1;

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

	draw_common(&game.rock_head, &rock, 1);

	al2prev = &game.alien2_head;

	for (al2 = game.alien2_head.next ; al2 != NULL ; al2 = al2->next)
	{
		al2->change_direction_speed += 1;

		if (al2->change_direction_speed == CHANGE_DIRECTION_SPEED || (al2->y > (GAME_HEIGHT - alien_2.h) - 10 && al2->can_change_now) || (al2->y < 10 && al2->can_change_now) || (al2->x > (GAME_WIDTH - alien_2.w) - 10 && al2->can_change_now) || (al2->x < 10 && al2->can_change_now))
		{
			al2->can_change_now = 1;
			al2->change_direction_speed = 0;
			al2->up = 0;
			al2->down = 0;
			al2->left = 0;
			al2->right =0;
			int num = generate_random_number(0, 400);

			if (num >= 0 && num <= 100)
			{
				al2->up = 1;
			}
			else if (num >= 101 && num <= 200)
			{
				al2->down = 1;
			}
			else if (num >= 201 && num <= 300)
			{
				al2->left = 1;
			}
			else
			{
				al2->right = 1;
			}
		}

		if (al2->down)
		{
			al2->y += ALIEN2_SPEED;
		}

		if (al2->up)
		{
			al2->y -= ALIEN2_SPEED;
		}

		if (al2->left)
		{
			al2->x -= ALIEN2_SPEED;
		}

		if (al2->right)
		{
			al2->x += ALIEN2_SPEED;
		}

		//check collision to player
		if (is_collided(al2->x, al2->y, alien_2.w, alien_2.h, player.x, player.y, player.w, player.h))
		{
			generate_explosion2(al2->x, al2->y);

			if (al2 == game.alien2_tail)
			{
				game.alien2_tail = al2prev;
			}

			al2prev->next = al2->next;
			free(al2);
			al2 = al2prev;

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
			if (is_collided(al2->x, al2->y, alien_2.w, alien_2.h, m->x, m->y, missile.w, missile.h))
			{
				generate_explosion2(al2->x, al2->y);
				if (al2 == game.alien2_tail)
				{
					game.alien2_tail = al2prev;
				}

				al2prev->next = al2->next;
				free(al2);
				al2 = al2prev;
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

		al2prev = al2;
	}

	draw_common(&game.alien2_head, &alien_2, 0);

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

	draw_common(&game.alien_head, &alien_1, 0);

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

	draw_common(&game.bullet_head, &enemy_bullet, 0);

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

	e2prev = &game.explosion2_head;

	for (al = game.explosion2_head.next ; al != NULL ; al = al->next)
	{
		al->cf += 1;
		if (al->cf == (7 * 3) + 1)
		{
			if (al == game.explosion2_tail)
			{
				game.explosion2_tail = e2prev;
			}

			e2prev->next = al->next;
			free(al);
			al = e2prev;
		}

		e2prev = al;
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

	draw_common(&game.missile_head, &missile, 0);

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

	for (al = game.explosion2_head.next ; al != NULL ; al = al->next)
	{
		switch (al->cf)
		{
		case 2 * 3:
			ex2.x = al->x;
			ex2.y = al->y;
			draw_image(ex2, game.renderer);
			break;
		case 3 * 3:
			ex3.x = al->x;
			ex3.y = al->y;
			draw_image(ex3, game.renderer);
			break;
		case 4 * 3:
			ex4.x = al->x;
			ex4.y = al->y;
			draw_image(ex4, game.renderer);
			break;
		case 5 * 3:
			ex5.x = al->x;
			ex5.y = al->y;
			draw_image(ex5, game.renderer);
			break;
		case 6 * 3:
			ex6.x = al->x;
			ex6.y = al->y;
			draw_image(ex6, game.renderer);
			break;
		case 7 * 3:
			ex7.x = al->x;
			ex7.y = al->y;
			draw_image(ex7, game.renderer);
			break;
		default:
			ex1.x = al->x;
			ex1.y = al->y;
			draw_image(ex1, game.renderer);
			break;
		}
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