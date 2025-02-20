#include "UIPlayerHPClass.h"

UIPlayerHPClass::UIPlayerHPClass(ModeBase* mode, VECTOR pos, const TCHAR* fname, int draworder )
	:UIChipClass(mode, pos,fname,draworder)
{
	new UIPlayerHPSpriteComponent(this, draworder);
}

UIPlayerHPClass::~UIPlayerHPClass() {
}

UIPlayerHPSpriteComponent::UIPlayerHPSpriteComponent(UIChipClass* owner, int drawOrder)
	:UIChipSpriteComponent(owner, drawOrder)
	, _Type(0)
{
	SetImage("res/UI/UI_HP_LOW.png");
	_CGData[0].pos = VGet(0, 0, 0);
	SetImage("res/UI/UI_HPBAR_LOW.png");
	_CGData[1].pos = VGet(0, 0, 0);
	SetImage("res/UI/UI_HP_HIGH.png");
	_CGData.back().pos = VGet(0, 0, 0);
	SetImage("res/UI/UI_HPBAR_MARGE.png");
}

UIPlayerHPSpriteComponent::~UIPlayerHPSpriteComponent() {
}



void UIPlayerHPSpriteComponent::Draw() {
	DrawGraph(_UIc->GetPosition().x, _UIc->GetPosition().y, _CGData[_Type].handle, TRUE);
	DrawGraph(_UIc->GetPosition().x, _UIc->GetPosition().y, _CGData[_Type + 1].handle, TRUE);
}
