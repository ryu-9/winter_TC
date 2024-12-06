#include "ModeTitle.h"
#include "ApplicationMain.h"
#include "ModeGame.h"

bool ModeTitle::Initialize()
{
	ChangeFont("x14y20pxScoreDozer");
	SetFontSize(48);
	return false;
}

bool ModeTitle::Terminate()
{
	return false;
}

bool ModeTitle::Process()
{
	if (ApplicationMain::GetInstance()->GetTrg() & PAD_INPUT_1)
	{
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeTitle::Render()
{
	base::Render();

	auto dw = GetDrawStringWidth("Title",-1);
	DrawFormatString((1280 - dw) / 2, 720 / 2 - 24, GetColor(255, 255, 255), "Title");
	return false;
}
