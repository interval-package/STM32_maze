#include "maze.h"

// global vars
player myPlayer;
TimeInfo myTime;
global_info myInfo;
u8 maze_arr[MAX_BOUND][MAX_BOUND];

u8 action_list[4][2] = {
    {2,0},{0,2},{-2,0},{0,-2}
};

void maze(){
    while (TRUE)
    {
        int ret;
        maze_init();
        ret = maze_action();
        show_res(ret);
    }
}

void brute_show_str(u16 x,u16 y,u8 size,char *p,u8 mode, u32 point_color, u32 bk_color){
	// TFT_ShowStr
	while(*p != '\0'){
        TFT_ShowChar(x,y,*p,size,mode,point_color,bk_color);
        x += size/2;
        p++;
    }  
}

void maze_disp_doc(){
	char _buffer[256];
    sprintf(_buffer, "Time: %03d:%02d", myTime.timestamp/60, myTime.timestamp%60);
	brute_show_str(336,16,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);

    sprintf(_buffer, "Remain: %02d:%02d", 
        (TOTAL_GAME_TIME - myTime.timestamp)/60, (TOTAL_GAME_TIME-myTime.timestamp)%60);
	brute_show_str(336,32,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);

	sprintf(_buffer, "wins: (%03d,%03d)", Lcd_Para.width, Lcd_Para.height);
	brute_show_str(336,48,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);

    sprintf(_buffer, "nums: (%02d)", BOUND_NUM);
	brute_show_str(336,64,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);

	sprintf(_buffer, "pos1: (%03d,%03d)", myPlayer.x/CHAR_SIZE, myPlayer.y/CHAR_SIZE);
	brute_show_str(336,80,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);

	sprintf(_buffer, "pos2: (%03d,%03d)", myPlayer.x, myPlayer.y);
	brute_show_str(336,96,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);
}

void draw_wall(int w, int h){
	// GUI_Draw_Line(MARGIN, MARGIN, BOUDER_POINT, MARGIN,	THICK_LINE, SOLID_LINE, COLOR_BOUND);
	// GUI_Draw_Line(MARGIN, MARGIN, MARGIN, BOUDER_POINT,	THICK_LINE, SOLID_LINE, COLOR_BOUND);
	// GUI_Draw_Line(MARGIN, BOUDER_POINT, BOUDER_POINT, BOUDER_POINT,	THICK_LINE, SOLID_LINE, COLOR_BOUND);
	// GUI_Draw_Line(BOUDER_POINT, MARGIN, BOUDER_POINT, BOUDER_POINT-EXIT_LEN, THICK_LINE, SOLID_LINE, COLOR_BOUND);
	for (int i = 0; i < w; i++){
        for(int j = 0; j < h; j+=2)
		    maze_arr[i][j] = 'b';
	}

	for (int i = 0; i < h; i++){
        for(int j = 0; j < w; j+=2)
		    maze_arr[j][i] = 'b';
	}

	for (int i = 0; i < w; i++){
        maze_arr[i][0] = maze_arr[i][h-1] = 'w';
	}

	for (int i = 0; i < h; i++){
		maze_arr[0][i] = maze_arr[w-1][i] = 'w';
	}
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

// is using the recurse method, error
#ifdef MAZE_RECUR_GEN
void maze_generate(int sx, int sy, int ex, int ey){
	// sx > sy
    maze_arr[sx][sy] = 'r';
    if(sx == ex && sy == ey) return;
    maze_draw_map();
    delay_ms(500);
    int actions[4] = {0,1,2,3};
    // shuffle(actions, 4);
    
    for (int i = 0; i < 4; i++)
    {
        int action = actions[i];
        int x, y;
        x = sx + action_list[action][0];
        y = sx + action_list[action][1];
        if(x > 0 && x < BOUND_NUM && y >0 && y < BOUND_NUM && maze_arr[x][y] != 'r'){
            int _x, _y;
            _x = sx + action_list[action][0]/2;
            _y = sy + action_list[action][1]/2;
            maze_arr[_x][_y] = 'k';
            maze_generate(x, y, ex, ey);
        }
    }
}
#else

void maze_generate(int sx, int sy, int ex, int ey){
    u8 _stack[256][4];
    // int toltal_blocks = (BOUND_NUM-1)/2;
    // toltal_blocks *= toltal_blocks;
    memset(_stack, 0, sizeof(_stack));
    int _top = 1;
    _stack[0][0] = sx;
    _stack[0][1] = sy;
    _stack[0][2] = -1;
    _stack[0][3] = -1;
    int cur_x, cur_y;
    
    while (_top)
    {
        _top--;
        cur_x = _stack[_top][0];
        cur_y = _stack[_top][1];
        if(maze_arr[cur_x][cur_y] == 'r'){
            continue;
        }else{
            maze_arr[cur_x][cur_y] = 'r';
        }

        int prev_x = _stack[_top][2], prev_y = _stack[_top][3];
        if(prev_x > 0 && prev_y > 0){
            prev_x += cur_x;
            prev_y += cur_y;
            prev_x /= 2;
            prev_y /= 2;
            maze_arr[prev_x][prev_y] = 'k';
        }
        
        maze_draw_map();
        delay_ms(100);
        
        int next_x, next_y, f_x, f_y;
        if(cur_x == ex && cur_y == ey){
            continue;
        }
        int actions[4] = {0,1,2,3};
        shuffle(actions, 4);
        for(int i=0; i<4; i++){
            int action = actions[i];
            next_x = cur_x + action_list[action][0];
            next_y = cur_y + action_list[action][1];
            if( next_x > 0 && next_x < BOUND_NUM && next_y > 0 && next_y < BOUND_NUM && 
                maze_arr[next_x][next_y] != 'r'){
                _stack[_top][0] = next_x;
                _stack[_top][1] = next_y;
                _stack[_top][2] = cur_x;
                _stack[_top][3] = cur_y;
                _top++;
            }
        }
        // if(last_x && last_y){
        //     last_x += cur_x;
        //     last_y += cur_y;

        //     last_x /= 2;
        //     last_y /= 2;
        //     maze_arr[last_x][last_y] = 'k';
        // }
    }
    
}
#endif
void draw_player(){
	GUI_Draw_FilledCircle(MARGIN+myPlayer.x,MARGIN+myPlayer.y,myPlayer.r,COLOR_PLAYER);
}
void clean_player(){
	GUI_Draw_FilledCircle(MARGIN+myPlayer.x,MARGIN+myPlayer.y,myPlayer.r,COLOR_BACKGROUND);
}

void maze_draw_dest(u16 x, u16 y){
    GUI_Draw_Rectangle(MARGIN+myInfo.tar_x*CHAR_SIZE, MARGIN+myInfo.tar_y*CHAR_SIZE, 
    CHAR_SIZE, CHAR_SIZE, THICK_LINE, COLOR_BOUND, 1);
}

int collision_detect(int x, int y){
#ifdef SIMPLE_MODE
    return maze_arr[x/CHAR_SIZE][y/CHAR_SIZE];
#else
    int lx, ly;
    lx = x/CHAR_SIZE;
    ly = y/CHAR_SIZE;


    if(x > lx*CHAR_SIZE + CHAR_SIZE/2){
        if(x%CHAR_SIZE && x + myPlayer.r > (lx+1)*CHAR_SIZE + CHAR_SIZE){
            lx++;
        }
    }else{
        if(x - myPlayer.r < lx*CHAR_SIZE)
            lx--;
    }

    if(y > ly*CHAR_SIZE + CHAR_SIZE/2){
        if(y%CHAR_SIZE && y + myPlayer.r > (ly+1)*CHAR_SIZE + CHAR_SIZE){
            ly++;
        }
    }else{
        if(y - myPlayer.r < ly*CHAR_SIZE)
            ly--;
    }

    return maze_arr[lx][ly];
#endif
}

int update_player(int move_x, int move_y){
	int cur_x, cur_y;
	cur_x = myPlayer.x + move_x;
	cur_y = myPlayer.y + move_y;

	// simplified collision detect
	if(collision_detect(cur_x, cur_y) == 'w'){

		return ACTION_FAIL;
	}else if(collision_detect(cur_x, cur_y) == 't'){
		clean_player();
		myPlayer.x = cur_x;
		myPlayer.y = cur_y;
		draw_player();
		return ACTION_SUCCESSS;
	}else if(collision_detect(cur_x, cur_y) == 'b'){
        // clean_player();
		// myPlayer.x = cur_x;
		// myPlayer.y = cur_y;
		// draw_player();
        return ACTION_UNDONE;
	}else{
		clean_player();
		myPlayer.x = cur_x;
		myPlayer.y = cur_y;
		draw_player();
        return ACTION_VALID;
	}

	return ACTION_VALID;
}

void maze_draw_map(){
	for (int i = 0; i < BOUND_NUM; i++){
		for (int j = 0; j < BOUND_NUM; j++){
			if(maze_arr[i][j] == 'w'){
				GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, COLOR_BOUND, 1);
			}else if(maze_arr[i][j] == 'b'){
                GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, COLOR_BLOCK, 1);
            }else if(maze_arr[i][j] == 't'){
                GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, COLOR_DEST, 1);
            }
            #ifdef MAZE_GEN_DEBUG
            else if(maze_arr[i][j] == 'k'){
                GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, BLUE, 1);
            }else if(maze_arr[i][j] == 'r'){
                GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, OLIVE, 1);
            }
            #endif
            else{
                GUI_Draw_Rectangle(MARGIN+i*CHAR_SIZE, MARGIN+j*CHAR_SIZE, CHAR_SIZE, CHAR_SIZE, THICK_LINE, COLOR_BACKGROUND, 1);
            }
		}
	}
}

void maze_draw()
{
	// memset(map_array, 0, sizeof(map_array));
	TFT_Clear(COLOR_BACKGROUND);
	memset(maze_arr, 0, sizeof(maze_arr));
	
	maze_disp_doc();
	// draw bound
	draw_wall(BOUND_NUM, BOUND_NUM);
	maze_generate(myInfo.start_x, myInfo.start_y, myInfo.tar_x, myInfo.tar_y);
    maze_arr[myInfo.tar_x][myInfo.tar_y] = 't';
	maze_draw_map();
	draw_player();
}

void show_res(int _info){
    char _buffer[256];
    TFT_Clear(LGRAY);
    int disp_x, disp_y;
    disp_x = CHAR_SIZE*2;
    disp_y = CHAR_SIZE*2;
    switch (_info)
    {
    case ACTION_SUCCESSS:
        sprintf(_buffer, "success, using time: %02d:%02d", myTime.timestamp/60, myTime.timestamp%60);
        brute_show_str(disp_x,disp_y,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);
        break;
    case ACTION_FAIL:
        sprintf(_buffer, "fail, using time: %02d:%02d", myTime.timestamp/60, myTime.timestamp%60);
        brute_show_str(disp_x,disp_y,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);
        break;
    case ACTION_TIMEOUT:
        sprintf(_buffer, "Time out, using time: %02d:%02d", myTime.timestamp/60, myTime.timestamp%60);
        brute_show_str(disp_x,disp_y,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);
        break;
    default:
        break;
    }

    delay_ms(1000);

    sprintf(_buffer, "Press any key to continue");
    brute_show_str(disp_x,disp_y + CHAR_SIZE,CHAR_SIZE, _buffer, 0, RED, COLOR_BACKGROUND);
    Key_Init();

    while(TRUE)
	{
		Key_Read();
		if(Key_Info.Num_Last != Key_Info.Num)
		{
	        Key_Info.Num_Last = Key_Info.Num;
			
			if(Key_Info.Num != 0)
			{
                break;
			}
		}
	}
}

void maze_init_glob(global_info *gl){
    gl->tar_x = gl->tar_y = 11;
    gl->start_x = gl->start_y = 1;
    maze_init_player(&myPlayer);
}

void maze_init_player(player *p){
    p->_x = myInfo.start_x;
    p->_y = myInfo.start_y;
	p->x = CHAR_SIZE/2 + CHAR_SIZE*p->_x;
	p->y = CHAR_SIZE/2 + CHAR_SIZE*p->_y;
    p->r = RADIAS;
}

void maze_init_time(TimeInfo *t){
    t->timestamp = 0;
}

void maze_init(){
    // maze functions with control

	// left
	LCD_Scan_Dir(D2U_L2R);
    maze_init_glob(&myInfo);
	// maze_init_time(t);
	maze_draw();
}

int maze_action(){
	Key_Init();
    maze_init_time(&myTime);
    int iter = 1;
	while(++iter)
	{
		Key_Read();
		int move_x, move_y;
		if(Key_Info.Num_Last != Key_Info.Num)
		{
	        Key_Info.Num_Last = Key_Info.Num;
			
			if(Key_Info.Num != 0)
			{
				move_x = move_y = 0;
				switch(Key_Info.Num)
				{
					case KEY_ONE:
						 move_x = MOVE_STEP;
						 break;
					case KEY_TWO:
						 move_x = -MOVE_STEP;
						 break;
					case KEY_THREE:
						 move_y = MOVE_STEP;
						 break;
					case KEY_FOUR:
						 move_y = -MOVE_STEP;
						 break;
					default: break;
				}
				int ret = update_player(move_x, move_y);
				if (ret == ACTION_FAIL)
				{
					return ACTION_FAIL;
				}else if(ret == ACTION_SUCCESSS)
				{
					return ACTION_SUCCESSS;
				}
			}
		}
		maze_disp_doc();
        if(iter%FRAME_FREQ == 0){
            myTime.timestamp++;
            if(myTime.timestamp == TOTAL_GAME_TIME)
                return ACTION_TIMEOUT;
        }
	}
	// check validation
    return ACTION_FAIL;
}

