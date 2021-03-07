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
int MP = 0;
int Player_Lv = 2;
int Gold = 0;
int Exp = 0;
std::vector<int> ItemBox = { 0,1,3,4 }; //‚à‚¿‚à‚Ì


void Wait(int start, int end);
void MENU_MODE();
void FIELD_MODE();
void DRAW_FIELD();
void SELECT_DIRE(int);
void Item_Select();
void STATUS_SHOW();