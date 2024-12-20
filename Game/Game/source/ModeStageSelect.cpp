#include "ModeStageSelect.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeOption.h"

bool ModeStageSelect::Initialize() {
	auto tmp = new UIChipClass(this, VGet(200, 200, 0), "res/select/stage1.png", 0);
	new UIChipFadeComponent(tmp, 255, 250);
	tmp =new UIChipClass(this, VGet(600, 200, 0), "res/select/stage2.png", 0);
	new UIChipFadeComponent(tmp, 255, 250);
	tmp = new UIChipClass(this, VGet(200, 600, 0), "res/select/stage3.png", 0);
	new UIChipFadeComponent(tmp, 255, 250);
	tmp = new UIChipClass(this, VGet(600, 600, 0), "res/select/stage4.png", 0);
	new UIChipFadeComponent(tmp, 255, 250);
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
		ModeServer::GetInstance()->Add(new ModeGame(),1,"game");
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}

	if (trg & PAD_INPUT_3) {
		ModeServer::GetInstance()->Add(new ModeOption(), 99, "option");
	}
	return false;
}

bool ModeStageSelect::Render() {
	base::Render();
	return false;
}
