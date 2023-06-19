#ifndef MAZE_H
#define MAZE_H

#include "stm32h7xx.h"
#include "stm32_system.h"
#include "delay.h"
#include "string.h"
#include "stdlib.h"

//驱动头文件
#include "led.h"
#include "Systick.h"
#include "Usart.h"
#include "TFT_driver.h"
#include "gui_ext.h"
#include "key.h"

#define COLOR_BACKGROUND BLACK
#define COLOR_PLAYER GREEN
#define COLOR_BOUND WHITE
#define COLOR_DEST RED
#define COLOR_BLOCK YELLOW

#define MARGIN 5
#define BOUND_LEN 304
#define EXIT_LEN 16
#define BOUDER_POINT (MARGIN+BOUND_LEN)
#define CHAR_SIZE 16
#define BOUND_NUM (BOUND_LEN/CHAR_SIZE)

#define MAX_BOUND 32

#ifdef SIMPLE_MODE
#define MOVE_STEP CHAR_SIZE
#define RADIAS CHAR_SIZE/2
#else
#define MOVE_STEP CHAR_SIZE/4
#define RADIAS CHAR_SIZE/4
#endif

#define TOTAL_GAME_TIME 300

#define FRAME_FREQ 128

typedef struct player{
	// same as in the 
	int x, y;
	int r;
    int _x, _y;
} player;

typedef struct TimeInfo
{
	int min, sec;
    int timestamp;
} TimeInfo;

typedef struct global_info
{
    int tar_x, tar_y;
    int start_x, start_y;
	// u8 maze[MAX_BOUND][MAX_BOUND];
} global_info;

void maze(void);

void maze_disp_doc(void);
void brute_show_str(u16 x,u16 y,u8 size,char *p,u8 mode, u32 point_color, u32 bk_color);
void maze_draw_dest(u16 x, u16 y);
void draw_wall(int w, int h);

void shuffle(int *array, size_t n);
void maze_generate(int sx, int sy, int ex, int ey);
void maze_draw_map(void);
void maze_draw(void);

void draw_player(void);
void clean_player(void);
int update_player(int move_x, int move_y);

void maze_init_glob(global_info *gl);
void maze_init_player(player *p);
void maze_init_time(TimeInfo *t);
void maze_init(void);

void show_res(int _info);

#define ACTION_SUCCESSS 0
#define ACTION_FAIL 1
#define ACTION_VALID 2
#define ACTION_UNDONE 3
#define ACTION_TIMEOUT 4
int maze_action(void);

void timetick_isr(void);

#endif