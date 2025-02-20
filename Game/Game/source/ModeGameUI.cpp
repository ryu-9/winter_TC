#include "ModeGameUI.h"
#include "UIPlayerHPClass.h"

bool ModeGameUI::Initialize() {
	new UIPlayerHPClass(this, VGet(50, 600, 0), "res/UI/UI_HP.png");
	return false;
}

bool ModeGameUI::Terminate() {
	return false;
}

bool ModeGameUI::Process() {
	return false;
}

bool ModeGameUI::Render() {
	base::Render();
	return false;
}
