#pragma once
#include <vector>

enum {
	MAP_KINGROOM,
	MAP_CASTLE,
};
typedef struct {
	int toMap_Num;
	int Start_X;
	int Start_Y;
}MAP_INFO;

typedef struct {
	const char* Map_Name;
	std::vector<MAP_INFO> map_info;
}MAP;

MAP map_data[] = {
	{"map_test_door.bmp",{{1,6,8}}},
	{"map_test_next.bmp",{{0,6,6}}},
};
