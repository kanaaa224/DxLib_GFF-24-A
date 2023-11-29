#include "main.h"

Credit::Credit()
{
	//クレジット画像の読込
	CreditImage = LoadGraph("resources/images/background.png");
	//時間の初期化
	g_WaitTime = 0;
}

Credit::~Credit()
{

}

Scene* Credit::update()
{
	//クレジットロール処理
	//時間がたてばタイトルへ遷移
	if (++g_WaitTime < 1800)g_PosY = 600 - g_WaitTime;
	if (++g_WaitTime > 1200) {
		return new Title;
	}
	return this;
}

void Credit::draw()const
{
	//クレジット画像の表示
	DrawExtendGraph(0, 0,1280,720, CreditImage, TRUE);
	//使ったサイトや関係者名の名前を表示
	SetFontSize(50);
	DrawString(550, 350+g_PosY, "pngtree", 0x000000);
	DrawString(300, 410+g_PosY, "グラフィックデザイン科の方々", 0x000000);
	DrawString(550, 470+g_PosY, "チコデザ", 0x000000);
	DrawString(550, 530+g_PosY, "ツカッテ", 0x000000);
	DrawString(560, 590+g_PosY, "BOOTH", 0x000000);
	DrawString(400, 650+g_PosY, "インディー8ビット", 0x000000);
	DrawString(560, 710+g_PosY, "AKKODiS", 0x000000);
	DrawString(560, 770+g_PosY, "romptnAI", 0x000000);
	DrawString(570, 830+g_PosY, "ITOTM", 0x000000);
	DrawString(500, 890+g_PosY, "くろくまそふと", 0x000000);
	DrawString(565, 950+g_PosY, "note_lilish", 0x000000);
	DrawString(455, 1010+g_PosY, "A.I. Art Stock", 0x000000);
	DrawString(550, 1070+g_PosY, "ドッタウン", 0x000000);
	DrawString(350, 1130+g_PosY, "ピクセルアートタウン", 0x000000);
}
