#pragma once
#define MAP_HEIGHT 50
#define MAP_WIDTH 52
#define FPS 144

int cells[MAP_HEIGHT][MAP_WIDTH];
int map[MAP_HEIGHT][MAP_WIDTH];
bool canmove[MAP_HEIGHT][MAP_WIDTH];

float mFPS = 1000.f / FPS;
int Selected_Menu = -1;
bool haveKey = FALSE;

int MAP_NUM=0;

int GameMode = GameMode_FIELD;
char keyState[256];
char old_E_keyState;
char old_RETURN_keyState = -1;
char old_ESCAPE_keyState = -1;
int Player_Time = 0;
int MENU_Time = 0;
int PlayerCount = 0;
int Player_X = 4;
int Player_Y = 4;
int old_Player_X = Player_X;
int old_Player_Y = Player_Y;
int Select_Menu_Num = 0;
int Select_Dire_Num = 0;
int Select_Item = 0;
int HP = 10;
int MP = 5;
int Player_Lv = 1;
int Gold = 0;
int Exp = 0;
std::vector<int> ItemBox = { 0,1,2,3,4 }; //‚à‚¿‚à‚Ì
int Screen_x = Player_X;
int Screen_y = Player_Y;
int Move_Count_X=0;
int Move_Count_Y = 0;

Player_STATUS now_player_status = (player_status[Player_Lv - 1]);
int Player_Status[STATUS_MAX];

void Wait(int start, int end);
void MENU_MODE();
void FIELD_MODE();
void DRAW_FIELD();
void SELECT_DIRE(int);
void Item_Select();
void STATUS_SHOW();
void LOAD_MAP(const char* map_name);
void DRAW_FIELD_CAMERA();
void Updata_Status(int Player_Lv);