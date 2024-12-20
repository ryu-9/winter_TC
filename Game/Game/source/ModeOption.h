#pragma once
#include "appframe.h"
#include "ModeMenu.h"


class ModeOption : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void Add(MenuItemBase* itemBase);
private:
	std::vector<MenuItemBase*> _vItems;
	int _Cur;
};

