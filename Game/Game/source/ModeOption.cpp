#include "ModeOption.h"
#include "ModeMenu.h"

class OpMenuItemContinue : public MenuItemBase {
public:
	OpMenuItemContinue(void* param, std::string text) : MenuItemBase(param, text) {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(0, 600, 0), "res/option/continue.png", 110);
		new UIChipFadeComponent(tmp, 255, 250);
	}
	virtual int Selected() {
		return 1;
	}
};

bool ModeOption::Initialize() {
	base::Initialize();
	Add(new OpMenuItemContinue(this, "‚Â‚Ã‚¯‚é"));
	_Cur = 0;
	return false;
}

bool ModeOption::Terminate() {
	base::Terminate();
	return false;
}

bool ModeOption::Process() {
	base::Process();
	return false;
}

bool ModeOption::Render() {
	base::Render();
	return false;
}

void ModeOption::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}
