#include "ModeTitleMenu.h"
#include "ApplicationMain.h"
#include "ModeGame.h"

bool ModeTitleMenu::Initialize()
{
	// 後メニュー式に変えたい
	_Text.emplace_back("はじめから");
	_Text.emplace_back("つづきから");
	_Text.emplace_back("ゲーム終了");
	
	return false;
}

bool ModeTitleMenu::Terminate()
{
	return false;
}

bool ModeTitleMenu::Process()
{
	auto trg = ApplicationMain::GetInstance()->GetTrg();

	if (trg & PAD_INPUT_UP) { _Cur--; }
	if (trg & PAD_INPUT_DOWN) {_Cur++;	}
	if (_Cur < 0) {	_Cur = 0; }
	if (_Cur >= _Text.size()) { _Cur = _Text.size() - 1; }
	if (trg & PAD_INPUT_1)
	{
		switch (_Cur)
		{
		case 0:
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
			break;
		case 1:
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
			break;
		case 2:
			ApplicationMain::GetInstance()->Terminate();
			break;
		}
	}
	return false;
}

bool ModeTitleMenu::Render()
{
	for (auto i = 0; i < _Text.size(); i++)
	{
		DrawFormatString(100, 100 + i * 20, GetColor(255, 255, 255), "%s", _Text[i].c_str());
	}
	DrawFormatString(80, 100 + _Cur * 20, GetColor(255, 255, 255), ">");
	return false;
}
