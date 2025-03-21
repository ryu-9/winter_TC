#pragma once
#include "appframe.h"
#include "ModeGame.h"

class UIBossHPClass : public UIChipClass {
public:
	UIBossHPClass(class ModeBase* mode, VECTOR pos, const TCHAR* fname, int draworder = 100);
	virtual ~UIBossHPClass();

};

class UIBossHPSpriteComponent : public UIChipSpriteComponent {
public:
	UIBossHPSpriteComponent(class UIChipClass* owner,  int drawOrder);
	virtual ~UIBossHPSpriteComponent();
	void Draw() override;
private:
};