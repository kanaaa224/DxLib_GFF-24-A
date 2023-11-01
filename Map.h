#pragma once
#include"DxLib.h"
#include"scene.h"

#define DATA_MAX 21

class Map : public Scene
{
private:
	int MapData[DATA_MAX];
	int RandNum[3];

	// アイコン座標
	int icon_loc_def[21][2] = {
	{300, 700}, {600, 700}, {1000, 700}, {400, 600}, {600, 600},
	{800, 600}, {1000, 600}, {700, 500}, {500, 400}, {700, 400},
	{900, 400}, {400, 300}, {800, 300}, {1000, 300}, {500, 200},
	{900, 200}, {800, 200}, {300, 100}, {900, 100}, {700, -200},
	{700, -300},
	};
	int icon_loc[21][2];
	int icon_vec;

	// 次に行けるステージ
	int next_stage[21][3]{
		{3,4},{4},{5,6},{7},{7},
		{7},{7},{8,9,10},{11,14},{12,14},
		{12,13},{14},{16},{15},{17,18},
		{16},{17,18},{19},{19},{20},
	};

	int cursor_pos;
	int move_cool;
	bool cursor_move;

	int battle_img = 0;
	int event_img = 0;
	int rest_img = 0;
	int anvil_img = 0;
	int boss_img = 0;
	int map_cursor = 0;
public:
	Map();

	~Map();

	virtual Scene* update() override;

	virtual void draw() const override;
};

