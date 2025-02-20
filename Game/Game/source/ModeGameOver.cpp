#include "ModeGameOver.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeTitle.h"

bool ModeGameOver::Initialize() {
	new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_GAMEOVER_BACK.png");
	new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_GAMEOVER_TEXT.png");
	auto u = new UIChipClass(this, VGet(960, 700, 0), "res/UI/UI_GAMEOVER_CONTINUE.png");
	u->AddImage("res/UI/UI_GAMEOVER_CONTINUE_SELECT.png");
	_Button.push_back(u);
	u->ChangeImage(1);
	u = new UIChipClass(this, VGet(960, 900, 0), "res/UI/UI_GAMEOVER_END.png");
	u->AddImage("res/UI/UI_GAMEOVER_END_SELECT.png");
	_Button.push_back(u);
	_Cur = 0;

	return false;
}

bool ModeGameOver::Terminate() {
	return false;
}

bool ModeGameOver::Process() {
	base::Process();
	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	int cur = 0;
	if (trg & PAD_INPUT_DOWN) {
		cur = 1;
	}
	if (trg & PAD_INPUT_UP) {
		cur = -1;
	}

	if (cur != 0) {
		_Button[_Cur]->ChangeImage(0);
		_Cur += cur;
		if (_Cur < 0) {
			_Cur = 0;
		}
		if (_Cur >= _Button.size()) {
			_Cur = _Button.size() - 1;
		}
		_Button[_Cur]->ChangeImage(1);
	}
	if (trg & PAD_INPUT_1) {
		if (_Cur == 0) {
			ModeServer::GetInstance()->Del(this);
			auto g = ModeServer::GetInstance()->Get("game");
			ModeServer::GetInstance()->Del(g);
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
		} else {
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
		}
	}
	return false;
}

bool ModeGameOver::Render() {
	base::Render();
	return false;
}
