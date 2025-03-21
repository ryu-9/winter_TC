#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"
#include "UITextComponent.h"
#include "PlayerActor.h"
#include "UIBossHPClass.h"


bool ModeGameUI::Initialize() {

	
//	_UIChip.push_back(new UIChipClass(this, VGet(960, 966, 0), "res/UI/BASE.png", 0));
//	_UIChip.push_back(new UIChipClass(this, VGet(960, 1030, 0), "res/UI/item_stock.png", 0));
//	_UIChip.push_back(new UIPlayerHPClass(this, VGet(150, 974, 0), "",0,1,1));
//	_UIChip.push_back(new UIPlayerHPClass(this, VGet(320, 1015, 0), "", 0, 0, 1));
//	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1770, 974, 0), "", 0, 1,0));
//	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1600, 1015, 0), "", 0, 0, 0));
//	auto ui = new UIChipClass(this,VGet(960,940,0),"res/UI/UI_WINDOW.png",0);
//	_UIChip.push_back(ui);
	// 8ŒÂ

	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_UI_MARGE.png", 0));
	_UIChip.push_back(new UIChipClass(this, VGet(0, 900, 0), "res/UI/L.png", 0));
	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/R.png", 0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(100, 900, 0), "", 1, 1, 1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(200, 900, 0), "", 1, 0, 1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1820, 900, 0), "", 1, 1, 0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1720, 900, 0), "", 1, 0, 0));

	_UIChip.push_back(new UIBossHPClass(this, VGet(415, 0, 0), "", 0));
//	_UIChip.push_back(new UIChipClass(this,VGet(960,540,0),"res/UI/TDX_UI_MARGE.png"));
//	_UIChip.push_back(new UIChipClass(this, VGet(1400, 920, 0), "res/UI/TDX_UI_MARGE_WINDOW.png"));
//	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1820, 900, 0), "",true,1,false));
//	new UITextComponent(_UIChip[3], "Scenario");
//	new UIChipFadeComponent(_UIChip[2],0, 0);
//	new UIChipFadeComponent(_UIChip[3], 0, 0);
//	new UIChipFadeComponent(_UIChip[4], 0, 0);

	if (gGlobal._SelectStage == 3) {
		_UIChip.push_back(new UIBossHPClass(this, VGet(415, 0, 0), "", 0));
	}
//	new UITextComponent(ui, "Scenario");

	return false;
}

bool ModeGameUI::Terminate() {
	base::Terminate();
	SetFontSize(60);
	return false;
}

bool ModeGameUI::Process() {
	base::Process();
	auto game = dynamic_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
	if (game->GetPlayer()->GetModeNum() > 0) {
		if (_AnimCount < 80) {
			_AnimCount++;
			auto pos = _UIChip[0]->GetPosition();
			_UIChip[0]->SetPosition(VGet(pos.x - 10, pos.y, 0));
			pos = _UIChip[1]->GetPosition();
			_UIChip[1]->SetPosition(VGet(pos.x + 15, pos.y , 0));
			new UIChipFadeComponent(_UIChip[2], 255, 1000);
			new UIChipFadeComponent(_UIChip[3], 255, 1000);
			new UIChipFadeComponent(_UIChip[4], 255, 1000);
		}
	} else {
		if (_AnimCount > 0) {
			_AnimCount--;
			auto pos = _UIChip[0]->GetPosition();
			_UIChip[0]->SetPosition(VGet(pos.x + 10, pos.y , 0));
			pos = _UIChip[1]->GetPosition();
			_UIChip[1]->SetPosition(VGet(pos.x - 15, pos.y, 0));
			new UIChipFadeComponent(_UIChip[2], 0, 1000);
			new UIChipFadeComponent(_UIChip[3], 0, 1000);
			new UIChipFadeComponent(_UIChip[4], 0, 1000);
		}
	}
	return false;
}

bool ModeGameUI::Render() {
	base::Render();
	if (gGlobal._StartTime == 0) { return false; }
	auto tm = gGlobal._StartTime;
	auto t = GetNowCount() - tm;
	auto min = t / 1000 / 60;
	auto sec = t / 1000 % 60;
	auto msec = t % 1000 / 10;
	SetFontSize(60);
	DrawFormatString(20, 20, GetColor(255, 255, 255), "%02d:%02d", min, sec);
	SetFontSize(30);
	DrawFormatString(210, 50, GetColor(255, 255, 255), "%02d", msec);
	return false;
}
