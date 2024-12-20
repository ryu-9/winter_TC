#include "ModeOption.h"
#include "ModeMenu.h"
#include "ApplicationMain.h"

class OpMenuItemContinue : public MenuItemBase {
public:
	OpMenuItemContinue(void* param, std::string text) : MenuItemBase(param, text) {
		ModeOption* mdOp = static_cast<ModeOption*>(_param);
		auto tmp = new UIChipClass(mdOp, VGet(640, 400, 0), "res/option/continue.png", 110);
		new UIChipFadeComponent(tmp, 255, 250);
	}
	virtual int Selected() {
		return 1;
	}
};

bool ModeOption::Initialize() {
	base::Initialize();
	Add(new OpMenuItemContinue(this, "�Â���"));
	auto tm = new UIChipClass(this, VGet(640, 360, 0), "res/fade.png", 1);
	new UIChipFadeComponent(tm, 150, 250);
	tm->GetUIData()->scale = VGet(80, 80, 1);
	_Cur = 0;
	return false;
}

bool ModeOption::Terminate() {
	base::Terminate();
	return false;
}

bool ModeOption::Process() {

	base::Process();
	auto trg = ApplicationMain::GetInstance()->GetTrg();

	// ���̃��[�h��艺�̃��C���[��Process()���Ă΂Ȃ�
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	bool close = false;

	if (trg & PAD_INPUT_LEFT) { _Cur--; }
	if (trg & PAD_INPUT_RIGHT) { _Cur++; }
	// �J�[�\���ʒu���㉺���[�v
	int itemNum = _vItems.size();
	if (itemNum <= 0) {}
	else {
		_Cur = (_Cur + itemNum) % itemNum;
	}
	// ����ŃA�C�e����Selected()���Ă�
	if (trg & PAD_INPUT_1) {
		int ret = _vItems[_Cur]->Selected();
		if (ret == 1) {
			// ���j���[�����
			close = true;
		}
	}

	// ���j���[�����
	if (close) {
		// ���̃��[�h���폜����
		ModeServer::GetInstance()->Del(this);
	}
	return false;
}

bool ModeOption::Render() {
	base::Render();
	return false;
}

void ModeOption::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}
