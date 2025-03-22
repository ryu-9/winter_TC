#include "UIPlayerHPClass.h"
#include "ApplicationGlobal.h"

UIPlayerHPClass::UIPlayerHPClass(ModeBase* mode, VECTOR pos, const TCHAR* fname, int type, int main,  int left, int draworder )
	:UIChipClass(mode, pos,fname,draworder)
{
	new UIPlayerHPSpriteComponent(this, type, main,left, draworder);
	
}

UIPlayerHPClass::~UIPlayerHPClass() {
}

UIPlayerHPSpriteComponent::UIPlayerHPSpriteComponent(UIChipClass* owner, int type, int main, int left, int drawOrder)
	:UIChipSpriteComponent(owner, drawOrder)
	, _DrawType(0)
	, _ModeType(type)
	, _Main(main)
	, _Side(left) {
	_CGData.clear();
	if (_ModeType == 0) {
		if (_Main) {
			SetImage("res/UI/HP/UI_HP_CIRCLE_EMPTY.png");
			SetImage("res/UI/HP/UI_HP_CIRCLE_EMPTY_M_OK.png");
			SetImage("res/UI/HP/UI_HP_CIRCLE.png");
			SetImage("res/UI/HP/UI_HP_CIRCLE_M_OK.png");
			if (_Side) {
				SetImage("res/UI/HP/JIN_ICON.png");
			} else {
				SetImage("res/UI/HP/BEL_ICON.png");
			}
		} else {
			SetImage("res/UI/HP/SUB/UI_HP_CIRCLE_EMPTY.png");
			SetImage("res/UI/HP/SUB/UI_HP_CIRCLE_EMPTY_M_OK.png");
			SetImage("res/UI/HP/SUB/UI_HP_CIRCLE.png");
			SetImage("res/UI/HP/SUB/UI_HP_CIRCLE_M_OK.png");
			if (_Side) {
				SetImage("res/UI/HP/SUB/BEL_ICON.png");
			} else {

				SetImage("res/UI/HP/SUB/JIN_ICON.png");
			}
		}
	} else {
		if (_Main) {
			SetImage("res/UI/MAIN/POWER_ENPTY.png");
			SetImage("res/UI/MAIN/POWER_BAR.png");
			SetImage("res/UI/MAIN/POWER_NAME.png");
		} else {
			SetImage("res/UI/SUB/POWER_ENPTY.png");
			SetImage("res/UI/SUB/POWER_BAR.png");
			SetImage("res/UI/SUB/POWER_NAME.png");
		}
	}
}

	UIPlayerHPSpriteComponent::~UIPlayerHPSpriteComponent() {
	}



	void UIPlayerHPSpriteComponent::Draw() {
		SetDrawBlendMode(_UIc->GetUIData()->blendMode, _UIc->GetUIData()->blendParam);

		auto tmpy = 0;
		auto tmpx = 0;
		if (_ModeType == 0) {
				auto hp = gGlobal._PlayerHP[_Side^_Main];
				if (hp >= 1) {
					hp -= 1;
					hp *= 10;
					hp = (hp / (hp + 10)) * 100;
					_DrawType = 1;
				} else {
					hp *= 100;
					_DrawType = 0;
				}
				if (gGlobal._IsPlayerDead[_Side ^ _Main] == TRUE) {
					hp = 0;
				} else {

				}


				DrawGraph(_UIc->GetPosition().x - (_CGData[1 * _DrawType + 0].width * 0.5), _UIc->GetPosition().y - (_CGData[1 * _DrawType + 0].height * 0.5), _CGData[1 * _DrawType + 0].handle, TRUE);
				DrawCircleGauge(_UIc->GetPosition().x, _UIc->GetPosition().y, hp, _CGData[1 * _DrawType + 2].handle, 0);
				DrawGraph(_UIc->GetPosition().x - (_CGData[4].width * 0.5), _UIc->GetPosition().y - (_CGData[4].height * 0.5), _CGData[4].handle, TRUE);
		
		} else {
			auto time = 0;
			if (_Main) {
				time = gGlobal._MargeTimer;
				time = (time / gGlobal._MaxMargeTime) * 100;
			} else {
				time = gGlobal._DashTimer;
				time = (time / gGlobal._MaxDashTime) * 100;
			}
			
			

			DrawGraph(_UIc->GetPosition().x - (_CGData[0].width * 0.5), _UIc->GetPosition().y - (_CGData[0].height * 0.5), _CGData[0].handle, TRUE);
			DrawCircleGauge(_UIc->GetPosition().x, _UIc->GetPosition().y, time, _CGData[1].handle, 0);
			DrawGraph(_UIc->GetPosition().x - (_CGData[2].width * 0.5), _UIc->GetPosition().y - (_CGData[2].height * 0.5), _CGData[2].handle, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
