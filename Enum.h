#pragma once
enum {
    CELL_TYPE_WALL,
    CELL_TYPE_FLOOR,
    CELL_TYPE_DOOR,
    CELL_TYPE_STEPS,
    CELL_TYPE_TRESURE,
    CELL_TYPE_PLAYER,
    CELL_TYPE_MAX,
};

enum {
    GRAPH_TYPE_WALL,
    GRAPH_TYPE_FLOOR,
    GRAPH_TYPE_DOOR,
    GRAPH_TYPE_STEPS,
    GRAPH_TYPE_TRESURE,
    GRAPH_TYPE_PLAYER,
    GRAPH_TYPE_PLAYER2,
    GRAPH_TYPE_MAX,
};

enum {
    GameMode_FIELD,
    GameMode_MENU,
    GameMode_BATTLE,
    GameMode_LOAD,
};

enum {
    MenuType_Talk,
    MenuType_SEARCH,
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

enum {
    ItemBox_1,
    ItemBox_2,
    ItemBox_3,
    ItemBox_4,
    ItemBox_5,
    ItemBox_6,
    ItemBox_7,
    ItemBox_8,
    ItemBox_9,
    ItemBox_10,
    ItemBox_Max,
};

enum {
    Item_Herb,//‚â‚­‚»‚¤
    Item_Key,//‚©‚¬
    Item_CupperSword,//"‚Ç‚¤‚Ì‚Â‚é‚¬"
    Item_Meil,//"‚©‚í‚Ì‚æ‚ë‚¢"
    Item_unchi,//"‚¤‚ñ‚¿"
    Item_Num_Max,
};

enum {
    STATUS_MAXHP,
    STATUS_HP,
    STATUS_MAXMP,
    STATUS_MP,
    STATUS_ATTACK,
    STATUS_DEFENSE,
    STATUS_SPEED,
    STATUS_WISE,
    STATUS_MAGICDEF,
    STATUS_GOLD,
    STATUS_EXP,
    STATUS_MAX,
};