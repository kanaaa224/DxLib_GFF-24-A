#pragma once
#include "sourceFiles/scene.h"

class Title :public Scene
{
private:
	//画像の読込変数
	int TitleImage;
	int EnemyImage0;
	int EnemyImage1;
	int EnemyImage2;
	int EnemyImage3;
	int EnemyImage4;

	int g_MenuNumber;
	int g_Menu;

public:
	Title();
	~Title();
	Scene* update() override;
	void draw() const override;

};