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

DIRE dire[] = {
    {510,270,"ñk"},
    {550,315,"ìå"},
    {510,360,"ìÏ"},
    {470,315,"êº"},
};

MENU menu[] = {
    {500,30,"ÇÕÇ»Ç∑"},
    {500,75,"Ç«Ç§ÇÆ"},
    {500,120,"Ç∆Ç—ÇÁ"},
    {500,165,"Ç¬ÇÊÇ≥"},
};

ITEM item[] = {
    {"Ç‚Ç≠ÇªÇ§"},
    {"Ç©Ç¨"},
    {"Ç«Ç§ÇÃÇ¬ÇÈÇ¨"},
    {"Ç©ÇÌÇÃÇÊÇÎÇ¢"},
    {"Ç§ÇÒÇø"},
};