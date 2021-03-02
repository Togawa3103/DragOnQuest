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

ITEM item[] = {
    {"やくそう"},
    {"かぎ"},
    {"どうのつるぎ"},
    {"かわのよろい"},
    {"うんち"},
};