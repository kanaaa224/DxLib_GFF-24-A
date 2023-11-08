#pragma once
#include"DxLib.h"
#include"SphereCollider.h"

#define DATA_MAX 21

class Map : SphereCollider
{
private:
	int MapDeta[DATA_MAX];
	int RandNum[3];

	// アイコン座標(初期)
	int icon_loc_def[21][2] = {
	{360, 590}, {630, 610}, {920, 580}, {390, 470}, {570, 510},
	{730, 480}, {910, 470}, {640, 370}, {440, 270}, {580, 220},
	{800, 290}, {340, 180}, {710, 160}, {960, 210}, {460, 80},
	{850, 130}, {760, 40}, {380, -80}, {860, -110}, {640, -200},
	{640, -300},
	};
	int icon_loc[21][2];  // アイコン座標
	int icon_vec;         // アイコン移動量

	// 次に行けるステージ
	int next_stage[21][3]{
		{3,4},{4},{5,6},{7},{7},
		{7},{7},{8,9,10},{11,14},{12,14},
		{12,13},{14},{16},{15},{17,18},
		{16},{17,18},{19},{19},{20},
	};

	int cursor_pos;    // カーソル位置
	int cursor_loc_x, cursor_loc_y;
	int move_cool;     // カーソル移動クールタイム
	bool cursor_move;  // カーソル移動によるカメラ移動をするか(スクロール移動すると解除)
	int angle;
	int r;


	// 画像
	int battle_img = 0;
	int event_img = 0;
	int rest_img = 0;
	int anvil_img = 0;
	int boss_img = 0;
	int map_cursor = 0;
public:
	Map();

	~Map();

	int update(bool& flg);

	void draw() const;
};