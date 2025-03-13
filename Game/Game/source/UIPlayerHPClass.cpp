#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"

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
	CreateMaskScreen();
	_Mask = MakeScreen(1920, 1080, TRUE);
	_MaskHandle = LoadMask("res/UI/UI_HPBAR_MASK.png");
	_CGData.clear();
	SetImage("res/UI/UI_HP_LOW.png");
	SetImage("res/UI/UI_HPBAR_LOW.png");
	_CGData.back().pos = VGet(164, 66, 0);
	SetImage("res/UI/UI_HP_MARGE.png");
	SetImage("res/UI/UI_HPBAR_MARGE.png");
	_CGData.back().pos = VGet(164, 66, 0);
}

UIPlayerHPSpriteComponent::~UIPlayerHPSpriteComponent() {
}



void UIPlayerHPSpriteComponent::Draw() {
	
	auto tmpy = 0;
	auto tmpx = 0;
	for (auto i = 0; i < 2; i++) {
		auto hp = gGlobal._PlayerHP[i];
		if (hp >= 1) {
			_Type = 2;
			hp -= 1;
			hp *= 10;
			// 0‚©‚ç1‚Ü‚Å‚ÌŠÔ‚Å•Ï‰»
			tmpx = (hp / (hp + 10)) * 256;


		} else {
			_Type = 0;
			tmpx = hp * 256;
			if (tmpx > 256) {
				tmpx = 256;
			}
		}
		if (gGlobal._IsPlayerDead[i] == TRUE) {
			tmpx = -200;
		} else {
			tmpx += 0;
		}
	
		
		DrawGraph(_UIc->GetPosition().x, _UIc->GetPosition().y + tmpy, _CGData[_Type].handle, TRUE);
		SetUseMaskScreenFlag(TRUE);
		FillMaskScreen(1);
		auto pos = GetCameraPosition();
		auto dir = GetCameraTarget();
		SetDrawScreen(_Mask);
		DrawMask(_UIc->GetPosition().x +_CGData[_Type + 1].pos.x  - 256 + tmpx, _UIc->GetPosition().y + _CGData[_Type + 1].pos.y + tmpy, _MaskHandle, DX_MASKTRANS_WHITE);
		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(_UIc->GetPosition().x + _CGData[_Type + 1].pos.x, _UIc->GetPosition().y + _CGData[_Type + 1].pos.y + tmpy, _CGData[_Type + 1].handle, TRUE);
		SetUseMaskScreenFlag(FALSE);
//		DrawGraph(_UIc->GetPosition().x + _CGData[_Type + 2].pos.x, _UIc->GetPosition().y + _CGData[_Type + 2].pos.y + tmpy, _CGData[_Type + 2].handle, TRUE);
		SetCameraPositionAndTarget_UpVecY(pos, dir);
		tmpy += 160;
	}

	
}
