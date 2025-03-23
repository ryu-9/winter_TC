#include "UIItemClass.h"
#include "ApplicationGlobal.h"

UIItemClass::UIItemClass(ModeBase* mode, VECTOR pos,const TCHAR* name, int draworder) 
	:UIChipClass(mode, pos,name, draworder)
{
	new UIItemSpriteComponent(this, draworder);
	gGlobal._ItemList.push_back(0);
	gGlobal._ItemList.push_back(1);
	gGlobal._ItemList.push_back(1);
	gGlobal._ItemList.push_back(0);
	gGlobal._ItemList.push_back(1);
	gGlobal._ItemList.push_back(1);
}

UIItemClass::~UIItemClass() {
}

UIItemSpriteComponent::UIItemSpriteComponent(UIChipClass* owner, int drawOrder)
	:UIChipSpriteComponent(owner, drawOrder)
{
	SetImage("res/UI/seed_blade.png");
	SetImage("res/UI/seed_lazer.png");
	SetImage("res/Stage/model/blade.png");
	SetImage("res/Stage/model/denchi.png");
}

UIItemSpriteComponent::~UIItemSpriteComponent() {
}

void UIItemSpriteComponent::Draw() {
	auto tmpx = 76;
	
	for (auto i = 0; i < gGlobal._ItemList.size(); i++) {
		auto x = tmpx * (6 - i);
		DrawGraph(_Owner->GetPosition().x - x - (_CGData[0].width * 0.5) - 24, _Owner->GetPosition().y -30, _CGData[gGlobal._ItemList[i]].handle, TRUE);
		DrawGraph(_Owner->GetPosition().x + x - (_CGData[0].width * 0.5) + 25 , _Owner->GetPosition().y -30, _CGData[gGlobal._ItemList[i]].handle, TRUE);
	}
	if (gGlobal._ItemNum > 0) {
		DrawExtendGraph(_Owner->GetPosition().x - (_CGData[gGlobal._ItemNum + 1].width * 0.05), _Owner->GetPosition().y - (_CGData[gGlobal._ItemNum + 1].height * 0.05), _Owner->GetPosition().x + (_CGData[gGlobal._ItemNum + 1].width * 0.05), _Owner->GetPosition().y + (_CGData[gGlobal._ItemNum + 1].height * 0.05), _CGData[gGlobal._ItemNum + 1].handle, TRUE);
	}
}
