#define _CRT_SECURE_NO_WARNINGS
#include"DxLib.h"
#include <stdio.h>
#include <windows.h>
#include <time.h>

#define MAP_HEIGHT 10
#define MAP_WIDTH 12

#define FPS 144

int cells[MAP_HEIGHT][MAP_WIDTH];
int map[MAP_HEIGHT][MAP_WIDTH];
bool canmove[MAP_HEIGHT][MAP_WIDTH];

float mFPS = 1000.f / FPS;
void Wait(int start, int end);
void MENU_MODE();
void FIELD_MODE();
void DRAW_FIELD();

enum {
    CELL_TYPE_WALL,
    CELL_TYPE_FLOOR,
    CELL_TYPE_DOOR,
    CELL_TYPE_PLAYER,
    CELL_TYPE_MAX,
};

enum {
    GRAPH_TYPE_WALL,
    GRAPH_TYPE_FLOOR,
    GRAPH_TYPE_DOOR,
    GRAPH_TYPE_PLAYER,
    GRAPH_TYPE_PLAYER2,
    GRAPH_TYPE_MAX,
};

enum {
    GameMode_FIELD,
    GameMode_MENU,
    GameMode_BATTLE,
};

enum {
    MenuType_Talk,
    MenuType_Item,
    MenuType_DOOR,
    MenuType_STATUS,
    MenuType_MAX,
};

enum {
    Dire_N,
    Dire_E,
    Dire_S,
    Dire_W,
    Dire_MAX,
};
typedef struct {
    const char* Graph_Name;
    int Graph_Handle;
}GRAPHDESC;

typedef struct {
    unsigned char b, g, r;
}BGR;

typedef struct {
    BGR bgr;
    bool f_canMove;
}CELLDESC;

typedef struct {
    int x;
    int y;
    const char* name;
}MENU;

typedef struct {
    int x;
    int y;
    const char* Dire;
}DIRE;

DIRE dire[] = {
    {510,270,"北"},
    {550,315,"東"},
    {510,360,"南"},
    {470,315,"西"},
};

MENU menu[] = {
    {500,30,"はなす"},
    {500,75,"どうぐ"},
    {500,120,"とびら"},
    {500,165,"つよさ"},
};

CELLDESC cellDescs[] = {
    {0,0,255,FALSE},//CELL_TYPE_WALL
    {0,255,0,TRUE},//CELL_TYPE_FLOOR
    {0,0,0,FALSE},//CELL_TYPE_DOOR
    {255,0,0,FALSE}//CELL_TYPE_PLAYER
};

//int Player_H, WALL_H, FLOOR_H;

GRAPHDESC graphDescs[] = {
    {"wall.bmp",},//GRAPH_TYPE_WALL,
    {"floor.bmp",},//GRAPH_TYPE_FLOOR,
    {"door.bmp",},//GRAPH_TYPE_DOOR,
    {"Player.bmp",},//GRAPH_TYPE_PLAYER,
    {"Player_turn.bmp"},//GRAPH_TYPE_PLAYER2,
};

int GameMode = GameMode_FIELD;
char keyState[256];
char old_E_keyState;
char old_RETURN_keyState;
int Player_Time = 0;
int MENU_Time = 0;
int PlayerCount = 0;
int Player_X = 4;
int Player_Y = 4;
int old_Player_X = Player_X;
int old_Player_Y = Player_Y;
int Select_Menu_Num = 0;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(600, 500, 32);

    DxLib_Init();   // DXライブラリ初期化処理
    SetDrawScreen(DX_SCREEN_BACK);

    SetTransColor(182, 185, 184);
    FILE* pFile = fopen("map_test_door.bmp", "rb");
    if (pFile == NULL) {
        DxLib_End();
    }

    BITMAPFILEHEADER bmfh;
    fread(&bmfh, sizeof BITMAPFILEHEADER, 1, pFile);
    BITMAPINFOHEADER bmih;
    fread(&bmih, sizeof BITMAPINFOHEADER, 1, pFile);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {

            BGR bgr;
            fread(&bgr, sizeof BGR, 1, pFile);
            int cellType = -1;
            ClearDrawScreen();
            for (int i = 0; i < CELL_TYPE_MAX; i++) {
                if ((bgr.b == cellDescs[i].bgr.b) && (bgr.g == cellDescs[i].bgr.g) && (bgr.r == cellDescs[i].bgr.r)) {
                    unsigned int Cr = GetColor(255, 255, 255);
                    DrawFormatString(50, 50, Cr, "%d %d", y, x);
                    DrawFormatString(100, 50, Cr, "%d %d %d", bgr.b, bgr.g, bgr.r);
                    //WaitKey();
                    cellType = i;
                }
            }
            if (cellType == -1) {
                unsigned int Cr = GetColor(255, 255, 255);
                DrawFormatString(50, 50, Cr, "%d %d", y, x);
                DrawFormatString(100, 50, Cr, "%d %d %d", bgr.b, bgr.g, bgr.r);
                WaitKey();
                DxLib_End();
            }
            map[MAP_HEIGHT-1-y][x] = cellType;
            cells[MAP_HEIGHT-1-y][x] = cellType;
            canmove[MAP_HEIGHT-1-y][x] = cellDescs[cellType].f_canMove;
        }
    }
    for (int i = 0; i < GRAPH_TYPE_MAX; i++) {
        graphDescs[i].Graph_Handle = LoadGraph(graphDescs[i].Graph_Name);
    }

    //map[Player_Y][Player_X] = CELL_TYPE_PLAYER;

    //int keyCount[256];
    /*for (int i = 0; i < 256;i++) {
        keyCount[i] = 0;
    }*/


    while (TRUE) {
        clock_t start = clock();

        if (GameMode == GameMode_FIELD) {
            FIELD_MODE();
        }
        if (GameMode == GameMode_MENU) {
            MENU_MODE();
        }

        clock_t end = clock();

        Wait(start, end);

    }
    WaitKey();      // キー入力待ち
    DxLib_End();    // DXライブラリ終了処理
    return 0;
}

void Wait(int start, int end) {
    double looptime = (double)(end - start);
    if (looptime < mFPS) {
        Sleep(mFPS - looptime);
    }
    ScreenFlip();
}

void MENU_MODE() {
    unsigned int Menu_Cr1 = GetColor(255, 255, 255);
    unsigned int Menu_Cr2 = GetColor(0, 0, 0);
    DrawBox(450, 10, 590, 200, Menu_Cr1, TRUE);
    DrawBox(460, 20, 580, 190, Menu_Cr2, TRUE);
    if (Player_Time == 0) {
        GetHitKeyStateAll(keyState);
        if (keyState[KEY_INPUT_S]) {
            Select_Menu_Num = (Select_Menu_Num + 1) % (MenuType_MAX);
            Player_Time = Player_Time + mFPS;
        }
        if (keyState[KEY_INPUT_W]) {
            Select_Menu_Num = (Select_Menu_Num + 3) % (MenuType_MAX);
            Player_Time = Player_Time + mFPS;
        }
        if (keyState[KEY_INPUT_E] && !old_E_keyState) {
            GameMode = GameMode_FIELD;
            Player_Time = Player_Time + mFPS;
        }
        old_E_keyState = keyState[KEY_INPUT_E];

        if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
            int Select_Menu = Select_Menu_Num;
            int Select_Dire_Num = 0;
            old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
            switch (Select_Menu)
            {
            case MenuType_DOOR:
                while (1) {
                    clock_t start = clock();
                    if (Player_Time == 0) {
                        GetHitKeyStateAll(keyState);
                        if (keyState[KEY_INPUT_W]) {
                            Select_Dire_Num = Dire_N;
                        }
                        if (keyState[KEY_INPUT_D]) {
                            Select_Dire_Num = Dire_E;
                        }
                        if (keyState[KEY_INPUT_S]) {
                            Select_Dire_Num = Dire_S;
                        }
                        if (keyState[KEY_INPUT_A]) {
                            Select_Dire_Num = Dire_W;
                        }
                        if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
                            switch (Select_Dire_Num)
                            {
                            case Dire_N:
                                if (map[Player_Y - 1][Player_X] == CELL_TYPE_DOOR) {
                                    canmove[Player_Y - 1][Player_X] = TRUE;
                                    map[Player_Y - 1][Player_X] = map[Player_Y][Player_X];
                                    GameMode = GameMode_FIELD;
                                }
                                else {
                                    GameMode = GameMode_FIELD;
                                    break;
                                }

                            case Dire_E:
                                if (map[Player_Y][Player_X + 1] == CELL_TYPE_DOOR) {
                                    canmove[Player_Y][Player_X + 1] = TRUE;
                                    map[Player_Y][Player_X + 1] = map[Player_Y][Player_X];
                                    GameMode = GameMode_FIELD;
                                }
                                else {
                                    GameMode = GameMode_FIELD;
                                    break;
                                }

                            case Dire_S:
                                if (map[Player_Y + 1][Player_X] == CELL_TYPE_DOOR) {
                                    canmove[Player_Y + 1][Player_X] = TRUE;
                                    map[Player_Y + 1][Player_X] = map[Player_Y][Player_X];
                                    GameMode = GameMode_FIELD;
                                }
                                else {
                                    GameMode = GameMode_FIELD;
                                    break;
                                }

                            case Dire_W:
                                if (map[Player_Y][Player_X - 1] == CELL_TYPE_DOOR) {
                                    canmove[Player_Y][Player_X - 1] = TRUE;
                                    map[Player_Y][Player_X - 1] = map[Player_Y][Player_X];
                                    GameMode = GameMode_FIELD;
                                }
                                else {
                                    GameMode = GameMode_FIELD;
                                    break;
                                }

                            }
                        }
                        old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
                    }
                    DrawBox(450, 250, 590, 400, Menu_Cr1, TRUE);
                    DrawBox(460, 260, 580, 390, Menu_Cr2, TRUE);
                    for (int i = 0; i < Dire_MAX; i++) {
                        if (i == Select_Dire_Num) {
                            DrawBox(dire[i].x - 10, dire[i].y - 10, dire[i].x + 30, dire[i].y + 20, Menu_Cr1, TRUE);
                            DrawFormatString(dire[i].x, dire[i].y, Menu_Cr2, dire[i].Dire);
                        }
                        else {
                            DrawFormatString(dire[i].x, dire[i].y, Menu_Cr1, dire[i].Dire);
                        }
                    }
                    if (GameMode==GameMode_FIELD) {
                        break;
                    }
                    DrawBox(450, 10, 590, 200, Menu_Cr1, TRUE);
                    DrawBox(460, 20, 580, 190, Menu_Cr2, TRUE);
                    for (int i = 0; i < MenuType_MAX; i++) {
                        if (Select_Menu_Num == i) {
                            DrawBox(menu[i].x - 10, menu[i].y - 10, menu[i].x + 60, menu[i].y + 20, Menu_Cr1, TRUE);
                            DrawFormatString(menu[i].x, menu[i].y, Menu_Cr2, menu[i].name);
                        }
                        else {
                            DrawFormatString(menu[i].x, menu[i].y, Menu_Cr1, menu[i].name);
                        }
                    }
                    if (Player_Time != 0) {
                        Player_Time = Player_Time + mFPS;
                    }
                    if (Player_Time > 200) {
                        Player_Time = 0;
                    }
                    clock_t end = clock();
                    Wait(start,end);
                }
                break;
            }

        }
        old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
    }
        DrawBox(450, 10, 590, 200, Menu_Cr1, TRUE);
        DrawBox(460, 20, 580, 190, Menu_Cr2, TRUE);
        for (int i = 0; i < MenuType_MAX; i++) {
            if (Select_Menu_Num == i) {
                DrawBox(menu[i].x - 10, menu[i].y - 10, menu[i].x + 60, menu[i].y + 20, Menu_Cr1, TRUE);
                DrawFormatString(menu[i].x, menu[i].y, Menu_Cr2, menu[i].name);
            }
            else {
                DrawFormatString(menu[i].x, menu[i].y, Menu_Cr1, menu[i].name);
            }
        }
    
    if (Player_Time != 0) {
        Player_Time = Player_Time + mFPS;
    }
    if (Player_Time > 200) {
        Player_Time = 0;
    }
}

void FIELD_MODE() {
    PlayerCount = (PlayerCount + 1) % FPS;
    int Next_X=Player_X;
    int Next_Y=Player_Y;
    if (Player_Time == 0) {
        GetHitKeyStateAll(keyState);
        if (keyState[KEY_INPUT_W]) {
            //keyCount[KEY_INPUT_W]++;
            if (canmove[Player_Y-1][Player_X]) {
                Next_Y = Player_Y - 1;
                //keyCount[KEY_INPUT_W] = 0;
            }
        }
        if (keyState[KEY_INPUT_S]) {
            //keyCount[KEY_INPUT_S]++;
            if (canmove[Player_Y + 1][Player_X] ) {
                Next_Y = Player_Y + 1;
                //keyCount[KEY_INPUT_S] = 0;
            }
        }
        if (keyState[KEY_INPUT_A]) {
            //keyCount[KEY_INPUT_A]++;
            if (canmove[Player_Y ][Player_X-1]) {
                Next_X = Player_X - 1;
                //keyCount[KEY_INPUT_A] = 0;
            }
        }
        if (keyState[KEY_INPUT_D]) {
            //keyCount[KEY_INPUT_D]++;
            if (canmove[Player_Y][Player_X+1]) {
                Next_X = Player_X + 1;
                //keyCount[KEY_INPUT_D] = 0;
            }
        }

        if (canmove[Next_Y][Next_X]) {
            Player_X = Next_X;
            Player_Y = Next_Y;
        }

        if (keyState[KEY_INPUT_E] && !old_E_keyState) {
            GameMode = GameMode_MENU;
            Player_Time = Player_Time + mFPS;

        }
        old_E_keyState = keyState[KEY_INPUT_E];

        if (Player_X != old_Player_X || Player_Y != old_Player_Y) {
            Player_Time = Player_Time + mFPS;
        }
    }
    //map[Player_Y][Player_X] = CELL_TYPE_PLAYER;
    DRAW_FIELD();


    old_Player_X = Player_X;
    old_Player_Y = Player_Y;
    if (Player_Time != 0) {
        Player_Time = Player_Time + mFPS;
    }
    if (Player_Time > 300) {
        Player_Time = 0;
    }
}

void DRAW_FIELD() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            //unsigned int Cr = GetColor(cellDescs[map[i][j]].bgr.r, cellDescs[map[i][j]].bgr.g, cellDescs[map[i][j]].bgr.b);
            //DrawBox(j * 50, i * 50, j * 50 + 50, i * 50 + 50, Cr, TRUE);
            DrawExtendGraph(j * 50, i * 50, j * 50 + 50, i * 50 + 50, graphDescs[map[i][j]].Graph_Handle, FALSE);
        }
    }
    //unsigned int Player_Cr = GetColor(cellDescs[CELL_TYPE_PLAYER].bgr.r, cellDescs[CELL_TYPE_PLAYER].bgr.g, cellDescs[CELL_TYPE_PLAYER].bgr.b);
    //DrawBox(Player_X * 50, Player_Y * 50, Player_X * 50 + 50, Player_Y * 50 + 50, Player_Cr, TRUE);
    if (PlayerCount / (FPS / 2) > 0) {
        DrawExtendGraph(Player_X * 50, Player_Y * 50, Player_X * 50 + 50, Player_Y * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER].Graph_Handle, TRUE);
    }
    else {
        DrawExtendGraph(Player_X * 50, Player_Y * 50, Player_X * 50 + 50, Player_Y * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER2].Graph_Handle, TRUE);
    }
}