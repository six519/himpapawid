#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "misc.h"
#include "sprite.h"
#include "game.h"
#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

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
Sprite alien_1;
Sprite alien_2;
Sprite explosion_1;
Sprite explosion_2;
Sprite explosion_3;
Sprite explosion_4;
Sprite explosion_5;
Sprite ex1;
Sprite ex2;
Sprite ex3;
Sprite ex4;
Sprite ex5;
Sprite ex6;
Sprite ex7;
Sprite enemy_bullet;
Sprite game_over;
Mix_Music *music;
Mix_Chunk *shot;
Mix_Chunk *plasma;
Mix_Chunk *explode;
TTF_Font *font;
SDL_Event game_event;
SDL_Texture *score_text;
SDL_Texture *score_value_text;
SDL_Texture *lives_text;
#ifdef TARGET_OS_SIMULATOR
SDL_Rect screen_rect;
SDL_Point touch_location;
int finger_y;
int finger_x;
#endif
int first_frame;
int loaded;
int play_bg;
int game_state;
int missile_can_spawn;
int missile_spawn_speed;
int rock_can_spawn;
int rock_spawn_speed;
int alien_can_spawn;
int alien_spawn_speed;
int alien2_can_spawn;
int alien2_spawn_speed;
int score;
int lives;

void exit_func()
{
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
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
	SDL_DestroyTexture(alien_1.texture);
	SDL_DestroyTexture(alien_2.texture);
	SDL_DestroyTexture(explosion_1.texture);
	SDL_DestroyTexture(explosion_2.texture);
	SDL_DestroyTexture(explosion_3.texture);
	SDL_DestroyTexture(explosion_4.texture);
	SDL_DestroyTexture(explosion_5.texture);
	SDL_DestroyTexture(ex1.texture);
	SDL_DestroyTexture(ex2.texture);
	SDL_DestroyTexture(ex3.texture);
	SDL_DestroyTexture(ex4.texture);
	SDL_DestroyTexture(ex5.texture);
	SDL_DestroyTexture(ex6.texture);
	SDL_DestroyTexture(ex7.texture);
	SDL_DestroyTexture(enemy_bullet.texture);
	SDL_DestroyTexture(game_over.texture);
	SDL_DestroyTexture(score_text);
	SDL_DestroyTexture(score_value_text);
	SDL_DestroyTexture(lives_text);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
	Mix_FreeChunk(shot);
	Mix_FreeChunk(explode);
	Mix_FreeChunk(plasma);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

void game_loop()
{
#ifdef TARGET_OS_SIMULATOR
	SDL_Rect screen_rect = { 0, 0, GAME_WIDTH, GAME_HEIGHT };
	SDL_Point touch_location = { screen_rect.w / 2, screen_rect.h / 2 };
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); //black on mobile
#else
	SDL_SetRenderDrawColor(game.renderer, 112, 176, 203, 255); //light blue
#endif
	SDL_RenderClear(game.renderer);

	switch (game_state)
	{	
	case 1:
		handle_game();
		break;
	case 2:
		handle_game_over();
		break;	
	default:
		handle_title();
		break;
	}

	SDL_RenderPresent(game.renderer);

#ifdef __EMSCRIPTEN__
	emscripten_sleep(10);
#else
	SDL_Delay(18);
#endif
	loaded = 1;

	if (!play_bg)
	{
		Mix_PlayMusic(music, -1);
		play_bg = 1;
	}
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
	alien_can_spawn = 0;
	alien_spawn_speed = 0;
	alien2_can_spawn = 0;
	alien2_spawn_speed = 0;
	rock_can_spawn = 1;
	rock_spawn_speed = 0;
	score = 0;
	lives = 3;
	printf("Himpapawid\n");
	printf("Created By: Ferdinand Silva\n");
	memset(&game, 0, sizeof(Game));
	game.up = 0;
	game.down = 0;
	game.left = 0;
	game.right = 0;

#ifdef __EMSCRIPTEN__
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
#else
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
#endif
	{
		print_error("Unable to initialize SDL: %s.\n", SDL_GetError());
	}

#ifdef TARGET_OS_SIMULATOR
	finger_x = 0;
	finger_y = 0;

	SDL_DisplayMode mode;
	if( SDL_GetCurrentDisplayMode( 0, &mode ) == 0 )
	{
		screen_rect.w = mode.w;
		screen_rect.h = mode.h;
	}

	game.firing = 1; //always firing on mobile
	game.window = SDL_CreateWindow(TITLE_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_rect.w, screen_rect.h, 0);
#else
	game.firing = 0;
	game.window = SDL_CreateWindow(TITLE_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, 0);
#endif
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

	music = Mix_LoadMUS("data/bg.ogg");
	shot = Mix_LoadWAV("data/shot.wav");
	plasma = Mix_LoadWAV("data/plasma.wav");
	explode = Mix_LoadWAV("data/explode.wav");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
#ifdef TARGET_OS_SIMULATOR
	SDL_RenderSetLogicalSize(game.renderer, GAME_WIDTH, GAME_HEIGHT);
#endif
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
	init_image(&alien_1, game.renderer, 0, 0, "data/alien_1.png");
	init_image(&alien_2, game.renderer, 0, 0, "data/alien_2.png");
	init_image(&explosion_1, game.renderer, 0, 0, "data/explosion_1.png");
	init_image(&explosion_2, game.renderer, 0, 0, "data/explosion_2.png");
	init_image(&explosion_3, game.renderer, 0, 0, "data/explosion_3.png");
	init_image(&explosion_4, game.renderer, 0, 0, "data/explosion_4.png");
	init_image(&explosion_5, game.renderer, 0, 0, "data/explosion_5.png");
	init_image(&enemy_bullet, game.renderer, 0, 0, "data/enemy_bullet.png");
	init_image(&ex1, game.renderer, 0, 0, "data/ex1.png");
	init_image(&ex2, game.renderer, 0, 0, "data/ex2.png");
	init_image(&ex3, game.renderer, 0, 0, "data/ex3.png");
	init_image(&ex4, game.renderer, 0, 0, "data/ex4.png");
	init_image(&ex5, game.renderer, 0, 0, "data/ex5.png");
	init_image(&ex6, game.renderer, 0, 0, "data/ex6.png");
	init_image(&ex7, game.renderer, 0, 0, "data/ex7.png");

	init_image(&game_over, game.renderer, 0, 0, "data/game_over.png");
	game_over.x = ((GAME_WIDTH / 2) - (game_over.w / 2));
	game_over.y = (GAME_HEIGHT / 2) - (game_over.h / 2);	

	init_image(&title, game.renderer, 0, 0, "data/title.png");
	title.x = ((GAME_WIDTH / 2) - (title.w / 2)) - title.w;
	title.y = (GAME_HEIGHT / 2) - (title.h / 2);

	init_image(&missile, game.renderer, 0, 0, "data/missile_2.png");
	game.missile_tail = &game.missile_head;
	game.rock_tail = &game.rock_head;
	game.alien_tail = &game.alien_head;
	game.explosion_tail = &game.explosion_head;
	game.bullet_tail = &game.bullet_head;
	game.alien2_tail = &game.alien2_head;
	game.explosion2_tail = &game.explosion2_head;

	atexit(exit_func);

	SDL_Point ls = get_image_size(rock);

	generate_rocks();
	reset_nep();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(game_loop, 0, 1);
#else
	while(1)
	{
		game_loop();
	}
#endif

	return EXIT_SUCCESS;
}
