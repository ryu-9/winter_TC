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
	SetImage("res/UI/denchi.png");
	SetImage("res/UI/blade.png");
	SetImage("res/UI/denchi.png");
}

UIItemSpriteComponent::~UIItemSpriteComponent() {
}

void UIItemSpriteComponent::Draw() {
	auto tmpx = 42;
	gGlobal._ItemList[0] = 6;
	gGlobal._ItemList[1] = 6;
		for (auto i = 0; i < gGlobal._ItemList[0]; i++) {
			auto x = tmpx * (6 - i);
			
			DrawExtendGraph(_Owner->GetPosition().x - x - (_CGData[0].width * 0.2) - 94, _Owner->GetPosition().y - (_CGData[0].height * 0.2) + 58, _Owner->GetPosition().x - x + (_CGData[0].width * 0.2) - 94, _Owner->GetPosition().y + (_CGData[0].height * 0.2) + 58, _CGData[0].handle, TRUE);
		}
		for (auto i = 0; i < gGlobal._ItemList[1]; i++) {
			auto x = tmpx * (6 - i);
			DrawExtendGraph(_Owner->GetPosition().x + x - (_CGData[1].width * 0.2) + 94, _Owner->GetPosition().y - (_CGData[1].height * 0.2) + 58, _Owner->GetPosition().x + x + (_CGData[1].width * 0.2) + 94, _Owner->GetPosition().y + (_CGData[1].height * 0.2) + 58, _CGData[1].handle, TRUE);

		}
	if (gGlobal._ItemNum > 0) {
		DrawExtendGraph(_Owner->GetPosition().x - (_CGData[gGlobal._ItemNum +1].width * 0.5 ), _Owner->GetPosition().y - (_CGData[gGlobal._ItemNum + 1].height * 0.5), _Owner->GetPosition().x + (_CGData[gGlobal._ItemNum + 1].width * 0.5), _Owner->GetPosition().y + (_CGData[gGlobal._ItemNum + 1].height * 0.5), _CGData[gGlobal._ItemNum +1].handle, TRUE);
	}

}