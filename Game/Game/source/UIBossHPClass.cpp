#include "UIBossHPClass.h"
#include "ApplicationGlobal.h"

UIBossHPClass::UIBossHPClass(ModeBase* mode, VECTOR pos, const TCHAR* fname, int draworder) 
	: UIChipClass(mode, pos, fname, draworder)
{
	new UIBossHPSpriteComponent(this, draworder);
}

UIBossHPClass::~UIBossHPClass() {
}

UIBossHPSpriteComponent::UIBossHPSpriteComponent(UIChipClass* owner, int drawOrder) 
	: UIChipSpriteComponent(owner, drawOrder)
{
	
	_CGData.clear();
	SetImage("res/UI/TDX_BOSS_HP.png");
	SetImage("res/UI/TDX_BOSS_HPBAR.png");
	_CGData.back().pos = VGet(0, 9, 0);
}

UIBossHPSpriteComponent::~UIBossHPSpriteComponent() {
}

void UIBossHPSpriteComponent::Draw() {

	SetDrawBlendMode(_UIc->GetUIData()->blendMode, _UIc->GetUIData()->blendParam);
	auto hp = gGlobal._BossHP;
	auto tmpx = 0;
	auto hpp = hp / 500;
	if (hpp < 0) { hpp = 0; }
	if (hpp > 1) { hpp = 1; }
	tmpx = 1453 * hpp;
	DrawGraph(_UIc->GetPosition().x, _UIc->GetPosition().y, _CGData[0].handle, TRUE);
	DrawGraph(1920 - tmpx, _UIc->GetPosition().y + _CGData[1].pos.y, _CGData[1].handle, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
