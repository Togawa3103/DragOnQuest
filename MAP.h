#pragma once
#include <vector>

enum {
	MAP_KINGROOM,
	MAP_CASTLE,
};

typedef struct {
	const char* Map_Name;
	std::vector<int> toMap_Num;
	std::vector<int> Start_X;
	std::vector<int> Start_Y;
}MAP;

MAP map_data[] = {
	{"map_test_door.bmp",{1},{6},{8}},
	{"map_test_next.bmp",{0},{6},{6}},
};
