#pragma once

#include "scene.h"

class Credit : public Scene
{
private:
	int CreditImage;
	int g_WaitTime;
	int g_PosY;

public:
	Credit();
	~Credit();
	Scene* update()override;
	void draw()const override;
};

