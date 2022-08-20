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
#define ALIEN_SPAWN_SPEED 50
#define ALIEN2_SPAWN_SPEED 300
#define BULLET_SPAWN_SPEED 30
#define SND_CHANNEL 4
#define MISSILE_SPEED 8
#define ROCK_SPEED 2
#define NEP_SPEED 1
#define ALIEN_SPEED 4
#define ALIEN2_SPEED 6
#define BULLET_SPEED 5
#define CHANGE_DIRECTION_SPEED 70

#ifdef __ANDROID__
#include <android/log.h>

#define  LOG_TAG    "Himpapawid"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define PLAYER_SPEED 6
#else
#define PLAYER_SPEED 4
#endif

typedef struct Obj
{
	int x;
	int y;
	int d;
	int cf; //current frame
	int down_only;
	int go_right;
	int bsp; //bullet spawn time
	int sp; //spawn bullet
	int up;
	int down;
	int left;
	int right;
	int change_direction_speed;
	int change_direction;
	int can_change_now;
	int lives;
	struct Obj *next;
} Object;

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window *window;
	int up;
	int down;
	int left;
	int right;
	int firing;
	Object missile_head, *missile_tail, rock_head, *rock_tail, alien_head, *alien_tail, explosion_head, *explosion_tail, bullet_head, *bullet_tail, alien2_head, *alien2_tail, explosion2_head, *explosion2_tail;
} Game;

extern Game game;
extern Sprite player;
extern Sprite bg;
extern Sprite turbo1;
extern Sprite turbo2;
extern Sprite title;
extern Sprite bg_title;
extern Sprite missile;
extern Sprite rock;
extern Sprite nep;
extern Sprite rocket;
extern Sprite alien_1;
extern Sprite alien_2;
extern Sprite explosion_1;
extern Sprite explosion_2;
extern Sprite explosion_3;
extern Sprite explosion_4;
extern Sprite explosion_5;
extern Sprite ex1;
extern Sprite ex2;
extern Sprite ex3;
extern Sprite ex4;
extern Sprite ex5;
extern Sprite ex6;
extern Sprite ex7;
extern Sprite enemy_bullet;
extern Sprite game_over;
extern Mix_Music *music;
extern Mix_Chunk *shot;
extern Mix_Chunk *plasma;
extern Mix_Chunk *explode;
extern SDL_Event game_event;
extern TTF_Font *font;
extern SDL_Texture *score_text;
extern SDL_Texture *lives_text;
extern SDL_Texture *score_value_text;
#ifdef TARGET_OS_SIMULATOR
extern SDL_Rect screen_rect;
extern SDL_Point touch_location;
extern int finger_y;
extern int finger_x;
#endif
extern int first_frame;
extern int loaded;
extern int play_bg;
extern int game_state;
extern int missile_can_spawn;
extern int missile_spawn_speed;
extern int rock_can_spawn;
extern int rock_spawn_speed;
extern int alien_can_spawn;
extern int alien_spawn_speed;
extern int alien2_can_spawn;
extern int alien2_spawn_speed;
extern int score;
extern int lives;

void handle_key(SDL_KeyboardEvent *event, int value, Game *game);
void handle_game();
void handle_title();
void handle_game_over();
void generate_missile();
void generate_rock(int x, int y);
void generate_rocks();
void reset_nep();
SDL_Texture *get_font_texture(char *txt);
void generate_explosion(int x, int y);
void generate_explosion2(int x, int y);
void generate_bullet(int x, int y, int down_only, int go_right);
void generate_alien(int x, int y);
void generate_alien2(int x, int y);
void clear_object(Object *head, Object **tail);
void draw_bg();
void set_speed(int *can_spawn, int *spawn_speed, int speed);
void draw_common(Object *head, Sprite *sprite, int is_scaled);
void back_to_title();

#endif
