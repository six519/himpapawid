#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "misc.h"
#include "sprite.h"
#include "game.h"

Game game;
Sprite player;

void exit_func()
{
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
	memset(&player, 0, sizeof(Sprite));

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		print_error("Unable to initialize SDL: %s.\n", SDL_GetError());
	}

	game.window = SDL_CreateWindow(TITLE_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, 0);

	if(!game.window)
	{
		print_error("Unable to create window: %s.\n", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	player.x = 200;
	player.y = 200;
	player.texture = load_image("data/ship_1.png", game.renderer);

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
				default:
					break;
			}
		}

		draw_image(player.texture, player.x, player.y, game.renderer);
		SDL_RenderPresent(game.renderer);
		SDL_Delay(500);
	}

	return 0;
}