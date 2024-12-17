#include "ModeTitleMenu.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "appframe.h"

class TMenuItemStart : public MenuItemBase
{
public:
	TMenuItemStart(void* param, std::string text) : MenuItemBase(param, text) {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		new UIChipClass(mdGame, VGet(960, 540, 0), "res/title/start.png",110);
	}
	virtual int Selected()
	{
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
		ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("titlemenu"));
		return 1;
	}
};

class TMenuItemContinue : public MenuItemBase
{

};

bool ModeTitleMenu::Initialize()
{
	Add(new TMenuItemStart(this, "‚Í‚¶‚ß‚©‚ç"));
	_Text.emplace_back("‚Í‚¶‚ß‚©‚ç");
	_Text.emplace_back("‚Â‚Ã‚«‚©‚ç");
	_Text.emplace_back("ƒQ[ƒ€I—¹");
	
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
	auto trg = ApplicationMain::GetInstance()->GetTrg();

	if (trg & PAD_INPUT_UP) { _Cur--; }
	if (trg & PAD_INPUT_DOWN) {_Cur++;	}
	if (_Cur < 0) {	_Cur = 0; }
	if (_Cur >= _Text.size()) { _Cur = _Text.size() - 1; }
	if (trg & PAD_INPUT_1)
	{
		switch (_Cur)
		{
		case 0:
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
			break;
		case 1:
			ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
			ModeServer::GetInstance()->Del(this);
			ModeServer::GetInstance()->Del(ModeServer::GetInstance()->Get("title"));
			break;
		case 2:
			ApplicationMain::GetInstance()->Terminate();
			break;
		}
	}
	return false;
}

bool ModeTitleMenu::Render()
{
	base::Render();
	for (auto i = 0; i < _Text.size(); i++)
	{
		DrawFormatString(100, 100 + i * 20, GetColor(255, 255, 255), "%s", _Text[i].c_str());
	}
	DrawFormatString(80, 100 + _Cur * 20, GetColor(255, 255, 255), ">");
	return false;
}

void ModeTitleMenu::Add(MenuItemBase* itemBase) {
	_vItems.push_back(itemBase);
}

void ModeTitleMenu::Clear() {
	// “o˜^‚µ‚½item‚ğ‘S•”íœ‚·‚é
	for (auto ite = _vItems.begin(); ite != _vItems.end();) {
		delete (*ite);
		ite = _vItems.erase(ite);
	}
}
