﻿//////////////////////////////////////////////////
// ゲームメイン シーン
// 作成：
//////////////////////////////////////////////////
#pragma once

#include "scene.h"
#include "Stage.h"
#include "weapon.h"
#include "second_weapon.h"
#include "Map.h"
#include "Player.h"
#include "Slime.h"
#include "Skeleton.h"
#include "Wizard.h"
#include "EnemyBullet.h"
#include "Common.h"
#include"scene_weapon.h"
#include "scene_gameclear.h"
#include "scene_levelup.h"
#include "scene_blacksmith.h"

class GameScene : public Scene {
private:
	int state, frameCounter;

	//////////////////////////////////////////////////

	Player* player;
	Stage* backimg;
	weapon* Weapon;
	second_weapon* secondweapon;
	bool bookFlg;
	GameUI* gameUI;
	Map* map;

	//////////////////////////////////////////////////

	int exp, level; // 仮

	//敵//
	//スライム
	Slime* slime[MAX_SLIME_NUM];
	int tmpSlimeNum = 0;
	//スケルトン
	Skeleton* skeleton[MAX_SKELETON_NUM];
	int tmpSkeletonNum = 0;
	//魔法使い
	Wizard* wizard[MAX_WIZARD_NUM];
	int tmpWizardNum = 0;
	//弾
	EnemyBullet* enemyBullet[MAX_BULLET_NUM];
	int tmpBulletNum = 0;

	//シーン
	Weapon_Selection* weapon_selection;
	WeaponLevelUp* weapon_level_up;
	Blacksmith* blacksmith;

	//////////

	int stage = 1;//ステージ
	bool hitFlg = false;
	int hitFrameCounter = 0;

	bool is_weapon_select;
	bool weapon_selected;

	bool open_level_up;				// レベルアップ画面が開いているか
	bool restor_cursor_position;	// レベルアップ画面のカーソル位置を元に戻すか

public:
	GameScene();

	~GameScene();

	virtual Scene* update() override;

	virtual void draw() const override;

	//////////////////////////////////////////////////

	int getState() {
		return state;
	};

	void HitCheck();

	//プレイヤーと敵の当たり判定
	void HitEnemy(EnemyBase* enemy);

	//敵
	void EnemyInc();
	//スライム
	void SlimeUpdate();
	void SlimeDraw() const;
	//スケルトン
	void SkeletonUpdate();
	void SkeletonDraw() const;
	//魔法使い
	void WizardUpdate();
	void WizardDraw() const;
	//弾
	void EnemyBulletUpdate(Location location);
	void EnemyBulletDraw() const;
};