#include "ModeStageSelect.h"

bool ModeStageSelect::Initialize() {
	new UIChipClass(this, VGet(0, 0, 0), "res/stage/stage1.png", 0);
	new UIChipClass(this, VGet(400, 0, 0), "res/stage/stage2.png", 0);
	new UIChipClass(this, VGet(0, 400, 0), "res/stage/stage3.png", 0);
	new UIChipClass(this, VGet(400, 400, 0), "res/stage/stage4.png", 0);
	return true;
}

bool ModeStageSelect::Terminate() {
	base::Terminate();
	return false;
}

bool ModeStageSelect::Process() {
	base::Process();
	auto trg = ApplicationMain::GetInstance()->GetTrg();
	if (trg & PAD_INPUT_UP) { _Cur -= 2; }
	if (trg & PAD_INPUT_DOWN) { _Cur += 2; }
	if (trg & PAD_INPUT_LEFT) { _Cur--; }
	if (trg & PAD_INPUT_RIGHT) { _Cur++; }
	if (trg & PAD_INPUT_1) {
		// ステージ選択
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeStageSelect::Render() {
	base::Render();
	return false;
}
