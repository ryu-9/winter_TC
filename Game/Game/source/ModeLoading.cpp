#include "ModeLoading.h"
#include "ApplicationGlobal.h"
#include "ApplicationMain.h"

bool ModeLoading::Initialize() {
	if (!base::Initialize()) { return true; }
	if (gGlobal._SelectStage == 0) {
		_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_LOAD_BACK1.png", 0));
	} else if (gGlobal._SelectStage == 1) {
		_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_LOAD_BACK2.png", 0));
	} else {
		_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_LOAD_BACK3.png", 0));
	}
	
	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_LOAD.png", 0));
	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_LOAD_TEXT1.png", 0));
	_UIChip.back()->AddImage("res/UI/UI_LOAD_TEXT2.png");
	_UIChip.back()->AddImage("res/UI/UI_LOAD_TEXT3.png");
	_UIChip.back()->AddImage("res/UI/UI_LOAD_TEXT4.png");
	_UIChip.back()->AddImage("res/UI/UI_LOAD_TEXT5.png");
	_UIChip.back()->AddImage("res/UI/UI_LOAD_TEXT6.png");
	auto n = rand() % 6;
	_UIChip.back()->ChangeImage(n);
	return false;
}

bool ModeLoading::Terminate() {
	return false;
}

bool ModeLoading::Process() {
	base::Process();
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	// 4•b‚²‚Æ‚É‰æ‘œ‚ğ•ÏX
	_Tm += GetStepTm();
	auto trg = ApplicationMain::GetInstance()->GetTrg();
	auto trg2 = ApplicationMain::GetInstance()->GetTrg(2);
	if (trg & PAD_INPUT_1 || trg2 & PAD_INPUT_1) {
		auto n = rand() % 6;
		while (n == _UIChip.back()->GetImageNum()) {
			n = rand() % 6;
		}
		_UIChip.back()->ChangeImage(n);
	}
	if (_Tm >= 4000) {
		_Tm = 0;
		ModeServer::GetInstance()->Del(this);
	}

	
	return false;
}

bool ModeLoading::Render() {
	base::Render();
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	return false;
}
