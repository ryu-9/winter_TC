#include "ModeStageSelect.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeOption.h"
#include "ModeGameUI.h"
#include "ModeScenario.h"
#include "ApplicationGlobal.h"
#include "UISoundActor.h"
#include "ModeLoading.h"
#include "ModeStory.h"

bool ModeStageSelect::Initialize() {

	// セーブデータで分岐

	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/UI_WORLD1.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(960 + (1920 * 1), 540, 0), "res/UI/UI_WORLD2.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UIChip.push_back(new UIChipClass(this, VGet(960 + (1920 * 2), 540, 0), "res/UI/UI_WORLD3.png", 0));
	new UIChipFadeComponent(_UIChip.back(), 255, 500);
	_UISound = new UISoundActor(this);
	_UISound->AddSound("enter", "enter");
	_UISound->AddSound("select", "select");
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
	if (trg & PAD_INPUT_LEFT) { cur--; }
	if (trg & PAD_INPUT_RIGHT) { cur++; }

	// カーソル位置修正
	if (cur < 0) { cur = 0; }
	if (cur >= _UIChip.size()) { cur = _UIChip.size() - 1; }
	if (_Tm <= 0 && cur!=_Cur) {
		if (cur > _Cur) {
			for (auto i = 0; i < _UIChip.size(); i++) {
				new UIChipMoveComponent(_UIChip[i], VAdd(_UIChip[i]->GetPosition(), VGet(-1920, 0, 0)), 500);
			}
		}
		if (cur < _Cur) {
			for (auto i = 0; i < _UIChip.size(); i++) {
				new UIChipMoveComponent(_UIChip[i], VAdd(_UIChip[i]->GetPosition(), VGet(1920, 0, 0)), 500);
			}
		}
		_Cur = cur;
		_UISound->PlayActSound("select");
		_Tm = 500;
	} else {
		_Tm -= GetStepTm();
	}
	

	if (trg & PAD_INPUT_1) {
		// ステージ選択
		gGlobal._SelectStage = _Cur;
		ModeServer::GetInstance()->Add(new ModeGame(),1,"game");
		ModeServer::GetInstance()->Add(new ModeGameUI(), 2, "gameui");
		ModeServer::GetInstance()->Add(new ModeLoading(), 100, "loading");

		// このモードを削除する
		ModeServer::GetInstance()->Del(this);

		_UISound->PlayActSound("enter");
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
