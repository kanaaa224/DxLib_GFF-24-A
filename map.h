#pragma once

#include "rest.h"

class GameScene;
class GameUI;

#define DATA_MAX 22

class Map
{
private:
	int MapDeta[DATA_MAX];
	int RandNum[3];

	// �A�C�R�����W(����)
	const int icon_loc_def[DATA_MAX][2] = {
	{360, 590}, {610, 620}, {920, 580}, {390, 470}, {520, 510},
	{670, 530}, {790, 510}, {910, 470}, {640, 370}, {440, 270},
	{580, 220}, {800, 290}, {340, 180}, {710, 160}, {910, 210},
	{850, 130}, {460, 80}, {760, 40}, {380, -80}, {860, -110},
	{640, -200}, {640, -300},
	};
	const int icon_loc_def_temp[3][DATA_MAX][2] = 
	{
		{
			{430, 590}, {630, 590}, {830, 590}, {390, 470}, {530, 480},
			{730, 480}, {880, 470}, {630, 370}, {430, 270}, {630, 200},
			{870, 270}, {330, 80}, {630, -30}, {870, 40}, {530, 80},
			{930, 140}, {810, 140}, {430, -100}, {870, -100}, {630, -200},
			{630, -300},
		},
		{
			{530, 590}, {730, 590}, {430, 470}, {630, 470}, {730, 470},
			{530, 370}, {730, 370}, {630, 320}, {430, 220}, {780, 220},
			{430, 130}, {520, 130}, {780, 130}, {430, 30}, {730, 30},
			{830, 30}, {780, -40}, {560, -90}, {780, -130}, {630, -200},
			{630, -300},
		},
		{
			{630, 590}, {530, 540}, {730, 540}, {470, 470}, {580, 470},
			{730, 470}, {530, 400}, {630, 320}, {530, 220}, {730, 220},
			{430, 120}, {530, 120}, {630, 120}, {480, 30}, {580, 30},
			{730, 30}, {530, -20}, {730, -70}, {630, -130}, {630, -200},
			{630, -300},
		}
	};

	int icon_loc[21][2];  // �A�C�R�����W
	int icon_vec;         // �A�C�R���ړ���

	// ���ɍs����X�e�[�W
	const int next_stage[DATA_MAX][3]{
		{3,4,-1},{4,5,-1},{6,7,-1},{8,-1,-1},{8,-1,-1},
		{8,-1,-1}, {8,-1,-1},{8,-1,-1},{9,10,11},{12,16,-1},{13,16,-1},
		{13,14,-1},{16,-1,-1},{17,-1,-1},{15,-1,-1},{17,-1,-1},
		{18,19,-1},{18,19,-1},{20,-1,-1},{20,-1,-1},{21,-1,-1},
		{0,1,2},
	};
	const int next_stage_temp[3][DATA_MAX][3]
	{
		{
			{3,-1},{4,5,-1},{6,-1},{7,-1},
			{7,-1},{7,-1},{7,-1},{8,9,10},{11,14,-1},
			{12,-1},{15,-1},{17,-1},{9,19,-1},{18,-1},
			{17,-1},{13,-1},{10,13,-1},{19,-1},{19,-1},
			{20,-1},{0,1,2}
		},
		{
			{2,3,-1},{4,-1},{5,-1},{5,-1},
			{6,-1},{7,-1},{7,-1},{8,9,-1},{10,11,-1},
			{12,-1},{13,-1},{17,-1},{14,15,-1},{17,-1},
			{16,-1},{16,-1},{18,-1},{19,-1},{19,-1},
			{20,-1},{0,1,2}
		},
		{
			{1,2,-1},{3,4,-1},{5,-1},{6,-1},
			{6,-1},{7,-1},{7,-1},{8,9,-1},{10,11,12},
			{15,-1},{13,-1},{13,14,-1},{14,-1},{16,-1},
			{16,-1},{17,-1},{18,-1},{18,-1},{19,-1},
			{20,-1},{0,1,2}
		}
	};

	int stage_log[10]{0};

	Rest* rest;
	GameUI* ui;

	int cursor_pos;    // �J�[�\���ʒu(���X�e�[�W)
	int cursor_loc;    // �J�[�\���ʒu(�S�̃}�b�v)
	int move_cool;     // �J�[�\���ړ��N�[���^�C��
	bool cursor_move;  // �J�[�\���ړ��ɂ��J�����ړ������邩(�X�N���[���ړ�����Ɖ���)
	int now_stage;     // ���݂̃X�e�[�W

	bool is_map_mode;

	bool is_rest;		//�x�e�X�e�[�W�̃t���O
	bool is_show_rest;	//�x�e�X�e�[�W���f���H

	// �摜
	int battle_img = 0;
	int event_img = 0;
	int rest_img = 0;
	int anvil_img = 0;
	int boss_img = 0;
	int icon_back_img = 0;
	int map_cursor = 0;
public:
	Map(GameUI* ui);

	~Map();

	int update(int&, bool&);


	void draw() const;

	bool GetIsMapMode() {
		return is_map_mode;
	}

	void SetIsMapMode(const bool flg) {
		is_map_mode = flg;
	}

	int NowStage() {
		return now_stage;
	}

	void ClearStage() {
		cursor_pos = 0;
		cursor_loc = next_stage[now_stage][0];
		for (int i = 0; i <= 10; i++) {
			if (stage_log[i] == -1) {
				stage_log[i] = now_stage;
				break;
			}
		}
	}

	void ResetStage();
};