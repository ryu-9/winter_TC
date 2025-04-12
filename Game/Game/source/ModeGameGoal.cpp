#include "ModeGameGoal.h"
#include "ApplicationGlobal.h"
#include "ModeStageSelect.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "ModeGameUI.h"
#include "ModeLoading.h"
#include "ModeStory.h"


bool ModeGameGoal::Initialize() {
	if (!base::Initialize()) { return true; }
	_UIChipList.push_back(new UIChipClass(this, VGet(2880, 87, 0), "res/UI/UI_CLEAR_UNDERBAR.png",100));
	new UIChipMoveComponent(_UIChipList.back(),VGet(960,87,0),1000);
	_UIChipList.push_back(new UIChipClass(this, VGet(-960, 993, 0), "res/UI/UI_CLEAR_UNDERBAR.png", 100));
	new UIChipMoveComponent(_UIChipList.back(), VGet(960, 993, 0), 1000);
	gGlobal._EndTime = GetNowCount();
	_Min = (gGlobal._EndTime - gGlobal._StartTime) / 1000 / 60;
	_Sec = (gGlobal._EndTime - gGlobal._StartTime) / 1000 % 60;
	_Msec = (gGlobal._EndTime - gGlobal._StartTime) % 1000 / 10;
	SetFontSize(60);
	return true;
}

bool ModeGameGoal::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGameGoal::Process() {
	base::Process();
	switch (_Step) {
	case 0:	// クリア文字
		if (GetModeTm() > 1000) {	
			_UIChipList.push_back(new UIChipClass(this, VGet(264, 75, 0), "res/UI/UI_CLEAR_MOJI.png", 110));
			_UIChipList.back()->GetUIData()->scale = VGet(1.8, 1.8, 1.8);
			new UIChipScaleComponent(_UIChipList.back(), VGet(1, 1, 1), 500);
			_Step = 1;
		}
		break;
	case 1:
		if (GetModeTm() > 1500) {
			_TimerFlag = true;
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
					ModeServer::GetInstance()->Add(new ModeStory(), 1, "story");
				
			}
		}
	}
	return true;
}



bool ModeGameGoal::Render() {
	base::Render();
	if (_TimerFlag) {
		
		
		DrawFormatString(1300, 920, GetColor(255, 255, 255), "クリアタイム %02d:%02d:%02d", _Min, _Sec,_Msec);
	}
	return true;
}
 