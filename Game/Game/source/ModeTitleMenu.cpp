#include "ModeTitleMenu.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "appframe.h"
#include "ModeStageSelect.h"
#include "UISoundActor.h"
#include "ApplicationGlobal.h"

// 初めから開始ボタン
class TMenuItemStart : public MenuItemBase
{
public:
	TMenuItemStart(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		auto ui = new UIChipClass(mdTm, VGet(2000, 550, 0), "res/UI/UI_START_ORIGIN.png", 110);
		_vAct.push_back(ui);
		ui->AddImage("res/UI/UI_START_ORIGIN_PICK.png");
		ui->ChangeImage(1);
		new UIChipMoveComponent(_vAct.back(), VGet(1529, 550, 0), 250);

	}
	virtual int Selected()
	{
		ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("titlemenu"));
		return 1;
	}
};

// 続きから開始ボタン
class TMenuItemContinue : public MenuItemBase {
public:
	TMenuItemContinue(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		auto ui = new UIChipClass(mdTm, VGet(2000, 800, 0), "res/UI/UI_START_LOAD.png", 110);
		_vAct.push_back(ui);
		ui->AddImage("res/UI/UI_START_LOAD_PICK.png");
		new UIChipMoveComponent(_vAct.back(), VGet(1529, 800, 0), 250);

	}
	virtual int Selected() {
		gGlobal._Stageflg = 3;
		ModeServer::GetInstance()->Add(new ModeStageSelect(), 1, "select");
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("titlemenu"));
		return 1;
	}
};

// ゲーム終了ボタン
class TMenuItemExit : public MenuItemBase {
public:
	TMenuItemExit(void* param, std::string text) : MenuItemBase(param, text) {
		ModeTitleMenu* mdTm = static_cast<ModeTitleMenu*>(_param);
		auto ui = new UIChipClass(mdTm, VGet(2100, 1000, 0), "res/UI/UI_START_END.png", 110);
		_vAct.push_back(ui);
		ui->AddImage("res/UI/UI_START_END_PICK.png");
		new UIChipMoveComponent(_vAct.back(), VGet(1677, 1000, 0), 250);
	}
	virtual int Selected() {
		ApplicationMain::GetInstance()->Terminate();
		return 1;
	}
};

bool ModeTitleMenu::Initialize() {
	if (!base::Initialize()) { return false; }
	// メニューの初期化
	new UIChipClass(this, VGet(1446, 540, 0), "res/UI/UI_START_BACKGROUND.png", 110);
	new UIChipClass(this, VGet(1518, 270, 0), "res/UI/UI_START_MENU.png", 110);
	Add(new TMenuItemStart(this, "はじめから"));
	Add(new TMenuItemContinue(this, "つづきから"));
	Add(new TMenuItemExit(this, "ゲーム終了"));
	
	// カーソルの初期化
	_Cur = 0;
	_vItems[_Cur]->Send(1);

	// サウンドの追加
	_UISound = new UISoundActor(this);
	_UISound->AddSound("enter", "enter");
	_UISound->AddSound("select", "select");
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
	if (trg & PAD_INPUT_UP) { cur--; }
	if (trg & PAD_INPUT_DOWN) {cur++;	}
	// カーソル位置を上下ループ
	int itemNum = _vItems.size();
	if (itemNum <= 0) {}
	else {
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
