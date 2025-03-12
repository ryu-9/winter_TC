#include "ModeSynopsis.h"
#include "ApplicationMain.h"

bool ModeSynopsis::Initialize() {
	if (!base::Initialize()) { return false; }
	_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/STORY_OP.png"));
	_UIChip.emplace_back(new UIChipClass(this, VGet(960, 920, 0), "res/UI/UI_TEXT_BACK.png"));
	_UIChip.emplace_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_STORY_SIDE.png"));

	return false;
}

bool ModeSynopsis::Terminate() {
	return false;
}

bool ModeSynopsis::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	if (trg & PAD_INPUT_2) {
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeSynopsis::Render() {
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	base::Render();



	return false;
}
