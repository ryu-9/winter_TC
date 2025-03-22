#pragma once
#include "appframe.h"
#include "ModeGame.h"

class UIItemClass : public UIChipClass {
public:
	UIItemClass(class ModeBase* mode, VECTOR pos, int draworder = 100);
	virtual ~UIItemClass();
};

class UIItemSpriteComponent : public UIChipSpriteComponent {

public:
	UIItemSpriteComponent(class UIChipClass* owner, int drawOrder);
	virtual ~UIItemSpriteComponent();
	void Draw() override;

private:

};

