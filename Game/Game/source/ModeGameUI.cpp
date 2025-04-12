#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"
#include "UITextComponent.h"
#include "PlayerActor.h"
#include "UIBossHPClass.h"
#include "UIItemClass.h"


bool ModeGameUI::Initialize() {

	if (!base::Initialize()) { return true; }
	// UIの初期化
	_UIChip.push_back(new UIChipClass(this, VGet(960, 966, 0), "res/UI/BASE.png", 0));
	_UIChip.push_back(new UIItemClass(this, VGet(960, 1000, 0), "res/UI/item_stock.png", 0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(150, 974, 0), "",0,1,1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(320, 1015, 0), "", 0, 0, 1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1770, 974, 0), "", 0, 1,0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1600, 1015, 0), "", 0, 0, 0));


	_UIChip.push_back(new UIChipClass(this, VGet(960, 540, 0), "res/UI/TDX_UI_MARGE.png", 0));
	_UIChip.push_back(new UIChipClass(this, VGet(286, 990, 0), "res/UI/L.png", 0));
	_UIChip.push_back(new UIChipClass(this, VGet(1634, 990, 0), "res/UI/R.png", 0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(123, 985, 0), "", 1, 1, 1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(420, 1025, 0), "", 1, 0, 1));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1800, 985, 0), "", 1, 1, 0));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(1500, 1025, 0), "", 1, 0, 0));
	new UIChipFadeComponent(_UIChip[6], 0, 0);
	new UIChipFadeComponent(_UIChip[7], 0, 0);
	new UIChipFadeComponent(_UIChip[8], 0, 0);
	new UIChipFadeComponent(_UIChip[9], 0, 0);
	new UIChipFadeComponent(_UIChip[10],0, 0);
	new UIChipFadeComponent(_UIChip[11], 0, 0);
	new UIChipFadeComponent(_UIChip[12], 0, 0);
	

	if (gGlobal._SelectStage == 3) {
		_UIChip.push_back(new UIBossHPClass(this, VGet(415, 0, 0), "", 0));
	}	

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
	if (game->GetPlayer()->GetModeNum() > 0) {// サンダークロス移行時
		if (_AnimCount < 80) {
			
			auto pos = _UIChip[2]->GetPosition();
			_UIChip[2]->SetPosition(VGet(pos.x - 15, pos.y, 0));
			pos = _UIChip[3]->GetPosition();
			_UIChip[3]->SetPosition(VGet(pos.x - 15, pos.y , 0));
			pos = _UIChip[4]->GetPosition();
			_UIChip[4]->SetPosition(VGet(pos.x + 15, pos.y, 0));
			pos = _UIChip[5]->GetPosition();
			_UIChip[5]->SetPosition(VGet(pos.x + 15, pos.y, 0));
			if (_AnimCount == 0) {
				new UIChipFadeComponent(_UIChip[7], 255, 1000);
				new UIChipFadeComponent(_UIChip[8], 255, 1000);
				new UIChipFadeComponent(_UIChip[9], 255, 1000);
				new UIChipFadeComponent(_UIChip[10], 255, 1000);
				new UIChipFadeComponent(_UIChip[11], 255, 1000);
				new UIChipFadeComponent(_UIChip[12], 255, 1000);
				new UIChipFadeComponent(_UIChip[6], 255, 1000);
			}
			_AnimCount++;
		}
	} else {
		if (_AnimCount > 0) {// 通常状態移行時
			
			auto pos = _UIChip[2]->GetPosition();
			_UIChip[2]->SetPosition(VGet(pos.x + 15, pos.y, 0));
			pos = _UIChip[3]->GetPosition();
			_UIChip[3]->SetPosition(VGet(pos.x + 15, pos.y, 0));
			pos = _UIChip[4]->GetPosition();
			_UIChip[4]->SetPosition(VGet(pos.x - 15, pos.y, 0));
			pos = _UIChip[5]->GetPosition();
			_UIChip[5]->SetPosition(VGet(pos.x - 15, pos.y, 0));
			if (_AnimCount == 80) {
				new UIChipFadeComponent(_UIChip[7], 0, 1000);
				new UIChipFadeComponent(_UIChip[8], 0, 1000);
				new UIChipFadeComponent(_UIChip[9], 0, 1000);
				new UIChipFadeComponent(_UIChip[10], 0, 1000);
				new UIChipFadeComponent(_UIChip[11], 0, 1000);
				new UIChipFadeComponent(_UIChip[12], 0, 1000);
				new UIChipFadeComponent(_UIChip[6], 0, 1000);
			}
			_AnimCount--;
		}
	}
	return false;
}

bool ModeGameUI::Render() {
	base::Render();
	if (gGlobal._StartTime == 0) { return false; }
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	auto tm = gGlobal._StartTime;
	auto t = GetNowCount() - tm;
	auto min = t / 1000 / 60;
	auto sec = t / 1000 % 60;
	auto msec = t % 1000 / 10;
	SetFontSize(60);
	DrawFormatString(40, 20, GetColor(255, 255, 255), "%02d:%02d", min, sec);
	SetFontSize(30);
	DrawFormatString(210, 50, GetColor(255, 255, 255), "%02d", msec);
	ChangeFontType(DX_FONTTYPE_NORMAL);
	return true;
}
