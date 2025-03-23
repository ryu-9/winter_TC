#include "UIItemClass.h"
#include "ApplicationGlobal.h"

UIItemClass::UIItemClass(ModeBase* mode, VECTOR pos,const TCHAR* name, int draworder) 
	:UIChipClass(mode, pos,name, draworder)
{
	new UIItemSpriteComponent(this, draworder);
}

UIItemClass::~UIItemClass() {
}

UIItemSpriteComponent::UIItemSpriteComponent(UIChipClass* owner, int drawOrder)
	:UIChipSpriteComponent(owner, drawOrder)
{
	SetImage("res/UI/seed_blade.png");
	SetImage("res/UI/seed_lazer.png");
}

UIItemSpriteComponent::~UIItemSpriteComponent() {
}

void UIItemSpriteComponent::Draw() {
	auto tmpx = 100;
	for (auto i = 0; i < gGlobal._ItemList.size(); i++) {
		DrawGraph(_Owner->GetPosition().x - (tmpx * (6-i)), _Owner->GetPosition().y, _CGData[gGlobal._ItemList[i]].handle, TRUE);
		DrawGraph(_Owner->GetPosition().x + (tmpx * (6 - i)), _Owner->GetPosition().y, _CGData[gGlobal._ItemList[i]].handle, TRUE);
	}
}
