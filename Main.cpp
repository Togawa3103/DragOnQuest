#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <vector>
#include "Enum.h"
#include "Struct.h"
#include "Declare.h"
#define MAP_HEIGHT 10
#define MAP_WIDTH 12
#define FPS 144

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
char old_RETURN_keyState=-1;
char old_ESCAPE_keyState=-1;
int Player_Time = 0;
int MENU_Time = 0;
int PlayerCount = 0;
int Player_X = 4;
int Player_Y = 4;
int old_Player_X = Player_X;
int old_Player_Y = Player_Y;
int Select_Menu_Num = 0;
int Select_Dire_Num = 0;
int Select_Item=0; 
std::vector<int> ItemBox = {0,1,2,4}; //もちもの
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
    clock_t end = clock();
    haveKey = TRUE;
    while (TRUE) {
        if (GameMode == GameMode_FIELD) {
            FIELD_MODE();
        }
        if (GameMode == GameMode_MENU) {
            DRAW_FIELD();
            if (Selected_Menu<0) {//メニューを選んでいないとき
                MENU_MODE();
            }
            else if(Selected_Menu==MenuType_DOOR) {//"とびら"を選んだとき
                SELECT_DIRE(Select_Menu_Num);
            }
            else if (Selected_Menu==MenuType_Item) {//"どうぐ"を選んだとき
                Item_Select();
            }
        }
        clock_t now = clock();
        double looptime = now- end;
        if (looptime<mFPS) {
            
            Sleep(mFPS-looptime);
            end = clock();
        }
        
        ScreenFlip();
        ClearDrawScreen();
    }
    WaitKey();      // キー入力待ち
    DxLib_End();    // DXライブラリ終了処理
    return 0;
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
            Selected_Menu = Select_Menu_Num;
        }
        if (keyState[KEY_INPUT_ESCAPE] && !old_ESCAPE_keyState) {
            Selected_Menu = -1;
            Select_Menu_Num = 0;
            GameMode = GameMode_FIELD;
        }
        old_ESCAPE_keyState = keyState[KEY_INPUT_ESCAPE];
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
            
            if (canmove[Player_Y-1][Player_X]) {
                Next_Y = Player_Y - 1;
            }
        }
        if (keyState[KEY_INPUT_S]) {
            if (canmove[Player_Y + 1][Player_X] ) {
                Next_Y = Player_Y + 1;
            }
        }
        if (keyState[KEY_INPUT_A]) {
            if (canmove[Player_Y ][Player_X-1]) {
                Next_X = Player_X - 1;
            }
        }
        if (keyState[KEY_INPUT_D]) {
            if (canmove[Player_Y][Player_X+1]) {
                Next_X = Player_X + 1;
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
            DrawExtendGraph(j * 50, i * 50, j * 50 + 50, i * 50 + 50, graphDescs[map[i][j]].Graph_Handle, FALSE);
        }
    }
    if (PlayerCount / (FPS / 2) > 0) {
        DrawExtendGraph(Player_X * 50, Player_Y * 50, Player_X * 50 + 50, Player_Y * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER].Graph_Handle, TRUE);
    }
    else {
        DrawExtendGraph(Player_X * 50, Player_Y * 50, Player_X * 50 + 50, Player_Y * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER2].Graph_Handle, TRUE);
    }
}

void SELECT_DIRE(int Selected_Menu_Num) {
    unsigned int Menu_Cr1 = GetColor(255, 255, 255);
    unsigned int Menu_Cr2 = GetColor(0, 0, 0);
    int Select_Menu = Selected_Menu_Num;
    
    old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
    switch (Select_Menu)
    {
    case MenuType_DOOR:
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
                if (keyState[KEY_INPUT_ESCAPE]&&!old_ESCAPE_keyState) {
                    Selected_Menu = -1;
                    Select_Dire_Num = 0;

                }
                old_ESCAPE_keyState = keyState[KEY_INPUT_ESCAPE];

                if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
                    Selected_Menu = FALSE;
                    Select_Menu_Num = 0;
                    Selected_Menu = -1;
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
                    Select_Dire_Num = 0;
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
            if (GameMode == GameMode_FIELD) {
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
           
    }

}
void Item_Select() {
    unsigned int Menu_Cr1 = GetColor(255, 255, 255);
    unsigned int Menu_Cr2 = GetColor(0, 0, 0);
    old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
    if (Player_Time == 0) {
        GetHitKeyStateAll(keyState);
        if (keyState[KEY_INPUT_S]) {
            Select_Item = (Select_Item + 1) % (ItemBox.size());
            Player_Time = Player_Time + mFPS;
        }
        
        if (keyState[KEY_INPUT_W]) {
            Select_Item = (Select_Item + ItemBox.size()-1) % (ItemBox.size());
            Player_Time = Player_Time + mFPS;
        }
        if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
            
        }
        if (keyState[KEY_INPUT_ESCAPE]) {
            Selected_Menu = -1;
            Select_Menu_Num = 0;
            GameMode = GameMode_FIELD;
        }
        old_RETURN_keyState = keyState[KEY_INPUT_RETURN];

    }
    DrawBox(200, 50, 400, 450, Menu_Cr1, TRUE);
    DrawBox(210, 60, 390, 440, Menu_Cr2, TRUE);
    for (int i = 0; i < ItemBox.size(); i++) {
        for (int j = 0; j < Item_Num_Max; j++) {
            if (ItemBox[i] == j) {
                if (Select_Item == i) {
                    DrawBox(215, 65 + i * 30 - 5, 385, 65 + i * 30 + 25, Menu_Cr1, TRUE);
                    DrawFormatString(220, 65 + i * 30, Menu_Cr2, item[j].Item_name);
                }
                else {
                    DrawFormatString(220, 65 + i * 30, Menu_Cr1, item[j].Item_name);
                }
            }
        }
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