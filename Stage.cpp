#include "main.h"
#include "Stage.h"
#include "Player.h"

Stage::Stage() {

	if (StageImg1 = LoadGraph("resources/images/lkvhg.png")) {}

	for (int i = 0; i < 9; i++) {

		StageArrayImg[i].img = LoadGraph("resources/images/lkvhg.png");


		switch (i)
		{
		case 0:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 1:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 2:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 3:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 4:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 5:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 6:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 7:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 8:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		default:
			break;
		}
	}



	/*StageX = 640.0;
	StageY = 369.5;*/

	Xspeed = 2.0;
	Yspeed = 2.0;
}

Stage::~Stage() {

	DeleteGraph(StageImg1);
}

void Stage::update(float moveX, float moveY) {

	for (int i = 0; i < 9; i++) {
		StageArrayImg[i].x -= moveX;
		StageArrayImg[i].y -= moveY;
	}

	/*for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			break;
		case 1:
			StageArrayImg[i].x += STAGEIMG_X;
			break;
		case 2:
			StageArrayImg[i].y += STAGEIMG_Y;

			break;
		case 3:
			StageArrayImg[i].x += STAGEIMG_X;
			StageArrayImg[i].y += STAGEIMG_Y;
			break;
		default:
			break;
		}
	}*/

}

void Stage::draw()const {

	////DrawGraph(StageX, StageY, StageImg1, TRUE);

	////左上
	//DrawGraph(StageArrayImg[0].x /*+ STAGEIMG_X*/, StageArrayImg[0].y /*+ STAGEIMG_Y*/, StageArrayImg[0].img, TRUE);
	////DrawRotaGraph(StageArrayImg[0].x, StageArrayImg[0].y, 0.1f, 1, StageArrayImg[0].img, TRUE);

	////右上
	//DrawGraph(StageArrayImg[1].x, StageArrayImg[1].y /*+ STAGEIMG_Y*/, StageArrayImg[1].img, TRUE);

	////左下
	//DrawGraph(StageArrayImg[2].x /*+ STAGEIMG_X*/, StageArrayImg[2].y, StageArrayImg[2].img, TRUE);

	////右下
	//DrawGraph(StageArrayImg[3].x, StageArrayImg[3].y, StageArrayImg[3].img, TRUE);

	//DrawRotaGraph(StageX, StageY, 1.0f, 1, StageImg, TRUE);

	for (int i = 0; i < 9; i++){
		DrawGraph(StageArrayImg[i].x, StageArrayImg[i].y, StageArrayImg[i].img, TRUE);
		//DrawRotaGraph(StageArrayImg[i].x, StageArrayImg[i].y, 0.4, 0.0, StageArrayImg[i].img, TRUE);
	}
}

void Stage::init()
{
	for (int i = 0; i < 9; i++) {
		switch (i)
		{
		case 0:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 1:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 2:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = -STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 3:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 4:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 5:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y / 2;
			break;
		case 6:
			StageArrayImg[i].x = -STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 7:
			StageArrayImg[i].x = STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		case 8:
			StageArrayImg[i].x = STAGEIMG_X + STAGEIMG_X / 2;
			StageArrayImg[i].y = STAGEIMG_Y + STAGEIMG_Y / 2;
			break;
		default:
			break;
		}
	}
}
