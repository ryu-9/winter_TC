#pragma once
#include "appframe.h"
#include "ModeGame.h"

class UIPlayerHPClass : public UIChipClass {
public:
	UIPlayerHPClass(class ModeBase* mode, VECTOR pos, const TCHAR* fname, int type, int main, int left, int draworder = 100);
	virtual ~UIPlayerHPClass();
	
};

class UIPlayerHPSpriteComponent : public UIChipSpriteComponent {
public:
	UIPlayerHPSpriteComponent(class UIChipClass* owner, int type, int main,  int left, int drawOrder);
	virtual ~UIPlayerHPSpriteComponent();
	void Draw() override;

private:
	int _Main;
	int _Side;
	int _DrawType;		// É^ÉCÉv	
	int _ModeType;
};

