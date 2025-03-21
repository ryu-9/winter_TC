#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"
#include "UITextComponent.h"
#include "PlayerActor.h"
#include "UIBossHPClass.h"


bool ModeGameUI::Initialize() {
	
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(30, 750, 0), "",0));
	auto ui = new UIChipClass(this,VGet(1400,920,0),"res/UI/UI_MESSAGE.png",100);
	_UIChip.push_back(ui);
	_UIChip.push_back(new UIChipClass(this,VGet(960,540,0),"res/UI/TDX_UI_MARGE.png"));
	_UIChip.push_back(new UIChipClass(this, VGet(1400, 920, 0), "res/UI/TDX_UI_MARGE_WINDOW.png"));
	_UIChip.push_back(new UIPlayerHPClass(this, VGet(30, 750, 0), "",1));
	new UITextComponent(_UIChip[3], "Scenario");
	new UIChipFadeComponent(_UIChip[2],0, 0);
	new UIChipFadeComponent(_UIChip[3], 0, 0);
	new UIChipFadeComponent(_UIChip[4], 0, 0);

	if (gGlobal._SelectStage == 3) {
		_UIChip.push_back(new UIBossHPClass(this, VGet(415, 0, 0), "", 0));
	}

	new UITextComponent(ui, "Scenario");
	ChangeFont("BIZ UDPÉSÉVÉbÉN Bold");

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
