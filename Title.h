#pragma once
#include "sourceFiles/scene.h"

class Title :public Scene
{
private:

	//画像の読込変数
	int TitleImage;

	int g_MenuNumber;
	int interval;
	short PadStick;
	int cursor;
	int state;

	float interval;

	bool Ctrl;

public:
	Title();
	Scene* update() override;
	void draw() const override;

};