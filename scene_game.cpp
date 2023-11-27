﻿//////////////////////////////////////////////////
// ゲームメイン シーン
// 作成：
//////////////////////////////////////////////////
#include "main.h"

GameScene::GameScene() {
	mode         = GameSceneMode::map;
	state        = play;
	frameCounter = 0;

	//////////////////////////////////////////////////

	player  = new Player;
	stage   = new Stage;
	weaponA = new weapon;
	weaponB = new second_weapon;
	gameUI  = new GameUI;

	//////////////////////////////////////////////////

	map           = new Map;
	weaponSelect  = new Weapon_Selection(weapon_selected);
	weaponLevelup = new WeaponLevelUp;
	blacksmith    = new Blacksmith;
	rest          = new Rest(gameUI);

	//////////////////////////////////////////////////
	
	minotaur = new Minotaur;

	//////////////////////////////////////////////////

	swordHitFlg = false;
	bookFlg     = false;

	weapon_selected = false;

	// レベルアップ画面用
	restor_cursor_position = true;

	//////////////////////////////////////////////////

	hp    = 0;
	exp   = 0;
	level = 0;
	point = 0;

	currentFloor = 0;
	currentStage = 0;
	battleMode   = 0;

	bossState = 0;


	map->ResetStage();


	gameUI->setBanner(std::to_string(currentFloor + 1) + "F - XXXの部屋", "全てのモンスターを倒してください");


	// とりあえず
	// 敵をどのステージでどれだけ出すかのデータ生成
	std::map<std::string, int> data;
	data["slime"]    = 10;
	data["skeleton"] = 0;
	data["wizard"]   = 0;
	shimabukuro.push_back(data);

	for (int i = 1; i < 20; i++) {
		data["slime"]    = shimabukuro[i - 1]["slime"]    + 10;
		data["skeleton"] = shimabukuro[i - 1]["skeleton"] + 3;
		data["wizard"]   = shimabukuro[i - 1]["wizard"]   + 2;
		shimabukuro.push_back(data);
	};

	enemySpawnData = shimabukuro[currentFloor];

	// 経験値の最大値データ生成
	for (int i = 1; i < 20; i++) {
		expData.push_back(i * 100);
	};
};

GameScene::~GameScene() {
	delete player;
	delete stage;
	delete weaponA;
	delete weaponB;
	delete gameUI;

	delete map;
	delete weaponSelect;
	delete weaponLevelup;
	delete blacksmith;
	delete rest;

	delete minotaur;
};

Scene* GameScene::update() {
	if (InputCtrl::GetKeyState(KEY_INPUT_ESCAPE)) return new DebugScene(); // 仮

	// ポーズ
	if (InputCtrl::GetKeyState(KEY_INPUT_P) == PRESS || InputCtrl::GetButtonState(XINPUT_BUTTON_START) == PRESS) {
		if (state) state = 0;
		else state++;
	};

	if (state == pause) return this;

	//////////////////////////////////////////////////

	if (mode >= GameSceneMode::main && gameUI->getState() == playerUI) {
		// 武器のレベルアップ画面 - Xボタンで表示と非表示を切り替え
		if (InputCtrl::GetKeyState(KEY_INPUT_X) == PRESS || InputCtrl::GetButtonState(XINPUT_BUTTON_X) == PRESS) {
			if (mode == GameSceneMode::weaponLevelup) mode = GameSceneMode::main;
			else
			{
				mode = GameSceneMode::weaponLevelup;
				//レベルアップ画面のカーソル位置初期化用フラグ
				// レベルアップ画面を開くたびに初期化
				restor_cursor_position = true;
			}
		};
	};

	// 鍛冶ステージテスト用
	if (InputCtrl::GetKeyState(KEY_INPUT_B) == PRESS) {
		if (mode == GameSceneMode::blacksmith) mode = GameSceneMode::main;
		else mode = GameSceneMode::blacksmith;
	};

	// 強制ゲームオーバー
	if (InputCtrl::GetButtonState(XINPUT_BUTTON_Y) == PRESS) return new GameOverScene;

	//////////////////////////////////////////////////

	if (mode == GameSceneMode::main) {
		gameUI->update(this);

		if (gameUI->getState() >= banner_playerUI) {

			//敵
			HitCheck();
			SlimeUpdate();
			SkeletonUpdate();
			WizardUpdate();
			MinotaurUpdate();


			//武器と敵の当たり判定
			if (true/*currentFloor == 1*/) {
				for (int i = 0; i < enemySpawnData["slime"]; i++) {
					if (slime[i] != nullptr) {
						if (weaponA->WeaponCollision(slime[i]->GetEnemyLocation(), slime[i]->GetEnemyRadius())) {
							if (slime[i]->GetHitFrameCnt() == 0) {
								slime[i]->SetHitWeaponFlg();
								//ダメージアップ
								slime[i]->SetHitHP(weaponA->GetDamage() * weaponB->GetAttackBufRate());
								slime[i]->SetHit1stFrameFlg(true);
								if (weaponA->GetIsAttacking() && !swordHitFlg) {
									swordHitFlg = true;
									weaponA->SetHitCnt(true);
									weaponA->SwordLevel8(player);
									weaponA->SwordLevel8Heel(player);
								}
							}
						}
						if (weaponB->WeaponCollision(slime[i]->GetEnemyLocation(), slime[i]->GetEnemyRadius())) {
							if (slime[i]->GetHitFrameCnt() == 0) {
								slime[i]->SetHitWeaponFlg();
								slime[i]->SetHitHP(weaponB->GetDamage() * weaponB->GetAttackBufRate());
								slime[i]->SetHit1stFrameFlg(true);
								
								if (weaponB->GetWeaponType() == spear && weaponB->GetWeaponLevel() == 8) {
									weaponB->SetThunderLocation(slime[i]->GetEnemyLocation());
									if (weaponB->SpearThunderCollision(slime[i]->GetEnemyLocation(), slime[i]->GetEnemyRadius())) {
										slime[i]->SetHitHP(weaponB->GetThunderDamage());
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < enemySpawnData["skeleton"]; i++) {
					if (skeleton[i] != nullptr) {
						if (weaponA->WeaponCollision(skeleton[i]->GetEnemyLocation(), skeleton[i]->GetEnemyRadius())) {
							if (skeleton[i]->GetHitFrameCnt() == 0) {
								skeleton[i]->SetHitWeaponFlg();
								skeleton[i]->SetHitHP(weaponA->GetDamage() * weaponB->GetAttackBufRate());
								skeleton[i]->SetHit1stFrameFlg(true);
								if (weaponA->GetIsAttacking() && !swordHitFlg) {
									swordHitFlg = true;
									weaponA->SetHitCnt(true);
									weaponA->SwordLevel8(player);
									weaponA->SwordLevel8Heel(player);
								}
							}
						}
						if (weaponB->WeaponCollision(skeleton[i]->GetEnemyLocation(), skeleton[i]->GetEnemyRadius())) {
							if (skeleton[i]->GetHitFrameCnt() == 0) {
								skeleton[i]->SetHitWeaponFlg();
								skeleton[i]->SetHitHP(weaponB->GetDamage() * weaponB->GetAttackBufRate());
								skeleton[i]->SetHit1stFrameFlg(true);

								if (weaponB->GetWeaponType() == spear && weaponB->GetWeaponLevel() == 8) {
									weaponB->SetThunderLocation(skeleton[i]->GetEnemyLocation());
									if (weaponB->SpearThunderCollision(skeleton[i]->GetEnemyLocation(), skeleton[i]->GetEnemyRadius())) {
										skeleton[i]->SetHitHP(weaponB->GetThunderDamage());
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < enemySpawnData["wizard"]; i++) {
					if (wizard[i] != nullptr) {
						if (weaponA->WeaponCollision(wizard[i]->GetEnemyLocation(), wizard[i]->GetEnemyRadius())) {
							if (wizard[i]->GetHitFrameCnt() == 0) {
								wizard[i]->SetHitWeaponFlg();
								//ダメージアップ
								wizard[i]->SetHitHP(weaponA->GetDamage() * weaponB->GetAttackBufRate());
								wizard[i]->SetHit1stFrameFlg(true);
								if (weaponA->GetIsAttacking() && !swordHitFlg) {
									swordHitFlg = true;
									weaponA->SetHitCnt(true);
									weaponA->SwordLevel8(player);
								}
							}
						}
						if (weaponB->WeaponCollision(wizard[i]->GetEnemyLocation(), wizard[i]->GetEnemyRadius())) {
							if (wizard[i]->GetHitFrameCnt() == 0) {
								wizard[i]->SetHitWeaponFlg();
								wizard[i]->SetHitHP(weaponB->GetDamage() * weaponB->GetAttackBufRate());
								wizard[i]->SetHit1stFrameFlg(true);

								if (weaponB->GetWeaponType() == spear && weaponB->GetWeaponLevel() == 8) {
									weaponB->SetThunderLocation(wizard[i]->GetEnemyLocation());
									if (weaponB->SpearThunderCollision(wizard[i]->GetEnemyLocation(), wizard[i]->GetEnemyRadius())) {
										wizard[i]->SetHitHP(weaponB->GetThunderDamage());
									}
								}
							}
						}
					}
				}
			}

			if (!weaponA->GetIsAttacking() && weaponA->GetOldIsAttacking()) {
				if (!swordHitFlg) {
					weaponA->SetHitCnt(false);
				}
				swordHitFlg = false;
			}

			//バリア
			if (weaponB->GetWeaponType() == book && weaponB->GetWeaponLevel() == 7 && weaponB->GetCoolTime() == 0) {
				weaponA->SetCoolTime(0.1f, true);
				weaponB->SetBarrierFlg(true);
				bookFlg = true;
			}
			else if (weaponB->GetCoolTime() < INIT_COOLTIME_BOOK_LEVEL7 * 0.5f && bookFlg == true) {
				weaponA->SetCoolTime(1.0f, false);
				weaponB->SetBarrierFlg(false);
				bookFlg = false;
			}

			//武器のレベルアップ（デバッグ用）
			if (!weaponA->GetLevelUpFlg()) {
				if (InputCtrl::GetKeyState(KEY_INPUT_1) == PRESS) {
					weaponA->SetWeaponType(sword);
				}
				if (InputCtrl::GetKeyState(KEY_INPUT_2) == PRESS) {
					weaponA->SetWeaponType(dagger);
				}
				if (InputCtrl::GetKeyState(KEY_INPUT_3) == PRESS) {
					weaponA->SetWeaponType(greatSword);
				}
			}

			if (!weaponB->GetLevelUpFlg()) {
				if (InputCtrl::GetKeyState(KEY_INPUT_4) == PRESS) {
					weaponB->SetWeaponType(spear);
				}
				if (InputCtrl::GetKeyState(KEY_INPUT_5) == PRESS) {
					weaponB->SetWeaponType(frail);
				}
				if (InputCtrl::GetKeyState(KEY_INPUT_6) == PRESS) {
					weaponB->SetWeaponType(book);
				}
			}

			////////////
			player->SetLeftTop(stage->GetStageArray(0));
			player->SetRightBottom(stage->GetStageArray(8));
			stage->update(player->Player_MoveX(), player->Player_MoveY());
			player->update();
			weaponA->Update(player->Player_AimingX(), player->Player_AimingY(), player->Player_Location(), player);
			Vector tmpV = { player->Player_MoveX(),player->Player_MoveY(),0 };
			weaponB->Update(player->Player_AimingX(), player->Player_AimingY(), player->Player_Location(), tmpV, player);

			EnemyInc();//敵のダメージストップ関係

			hp = player->GetPlayer_HP();
			int maxHP = 100;

			int maxEXP = expData[level];

			//////////////////////////////////////////////////
			// GameUI 仮
			gameUI->setHP(hp, maxHP, ((float)hp / (float)maxHP) * 100);
			gameUI->setEXP(exp, maxEXP, ((float)exp / (float)maxEXP) * 100);
			gameUI->setPoint(point);

			gameUI->setFloor(currentFloor + 1);
			gameUI->setEnemy(getEnemyNum(0), getEnemyMax(0));

			gameUI->setWeapon({ weaponA->GetWeaponType(), weaponA->GetWeaponLevel(), false }, { weaponB->GetWeaponType(), weaponB->GetWeaponLevel(), false });
			//////////////////////////////////////////////////
			if (getEnemyNum(0) <= 0 && frameCounter) {
				gameUI->setBanner("クリア！", "全てのモンスターを倒しました");
				if (gameUI->getState() == playerUI) {
					gameUI->init();
					gameUI->setState(banner);
				};
				if (gameUI->getState() == banner_playerUI) {
					map->ClearStage();

					currentFloor++;
					init();
					
					mode = GameSceneMode::map;
				};
			};
			if (player->GetPlayer_HP() <= 0) {
				gameUI->setBanner("失敗、、", "体力が尽きました、、");
				if (gameUI->getState() == playerUI) {
					gameUI->init();
					gameUI->setState(banner);
				};
				if (gameUI->getState() == banner_playerUI) return new GameOverScene;
			};
			//////////////////////////////////////////////////
			if (battleMode == GameSceneBattleMode::boss) gameUI->setEnemyHP("魔王 猫スライム", getEnemyNum(0), getEnemyMax(0), ((float)getEnemyNum(0) / (float)getEnemyMax(0)) * 100); // 怪奇現象発生中
			//printfDx("%d\n", static_cast<int>((SLIME_1_STAGE_NUM / c) * 100.0f));
			//printfDx("%f\n", (c / SLIME_1_STAGE_NUM) * 100.0f);
			if (InputCtrl::GetKeyState(KEY_INPUT_V) == PRESSED) battleMode = GameSceneBattleMode::boss;
			//////////////////////////////////////////////////

			// 経験値、レベル、ポイント処理
			if (exp >= expData[level]) {
				exp = 0;
				level++;
				point++;

				gameUI->notification("武器強化可能！", "Xボタンで確認", "btnX");
			};

			if (battleMode == GameSceneBattleMode::midBoss) bossState = 1; // 中ボス討伐状態
		};

		frameCounter++;
	};

	if (mode == GameSceneMode::weaponSelect) {
		weaponSelect->update(weaponA, weaponB, weapon_selected, mode);
		return this;
	};

	if (mode == GameSceneMode::weaponLevelup) {
		weaponLevelup->update(weaponA, weaponB, player, restor_cursor_position, point);
		return this;
	};

	//////////////////////////////////////////////////

	if (mode == GameSceneMode::map) {
		map->update(mode, weapon_selected);
		return this;
	};

	if (mode == GameSceneMode::blacksmith) {
		blacksmith->update(weaponA, weaponB, weaponLevelup, player, point);
		weaponLevelup->SetIsBlacksmith(false);
		return this;
	};

	if (mode == GameSceneMode::rest) {
		rest->update(player, mode, currentFloor);
		return this;
	};

	return this;
};

void GameScene::draw() const {
	if (mode >= GameSceneMode::main) {
		stage->draw();

		player->draw();

		weaponA->Draw();
		weaponB->Draw();

		// 敵
		SlimeDraw();
		SkeletonDraw();
		WizardDraw();
		EnemyBulletDraw();
		MinotaurDraw();

		//////////////////////////////////////////////////

		if (mode == GameSceneMode::weaponSelect) {
			weaponSelect->draw(weapon_selected);
		}
		else {
			gameUI->draw();
			if (battleMode == GameSceneBattleMode::boss) gameUI->drawEnemyHP(); // ボスの体力ゲージ
		};

		if (mode == GameSceneMode::weaponLevelup) weaponLevelup->draw();
	};

	//////////////////////////////////////////////////

	if (mode == GameSceneMode::map) map->draw();

	if (mode == GameSceneMode::blacksmith) blacksmith->draw(weaponLevelup);

	if (mode == GameSceneMode::rest)rest->draw();

	//////////////////////////////////////////////////

	if (state == pause) gameUI->drawPause();

	//////////////////////////////////////////////////

	SetFontSize(16);
	if (mode == GameSceneMode::main) DrawFormatString(0, 80, 0xffffff, "キーボードBで鍛冶画面\nキーボードVでボス戦闘モード");
};

void GameScene::init() {
	//delete player;
	//player = new Player();

	player->SetPlayerHP(hp);

	delete stage;
	stage = new Stage();

	weaponA->InitWeapon();
	weaponB->InitWeapon(1);
	
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		slime[i] = nullptr;
	};
	tmpSlimeNum = 0;
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		skeleton[i] = nullptr;
	};
	tmpSkeletonNum = 0;
	for (int i = 0; i < MAX_WIZARD_NUM; i++) {
		wizard[i] = nullptr;
	};
	tmpWizardNum = 0;
	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		enemyBullet[i] = nullptr;
	};
	tmpBulletNum = 0;

	     if (battleMode == GameSceneBattleMode::normal) gameUI->setBanner(std::to_string(currentFloor + 1) + "F - XXXの部屋", "全てのモンスターを倒してください");
	else if (battleMode == GameSceneBattleMode::boss)   gameUI->setBanner("最上階 - ラスボス", "全てのモンスターを倒してください");
	gameUI->init();
	gameUI->setState(banner);

	enemySpawnData = shimabukuro[currentFloor];

	exp = 0;
};

int GameScene::getEnemyMax(int type) {
	if (type == 0) return (enemySpawnData["slime"] + enemySpawnData["skeleton"] + enemySpawnData["wizard"]);
	if (type == 1) return enemySpawnData["slime"];
	if (type == 2) return enemySpawnData["skeleton"];
	if (type == 3) return enemySpawnData["wizard"];
};

int GameScene::getEnemyNum(int type) {
	int slimeNum    = 0;
	int skeletonNum = 0;
	int wizardNum   = 0;

	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) slimeNum++;
	};
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) skeletonNum++;
	};
	for (int i = 0; i < MAX_WIZARD_NUM; i++) {
		if (wizard[i] != nullptr) wizardNum++;
	};

	if (type == 0) return (slimeNum + skeletonNum + wizardNum);
	if (type == 1) return slimeNum;
	if (type == 2) return skeletonNum;
	if (type == 3) return wizardNum;
};


//////////////////////////////////////////////////
// 当たり判定・敵の処理
//////////////////////////////////////////////////

void GameScene::HitCheck()
{
	//スライムの当たり判定
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			HitEnemy(slime[i]);//プレイヤーとの当たり判定
			for (int j = 0; j < MAX_SLIME_NUM; j++) {
				if (slime[j] != nullptr && i != j) {
					if (slime[i]->CheckCollision(static_cast<SphereCollider>(*slime[j]), player) == HIT) {//当たっている
						slime[i]->SetHitFlg(HIT);
						slime[j]->SetHitFlg(HIT);

						slime[i]->HitVectorCale(static_cast<SphereCollider>(*slime[j]), player);
						slime[j]->HitVectorCale(static_cast<SphereCollider>(*slime[i]), player);
					}
				}
			}
		}
	}

	//スケルトンの当たり判定
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) {
			HitEnemy(skeleton[i]);//プレイヤーとの当たり判定
			for (int j = 0; j < MAX_SKELETON_NUM; j++) {
				if (skeleton[j] != nullptr && i != j) {
					if (skeleton[i]->CheckCollision(static_cast<SphereCollider>(*skeleton[j]), player) == HIT) {
						skeleton[i]->SetHitFlg(HIT);
						skeleton[j]->SetHitFlg(HIT);

						skeleton[i]->HitVectorCale(static_cast<SphereCollider>(*skeleton[j]), player);
						skeleton[j]->HitVectorCale(static_cast<SphereCollider>(*skeleton[i]), player);
					}
				}
			}
		}
	}

	//魔法使いの当たり判定
	for (int i = 0; i < MAX_WIZARD_NUM; i++) {
		if (wizard[i] != nullptr) {
			HitEnemy(wizard[i]);//プレイヤーとの当たり判定
			for (int j = 0; j < MAX_WIZARD_NUM; j++) {
				if (wizard[j] != nullptr && i != j) {
					if (wizard[i]->CheckCollision(static_cast<SphereCollider>(*wizard[j]), player) == HIT) {
						wizard[i]->SetHitFlg(HIT);
						wizard[j]->SetHitFlg(HIT);

						wizard[i]->HitVectorCale(static_cast<SphereCollider>(*wizard[j]), player);
						wizard[j]->HitVectorCale(static_cast<SphereCollider>(*wizard[i]), player);
					}
				}
			}
		}
	}

	//スライムとスケルトンの当たり判定
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			for (int j = 0; j < MAX_SKELETON_NUM; j++) {
				if (skeleton[j] != nullptr) {
					if (slime[i]->CheckCollision(static_cast<SphereCollider>(*skeleton[j]), player) == HIT) {
						slime[i]->SetHitFlg(HIT);
						skeleton[j]->SetHitFlg(HIT);

						slime[i]->HitVectorCale(static_cast<SphereCollider>(*skeleton[j]), player);
						skeleton[j]->HitVectorCale(static_cast<SphereCollider>(*slime[i]), player);
					}
				}
			}
		}
	}

	//スライムと魔法使いの当たり判定
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			for (int j = 0; j < MAX_WIZARD_NUM; j++) {
				if (wizard[j] != nullptr) {
					if (slime[i]->CheckCollision(static_cast<SphereCollider>(*wizard[j]), player) == HIT) {
						slime[i]->SetHitFlg(HIT);
						wizard[j]->SetHitFlg(HIT);

						slime[i]->HitVectorCale(static_cast<SphereCollider>(*wizard[j]), player);
						wizard[j]->HitVectorCale(static_cast<SphereCollider>(*slime[i]), player);
					}
				}
			}
		}
	}

	//魔法使いとスケルトンの当たり判定
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) {
			for (int j = 0; j < MAX_WIZARD_NUM; j++) {
				if (wizard[j] != nullptr) {
					if (skeleton[i]->CheckCollision(static_cast<SphereCollider>(*wizard[j]), player) == HIT) {
						skeleton[i]->SetHitFlg(HIT);
						wizard[j]->SetHitFlg(HIT);

						skeleton[i]->HitVectorCale(static_cast<SphereCollider>(*wizard[j]), player);
						wizard[j]->HitVectorCale(static_cast<SphereCollider>(*skeleton[i]), player);
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		if (enemyBullet[i] != nullptr) {
			if (HitEnemy(enemyBullet[i]) == true) {
				enemyBullet[i] = nullptr;
				tmpBulletNum--;
			}
		}
	}

	//ミノタウロスとプレイヤーの当たり判定
	if (minotaur != nullptr) {
		HitEnemy(minotaur);
	}
}

bool GameScene::HitEnemy(EnemyBase* enemy)
{
	if (player->GetIsHit() != true && player->GetPlayer_Avoidance() != true)
	{
		if (player->CheckCollision(*(enemy), player) == HIT)
		{
			//バリア
			if (!weaponB->GetBarrierFlg()) {
				player->SetPlayer_HP(enemy->GetDamage());
				player->SetIsHit(true);
			}
			return true;
		}
	}
	return false;
}

//----------敵----------//
void GameScene::EnemyInc()
{
	//スライム
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			if (slime[i]->GetHitFrameCnt() >= DAMAGE_STOP_FRAME) {
				slime[i]->SetHit1stFrameFlg(false);
				slime[i]->SetHitFrameCnt(0);
			}
		}
	}

	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			if (slime[i]->GetHit1stFrameFlg() == true) {
				slime[i]->hitFrameCntInc();
			}
		}
	}

	//スケルトン
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) {
			if (skeleton[i]->GetHitFrameCnt() >= DAMAGE_STOP_FRAME) {
				skeleton[i]->SetHit1stFrameFlg(false);
				skeleton[i]->SetHitFrameCnt(0);
			}
		}
	}

	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) {
			if (skeleton[i]->GetHit1stFrameFlg() == true) {
				skeleton[i]->hitFrameCntInc();
			}
		}
	}

	//魔法使い
	for (int i = 0; i < MAX_WIZARD_NUM; i++) {
		if (wizard[i] != nullptr) {
			if (wizard[i]->GetHitFrameCnt() >= DAMAGE_STOP_FRAME) {
				wizard[i]->SetHit1stFrameFlg(false);
				wizard[i]->SetHitFrameCnt(0);
			}
		}
	}

	for (int i = 0; i < MAX_WIZARD_NUM; i++) {
		if (wizard[i] != nullptr) {
			if (wizard[i]->GetHit1stFrameFlg() == true) {
				wizard[i]->hitFrameCntInc();
			}
		}
	}
}

//----------スライム----------//
void GameScene::SlimeUpdate()
{
	if (tmpSlimeNum < enemySpawnData["slime"]) {
		slime[tmpSlimeNum] = new Slime(player,tmpSlimeNum, enemySpawnData["slime"]);
		tmpSlimeNum++;
	}
	for (int i = 0; i < enemySpawnData["slime"]; i++) {
		if (slime[i] != nullptr) {
			slime[i]->Update(i, player, weaponA, *(stage));
			if (slime[i]->GetHP() <= 0) {
				slime[i] = nullptr;
				//tmpSlimeNum--;
				exp += 20;
			}
		}
	}
}

void GameScene::SlimeDraw() const
{
	for (int i = 0; i < MAX_SLIME_NUM; i++) {
		if (slime[i] != nullptr) {
			slime[i]->Draw(i);
		}
	}
}

//----------スケルトン----------//
void GameScene::SkeletonUpdate()
{
	if (tmpSkeletonNum < enemySpawnData["skeleton"]) {
		skeleton[tmpSkeletonNum] = new Skeleton(player,tmpSkeletonNum, enemySpawnData["skeleton"]);
		tmpSkeletonNum++;
	}
	for (int i = 0; i < enemySpawnData["skeleton"]; i++) {
		if (skeleton[i] != nullptr) {
			skeleton[i]->Update(i, player, weaponA, *(stage));
			if (skeleton[i]->GetHP() <= 0) {
				skeleton[i] = nullptr;
				//tmpSkeletonNum--;
				exp += 30;
			}
		}
	}
}

void GameScene::SkeletonDraw() const
{
	for (int i = 0; i < MAX_SKELETON_NUM; i++) {
		if (skeleton[i] != nullptr) {
			skeleton[i]->Draw(i);
		}
	}
}

//----------魔法使い----------//
void GameScene::WizardUpdate()
{
	for (int i = 0; i < enemySpawnData["wizard"]; i++) {
		if (wizard[i] != nullptr) {
			wizard[i]->Update(i, player, weaponA, *(stage));

				if (wizard[i]->GetShootFlg() == true) {
					if (wizard[i]->GetCreateBulletFlg() == true) {//弾の生成処理
						if (tmpBulletNum < MAX_BULLET_NUM) {
							enemyBullet[tmpBulletNum] = new EnemyBullet(wizard[i]->GetEnemyLocation() , player);
						}
						tmpBulletNum++;
					}
					EnemyBulletUpdate(wizard[i]);
				}

			if (wizard[i]->GetHP() <= 0) {
				wizard[i] = nullptr;
				//tmpWizardNum--;
				exp += 40;
			}
		}
	}
	if (tmpWizardNum < enemySpawnData["wizard"]) {
		wizard[tmpWizardNum] = new Wizard(player,tmpWizardNum, enemySpawnData["wizard"]);
		tmpWizardNum++;
	}
}

void GameScene::WizardDraw() const
{
	int maxWizard = 0;
	if (enemySpawnData.find("wizard") != enemySpawnData.end()) maxWizard = enemySpawnData.at("wizard");

	for (int i = 0; i < maxWizard; i++) {
		if (wizard[i] != nullptr) {
			wizard[i]->Draw(i);
		}
	}
}

//----------弾----------//
void GameScene::EnemyBulletUpdate(Wizard* enemy)
{
	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		if (enemyBullet[i] != nullptr) {
			enemyBullet[i]->Update(player,enemy);
			if (enemyBullet[i]->GetlifeTimeCnt() <= 0) {
				enemyBullet[i] = nullptr;
				tmpBulletNum--;
			}
		}
	}
}

void GameScene::EnemyBulletDraw() const
{
	for (int i = 0; i < MAX_BULLET_NUM; i++) {
		if (enemyBullet[i] != nullptr) {
			enemyBullet[i]->Draw();
		}
	}
}

void GameScene::MinotaurUpdate()
{
	minotaur->Update(player);
}

void GameScene::MinotaurDraw() const
{
	minotaur->Draw();
}
