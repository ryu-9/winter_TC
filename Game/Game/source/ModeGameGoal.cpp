#include "ModeGameGoal.h"

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
	if (_Step == 0) {
		if (GetModeTm() > 1000) {
			_UIChipList.push_back(new UIChipClass(this, VGet(264, 75, 0), "res/UI/UI_CLEAR_MOJI.png", 110));
			//_UIChipList.back()->SetSize(VGet(2,2,2));
			_Step = 1;
		}
	}
	return false;
}



bool ModeGameGoal::Render() {
	base::Render();
	return false;
}
 