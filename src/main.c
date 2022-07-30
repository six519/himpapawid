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
Sprite turbo1;
Sprite turbo2;
Sprite title;
Mix_Music *music;
SDL_Event game_event;
int first_frame;

void exit_func()
{
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(bg.texture);
	SDL_DestroyTexture(turbo1.texture);
	SDL_DestroyTexture(turbo2.texture);
	SDL_DestroyTexture(title.texture);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
	SDL_Quit();
}

int main()
{
	int game_state = 0;
	first_frame = 1;
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
	init_image(&turbo1, game.renderer, 0, 0, "data/turbo1.png");
	init_image(&turbo2, game.renderer, 0, 0, "data/turbo2.png");

	init_image(&title, game.renderer, 0, 0, "data/title.png");
	title.x = (GAME_WIDTH / 2) - (title.w / 2);
	title.y = (GAME_HEIGHT / 2) - (title.h / 2);

	atexit(exit_func);

	while(1)
	{
		SDL_SetRenderDrawColor(game.renderer, 127, 0, 255, 255); //violet
		SDL_RenderClear(game.renderer);

		switch (game_state)
		{	
		case 1:
			handle_game();
			break;	
		default:
			handle_title();
			break;
		}

		SDL_RenderPresent(game.renderer);
		SDL_Delay(18);
	}

	return 0;
}