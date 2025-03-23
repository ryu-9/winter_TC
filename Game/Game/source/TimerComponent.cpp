#include "TimerComponent.h"


TimerComponent::TimerComponent(ActorClass* owner,int time, int order)
	:Component(owner, order)
	, _Limit(time)
	, _Cnt(0) {
}

TimerComponent::~TimerComponent() {
}

void TimerComponent::Update() {
	_Cnt += _Owner->GetMode()->GetStepTm();
	if (_Cnt > _Limit) {
		_Owner->SetState(ActorClass::State::eDead);
	}
}
