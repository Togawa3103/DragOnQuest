#pragma once
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

typedef struct {
    const char* Item_name;
}ITEM;

typedef struct {
    int MAXHP;
    int HP;
    int MAXMP;
    int MP;
    int ATTACK;
    int DEFENSE;
    int SPEED;
    int WISE;
    int MAGICDEF;
    int GOLD;
    int EXP;
}Player_STATUS;

typedef struct {
    const char* status_name;
}STATUS;

DIRE dire[] = {
    {510,280,"ñk"},
    {550,325,"ìå"},
    {510,370,"ìÏ"},
    {470,325,"êº"},
};

MENU menu[] = {
    {500,30,"ÇÕÇ»Ç∑"},
    {490,75,"ÇµÇÁÇ◊ÇÈ"},
    {500,120,"Ç«Ç§ÇÆ"},
    {500,165,"Ç∆Ç—ÇÁ"},
    {500,210,"Ç¬ÇÊÇ≥"},
};

ITEM item[] = {
    {"Ç‚Ç≠ÇªÇ§"},
    {"Ç©Ç¨"},
    {"Ç«Ç§ÇÃÇ¬ÇÈÇ¨"},
    {"Ç©ÇÌÇÃÇÊÇÎÇ¢"},
    {"Ç§ÇÒÇø"},
};

Player_STATUS player_status[] = {
    {20,-1,0,-1,5,5,5,5,5,-1,-1},//Lv1
    {25,-1,5,-1,5,5,5,5,5,-1,-1},//Lv2
    {23,-1,0,-1,5,5,5,5,5,-1,-1},//Lv3
    {25,-1,0,-1,5,5,5,5,5,-1,-1},//Lv4
};

STATUS status[] = {
    {"Ç≥Ç¢ÇæÇ¢HP"},
    {"HP"},
    {"Ç≥Ç¢ÇæÇ¢MP"},
    {"MP"},
    {"Ç±Ç§Ç∞Ç´"},
    {"Ç⁄Ç§Ç¨ÇÂ"},
    {"ÇÕÇ‚Ç≥"},
    {"Ç©ÇµÇ±Ç≥"},
    {"Ç‹ÇŸÇ§Ç⁄"},
    {"Ç®Ç©ÇÀ"},
    {"ÇØÇ¢ÇØÇÒÇø"},
};

CELLDESC cellDescs[] = {
    {0,0,255,FALSE},//CELL_TYPE_WALL
    {0,255,0,TRUE},//CELL_TYPE_FLOOR
    {0,0,0,FALSE},//CELL_TYPE_DOOR
    {232,162,0,TRUE},//CELL_TYPE_STEPS
    {255,0,0,FALSE}//CELL_TYPE_PLAYER
};


GRAPHDESC graphDescs[] = {
    {"wall.bmp",},//GRAPH_TYPE_WALL,
    {"floor.bmp",},//GRAPH_TYPE_FLOOR,
    {"door.bmp",},//GRAPH_TYPE_DOOR,
    {"steps.bmp"},//GRAPH_TYPE_STEPS
    {"Player.bmp",},//GRAPH_TYPE_PLAYER,
    {"Player_turn.bmp"},//GRAPH_TYPE_PLAYER2,
};