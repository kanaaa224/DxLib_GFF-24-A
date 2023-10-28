﻿//////////////////////////////////////////////////
// ゲームメイン シーン
// 作成：
//////////////////////////////////////////////////
#include "main.h"

GameScene::GameScene() {
	state = 0;
	frameCounter = 0;

	player = new Player;
	backimg = new Stage;
	Weapon = new weapon;
	gameUI = new GameUI;

	
	//
	weapon_selection = new Weapon_Selection();

	is_weapon_selct = false;


	//////////////////////////////////////////////////

	//img_background = LoadGraph("./resources/images/background.png"); // 仮
};

GameScene::~GameScene() {

	delete player;
	delete backimg;
	delete Weapon;
	delete gameUI;

	//////////////////////////////////////////////////

	//DeleteGraph(img_background); // 仮
};

Scene* GameScene::update() {
	frameCounter++;

	if (InputCtrl::GetKeyState(KEY_INPUT_ESCAPE)) return new DebugScene(); // 仮

	//武器選択画面
	if (is_weapon_selct != true)
	{
		weapon_selection->update(Weapon, is_weapon_selct);
		if (is_weapon_selct == true)
		{
			delete weapon_selection;
			weapon_selection = nullptr;
		}
		return this;
	}

	//敵//
	if (tmpSlimeNum < SLIME_1_STAGE_NUM) {
		slime[tmpSlimeNum] = new Slime(tmpSlimeNum);
		tmpSlimeNum++;
	}

	if (stage == 1) {
		for (int i = 0; i < SLIME_1_STAGE_NUM; i++) {
			if (slime[i] != nullptr) {
				slime[i]->Update(i, player);
			}
		}
	}

	HitCheck();

	//武器と敵の当たり判定
	if (stage == 1) {
		for (int i = 0; i < SLIME_1_STAGE_NUM; i++) {
			if (slime[i] != nullptr) {
				if (Weapon->WeaponCollision(slime[i]->GetEnemyLocation(), slime[i]->GetEnemyRadius())) {
					//ここに敵のHPが削れる処理
					//例：slime[i].subtractionHP(Weapon->GetDamage);
				}
			}
		}
	}

	//武器のレベルアップ（デバッグ用）
	if (!Weapon->GetLevelUpFlg()) {
		if (InputCtrl::GetKeyState(KEY_INPUT_1) == PRESS) {
			Weapon->SetWeaponType(sword);
		}
		if (InputCtrl::GetKeyState(KEY_INPUT_2) == PRESS) {
			Weapon->SetWeaponType(dagger);
		}
		if (InputCtrl::GetKeyState(KEY_INPUT_3) == PRESS) {
			Weapon->SetWeaponType(greatSword);
		}
	}
	////////////
	backimg->update();
	player->update();
	Weapon->Update(player->Player_AimingX(), player->Player_AimingY());
	gameUI->update(this);


	// ゲームオーバー画面へ遷移
	if (InputCtrl::GetButtonState(XINPUT_BUTTON_Y) == PRESS) {
		return new GameOverScene;
	}

	return this;
};

void GameScene::draw() const {
	//DrawExtendGraph(0, 0, 1280, 720, img_background, TRUE); // 仮

	backimg->draw();
	Weapon->Draw();
	player->draw();

	//敵//
	if (stage == 1) {
		for (int i = 0; i < MAX_SLIME_NUM; i++) {
			if (slime[i] != nullptr) {
				slime[i]->Draw(i);
			}
		}
	}
	////////////

	if (is_weapon_selct != true)
	{
		weapon_selection->draw();
	}

	//gameUI->draw();
}

void GameScene::HitCheck()
{

	//スライムの当たり判定
	for (int i = 0; i <= SLIME_1_STAGE_NUM; i++) {
		if (slime[i] != nullptr) {
			for (int j = 0; j <= SLIME_1_STAGE_NUM; j++) {
				if (i == j) {
					j = i + 1;
				}

				if (slime[j] != nullptr) {
					if (slime[i]->CheckCollision(static_cast<SphereCollider>(*slime[j]), player) == OVERLAP) {//重なっている
						slime[i]->SetHitFlg(OVERLAP);
						slime[j]->SetHitFlg(OVERLAP);
					}
					else if (slime[i]->CheckCollision(static_cast<SphereCollider>(*slime[j]), player) == HIT) {//当たっている
						slime[i]->SetHitFlg(HIT);
						slime[j]->SetHitFlg(HIT);
					}
					else if (slime[i]->CheckCollision(static_cast<SphereCollider>(*slime[j]), player) == NO_COLLISION) {//当たってない
						slime[i]->SetHitFlg(NO_COLLISION);
						slime[j]->SetHitFlg(NO_COLLISION);
					}
				}
			}

		}
	}
	
}
;