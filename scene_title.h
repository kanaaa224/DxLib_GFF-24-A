#pragma once

#include "scene.h"

class Title :public Scene
{
private:

	//画像の読込変数
	int TitleImage;

	int g_MenuNumber;
	short PadStick;
	int cursor;
	int frameCounter;

	bool Ctrl;

public:
	Title();
	Scene* update() override;
	void draw() const override;

};