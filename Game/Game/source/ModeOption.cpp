#include "ModeOption.h"
#include "ModeMenu.h"
#include "ApplicationMain.h"

class OpMenuItemContinue : public MenuItemBase {
public:
	OpMenuItemContinue(void* param, std::string text) : MenuItemBase(param, text) {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(640, 400, 0), "res/option/continue.png", 110);
		new UIChipFadeComponent(tmp, 255, 250);
	}
	virtual int Selected() {
		return 1;
	}
};

bool ModeOption::Initialize() {
	base::Initialize();
	Add(new OpMenuItemContinue(this, "つづける"));
	auto tm = new UIChipClass(this, VGet(640, 360, 0), "res/fade.png", 1);
	new UIChipFadeComponent(tm, 150, 250);
	tm->GetUIData()->scale = VGet(80, 80, 1);
	_Cur = 0;
	return false;
}

bool ModeOption::Terminate() {
	base::Terminate();
	return false;
}

bool ModeOption::Process() {

	base::Process();
	auto trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	bool close = false;

	if (trg & PAD_INPUT_LEFT) { _Cur--; }
	if (trg & PAD_INPUT_RIGHT) { _Cur++; }
	// カーソル位置を上下ループ
	int itemNum = _vItems.size();
	if (itemNum <= 0) {}
	else {
		_Cur = (_Cur + itemNum) % itemNum;
	}
	// 決定でアイテムのSelected()を呼ぶ
	if (trg & PAD_INPUT_1) {
		int ret = _vItems[_Cur]->Selected();
		if (ret == 1) {
			// メニューを閉じる
			close = true;
		}
	}

	// メニューを閉じる
	if (close) {
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeOption::Render() {
	base::Render();
	return false;
}

void ModeOption::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}
