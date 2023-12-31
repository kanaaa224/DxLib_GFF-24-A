﻿#pragma once
#include "EnemyBase.h"
#include "Player.h"
#include "weapon.h"

class Stage;

class Skeleton :public EnemyBase
{
private:

public:
	Skeleton(Player* player,int arrayNum,int SkeletonMaxNum);
	~Skeleton();

	void Update(int arrayNum, Player* player, weapon* w, Stage stage);
	void Draw(int arrayNum) const;

	//移動処理
	void X();
	void Y();

	//スケルトンのダメージ取得
	float GetSkeletonDamage();
};

