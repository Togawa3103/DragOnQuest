#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <vector>
#include "Enum.h"
#include "Struct.h"
#include "Declare.h"
#include "MAP.h"
#define MAP_HEIGHT 50
#define MAP_WIDTH 52
#define FPS 144
#define VIEW_RANGE_WIDHT 12
#define VIEW_RANGE_HEIGHT 10

const char* File_Name = "map_test_door.bmp";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    SetGraphMode(600, 500, 32);

    DxLib_Init();   // DXライブラリ初期化処理
    SetDrawScreen(DX_SCREEN_BACK);

    SetTransColor(182, 185, 184);
    FILE* pFile = fopen(File_Name, "rb");
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
    haveKey =FALSE;
    ClearDrawScreen();
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        Updata_Status(Player_Lv);
        GetHitKeyStateAll(keyState);
        if (GameMode == GameMode_FIELD) {
            FIELD_MODE();
        }
        else if (GameMode == GameMode_MENU) {
            DRAW_FIELD_CAMERA();
            if (Selected_Menu<0) {//メニューを選んでいないとき
                MENU_MODE();
            }
            else if(Selected_Menu==MenuType_DOOR) {//"とびら"を選んだとき
                SELECT_DIRE(Selected_Menu);
            }
            else if (Selected_Menu==MenuType_Item) {//"どうぐ"を選んだとき
                Item_Select();
            }
            else if (Selected_Menu==MenuType_STATUS) {//"つよさ"を選んだとき
                STATUS_SHOW();
            }
            else if (Selected_Menu==MenuType_SEARCH) {
                SELECT_DIRE(Selected_Menu);
            }
        }
        clock_t now = clock();
        double looptime = now- end;
        if (looptime<mFPS) {            
            Sleep(mFPS-looptime);
        }
        old_E_keyState = keyState[KEY_INPUT_E];
        old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
        old_ESCAPE_keyState = keyState[KEY_INPUT_RETURN];
        //ScreenFlip();
        //ClearDrawScreen();
        end = clock();
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
        if (keyState[KEY_INPUT_S]) {
            Select_Menu_Num = (Select_Menu_Num + 1) % (MenuType_MAX);
            Player_Time = Player_Time + mFPS;
        }
        if (keyState[KEY_INPUT_W]) {
            Select_Menu_Num = (Select_Menu_Num + MenuType_MAX-1) % (MenuType_MAX);
            Player_Time = Player_Time + mFPS;
        }
        if (keyState[KEY_INPUT_E] && !old_E_keyState) {
            GameMode = GameMode_FIELD;
            Player_Time = Player_Time + mFPS;
        }
        

        if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
            Selected_Menu = Select_Menu_Num;
        }
        if (keyState[KEY_INPUT_ESCAPE] && !old_ESCAPE_keyState) {
            Selected_Menu = -1;
            Select_Menu_Num = 0;
            GameMode = GameMode_FIELD;
        }
     
    }
        DrawBox(450, 10, 590, 250, Menu_Cr1, TRUE);
        DrawBox(460, 20, 580, 240, Menu_Cr2, TRUE);
        for (int i = 0; i < MenuType_MAX; i++) {
            if (Select_Menu_Num == i) {
                if (i==MenuType_SEARCH) {
                    DrawBox(menu[i].x - 10, menu[i].y - 10, menu[i].x + 80, menu[i].y + 20, Menu_Cr1, TRUE);
                    DrawFormatString(menu[i].x, menu[i].y, Menu_Cr2, menu[i].name);
                }
                else {
                    DrawBox(menu[i].x - 10, menu[i].y - 10, menu[i].x + 60, menu[i].y + 20, Menu_Cr1, TRUE);
                    DrawFormatString(menu[i].x, menu[i].y, Menu_Cr2, menu[i].name);
                }

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
        
        if (keyState[KEY_INPUT_W]&&!keyState[KEY_INPUT_S]) {
            
            if (canmove[Player_Y-1][Player_X]) {
                Next_Y = Player_Y - 1;
            }
        }
        if (keyState[KEY_INPUT_S] && !keyState[KEY_INPUT_W]) {
            if (canmove[Player_Y + 1][Player_X] ) {
                Next_Y = Player_Y + 1;
            }
        }
        if (keyState[KEY_INPUT_A] && !keyState[KEY_INPUT_D]) {
            if (canmove[Player_Y ][Player_X-1]) {
                Next_X = Player_X - 1;
            }
        }
        if (keyState[KEY_INPUT_D] && !keyState[KEY_INPUT_A]) {
            if (canmove[Player_Y][Player_X+1]) {
                Next_X = Player_X + 1;
            }
        }

        if (canmove[Next_Y][Next_X]) {
            Player_X = Next_X;
            Player_Y = Next_Y;
            //Screen_x = Player_X;
            //Screen_y = Player_Y;
        }

        if (keyState[KEY_INPUT_E] && !old_E_keyState) {
            GameMode = GameMode_MENU;
            Player_Time = Player_Time + mFPS;

        }
        

        if (Player_X != old_Player_X || Player_Y != old_Player_Y) {
            Player_Time = Player_Time + mFPS;
        }
    }
    
    DRAW_FIELD_CAMERA();
    
    if (map[Player_Y][Player_X]==CELL_TYPE_STEPS) {
        for (int i = 0; i < map_data[MAP_NUM].map_info.size(); i++) {
            if (Player_X == map_data[MAP_NUM].map_info[i].Start_X && Player_Y == map_data[MAP_NUM].map_info[i].Start_Y) {
                MAP_NUM = map_data[MAP_NUM].map_info[i].toMap_Num;
                File_Name = map_data[MAP_NUM].Map_Name;
                LOAD_MAP(File_Name);
                Player_X = map_data[MAP_NUM].map_info[i].Start_X;
                Player_Y = map_data[MAP_NUM].map_info[i].Start_Y;
                Move_Count_X = 0;
                Move_Count_Y = 0;
            }
        }
    }
    
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
    
    
    switch (Select_Menu)
    {
    case MenuType_DOOR:
        if (Player_Time == 0) {

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
            if (keyState[KEY_INPUT_ESCAPE] && !old_ESCAPE_keyState) {
                Selected_Menu = -1;
                Select_Dire_Num = 0;

            }


            if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
                Selected_Menu = FALSE;
                Select_Menu_Num = 0;
                Selected_Menu = -1;
                if (haveKey) {
                    switch (Select_Dire_Num)
                    {
                    case Dire_N:
                        if (map[Player_Y - 1][Player_X] == CELL_TYPE_DOOR) {
                            for (int i = 0; i < ItemBox.size(); i++) {
                                if (ItemBox[i] == Item_Key) {
                                    ItemBox.erase(ItemBox.begin() + i);
                                }
                            }
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
                            for (int i = 0; i < ItemBox.size(); i++) {
                                if (ItemBox[i] == Item_Key) {
                                    ItemBox.erase(ItemBox.begin() + i);
                                }
                            }
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
                            for (int i = 0; i < ItemBox.size(); i++) {
                                if (ItemBox[i] == Item_Key) {
                                    ItemBox.erase(ItemBox.begin() + i);
                                }
                            }
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
                            for (int i = 0; i < ItemBox.size();i++) {
                                if (ItemBox[i]==Item_Key) {
                                    ItemBox.erase(ItemBox.begin()+i);
                                }
                            }
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
                else {
                    Select_Dire_Num = 0;
                    GameMode = GameMode_FIELD;
                }
            }
        }


        DrawBox(450, 260, 590, 410, Menu_Cr1, TRUE);
        DrawBox(460, 270, 580, 400, Menu_Cr2, TRUE);
        for (int i = 0; i < Dire_MAX; i++) {
            if (i == Select_Dire_Num) {
                DrawBox(dire[i].x - 10, dire[i].y - 10, dire[i].x + 30, dire[i].y + 20, Menu_Cr1, TRUE);
                DrawFormatString(dire[i].x, dire[i].y, Menu_Cr2, dire[i].Dire);
            }
            else {
                DrawFormatString(dire[i].x, dire[i].y, Menu_Cr1, dire[i].Dire);
            }
        }
        DrawBox(450, 10, 590, 250, Menu_Cr1, TRUE);
        DrawBox(460, 20, 580, 240, Menu_Cr2, TRUE);
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
        break;

    case MenuType_SEARCH:
        if (Player_Time == 0) {

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
            if (keyState[KEY_INPUT_ESCAPE] && !old_ESCAPE_keyState) {
                Selected_Menu = -1;
                Select_Dire_Num = 0;

            }


            if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
                Selected_Menu = FALSE;
                Select_Menu_Num = 0;
                Selected_Menu = -1;

                switch (Select_Dire_Num)
                {
                case Dire_N:
                    if (map[Player_Y - 1][Player_X] == CELL_TYPE_TRESURE) {
                        ItemBox.push_back(Item_Key);
                        GameMode = GameMode_FIELD;

                    }
                    else {
                        GameMode = GameMode_FIELD;
                        break;
                    }

                case Dire_E:
                    if (map[Player_Y][Player_X + 1] == CELL_TYPE_TRESURE) {
                        ItemBox.push_back(Item_Key);
                        GameMode = GameMode_FIELD;
                    }
                    else {
                        GameMode = GameMode_FIELD;
                        break;
                    }

                case Dire_S:
                    if (map[Player_Y + 1][Player_X] == CELL_TYPE_TRESURE) {
                        ItemBox.push_back(Item_Key);
                        GameMode = GameMode_FIELD;
                    }
                    else {
                        GameMode = GameMode_FIELD;
                        break;
                    }

                case Dire_W:
                    if (map[Player_Y][Player_X - 1] == CELL_TYPE_TRESURE) {
                        ItemBox.push_back(Item_Key);
                        GameMode = GameMode_FIELD;
                    }
                    else {
                        GameMode = GameMode_FIELD;
                        break;
                    }

                }
                Select_Dire_Num = 0;
            }
        }



        DrawBox(450, 260, 590, 410, Menu_Cr1, TRUE);
        DrawBox(460, 270, 580, 400, Menu_Cr2, TRUE);
        for (int i = 0; i < Dire_MAX; i++) {
            if (i == Select_Dire_Num) {
                DrawBox(dire[i].x - 10, dire[i].y - 10, dire[i].x + 30, dire[i].y + 20, Menu_Cr1, TRUE);
                DrawFormatString(dire[i].x, dire[i].y, Menu_Cr2, dire[i].Dire);
            }
            else {
                DrawFormatString(dire[i].x, dire[i].y, Menu_Cr1, dire[i].Dire);
            }
        }
        DrawBox(450, 10, 590, 250, Menu_Cr1, TRUE);
        DrawBox(460, 20, 580, 240, Menu_Cr2, TRUE);
        for (int i = 0; i < MenuType_MAX; i++) {
            if (Select_Menu_Num == i) {
                DrawBox(menu[i].x - 10, menu[i].y - 10, menu[i].x + 80, menu[i].y + 20, Menu_Cr1, TRUE);
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
        break;
    }


}
void Item_Select() {
    unsigned int Menu_Cr1 = GetColor(255, 255, 255);
    unsigned int Menu_Cr2 = GetColor(0, 0, 0);
    old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
    if (Player_Time == 0) {
        
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
        if (Select_Item == i) {
            DrawBox(215, 65 + i * 30 - 5, 385, 65 + i * 30 + 25, Menu_Cr1, TRUE);
            DrawFormatString(220, 65 + i * 30, Menu_Cr2, item[ItemBox[i]].Item_name);
        }
        else {
            DrawFormatString(220, 65 + i * 30, Menu_Cr1, item[ItemBox[i]].Item_name);
        }
    }
    DrawBox(450, 10, 590, 250, Menu_Cr1, TRUE);
    DrawBox(460, 20, 580, 240, Menu_Cr2, TRUE);
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

void STATUS_SHOW() {
    unsigned int Menu_Cr1 = GetColor(255, 255, 255);
    unsigned int Menu_Cr2 = GetColor(0, 0, 0);
    old_RETURN_keyState = keyState[KEY_INPUT_RETURN];
    
    
    int* p;
    p = &(now_player_status.MAXHP);
    
    if (Player_Time == 0) {
        
        if (keyState[KEY_INPUT_RETURN] && !old_RETURN_keyState) {
            Selected_Menu = -1;
            Select_Menu_Num = 0;
            GameMode = GameMode_FIELD;
        }
        if (keyState[KEY_INPUT_ESCAPE]) {
            Selected_Menu = -1;
            Select_Menu_Num = 0;
            GameMode = GameMode_FIELD;
        }
        old_RETURN_keyState = keyState[KEY_INPUT_RETURN];

    }
    DrawBox(200, 50, 400, 400, Menu_Cr1, TRUE);
    DrawBox(210, 60, 390, 390, Menu_Cr2, TRUE);
    for (int i = 0; i < STATUS_MAX; i++) {
        DrawFormatString(220, 65 + i * 30, Menu_Cr1, status[i].status_name);
        DrawFormatString(350, 65 + i * 30, Menu_Cr1, "%d", *(p+i));
    }
    

    /*
    DrawFormatString(350, 65 + 0 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].MAXHP);
    DrawFormatString(350, 65 + 1 * 30, Menu_Cr1, "%d", player_status[Player_Lv-1].HP);
    DrawFormatString(350, 65 + 2 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].MAXMP);
    DrawFormatString(350, 65 + 3 * 30, Menu_Cr1, "%d", player_status[Player_Lv-1].MP);
    DrawFormatString(350, 65 + 4 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].ATTACK);
    DrawFormatString(350, 65 + 5 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].DEFENSE);
    DrawFormatString(350, 65 + 6 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].SPEED);
    DrawFormatString(350, 65 + 7 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].WISE);
    DrawFormatString(350, 65 + 8 * 30, Menu_Cr1, "%d", player_status[Player_Lv - 1].MAGICDEF);
    DrawFormatString(350, 65 + 9 * 30, Menu_Cr1, "%d", player_status[Player_Lv-1].GOLD);
    DrawFormatString(350, 65 + 10 * 30, Menu_Cr1, "%d", player_status[Player_Lv-1].EXP);
    */
    DrawBox(450, 10, 590, 250, Menu_Cr1, TRUE);
    DrawBox(460, 20, 580, 240, Menu_Cr2, TRUE);
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

void LOAD_MAP(const char* map_name) {
    FILE* pFile = fopen(map_name, "rb");
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
            map[MAP_HEIGHT - 1 - y][x] = cellType;
            cells[MAP_HEIGHT - 1 - y][x] = cellType;
            canmove[MAP_HEIGHT - 1 - y][x] = cellDescs[cellType].f_canMove;
        }
    }
    for (int i = 0; i < GRAPH_TYPE_MAX; i++) {
        graphDescs[i].Graph_Handle = LoadGraph(graphDescs[i].Graph_Name);
    }


}

void DRAW_FIELD_CAMERA() {
    if (Player_X > old_Player_X) {
        Screen_x++;
    }
    if (Player_X < old_Player_X) {
        Screen_x--;
    }
    if (Screen_x>8&&Player_X>old_Player_X) {
        Screen_x = 8;
        Move_Count_X++;
    }else if (Screen_x < 3 && Player_X < old_Player_X) {
        Screen_x = 3;
        if (Move_Count_X>0) {
            Move_Count_X--;
        }
    }

    if (Player_Y > old_Player_Y) {
        Screen_y++;
    }
    if (Player_Y< old_Player_Y) {
        Screen_y--;
    }
    if (Screen_y > 7 && Player_Y > old_Player_Y) {
        Screen_y = 7;
        Move_Count_Y++;
    }
    else if (Screen_y < 4 && Player_Y < old_Player_Y ) {
        Screen_y = 4;
        if (Move_Count_Y > 0) {
            Move_Count_Y--;
        }
    }
        

    for (int i = 0; i < VIEW_RANGE_HEIGHT; i++) {
        for (int j = 0; j < VIEW_RANGE_WIDHT; j++) {
            DrawExtendGraph(j * 50, i * 50, j * 50 + 50, i * 50 + 50, graphDescs[map[i+Move_Count_Y][j+Move_Count_X]].Graph_Handle, FALSE);
        }
    }
    if (PlayerCount / (FPS / 2) > 0) {
        DrawExtendGraph((Player_X-Move_Count_X) * 50, (Player_Y-Move_Count_Y) * 50, (Player_X-Move_Count_X) * 50 + 50, (Player_Y-Move_Count_Y) * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER].Graph_Handle, TRUE);
    }
    else {
        DrawExtendGraph((Player_X - Move_Count_X) * 50, (Player_Y-Move_Count_Y) * 50, (Player_X- Move_Count_X) * 50 + 50, (Player_Y-Move_Count_Y) * 50 + 50, graphDescs[GRAPH_TYPE_PLAYER2].Graph_Handle, TRUE);
    }

}

void Updata_Status(int Player_Lv) {
    now_player_status = (player_status[Player_Lv - 1]);
    now_player_status.HP = HP;
    now_player_status.MP = MP;
    now_player_status.GOLD = Gold;
    now_player_status.EXP = Exp;
    int* p;
    p = &(now_player_status.MAXHP);
    for (int i = 0; i < STATUS_MAX;i++) {
        Player_Status[i] = *(p+i);
    }
    for (int i = 0; i < ItemBox.size();i++) {
        if (ItemBox[i]==Item_Key) {
            haveKey = TRUE;
        }
    }
}