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
		auto tmp = new UIChipClass(mdTm, VGet(0, 600, 0), "res/title/start.png",110);
		new UIChipFadeComponent(tmp, 255, 250);
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
		auto tmp = new UIChipClass(mdTm, VGet(300, 600, 0), "res/title/continue.png", 110);
		new UIChipFadeComponent(tmp, 255, 250);
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
		auto tmp= new UIChipClass(mdTm, VGet(600, 600, 0), "res/title/exit.png", 110);
		new UIChipFadeComponent(tmp, 255, 250);
	}
	virtual int Selected() {
		ApplicationMain::GetInstance()->Terminate();
		return 1;
	}
};

bool ModeTitleMenu::Initialize()
{
	Add(new TMenuItemStart(this, "�͂��߂���"));
	// if(�����Z�[�u�f�[�^����������)
	Add(new TMenuItemContinue(this, "�Â�����"));
	Add(new TMenuItemExit(this, "�Q�[���I��"));
	
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

	bool close = false;

	if (trg & PAD_INPUT_LEFT) { _Cur--; }
	if (trg & PAD_INPUT_RIGHT) {_Cur++;	}
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

bool ModeTitleMenu::Render()
{
	base::Render();
	return false;
}

void ModeTitleMenu::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}

void ModeTitleMenu::Clear() {
	// �o�^����item��S���폜����
	for (auto ite = _vItems.begin(); ite != _vItems.end();) {
		delete (*ite);
		ite = _vItems.erase(ite);
	}
}
