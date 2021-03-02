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
    {510,270,"�k"},
    {550,315,"��"},
    {510,360,"��"},
    {470,315,"��"},
};

MENU menu[] = {
    {500,30,"�͂Ȃ�"},
    {500,75,"�ǂ���"},
    {500,120,"�Ƃт�"},
    {500,165,"�悳"},
};

ITEM item[] = {
    {"�₭����"},
    {"����"},
    {"�ǂ��̂邬"},
    {"����̂�낢"},
    {"����"},
};