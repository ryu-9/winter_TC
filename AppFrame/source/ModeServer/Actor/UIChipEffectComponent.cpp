#include "UIChipEffectComponent.h"


UIChipEffectComponent::UIChipEffectComponent(ActorClass* owner, int updateOrder)
	:Component(owner, updateOrder)
{
	_UIChip = dynamic_cast<UIChipClass*>(owner);
}




UIChipFadeComponent::UIChipFadeComponent(ActorClass* owner, float alpha, int tm, int updateOrder)
	:base(owner, updateOrder)
{
	_FadeData.start = _UIChip->GetUIData()->blendParam;
	_FadeData.tm = tm;
	_FadeData.end = alpha;
}

UIChipFadeComponent::~UIChipFadeComponent()
{
}

void UIChipFadeComponent::ProcessInput() {
	_FadeData.cnt += _Owner->GetMode()->GetStepTm();
	_UIChip->GetUIData()->blendParam = (_FadeData.end - _FadeData.start) * ((float)_FadeData.cnt / (float)_FadeData.tm) + _FadeData.start;
	if (_FadeData.cnt>= _FadeData.tm) {
		_UIChip->GetUIData()->blendParam = _FadeData.end;
		_Owner->RemoveComponent(this);
	}
}




