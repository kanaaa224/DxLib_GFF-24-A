﻿#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include "weapon.h"
#include "Common.h"

class Wizard :public EnemyBase
{
private:
	bool shootFlg = false;
	bool createBulletFlg = false;
	int reroadCnt = 0;
	int reroadTime;
public:
	Wizard(Player* player, int arrayNum, int WizardMaxNum);
	~Wizard();

	void Update(int arrayNum, Player* player, weapon* w, Stage stage);
	void Draw(int arrayNum) const;

	//移動処理
	void X();
	void Y();

	//スライムのダメージ取得
	float GetWizardDamage();

	//移動の状態が立ち止まっているかどうかのFlgを返す
	bool GetStopFlg();
	//弾を生成をしてもよいか
	bool GetCreateBulletFlg();
	//弾を発射中かどうか
	bool GetShootFlg();
};

