#include "ModeTitleMenu.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "appframe.h"
#include "ModeStageSelect.h"

class TMenuItemStart : public MenuItemBase
{
public:
	TMenuItemStart(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		_vAct.push_back(new UIChipClass(mdTm, VGet(300, 600, 0), "res/title/start.png",110));
		new UIChipFadeComponent(_vAct.back(), 255, 250);
		new UIChipFocusComponent(_vAct.back(), VGet(1.2,1.2,1.2),300);
	}
	virtual int Selected()
	{
		ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("titlemenu"));
		return 1;
	}
};


class TMenuItemContinue : public MenuItemBase {
public:
	TMenuItemContinue(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		_vAct.push_back(new UIChipClass(mdTm, VGet(720, 600, 0), "res/title/continue.png", 110));
		new UIChipFadeComponent(_vAct.back(), 255, 250);
		new UIChipFocusComponent(_vAct.back(), VGet(1.2, 1.2, 1.2), 300);
	}
	virtual int Selected() {
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("titlemenu"));
		return 1;
	}
};

class TMenuItemExit : public MenuItemBase {
public:
	TMenuItemExit(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		_vAct.push_back( new UIChipClass(mdTm, VGet(1200, 600, 0), "res/title/exit.png", 110));
		new UIChipFadeComponent(_vAct.back(), 255, 250);
		new UIChipFocusComponent(_vAct.back(), VGet(1.2, 1.2, 1.2), 300);
	}
	virtual int Selected() {
		ApplicationMain::GetInstance()->Terminate();
		return 1;
	}
};

bool ModeTitleMenu::Initialize()
{
	Add(new TMenuItemStart(this, "はじめから"));
	// if(もしセーブデータがあったら)
	Add(new TMenuItemContinue(this, "つづきから"));
	Add(new TMenuItemExit(this, "ゲーム終了"));
	
	_Cur = 0;
	_vItems[_Cur]->Send(1);
	return false;
}

bool ModeTitleMenu::Terminate()
{
	base::Terminate();
	Clear();
	return false;
}

bool ModeTitleMenu::Process()
{
	base::Process();
	auto trg = ApplicationMain::GetInstance()->GetTrg();

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	bool close = false;
	auto cur = _Cur;
	if (trg & PAD_INPUT_LEFT) { cur--; }
	if (trg & PAD_INPUT_RIGHT) {cur++;	}
	// カーソル位置を上下ループ
	int itemNum = _vItems.size();
	if (itemNum <= 0) {}
	else {
		cur = (cur + itemNum) % itemNum;
	}
	// 決定でアイテムのSelected()を呼ぶ
	if (trg & PAD_INPUT_1) {
		int ret = _vItems[cur]->Selected();
		if (ret == 1) {
			// メニューを閉じる
			close = true;
		}
	}
	if (cur != _Cur) {
		_vItems[_Cur]->Send(0);
		_Cur = cur;
		_vItems[_Cur]->Send(1);
	}
	// メニューを閉じる
	if (close) {
		// このモードを削除する
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeTitleMenu::Render()
{
	base::Render();
	return false;
}

void ModeTitleMenu::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}

void ModeTitleMenu::Clear() {
	// 登録したitemを全部削除する
	for (auto ite = _vItems.begin(); ite != _vItems.end();) {
		delete (*ite);
		ite = _vItems.erase(ite);
	}
}
