#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "misc.h"
#include "sprite.h"
#include "game.h"

Game game;
Sprite player;
Sprite bg;
Sprite turbo1;
Sprite turbo2;
Sprite title;
Sprite bg_title;
Sprite missile;
Sprite rock;
Sprite nep;
Sprite rocket;
Mix_Music *music;
Mix_Chunk *shot;
TTF_Font *font;
SDL_Event game_event;
SDL_Texture *score_text;
SDL_Texture *lives_text;
int first_frame;
int loaded;
int play_bg;
int game_state;
int missile_can_spawn;
int missile_spawn_speed;
int rock_can_spawn;
int rock_spawn_speed;
int score;
int lives;

void exit_func()
{
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(bg.texture);
	SDL_DestroyTexture(turbo1.texture);
	SDL_DestroyTexture(turbo2.texture);
	SDL_DestroyTexture(title.texture);
	SDL_DestroyTexture(bg_title.texture);
	SDL_DestroyTexture(missile.texture);
	SDL_DestroyTexture(rock.texture);
	SDL_DestroyTexture(nep.texture);
	SDL_DestroyTexture(rocket.texture);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(lives_text);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
	TTF_Quit();
	SDL_Quit();
}

int main()
{
	srand(time(NULL));
	game_state = 0;
	loaded = 0;
	play_bg = 0;
	first_frame = 1;
	missile_can_spawn = 1;
	missile_spawn_speed = 0;
	rock_can_spawn = 1;
	rock_spawn_speed = 0;
	score = 0;
	lives = 5;
	printf("Himpapawid\n");
	printf("Created By: Ferdinand Silva\n");
	memset(&game, 0, sizeof(Game));
	game.up = 0;
	game.down = 0;
	game.left = 0;
	game.right = 0;
	game.firing = 0;

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

	if (TTF_Init() < 0)
	{
		print_error("Unable to initialize TTF: %s.\n", SDL_GetError());
	}
	font = TTF_OpenFont("data/tandy.ttf", 25);

	Mix_AllocateChannels(SND_CHANNEL);

	music = Mix_LoadMUS("data/bg.mp3");
	shot = Mix_LoadWAV("data/shot.wav");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	init_image(&player, game.renderer, 0, 0, "data/ship_1.png");
	player.x = (GAME_WIDTH / 2) - (player.w / 2);
	player.y = GAME_HEIGHT - (player.h + 20);

	init_image(&bg, game.renderer, 0, 0 - GAME_HEIGHT, "data/bg_1.png");
	init_image(&turbo1, game.renderer, 0, 0, "data/turbo1.png");
	init_image(&turbo2, game.renderer, 0, 0, "data/turbo2.png");
	init_image(&bg_title, game.renderer, 0, 40, "data/bg_title.jpg");
	init_image(&rock, game.renderer, 0, 0, "data/rock.png");
	init_image(&nep, game.renderer, 0, 0, "data/nep.png");
	init_image(&rocket, game.renderer, 0, 0, "data/rocket.png");

	init_image(&title, game.renderer, 0, 0, "data/title.png");
	title.x = ((GAME_WIDTH / 2) - (title.w / 2)) - title.w;
	title.y = (GAME_HEIGHT / 2) - (title.h / 2);

	init_image(&missile, game.renderer, 0, 0, "data/missile.png");
	game.missile_tail = &game.missile_head;
	game.rock_tail = &game.rock_head;

	atexit(exit_func);

	SDL_Point ls = get_image_size(rock);

	generate_rocks();
	reset_nep();

	while(1)
	{
		SDL_SetRenderDrawColor(game.renderer, 112, 176, 203, 255); //light blue
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
		loaded = 1;

		if (!play_bg)
		{
			Mix_PlayMusic(music, -1);
			play_bg = 1;
		}
	}

	return 0;
}