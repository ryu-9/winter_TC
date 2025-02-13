#include "ModeStageSelect.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeOption.h"

bool ModeStageSelect::Initialize() {

	// セーブデータで分岐

	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_WORLD1.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(960 + (1920 * 1), 540, 0), "res/UI/UI_WORLD2.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(960 + (1920 * 2), 540, 0), "res/UI/UI_WORLD3.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
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
	if (trg & PAD_INPUT_LEFT) { cur--; 
	for (auto i = 0; i < _UIChip.size(); i++) {
		// TODO: 引数を座標から移動量に変更
		new UIChipMoveComponent(_UIChip[i], VAdd(_UIChip[i]->GetPosition(),VGet(1920,0,0)), 500);
	}
	}
	if (trg & PAD_INPUT_RIGHT) { cur++; for (auto i = 0; i < _UIChip.size(); i++) {
		new UIChipMoveComponent(_UIChip[i], VAdd(_UIChip[i]->GetPosition(), VGet(-1920, 0, 0)), 500);
	}
	}
	
	// カーソル位置をループ
	int itemNum = 4;
	if (itemNum <= 0) {}
	else {
		_Cur = (_Cur + itemNum) % itemNum;
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
