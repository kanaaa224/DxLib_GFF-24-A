﻿#include "Wizard.h"
#include "Common.h"
#include <math.h>
#include "inputCtrl.h"
#include"Stage.h"

//#define DEBUG

Wizard::Wizard(Player* player, int arrayNum, int WizardMaxNum)
{
	//変数の初期化
	hp = WIZARD_HP_MAX;
	damage = WIZARD_ATTAK_DAMAGE;
	location.x = 0;
	location.y = 0;
	vector.x = 0;
	vector.y = 0;
	reroadTime = BULLET_LIFE_TIME;

	respawnTimeCnt = 0;
	respawnTime = SetRespawnTime(arrayNum, WizardMaxNum, 10);

	//リスポーンポイント決め
	SetRespawnPoint(player,1, arrayNum);

	//-----画像に関わる変数-----//
	//画像読込
	imgArray[0] = LoadGraph("resources/images/enemy_images/wizard/wizard_move_left_2-2.png");
	imgArray[1] = LoadGraph("resources/images/enemy_images/wizard/wizard_move_left_2-1.png");
	//画像切り替え用フレームカウント変数の初期化
	imgFrameCounter = 0;
	//画像格納用変数の初期化
	img = imgArray[0];
	//画像左右
	imgAngle = 0;
}

Wizard::~Wizard()
{
	DeleteGraph(img);
}

void Wizard::Update(int arrayNum, Player* player, weapon* w, Stage stage)
{
	//画像切り替え用フレームカウントをインクリメント
	imgFrameCounter++;

	if (respawnFlg == true && hp > 0) {

		//画像左右
		imgAngle = CheckImgAngle();

		//画像切り替え
		if (imgFrameCounter >= 1 && imgFrameCounter <= 60) {
			img = imgArray[0];
		}
		else if (imgFrameCounter >= 61 && imgFrameCounter <= 120) {
			img = imgArray[1];
		}
		

		//プレイヤーの移動量をdiffにセット
		SetPlayerAmountOfTravel_X(player->Player_MoveX());
		SetPlayerAmountOfTravel_Y(player->Player_MoveY());
		//プレイヤーの座標をdiffLocationにセット
		SetPlayer_Location(player->GetLocation());
	
		//移動処理//
		if (hitWeaponFlg == false) {
			X();
			location.x += vector.x - diff.x;
			Y();
			location.y += vector.y - diff.y;
		}
		else if (hitWeaponFlg == true) {
			vector.x = -vector.x * KNCKBACK;
			location.x += vector.x - diff.x;
			vector.y = -vector.y * KNCKBACK;
			location.y += vector.y - diff.y;
			//武器からの攻撃とHPが０以上なら赤く表示する
			if (hitWeaponFlg == true && hp > 0) {
				redDrawFlg = true;
			}
			hitWeaponFlg = false;
		}
	}

	if (reroadCnt == 1) {
		createBulletFlg = true;
	}
	else if(reroadCnt != 1){
		createBulletFlg = false;
	}

	if (hp <= 0) {
		alphaNum -= 5;
	}

	//Cnt
	if (respawnTimeCnt == respawnTime) {//設定された時間になったらrespawnFlgをtrue
		respawnFlg = true;
	}
	respawnTimeCnt++;//リスポーンCnt


	if (redFrameCounter == RED_FRAME) {
		redDrawFlg = false;
		redFrameCounter = 0;
	}
	if (redDrawFlg == true) {
		redFrameCounter++;
	}

	if (stopFlg == true) {
		shootFlg = true;
	}
	if (reroadCnt == reroadTime) {
		shootFlg = false;
		reroadCnt = 0;
	}
	if (shootFlg == true) {
		reroadCnt++;
	}

	//画像切り替え用フレームカウント変数が３０になったら０にする
	if (imgFrameCounter >= 120) {
		imgFrameCounter = 0;
	}

	else {
		hitWeaponFlg = false;
	}
	//デバッグ（マクロのDEBUGをコメントアウト又はReleaseにすれば使えなくなります）
#ifdef DEBUG
	if (InputCtrl::GetKeyState(KEY_INPUT_D) == PRESS && hp >= 0) {
		hitWeaponFlg = true;
		hp -= 9999;
	}
	else {
		hitWeaponFlg = false;
	}
#endif // DEBUG
}

void Wizard::Draw(int arrayNum) const
{
	if (respawnFlg == true) {

		if (imgAngle == IMG_L) {//左向き表示
			if (hp <= 0) {//HPが０の時
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaNum);
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_L);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			}
			else {//通常時
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_L);
			}

			if (redDrawFlg == true) {//武器からダメージを受けた時とHPが０じゃない時、敵を赤色表示
				SetDrawBright(255, 0, 0);
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_L);
				SetDrawBright(255, 255, 255);
			}
		}
		else if (imgAngle == IMG_R) {//右向き表示
			if (hp <= 0) {//HPが０の時
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaNum);
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_R);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			}
			else {//通常時
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_R);
			}

			if (redDrawFlg == true) {//武器からダメージを受けた時とHPが０じゃない時、敵を赤色表示
				SetDrawBright(255, 0, 0);
				DrawRotaGraph((int)location.x, (int)location.y, 1, 0, img, TRUE, IMG_R);
				SetDrawBright(255, 255, 255);
			}
		}

		//デバッグ表示（マクロのDEBUGをコメントアウト又はReleaseにすれば使えなくなります）
#ifdef DEBUG
		float hpRate = hp / SLIME_HP_MAX;
		float sizeRate = -20.0f + 40.0f * hpRate;

		if (InputCtrl::GetKeyState(KEY_INPUT_H) == PRESSED) {//HP表示
			if (hp > 0) {
				DrawBox((int)location.x - 20, (int)location.y - 30, (int)location.x + 20, (int)location.y - 25, C_BLACK, TRUE);
				DrawBox((int)location.x - 20, (int)location.y - 30, (int)location.x + (int)sizeRate, (int)location.y - 25, C_RED, TRUE);
				DrawFormatString((int)location.x, (int)location.y, C_RED, "hpRate:%.2f", hpRate);
				DrawFormatString((int)location.x, (int)location.y + 15, C_RED, "sizeRate:%.2f", sizeRate);
			}
		}

		if (InputCtrl::GetKeyState(KEY_INPUT_S) == PRESSED) {//ステータス表示
			DrawFormatString((int)location.x, (int)location.y, C_RED, "array:%d", arrayNum);
			DrawFormatString((int)location.x, (int)location.y + 15, C_RED, "VX:%.2f, VY:%.2f", vector.x, vector.y);
			DrawFormatString((int)location.x, (int)location.y + 30, C_RED, "dx:%.2f, dy:%.2f", diff.x, diff.y);
			DrawFormatString((int)location.x, (int)location.y + 45, C_RED, "HP:%d", hp);
			DrawFormatString((int)location.x, (int)location.y + 60, C_RED, "HitFlg:%d", hitFlg);
		}
#endif // DEBUG
	}
}

void Wizard::X()
{
	if (PlayerLoad(location, true) <= STOP_SHOOTING_DISTANCE) {
		vector.x = vector.x * 0.87;
	}
	else if (PlayerLoad(location, true) >= STOP_SHOOTING_DISTANCE) {
		if (hitFlg == HIT) {
			vector.x = Normalization_X(PlayerLoad_X(location.x), PlayerLoad_Y(location.y)) * ENEMY_SPEED;
			vector.x = vector.x + caleVector.x;
		}
		else if (hitFlg == NO_COLLISION) {
			vector.x = Normalization_X(PlayerLoad_X(location.x), PlayerLoad_Y(location.y)) * ENEMY_SPEED;
		}
	}
}

void Wizard::Y()
{
	if (PlayerLoad(location, true) <= STOP_SHOOTING_DISTANCE) {
		vector.y = vector.y * 0.87;
		stopFlg = true;
	}
	else if (PlayerLoad(location, true) >= STOP_SHOOTING_DISTANCE) {
		if (hitFlg == HIT) {
			vector.y = Normalization_Y(PlayerLoad_X(location.x), PlayerLoad_Y(location.y)) * ENEMY_SPEED;
			vector.y = vector.y + caleVector.y;
			hitFlg = NO_COLLISION;
		}
		else if (hitFlg == NO_COLLISION) {
			vector.y = Normalization_Y(PlayerLoad_X(location.x), PlayerLoad_Y(location.y)) * ENEMY_SPEED;
		}
		stopFlg = false;
	}
}

float Wizard::GetWizardDamage()
{
	return damage;
}

bool Wizard::GetStopFlg()
{
	return stopFlg;
}

bool Wizard::GetCreateBulletFlg()
{
	return createBulletFlg;
}

bool Wizard::GetShootFlg() 
{
	return shootFlg;
}

