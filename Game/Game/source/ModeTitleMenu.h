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

	std::vector<MenuItemBase*> _vItems;		// メニューアイテム
	int _Cur;								// 現在のカーソル
	class UISoundActor* _UISound;			// UIサウンド
};

