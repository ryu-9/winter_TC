#pragma once
#include "appframe.h"
#include "ModeMenu.h"

class ModeTitleMenu : public ModeBase
{
	typedef ModeBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void Add(MenuItemBase* itemBase);
	void Clear();

private:
	std::vector<MenuItemBase*> _vItems;

	int _Cur;
	std::vector<std::string> _Text;

};

