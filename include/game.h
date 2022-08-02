#ifndef _GAME_H_
#define _GAME_H_
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "sprite.h"
#include "misc.h"

#define GAME_WIDTH 576
#define GAME_HEIGHT 720
#define BG_SPEED 3
#define MISSILE_SPAWN_SPEED 20
#define ROCK_SPAWN_SPEED 65
#define ALIEN_SPAWN_SPEED 70
#define SND_CHANNEL 4
#define MISSILE_SPEED 8
#define PLAYER_SPEED 4
#define ROCK_SPEED 2
#define NEP_SPEED 1
#define ALIEN_SPEED 4

struct Object
{
	int x;
	int y;
	int d;
	struct Object *next;
};

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	int up;
	int down;
	int left;
	int right;
	int firing;
	struct Object missile_head, *missile_tail, rock_head, *rock_tail, alien_head, *alien_tail;
} Game;

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
Sprite enemy_bullet;
Mix_Music *music;
Mix_Chunk *shot;
Mix_Chunk *explode;
SDL_Event game_event;
TTF_Font *font;
SDL_Texture *score_text;
SDL_Texture *lives_text;
SDL_Texture *score_value_text;
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
int score;
int lives;

void handle_key(SDL_KeyboardEvent *event, int value, Game *game);
void handle_game();
void handle_title();
void generate_missile();
void generate_rock(int x, int y);
void generate_rocks();
void reset_nep();
SDL_Texture *get_font_texture(char *txt);

#endif