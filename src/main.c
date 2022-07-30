#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "misc.h"
#include "sprite.h"
#include "game.h"
#include "sound.h"

Game game;
Sprite player;
Sprite bg;
Mix_Music *music;

void exit_func()
{
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(bg.texture);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
	SDL_Quit();
}

int main()
{
	SDL_Event game_event;
	printf("Himpapawid\n");
	printf("Created By: Ferdinand Silva\n");
	memset(&game, 0, sizeof(Game));
	game.up = 0;
	game.down = 0;
	game.left = 0;
	game.right = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		print_error("Unable to initialize SDL: %s.\n", SDL_GetError());
	}

	game.window = SDL_CreateWindow(TITLE_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, 0);

	if(!game.window)
	{
		print_error("Unable to create window: %s.\n", SDL_GetError());
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
	{
		print_error("Unable to initialize Mixer: %s.\n", SDL_GetError());
	}

	Mix_AllocateChannels(SND_CHANNEL);

	init_game_music("data/bg.mp3");
	Mix_PlayMusic(music, -1);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	init_image(&player, game.renderer, 0, 0, "data/ship_1.png");
	player.x = (GAME_WIDTH / 2) - (player.w / 2);
	player.y = GAME_HEIGHT - (player.h + 20);

	init_image(&bg, game.renderer, 0, 0 - GAME_HEIGHT, "data/bg_1.png");

	atexit(exit_func);

	while(1)
	{
		SDL_SetRenderDrawColor(game.renderer, 127, 0, 255, 255); //violet
		SDL_RenderClear(game.renderer);

		while (SDL_PollEvent(&game_event))
		{
			switch (game_event.type)
			{
				case SDL_QUIT:
					return 0;
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

		if (bg.y == 0)
		{
			bg.y = 0 - GAME_HEIGHT;
		}
		bg.y = bg.y + 1;

		SDL_RenderPresent(game.renderer);
		SDL_Delay(12);
	}

	return 0;
}