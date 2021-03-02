#pragma once
#define MAP_HEIGHT 10
#define MAP_WIDTH 12
#define FPS 144

int cells[MAP_HEIGHT][MAP_WIDTH];
int map[MAP_HEIGHT][MAP_WIDTH];
bool canmove[MAP_HEIGHT][MAP_WIDTH];

float mFPS = 1000.f / FPS;
int Selected_Menu = -1;
bool haveKey = FALSE;

void Wait(int start, int end);
void MENU_MODE();
void FIELD_MODE();
void DRAW_FIELD();
void SELECT_DIRE(int);
void Item_Select();