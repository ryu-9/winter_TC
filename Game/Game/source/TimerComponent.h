#pragma once
#include "appframe.h"
class TimerComponent : public Component {
public:
	TimerComponent(class ActorClass* owner,int time, int order = 100);
	~TimerComponent();
	void Update() override;

private:
	int _Cnt;
	int _Limit;
};

