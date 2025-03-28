#include "ModeOption.h"
#include "ModeMenu.h"
#include "ApplicationMain.h"
#include "ModeStageSelect.h"
#include "ModeTitle.h"
#include "applicationGlobal.h"

class OpMenuItemContinue : public MenuItemBase {
public:
	OpMenuItemContinue(void* param) : MenuItemBase(param, "") {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(2000, 550, 0), "res/UI/UI_POSE_CONTINUE.png");
		_vAct.push_back(tmp);
		tmp->AddImage("res/UI/UI_POSE_CONTINUE_PICK.png");
		tmp->ChangeImage(1);
		new UIChipMoveComponent(_vAct.back(), VGet(1529, 550, 0), 250);
	}
	virtual int Selected() {
		return 1;
	}
};

class OpMenuOpenSelect : public MenuItemBase {
public:
	OpMenuOpenSelect(void* param) :MenuItemBase(param, "") {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(2000, 800, 0), "res/UI/UI_POSE_STAGE.png");
		_vAct.push_back(tmp);
		tmp->AddImage("res/UI/UI_POSE_STAGE_PICK.png");
		new UIChipMoveComponent(_vAct.back(), VGet(1529, 800, 0), 250);
	}
	virtual int Selected() {
		gGlobal._SelectStage--;
		ModeServer::GetInstance()->AllDel();
		ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
		return 1;
	}
};

class OpMenuExit : public MenuItemBase {
public:
	OpMenuExit(void* param) : MenuItemBase(param, "") {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(2000, 1000, 0), "res/UI/UI_POSE_TITLE.png");
		_vAct.push_back(tmp);
		tmp->AddImage("res/UI/UI_POSE_TITLE_PICK.png");
		new UIChipMoveComponent(_vAct.back(), VGet(1677, 1000, 0), 250);
	}
	virtual int Selected() {

		ModeServer::GetInstance()->AllDel();
		ModeServer::GetInstance()->GetInstance()->Add(new ModeTitle(), 1, "title");

		return 1;
	}
};

bool ModeOption::Initialize() {
	base::Initialize();
	new UIChipClass(this, VGet(960, 540, 0), "res/UI/BACK_ALL.png");
	new UIChipClass(this, VGet(1446, 540, 0), "res/UI/UI_POSE_BACKGROUND.png");
	new UIChipClass(this, VGet(1518, 270, 0), "res/UI/UI_POSE_MENU.png");
	new UIChipClass(this, VGet(438, 540, 0), "res/UI/UI_POSE_HOW_MOVE.png");
	_Cur = 0;
	Add(new OpMenuItemContinue(this));
	Add(new OpMenuOpenSelect(this));
	Add(new OpMenuExit(this));
	_vItems[_Cur]->Send(1);
	

	_UISound = new UISoundActor(this);
	_UISound->AddSound("enter", "enter");
	_UISound->AddSound("select", "select");
	

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
	auto cur = _Cur;
	if (trg & PAD_INPUT_UP) { cur--; }
	if (trg & PAD_INPUT_DOWN) { cur++; }
	// カーソル位置を上下ループ
	int itemNum = _vItems.size();
	if (itemNum <= 0) {} else {
		cur = (cur + itemNum) % itemNum;
	}
	// 決定でアイテムのSelected()を呼ぶ
	if (trg & PAD_INPUT_1) {
		int ret = _vItems[cur]->Selected();
		_UISound->PlayActSound("enter");
		if (ret == 1) {
			// メニューを閉じる
			close = true;
		}
	}
	if (cur != _Cur) {
		dynamic_cast<UIChipClass*>(_vItems[_Cur]->_vAct[0])->ChangeImage(0);
		_Cur = cur;
		dynamic_cast<UIChipClass*>(_vItems[_Cur]->_vAct[0])->ChangeImage(1);
		_UISound->PlayActSound("select");
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
	ModeServer::GetInstance()->SkipRenderUnderLayer();
	return false;
}

void ModeOption::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}
