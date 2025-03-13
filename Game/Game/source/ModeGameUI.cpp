#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"
#include "UITextComponent.h"

bool ModeGameUI::Initialize() {
	
	new UIPlayerHPClass(this, VGet(30, 750, 0), "");
	auto ui = new UIChipClass(this,VGet(1400,920,0),"res/UI/UI_MESSAGE.png",100);
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
