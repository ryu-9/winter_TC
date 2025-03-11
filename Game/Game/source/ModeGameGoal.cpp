#include "ModeGameGoal.h"
#include "ApplicationGlobal.h"
#include "ModeStageSelect.h"
#include "ApplicationMain.h"

bool ModeGameGoal::Initialize() {
	if (!base::Initialize()) { return true; }
	_UIChipList.push_back(new UIChipClass(this, VGet(2880, 87, 0), "res/UI/UI_CLEAR_UNDERBAR.png",100));
	new UIChipMoveComponent(_UIChipList.back(),VGet(960,87,0),1000);
	_UIChipList.push_back(new UIChipClass(this, VGet(-960, 993, 0), "res/UI/UI_CLEAR_UNDERBAR.png", 100));
	new UIChipMoveComponent(_UIChipList.back(), VGet(960, 993, 0), 1000);
	return false;
}

bool ModeGameGoal::Terminate() {
	base::Terminate();
	return false;
}

bool ModeGameGoal::Process() {
	base::Process();
	switch (_Step) {
	case 0:
		if (GetModeTm() > 1000) {
			_UIChipList.push_back(new UIChipClass(this, VGet(264, 75, 0), "res/UI/UI_CLEAR_MOJI.png", 110));
			_UIChipList.back()->GetUIData()->scale = VGet(1.8, 1.8, 1.8);
			new UIChipScaleComponent(_UIChipList.back(), VGet(1, 1, 1), 500);
			_Step = 1;
		}
		break;
	case 1:
		if (GetModeTm() > 1500) {
			// TODO: タイマーテキスト表示
			_Step = 2;
		}
		break;
	case 2:
		if (GetModeTm() > 2000) {
			if (gGlobal._SelectStage < 2) {
				_UIChipList.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_UI_CON.png", 110));
			} else {
				_UIChipList.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_UI_END.png", 110));
			}
			_UIChipList.back()->GetUIData()->scale = VGet(1.8, 1.8, 1.8);
			new UIChipScaleComponent(_UIChipList.back(), VGet(1, 1, 1), 500);
			_Step = 3;
		}
	case 3:
		if (GetModeTm() > 4000) {
			auto trg = ApplicationMain::GetInstance()->GetTrg(1);
			auto trg2 = ApplicationMain::GetInstance()->GetTrg(2);
			if (trg & PAD_INPUT_1 || trg2 & PAD_INPUT_1) {
				ModeServer::GetInstance()->AllDel();
				ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
			}
		}
	}
	return false;
}



bool ModeGameGoal::Render() {
	base::Render();
	return false;
}
 