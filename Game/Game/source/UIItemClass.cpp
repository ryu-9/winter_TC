#include "UIItemClass.h"

UIItemClass::UIItemClass(ModeBase* mode, VECTOR pos, int draworder) 
	:UIChipClass(mode, pos,"", draworder)
{
}

UIItemClass::~UIItemClass() {
}

UIItemSpriteComponent::UIItemSpriteComponent(UIChipClass* owner, int drawOrder)
	:UIChipSpriteComponent(owner, drawOrder)
{
}

UIItemSpriteComponent::~UIItemSpriteComponent() {
}

void UIItemSpriteComponent::Draw() {
}
