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
	void Update() override;

private:
	EASING _Data;
};

class UIChipFocusComponent : public UIChipEffectComponent {
	typedef UIChipEffectComponent base;

	struct  EASING {
		VECTOR start;
		VECTOR end;
		int cnt;
		int tm;
	};

public:
	UIChipFocusComponent(class ActorClass* owner, VECTOR scale, int tm, int updateOrder = 100);
	virtual ~UIChipFocusComponent();

	void ProcessInput() override;
	void Update() override;
	void Receive(int message) override;
private:
	EASING _Data;
	bool _isFocus;
};

class UIChipMoveComponent : public UIChipEffectComponent {
	typedef UIChipEffectComponent base;
	struct  EASING {
		VECTOR start;
		VECTOR end;
		int cnt;
		int tm;
	};

public:
	UIChipMoveComponent(class ActorClass* owner, VECTOR pos, int tm, int updateOrder = 100);
	virtual ~UIChipMoveComponent();

	void ProcessInput() override;
	void Update() override;

private:
	EASING _Data;
};

class UIChipScaleComponent : public UIChipEffectComponent {
	typedef UIChipEffectComponent base;
	struct  EASING {
		VECTOR start;
		VECTOR end;
		int cnt;
		int tm;
	};

public:
	UIChipScaleComponent(class ActorClass* owner, VECTOR scale, int tm, int updateOrder = 100);
	virtual ~UIChipScaleComponent();
	void ProcessInput() override;
	void Update() override;

private:
	EASING _Data;
};