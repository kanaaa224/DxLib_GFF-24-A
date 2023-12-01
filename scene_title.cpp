#include "main.h"

//コンストラクタ
Title::Title()
{
	TitleImage = LoadGraph("resources/images/Title.png");
	Titlelogo = LoadGraph("resources/images/ロゴ/戦塔ロゴ.png");
	Titlelogo_white = LoadGraph("resources/images/ロゴ/戦塔ロゴ-白.png");
	g_MenuNumber = 0;
	TitleInterval = 0;
	cursor = LoadGraph("resources/images/cursor.png");
}

Title::~Title()
{
	DeleteGraph(TitleImage);
}

//更新
Scene*Title::update()
{
	//スティックの制御
	if (TitleInterval < TITLEINTERVAL)
	{
		TitleInterval++;
	}

	if (InputCtrl::GetButtonState(XINPUT_BUTTON_DPAD_UP) == PRESS || InputCtrl::GetStickRatio(L).y > 0.8 && TitleInterval >= 15 || InputCtrl::GetKeyState(KEY_INPUT_UP) == PRESS)
	{
		//スティック移動の初期化
		TitleInterval = 0;
		//タイトルカーソルの移動
		if (--g_MenuNumber > TITLECURSOR);
		//タイトルカーソルの移動量の制御
		if (g_MenuNumber < 0) g_MenuNumber = 3;
	}
	if (InputCtrl::GetButtonState(XINPUT_BUTTON_DPAD_DOWN) == PRESS || InputCtrl::GetStickRatio(L).y < -0.8 && TitleInterval >= 15)
	{
		//スティック移動の初期化
		TitleInterval = 0;
		//タイトルカーソルの移動
		if (++g_MenuNumber < -TITLECURSOR);
		//タイトルカーソルの移動量の制御
		if (g_MenuNumber > 3)g_MenuNumber = 0;
	}
	g_MenuY = g_MenuNumber * 52;
	
	//Aボタンでメニュー決定・画面遷移
	if (InputCtrl::GetButtonState(XINPUT_BUTTON_A) == PRESS)
	{
		if (g_MenuNumber == 0) {
			return new GameScene;
		}
		if (g_MenuNumber == 1) {
			return new Help;
		}
		if (g_MenuNumber == 2) {
			return new Credit;
		}
		if (g_MenuNumber == 3) {
			return new End;
		}
	}
	
	return this;
}

//描画
void Title::draw() const
{
	//タイトル背景の描画
	DrawGraph(0, 0, TitleImage, TRUE);
	//タイトルの表示
	/*SetFontSize(100);
	DrawString(520, 200, "戦塔", 0x000000);*/
	DrawRotaGraph(650, 150, 1.0f, 0.0f, Titlelogo, TRUE);
	DrawRotaGraph(640, 140, 1.0f, 0.0f, Titlelogo_white, TRUE);
	//タイトルメニューの表示
	SetFontSize(55);
	DrawString(550, 320, "Start", 0x000000);
	DrawString(550, 370, "Help", 0x000000);
	DrawString(550, 420, "Credit", 0x000000);
	DrawString(550, 470, "End", 0x000000);

	//タイトルカーソルの描画
	DrawGraph(470, 290 + g_MenuY, cursor, TRUE);

}