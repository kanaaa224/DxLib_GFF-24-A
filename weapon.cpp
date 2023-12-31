#include "main.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>


weapon::weapon()
{
	weaponType = none;
	baseVec = { 80,0,80 };
	relativeRot = 90.0f;		//武器によって変える
	maxRot = 90.0f;
	coolTime = 20;
	isAttacking = false;
	oldIsAttacking = false;
	weaponLevel = 0;
	levelUpFlg = false;

	fps = 0;
	
	P_speed = 0.0;
	P_cooltime = 0.0f;
	P_limit = 0.0;

	tmp = 0;

	location = { 640.360 };

	sword_img = LoadGraph("resources/images/武器/片手剣50・50.png");
	dagger_img = LoadGraph("resources/images/武器/短剣50・50.png");
	greatsword_img = LoadGraph("resources/images/武器/大剣50・50.png");
	attackbuf_img = LoadGraph("resources/images/attack_buf.png");
	tornado_img = LoadGraph("resources/images/tornado_1.png");
	arrow_img = LoadGraph("resources/images/arrow_red.png");
	daggerslash_img = LoadGraph("resources/images/daggerslash.png");
	
	sword_level7_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/densetu.png");
	sword_level8_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/maken.png");
	dagger_level7_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/asasin.png");
	dagger_level8_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/nagenaihu.png");
	greatsword_level7_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/senpuu.png");
	greatsword_level8_img = LoadGraph("resources/images/武器/最終強化(仮)/完成/sajin.png");

	soundFlg = false;

	attackbuf = 1.0f;

	rotSpeed = 4.0f;
	for (int i = 0; i < 10; i++)
	{
		swordSlash[i] = { {0,0},{0,0,0},false };
	}
	hitCnt = 0;
	fpsCnt = 0;
	heelAmount = 10;
	for (int i = 0; i < 10; i++)
	{
		throwDagger[i] = { {0,0},{0,0,0},false };
	}
	slash_img = LoadGraph("resources/images/武器/Sword_S.png");
	slashFlg = false;

	avoidanceDamageFlg = false;


	for (int i = 0; i < MAX_DUST; i++){
		dust[i].flg = false;
		dust[i].startcnt = 0;
		dust[i].endcnt = 0;
	}
	dustcnt = 0;
	dustDamage = 1.0f;

	totalDamage = 0;
}

weapon::weapon(int type)
{
	//使うか不明
	weaponType = type;
	switch (weaponType)
	{
	case sword:
		baseVec = { 80,0,80 };
		break;

	case dagger:
		baseVec = { 50,0,50 };
		break;

	case greatSword:
		baseVec = { 100,0,100 };
		break;
	}
}

weapon::~weapon()
{
	DeleteGraph(sword_img);
	DeleteGraph(dagger_img);
	DeleteGraph(greatsword_img);
	DeleteGraph(attackbuf_img);
}

void weapon::Update(float cursorX, float cursorY, Location playerLocation, Player* player)
{
	location = playerLocation;
	playerVector = { player->Player_MoveX(),player->Player_MoveY() };
	oldIsAttacking = isAttacking;
	

	float x = cursorX - location.x; 
	float y = cursorY - location.y;	
	float length = sqrt((x) * (x) + (y) * (y));


	float innerProduct = ((x) * baseVec.x) + ((y) * baseVec.y);
	if (!isAttacking) {
		weaponAngle = acos(innerProduct / (length * baseVec.length));
		if (y > 0) {
			weaponAngle = (M_PI - weaponAngle);
			weaponAngle += M_PI;
		}
	}
	

	//武器所有中なら
	if (weaponType != none) {
		//クールタイム中
		if (!(isAttacking) && --coolTime <= 0) {
			isAttacking = true;
			coolTime = maxCoolTime;
		}
		//効果音
		if (!soundFlg && coolTime < 5) {
			switch (weaponType)
			{
			case sword:
				if (weaponLevel == 8) {
					SoundManager::PlaySoundSE("se_weapon_sword_Lv8");
				}
				else {
					SoundManager::PlaySoundSE("se_weapon_sword_swing");
				}
				soundFlg = true;
				break;

			case dagger:
				if (weaponLevel == 8) {
					SoundManager::PlaySoundSE("se_weapon_dagger_Lv8");
				}
				else {
					SoundManager::PlaySoundSE("se_weapon_dagger_swing");
				}
				soundFlg = true;
				break;

			case greatSword:
				if (weaponLevel == 8) {
					SoundManager::PlaySoundSE("se_weapon_greatsword_Lv8");
				}
				else {
					SoundManager::PlaySoundSE("se_weapon_greatsword_swing");
				}
				soundFlg = true;
				break;

			default:
				break;
			}

		}

		//攻撃中
		if (isAttacking) {
			if (weaponLevel == 7 && weaponType == greatSword) {
				if (relativeRot < 0) {
					relativeRot = maxRot;
					isAttacking = false;
					slashFlg = false;
					soundFlg = false;
				}
			}
			else {
				if (relativeRot < -maxRot) {
					relativeRot = maxRot;
					isAttacking = false;
					slashFlg = false;
					soundFlg = false;
				}
			}
			rot = -1 * (weaponAngle - (d_r(relativeRot)));

			//回転中の武器の座標
			collisionX = (baseVec.x * cos((rot)) - baseVec.y * sin((rot))) + location.x;	//kk
			collisionY = (baseVec.x * sin((rot)) + baseVec.y * cos((rot))) + location.y;	//kk

			//回転中の武器のベクトル
			collisionVec.x = collisionX - location.x;	//kk
			collisionVec.y = collisionY - location.y;	//kk
			collisionVec.length = sqrt((collisionVec.x) * (collisionVec.x) + (collisionVec.y) * (collisionVec.y));

			//回転
			relativeRot -= rotSpeed;


			//単位ベクトル
			unitVec.x = collisionVec.x / collisionVec.length;
			unitVec.y = collisionVec.y / collisionVec.length;
			unitVec.length = sqrt((unitVec.x) * (unitVec.x) + (unitVec.y) * (unitVec.y));
			
			

			//(仮)斬撃
			if (weaponType == sword && weaponLevel == 7) {
				if (relativeRot < 0 && !slashFlg) {
					SpawnSwordSlash();
					slashFlg = true;
					slashRot = rot;
				}
			}
			//(仮)投げナイフ
			if (weaponType == dagger && weaponLevel == 8) {
				if (relativeRot < 0 && !slashFlg) {
					for (int i = 0; i < 5; i++) {
						SpawnThrowDagger(i);
					}
					slashFlg = true;
					slashRot = rot;
				}
			}
			//砂塵の大剣
			if (weaponType == greatSword && weaponLevel == 8) {
				if (fps % 10 == 0 && dustcnt < 3) {
					if (SpawnDust()) {
						dustcnt++;
						fps = 0;
					}
				}
			}
			//砂塵の大剣用
			fps++;
		
		}

		//回避中にダメージ
		if (weaponType == dagger && weaponLevel == 7) {
			if (player->GetPlayer_Avoidance()) {
				if (!avoidanceDamageFlg) {
					SoundManager::PlaySoundSE("se_weapon_dagger_Lv7");
				}
				avoidanceDamageFlg = true;
			}
			else {
				avoidanceDamageFlg = false;
			}
		}

		if (!isAttacking) {
			fps = 0;
			dustcnt = 0;
		}


		SwordSlashAnim();
		ThrowDaggerAnim();
		DustAnim();
	}

	SwordLevel8(player);

	/*if (InputCtrl::GetKeyState(KEY_INPUT_7)) {
		weaponLevel = 7;
		LevelState();
	}
	if (InputCtrl::GetKeyState(KEY_INPUT_8)) {
		weaponLevel = 8;
		LevelState();
	}
	if (InputCtrl::GetKeyState(KEY_INPUT_9)) {
		weaponLevel = 6;
		LevelState();
	}*/
}

void weapon::Draw() const
{
	//武器描画	
	if (isAttacking) {
		switch (weaponType)
		{
		case sword:
			switch (weaponLevel)
			{
			case 7:
				DrawRotaGraph2(location.x, location.y, 5,570 , /*0.16*/0.18, rot + (M_PI / 4) + d_r(12), sword_level7_img, TRUE, FALSE);
				break;

			case 8:
				DrawRotaGraph2(location.x, location.y, 5, 590,/*0.16*/0.18, rot + (M_PI / 4) + d_r(12), sword_level8_img, TRUE, FALSE);
				break;

			default:
				DrawRotaGraph2(location.x, location.y, 5, 45, /*0.16*/3, rot + (M_PI / 4) + d_r(12), sword_img, TRUE, FALSE);
				break;
			}
			
			break;

		case dagger:
			switch (weaponLevel)
			{
			case 7:
				DrawRotaGraph2(location.x, location.y, 3, 580, 0.15, rot + (M_PI / 4) + d_r(12), dagger_level7_img, TRUE, FALSE);
				break;

			case 8:
				DrawRotaGraph2(location.x, location.y, 3, 550, 0.15, rot + (M_PI / 4) + d_r(12), dagger_level8_img, TRUE, FALSE);
				break;

			default:
				DrawRotaGraph2(location.x, location.y, 3, 48, 2, rot + (M_PI / 4) + d_r(12), dagger_img, TRUE, FALSE);
				break;
			}
			
			break;

		case greatSword:
			switch (weaponLevel)
			{
			case 7:
				DrawRotaGraph2(location.x, location.y, 3, 580, 0.22, rot + (M_PI / 4) + d_r(12), greatsword_level7_img, TRUE, FALSE);
				break;

			case 8:
				DrawRotaGraph2(location.x, location.y, 3, 580, 0.22, rot + (M_PI / 4) + d_r(12), greatsword_level8_img, TRUE, FALSE);
				break;

			default:
				DrawRotaGraph2(location.x, location.y, 10, 50, 3.7, rot + (M_PI / 4) + d_r(12), greatsword_img, TRUE, FALSE);
				break;
			}
			
			break;
		default:
			break;
		}
	}
	//斬撃
	for (int i = 0; i < 10; i++){
		if (swordSlash[i].flg) {
			DrawRotaGraph2(swordSlash[i].l.x, swordSlash[i].l.y, 250, 250, 0.4, slashRot - (M_PI / 4) + M_PI + d_r(40), slash_img, TRUE);
		}
	}
	//投げナイフ
	for (int i = 0; i < MAX_THROW_DAGGER; i++){
		if (throwDagger[i].flg) {
			DrawRotaGraph2(throwDagger[i].l.x, throwDagger[i].l.y, 3, 550, 0.15, throwDagger[i].rot + d_r(throwDagger[i].relativeRot) +(M_PI / 4) + d_r(12), dagger_level8_img, TRUE, FALSE);
		}
	}

	//回避中のダメージ
	if (avoidanceDamageFlg) {
		int randx = rand() % 200 - 100;
		int randy = rand() % 200 - 100;
		int randrot = rand() % 360;
		DrawRotaGraph2(location.x + randx , location.y + randy , 256, 256, 0.3, d_r(randrot), daggerslash_img, TRUE);
		
	}

	//バフ
	if (attackbuf > 1.0f) {
		DrawRotaGraph2(location.x - 25, location.y - 47, 250, 250, 0.07, 0, attackbuf_img, TRUE, TRUE);

		DrawRotaGraph2(location.x + 5, location.y - 47, 250, 250, 0.05, M_PI / 2 + M_PI, arrow_img, TRUE, TRUE);
		if (attackbuf > 2.0f) {
			DrawRotaGraph2(location.x - 5, location.y - 47, 250, 250, 0.05, M_PI / 2 + M_PI, arrow_img, TRUE, TRUE);
		}
	}


	for (int i = 0; i < MAX_DUST; i++) {
		if (dust[i].flg) {

			if (dust[i].startcnt < 70) {
				if (dust[i].startcnt % 5 == 0) {
					DrawRotaGraph2(dust[i].l.x, dust[i].l.y + dust[i].radius, 1000 / 2, 906, 0.0022 * dust[i].radius, 0, tornado_img, TRUE, FALSE);
				}
				else {
					DrawRotaGraph2(dust[i].l.x, dust[i].l.y + dust[i].radius, 1000 / 2, 906, 0.0022 * dust[i].radius, 0, tornado_img, TRUE, TRUE);
				}
			}
			else {
				if (dust[i].endcnt % 5 == 0) {
					DrawRotaGraph2(dust[i].l.x, dust[i].l.y + dust[i].radius, 1000 / 2, 906, 0.0022 * dust[i].radius, 0, tornado_img, TRUE, FALSE);
				}
				else {
					DrawRotaGraph2(dust[i].l.x, dust[i].l.y + dust[i].radius, 1000 / 2, 906, 0.0022 * dust[i].radius, 0, tornado_img, TRUE, TRUE);
				}
			}
			
		}
	}
	
}

void weapon::SetWeaponType(int type)
{
	weaponType = type;
	LevelState();
	
}

void weapon::LevelState()
{
	attackbuf = 1.0f;

	switch (weaponLevel)
	{
	case 0:
		switch (weaponType)
		{
		case sword:
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_SWORD; //片手剣は、大剣　短剣の強化内容を全て取り入れる
			damage = INIT_DAMAGE_SWORD;
			P_speed = 2.0f;
			P_limit = 1.5f;
			P_cooltime = 2.0f;
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetAvoidance_limit(P_cooltime);
			rotSpeed = 4.0f;
			break;

		case dagger:
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER; //プレイヤーの移動速度、回避の速度等を上げれるようにする
			damage = INIT_DAMAGE_DAGGER;
			P_speed = 2.0f;
			P_limit = 1.5f;
			P_cooltime = 2.0f;
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetAvoidance_limit(P_cooltime);
			rotSpeed = 12.0f;
			break;

		case greatSword:
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_GREATSWORD; //クールダウンは一撃が重い分　激重にしたい
			damage = INIT_DAMAGE_GREATSWORD;
			rotSpeed = 4.0f;
			break;
		}

		break;
	case 1://少しだけ全てのステータスを上げる
		switch (weaponType)
		{
		case sword:
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_SWORD * 0.8f; // クールタイムを微量ながら上げる
			damage = INIT_DAMAGE_SWORD;
			rotSpeed = 4.0f;
			break;

		case dagger:
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.8f; //短剣は弱すぎるため最初は敵を４回で倒せるようにする
			damage = INIT_DAMAGE_DAGGER + 1;
			rotSpeed = 12.0f;
			
			break;

		case greatSword:
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 5.0f; // 60 + 10 = 70度
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.8f;
			damage = INIT_DAMAGE_GREATSWORD + 5; // 序盤のステージは一回で敵を倒したい
			rotSpeed = 4.0f;
			break;
		}

		break;
	case 2:
		switch (weaponType)
		{
		case sword:
			//片手剣　攻撃範囲を上げる　移動速度を上げる
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD + 5.0f; // 60 + 20 = 80
			maxCoolTime = INIT_COOLTIME_SWORD * 0.7f;
			damage = INIT_DAMAGE_SWORD;
			P_speed = 2.1f;
			P_limit = 1.5f;//初期値に戻す
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			rotSpeed = 4.0f;

			break;

		case dagger:
			//短剣　ダメージを上げる　移動速度を上げる
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.6f;
			damage = INIT_DAMAGE_DAGGER + 4;
			P_speed = 2.5f;
			P_limit = 1.5f;//初期値に戻す
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			rotSpeed = 12.0f;

			break;

		case greatSword:
			//大剣　ダメージを多く上げる
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 5.0f; // 60 + 10 = 70度
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.8f;
			damage = INIT_DAMAGE_GREATSWORD + 10; // 35
			rotSpeed = 4.0f;

			break;
		}

		break;
	case 3:
		switch (weaponType)
		{
		case sword:
			//片手剣　回避のスピードを上げる　ダメージを上げる
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_SWORD * 0.7f;
			damage = INIT_DAMAGE_SWORD + 1;
			P_limit = 2.0f;
			P_speed = 2.0f;//初期値に戻す
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetPlayer_Speed(P_speed);
			rotSpeed = 4.0f;

			break;

		case dagger:
			//短剣　ダメージよりも振る速度を上げる　回避の速度を上げる
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER + 5.0;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.5f;
			damage = INIT_DAMAGE_DAGGER + 2;
			P_limit = 2.5f;
			P_speed = 2.0f;//初期値に戻す
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetPlayer_Speed(P_speed);
			rotSpeed = 12.0f;

			break;

		case greatSword:
			//大剣　攻撃範囲を上げる
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 10.0f; // 60 + 20 = 80度
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.8f;
			damage = INIT_DAMAGE_GREATSWORD + 5; // 30
			rotSpeed = 4.0f;

			break;
		}

		break;
	case 4:
		switch (weaponType)
		{
		case sword:
			//片手剣　攻撃範囲を上げる　移動速度を上げる
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD + 15; // 60 + 30 = 90 
			maxCoolTime = INIT_COOLTIME_SWORD * 0.7f;
			damage = INIT_DAMAGE_SWORD;
			P_speed = 2.5f;
			P_limit = 1.5f;//初期値に戻す
			P_cooltime = 2.0f;//初期値に戻す
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetAvoidance_limit(P_cooltime);
			rotSpeed = 4.0f;

			break;

		case dagger:
			//短剣　ダメージを上げる　移動速度を上げる
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.5f;
			damage = INIT_DAMAGE_DAGGER + 9;
			P_speed = 3.0f;
			P_cooltime = 2.0f;//初期値に戻す
			Player::SetPlayer_Speed(P_speed);
			Player::SetAvoidance_limit(P_cooltime);
			rotSpeed = 12.0f;

			break;

		case greatSword:
			//大剣　ダメージを多く上げる
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 10.0f; // 60 + 20 = 80
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.8f;
			damage = INIT_DAMAGE_GREATSWORD + 25; // 45
			rotSpeed = 4.0f;

			break;
		}

		break;
	case 5:
		switch (weaponType)
		{
		case sword:
			//片手剣　回避のスピードを上げる　ダメージを上げる
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD + 5; // 60 + 10 = 70
			maxCoolTime = INIT_COOLTIME_SWORD * 0.7f;
			damage = INIT_DAMAGE_SWORD + 1;
			P_limit = 2.3f;
			P_cooltime = 0.0f;
			P_speed = 2.0f;//初期値に戻す
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetAvoidance_limit(P_cooltime);
			Player::SetPlayer_Speed(P_speed);
			rotSpeed = 4.0f;

			break;

		case dagger:
			//短剣　ダメージよりも振る速度を上げる　回避のクールタイムを短くする
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER + 10.0f;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.4f;
			damage = INIT_DAMAGE_DAGGER + 6;
			P_cooltime = 0.0f;
			P_speed = 2.0f;//レベル２に戻す
			Player::SetAvoidance_limit(P_cooltime);
			Player::SetPlayer_Speed(P_speed);
			rotSpeed = 12.0f;

			break;

		case greatSword:
			//大剣　攻撃範囲を上げる
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 20.0f; // 60 + 40 = 100
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.8f;
			damage = INIT_DAMAGE_GREATSWORD + 10; // 35
			rotSpeed = 4.0f;

			break;
		}

		break;
	case 6:
		switch (weaponType)
		{
		case sword:
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD + 15; // 60 + 30 = 90
			maxCoolTime = INIT_COOLTIME_SWORD * 0.6f;
			damage = INIT_DAMAGE_SWORD + 2;
			P_limit = 2.3f;
			P_cooltime = 0.0f;
			P_speed = 2.5f;
			Player::SetPlayer_Upperlimit(P_limit);
			Player::SetAvoidance_limit(P_cooltime);
			Player::SetPlayer_Speed(P_speed);
			rotSpeed = 4.0f;

			break;

		case dagger:
			baseVec = { 80,0,80 };
			// 内容は、4 5 の時よりかは低いが最終強化するとあほみたいに強くなる
			maxRot = INIT_ROTATION_DAGGER + 5.0f; // 60 + 10 = 70
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.4f;
			damage = INIT_DAMAGE_DAGGER + 20;
			P_limit = 2.5f;
			P_cooltime = 1.0f;
			P_speed = 3.0f;
			Player::SetAvoidance_limit(P_cooltime);
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			rotSpeed = 12.0f;

			break;

		case greatSword:
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD + 30.0f; // 60 + 60 = 120度
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.7f;
			damage = INIT_DAMAGE_GREATSWORD + 20; // 45
			rotSpeed = 4.0f;

			break;
		}

		break;
	case 7:		//最終強化１
		switch (weaponType)
		{
		case sword:
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_SWORD * 0.4f;
			damage = INIT_DAMAGE_SWORD;
			rotSpeed = 4.0f;

			break;

		case dagger:
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.4f;
			damage = INIT_DAMAGE_DAGGER + 20;
			P_cooltime = 0.0f;
			P_speed = 3.0f;
			P_limit = 2.5f;
			Player::SetAvoidance_limit(P_cooltime);
			Player::SetPlayer_Speed(P_speed);
			Player::SetPlayer_Upperlimit(P_limit);
			rotSpeed = 12.0f;

			break;

		case greatSword: //回転攻撃
			baseVec = { 130,0,130 };
			maxRot = 360.0f;
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.4f;
			damage = INIT_DAMAGE_GREATSWORD;
			rotSpeed = 8.0f;
			relativeRot = maxRot;
			break;
		}

		break;
	case 8:
		switch (weaponType)
		{
		case sword:
			baseVec = { 120,0,120 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_SWORD * 0.3f;
			damage = INIT_DAMAGE_SWORD;
			attackbuf = 2.0f;
			rotSpeed = 4.0f;

			break;

		case dagger:
			baseVec = { 80,0,80 };
			maxRot = INIT_ROTATION_DAGGER;
			maxCoolTime = INIT_COOLTIME_DAGGER * 0.4f;
			damage = INIT_DAMAGE_DAGGER;
			P_cooltime = 0.0f;
			Player::SetAvoidance_limit(P_cooltime);
			rotSpeed = 4.0f;

			break;

		case greatSword:
			baseVec = { 130,0,130 };
			maxRot = INIT_ROTATION_SWORD;
			maxCoolTime = INIT_COOLTIME_GREATSWORD * 0.5f;
			damage = INIT_DAMAGE_GREATSWORD;
			rotSpeed = 4.0f;

			break;
		}

		break;
	default:
		break;
	}
	coolTime = maxCoolTime;
	//damage = damage * attackbuf;
}

bool weapon::WeaponCollision(Location enemyLocation, float radius)
{
	Location weaponCollisionLocation = { 0,0 };

	if (isAttacking) {

		for (int i = 0; i < (baseVec.length / 10) + 1; i++) {
			weaponCollisionLocation.x = location.x + unitVec.x * (i * 10);		//プレイヤー座標＋単位ベクトル＊半径	//kk
			weaponCollisionLocation.y = location.y + unitVec.y * (i * 10);			//kk

			float tmp_x = weaponCollisionLocation.x - enemyLocation.x;
			float tmp_y = weaponCollisionLocation.y - enemyLocation.y;
			float tmp_length = sqrt(tmp_x * tmp_x + tmp_y * tmp_y);

			if (tmp_length < radius + 10) {
				return true;
			}
		}
	}

	
	//飛ぶ斬撃
	if (weaponLevel == 7 && weaponType == sword) {
		for (int i = 0; i < 10; i++) {
			if (swordSlash[i].flg) {
				float tmpx = swordSlash[i].collsion1.x - swordSlash[i].collsion2.x;
				float tmpy = swordSlash[i].collsion1.y - swordSlash[i].collsion2.y;
				float slashLength = sqrtf(tmpx * tmpx + tmpy * tmpy);
				Vector uv = { tmpx / slashLength ,tmpy / slashLength };
				uv.length = sqrtf(uv.x * uv.x + uv.y * uv.y);

				for (int j = 0; j < (slashLength / 10) + 1; j++) {
					weaponCollisionLocation.x = swordSlash[i].collsion2.x + (uv.x * (j * 10));
					weaponCollisionLocation.y = swordSlash[i].collsion2.y + (uv.y * (j * 10));

					float tmp_x2 = weaponCollisionLocation.x - enemyLocation.x;
					float tmp_y2 = weaponCollisionLocation.y - enemyLocation.y;
					float tmp_length2 = sqrt(tmp_x2 * tmp_x2 + tmp_y2 * tmp_y2);

					if (tmp_length2 < radius + 10) {
						return true;
					}
				}

			}
		}
	}
	
	//投げナイフ
	if (weaponLevel == 8 && weaponType == dagger) {
		for (int i = 0; i < MAX_THROW_DAGGER; i++){
			if (throwDagger[i].flg) {
				for (int i = 0; i < (baseVec.length / 10) + 1; i++) {
					weaponCollisionLocation.x = throwDagger[i].l.x + throwDagger[i].unit.x * (i * 10);		//プレイヤー座標＋単位ベクトル＊半径	//kk
					weaponCollisionLocation.y = throwDagger[i].l.y + throwDagger[i].unit.y * (i * 10);			//kk

					float tmp_x = weaponCollisionLocation.x - enemyLocation.x;
					float tmp_y = weaponCollisionLocation.y - enemyLocation.y;
					float tmp_length = sqrt(tmp_x * tmp_x + tmp_y * tmp_y);

					if (tmp_length < radius + 30) {
						return true;
					}
				}
			}
		}
	}

	//回避中のダメージ
	if (weaponLevel == 7 && weaponType == dagger && avoidanceDamageFlg) {
		float tmp_x = location.x - enemyLocation.x;
		float tmp_y = location.y - enemyLocation.y;
		float tmp_length = sqrt(tmp_x * tmp_x + tmp_y * tmp_y);

		if (tmp_length < radius + AVOIDANCE_DAMAGE_RADIUS) {
			return true;
		}
	}
	return false;
}

bool weapon::SpawnSwordSlash()
{
	for (int i = 0; i < 10; i++) {
		if (!swordSlash[i].flg) {
			swordSlash[i].flg = true;
			swordSlash[i].v.x = unitVec.x * 10;
			swordSlash[i].v.y = unitVec.y * 10;
			swordSlash[i].l.x = collisionX;
			swordSlash[i].l.y = collisionY;
			return true;
		}
	}


	return false;
}

void weapon::SwordSlashAnim()
{
	for (int i = 0; i < 10; i++){
		if (swordSlash[i].flg) {
			swordSlash[i].l.x += swordSlash[i].v.x;
			swordSlash[i].l.y += swordSlash[i].v.y;
		}

		if (swordSlash[i].l.x < 0 || swordSlash[i].l.x > 1280 ||
			swordSlash[i].l.y < 0 || swordSlash[i].l.y > 720) {
			swordSlash[i].flg = false;
		}

		for (int i = 0; i < 10; i++) {
			if (swordSlash[i].flg) {

				//ここ修正
				swordSlash[i].collsion1.x = (baseVec.x - 20) * cos(d_r(90.0f) + slashRot) - baseVec.y * sin(d_r(90.0f) + slashRot) + swordSlash[i].l.x;
				swordSlash[i].collsion1.y =( baseVec.x - 20 )* sin(d_r(90.0f) + slashRot) + baseVec.y * cos(d_r(90.0f) + slashRot) + swordSlash[i].l.y;

				swordSlash[i].collsion2.x =( baseVec.x - 20 )* cos(d_r(270.0f) + slashRot) - baseVec.y * sin(d_r(270.0f) + slashRot) + swordSlash[i].l.x;
				swordSlash[i].collsion2.y =( baseVec.x - 20 )* sin(d_r(270.0f) + slashRot) + baseVec.y * cos(d_r(270.0f) + slashRot) + swordSlash[i].l.y;
			}
		}
	}

	
}

void weapon::SwordLevel8(Player* player)
{
	if (weaponType == sword && weaponLevel == 8) {

		if (isAttacking && !oldIsAttacking) {
			if (player->GetPlayer_HP() > MAX_HP / 2) {
				player->SetPlayer_HP(MAX_HP / 100);//ダメージを受ける
				attackbuf = ATTACKBUF;
			}
		}

	
		if (attackbuf == ATTACKBUF) {
			fpsCnt++;
			if (fpsCnt > 120) {
				damage = INIT_DAMAGE_SWORD;
				fpsCnt = 0;
			}
		}
	}
}

void weapon::SwordLevel8Heel(Player* player)
{
	if (weaponType == sword && weaponLevel == 8) {
		if (hitCnt > 15) {
			heelAmount = 6;
			player->SetPlayer_HP(-heelAmount);
		}
		else if (hitCnt > 12) {
			heelAmount = 5;
			player->SetPlayer_HP(-heelAmount);
		}
		else if (hitCnt > 9) {
			heelAmount = 4;
			player->SetPlayer_HP(-heelAmount);
		}
		else if (hitCnt > 6) {
			heelAmount = 3;
			player->SetPlayer_HP(-heelAmount);
		}
		else if (hitCnt > 3) {
			heelAmount = 2;
			player->SetPlayer_HP(-0.1);
		}
	}
}

bool weapon::SpawnThrowDagger(int num)
{
	for (int i = 0; i < MAX_THROW_DAGGER; i++) {
		if (!throwDagger[i].flg) {
			throwDagger[i].flg = true;
			throwDagger[i].relativeRot = (num * 20) - 40;
			throwDagger[i].rot = rot;
			throwDagger[i].v.x = 8 * cos(d_r(throwDagger[i].relativeRot) + rot);
			throwDagger[i].v.y = 8 * sin(d_r(throwDagger[i].relativeRot) + rot);
			throwDagger[i].v.length = sqrtf(throwDagger[i].v.x * throwDagger[i].v.x + throwDagger[i].v.y * throwDagger[i].v.y);
			throwDagger[i].l.x = location.x;
			throwDagger[i].l.y = location.y;

			throwDagger[i].unit.x = throwDagger[i].v.x / throwDagger[i].v.length;
			throwDagger[i].unit.y = throwDagger[i].v.y / throwDagger[i].v.length;
			return true;
		}
	}

	return false;
}

void weapon::ThrowDaggerAnim()
{
	for (int i = 0; i < MAX_THROW_DAGGER; i++) {
		if (throwDagger[i].flg) {
			throwDagger[i].l.x += throwDagger[i].v.x;
			throwDagger[i].l.y += throwDagger[i].v.y;

			if (throwDagger[i].l.x < 0 || throwDagger[i].l.x > 1280 ||
				throwDagger[i].l.y < 0 || throwDagger[i].l.y > 720) {
				throwDagger[i].flg = false;
			}
		}
	}
}

bool weapon::SpawnDust()
{
	for (int i = 0; i < MAX_DUST; i++){
		if (!dust[i].flg) {
			dust[i].v.x = unitVec.x * 5;
			dust[i].v.y = unitVec.y * 5;
			dust[i].l = location;
			dust[i].flg = true;
			dust[i].radius = 0.0f;
			dust[i].startcnt = 0;
			dust[i].endcnt = 0;

			return true;
		}
	}
	return false;
}

void weapon::DustAnim()
{
	for (int i = 0; i < MAX_DUST; i++) {
		if (dust[i].flg) {
			int len = sqrtf((dust[i].l.x - location.x) * (dust[i].l.x - location.x) + (dust[i].l.y - location.y) * (dust[i].l.y - location.y));
			
			if (dust[i].startcnt++ < 70) {
				dust[i].l.x += dust[i].v.x;
				dust[i].l.y += dust[i].v.y;
				dust[i].radius += 1;
			}
			else {
				dust[i].endcnt++;
				dust[i].l.x -= playerVector.x;
				dust[i].l.y -= playerVector.y;
			}

			if (dust[i].endcnt > 120) {
				dust[i].flg = false;
				dust[i].endcnt = 0;
				dust[i].startcnt = 0;
			}

			if (dust[i].l.x < 0 || dust[i].l.x > 1280 ||
				dust[i].l.y < 0 || dust[i].l.y > 720) {
				dust[i].flg = false;
			}
		}
	}
}

bool weapon::DustCollision(Location enemyLocation, float radius)
{
	for (int i = 0; i < MAX_DUST; i++){
		if (dust[i].flg) {
			float tmp_x = dust[i].l.x - enemyLocation.x;
			float tmp_y = dust[i].l.y - enemyLocation.y;
			float tmp_length = sqrt(tmp_x * tmp_x + tmp_y * tmp_y);

			if (tmp_length < radius + dust[i].radius) {
				return true;
			}
		}
	}

	return false;
}

void weapon::InitWeapon()
{
	LevelState();
	relativeRot = maxRot;
	isAttacking = false;
	oldIsAttacking = false;
	soundFlg = false;

	for (int i = 0; i < 10; i++)
	{
		swordSlash[i] = { {0,0},{0,0,0},false };
	}
	hitCnt = 0;
	fpsCnt = 0;
	heelAmount = 10;
	for (int i = 0; i < MAX_THROW_DAGGER; i++)
	{
		throwDagger[i] = { {0,0},{0,0,0},false };
	}

	slashFlg = false;

	avoidanceDamageFlg = false;

	for (int i = 0; i < MAX_DUST; i++) {
		dust[i].flg = false;
		dust[i].startcnt = 0;
		dust[i].endcnt = 0;
	}
	dustcnt = 0;
	dustDamage = 1.0f;
}

void weapon::AddTotalDamage()
{
	totalDamage += damage;
}

void weapon::AddTotalDamageDust()
{
	totalDamage += dustDamage;
}


//回転の公式
//r: 角度(ラジアン)
//x : 元のX座標
//y : 元のY座標

//X = x * cos(r) - y * sin(r)
//Y = x * sin(r) + y * cos(r)

