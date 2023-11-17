#pragma once

#include "scene.h"
#define TITLEINTERVAL 15
#define TITLECURSOR 4

class Title :public Scene
{
private:

	//画像の読込変数
	int TitleImage;

	int g_MenuNumber;
	int cursor;
	int g_MenuY;
	int TitleInterval;

public:
	Title();
	Scene* update() override;
	void draw() const override;

};