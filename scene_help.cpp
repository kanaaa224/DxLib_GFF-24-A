#include "main.h"

Help::Help()
{
	//ヘルプ画像の読み込み
	if (HelpImage1 = LoadGraph("resources/images/Help/Help_Img3.png")) {}
	if (HelpImage2 = LoadGraph("resources/images/Help/Help_Stage_Img_1.png")) {}
	if (HelpImage3 = LoadGraph("resources/images/Help/Help_weapon_system_1.png")) {}

	Help_Abtn_Count = 0;
}

Help::~Help()
{
	DeleteGraph(HelpImage1);
	DeleteGraph(HelpImage2);
	DeleteGraph(HelpImage3);
}

Scene* Help::update()
{
	//ヘルプ2画面へ遷移
	if (InputCtrl::GetButtonState(XINPUT_BUTTON_A) == PRESS) {

		SoundManager::PlaySoundSE("se_system_normal_decision", false);
		Help_Abtn_Count++;
	}

	if (Help_Abtn_Count > 2 && Help_Abtn_Count < 4) {

		SoundManager::StopSoundBGMs();
		return new GameScene;
	}
	else if(Help_Abtn_Count > 1 && Help_Abtn_Count < 3 && InputCtrl::GetButtonState(XINPUT_BUTTON_B) == PRESS) {

		SoundManager::StopSoundBGMs();
		return new Title;
	}
	
	return this;
}
 
void Help::draw()const
{
	if (Help_Abtn_Count == 0) {

		DrawGraph(0, 0, HelpImage1, TRUE);
	}
	else if(Help_Abtn_Count > 0 && Help_Abtn_Count < 2){//1

		//ヘルプ画像の表示
		DrawGraph(0, 0, HelpImage2, TRUE);
	}
	else if (Help_Abtn_Count > 1 && Help_Abtn_Count < 3) {//2


		//ヘルプ画像の表示
		DrawGraph(0, 0, HelpImage3, TRUE);
	}
}
