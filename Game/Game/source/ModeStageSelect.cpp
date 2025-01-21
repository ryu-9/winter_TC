#include "ModeStageSelect.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeOption.h"

bool ModeStageSelect::Initialize() {

	// セーブデータで分岐

	_UIChip.push_back(new UIChipClass(this, VGet(200, 200, 0), "res/select/stage1.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	new UIChipFocusComponent(_UIChip.back(), VGet(1.2, 1.2, 1.2), 300);
	_UIChip.push_back(new UIChipClass(this, VGet(600, 200, 0), "res/select/stage2.png", 0));
	new UIChipFocusComponent(_UIChip.back(), VGet(1.2, 1.2, 1.2), 300);
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(200, 600, 0), "res/select/stage3.png", 0));
	new UIChipFocusComponent(_UIChip.back(), VGet(1.2, 1.2, 1.2), 300);
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(600, 600, 0), "res/select/stage4.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	new UIChipFocusComponent(_UIChip.back(), VGet(1.2, 1.2, 1.2), 300);
	return true;
}

bool ModeStageSelect::Terminate() {
	base::Terminate();
	return false;
}

bool ModeStageSelect::Process() {
	base::Process();
	auto trg = ApplicationMain::GetInstance()->GetTrg();
	auto cur = _Cur;
	if (trg & PAD_INPUT_UP) { cur -= 2; }
	if (trg & PAD_INPUT_DOWN) { cur += 2; }
	if (trg & PAD_INPUT_LEFT) { cur--; }
	if (trg & PAD_INPUT_RIGHT) { cur++; }

	// カーソル位置を上下ループ
	int itemNum = 4;
	if (itemNum <= 0) {}
	else {
		_Cur = (_Cur + itemNum) % itemNum;
	}
	if (cur != _Cur) {
		_UIChip[_Cur]->Send(0);
		_Cur = cur;
		_UIChip[_Cur]->Send(1);
	}

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
