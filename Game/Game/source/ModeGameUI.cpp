#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"

bool ModeGameUI::Initialize() {
	SetUseASyncLoadFlag(TRUE);
	new UIPlayerHPClass(this, VGet(30, 700, 0), "");
	SetUseASyncLoadFlag(FALSE);
	return false;
}

bool ModeGameUI::Terminate() {
	return false;
}

bool ModeGameUI::Process() {
	base::Process();
	return false;
}

bool ModeGameUI::Render() {
	base::Render();
	return false;
}
