#pragma once
#include "appframe.h"
#include "ModeGame.h"

class UIPlayerHPClass : public UIChipClass {
public:
	UIPlayerHPClass(class ModeBase* mode, VECTOR pos, const TCHAR* fname, int draworder = 100);
	virtual ~UIPlayerHPClass();
	
};

class UIPlayerHPSpriteComponent : public UIChipSpriteComponent {
public:
	UIPlayerHPSpriteComponent(class UIChipClass* owner, int drawOrder);
	virtual ~UIPlayerHPSpriteComponent();
	void Draw() override;

private:
	int _Type;
};

