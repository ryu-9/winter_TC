#pragma once
#include "../Component.h"
#include "UIChipClass.h"

class UIChipEffectComponent : public Component {
public:
	UIChipEffectComponent(class ActorClass* owner, int updateOrder = 100);
	virtual ~UIChipEffectComponent() {};

protected:
	UIChipClass* _UIChip;
};


class UIChipFadeComponent : public UIChipEffectComponent
{
	typedef UIChipEffectComponent base;

	struct  EASING
	{
		float start;
		float end;
		int cnt;
		int tm;
	};
public:
	UIChipFadeComponent(class ActorClass* owner, float alpha,int tm, int updateOrder = 100);
	virtual ~UIChipFadeComponent();

	void ProcessInput() override;

private:
	EASING _FadeData;
};

