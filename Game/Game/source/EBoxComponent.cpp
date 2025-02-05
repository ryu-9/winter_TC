#include "EBoxComponent.h"
#include "PlayerActor.h"

EBoxComponent::EBoxComponent(ActorClass* owner) 
	:EnemyComponent(owner)
	, _WaitAction(WAIT_ACTION::NON)
{
	_Status = STATUS::SEARCH;
	_Duration = 1000;
	_Target.push_back(_En->GetMode()->GetPlayer(0));
	_Target.push_back(_En->GetMode()->GetPlayer(1));

}


EBoxComponent::~EBoxComponent() {}

void EBoxComponent::ProcessInput() {
	bool flag = false;
	switch (_Status) {
	case STATUS::NON:
		break;
	case STATUS::SEARCH: {
		if (Search(_Target)) { _Status = STATUS::DISCOVER; } else {
			_Status = STATUS::WAIT;
			auto n = Drawing(4, 1, 3);
			_WaitAction = static_cast<WAIT_ACTION>(n);
		}
		break;
	}
	case STATUS::WAIT: {
		switch (_WaitAction) {
		case 1:
			// ˆÚ“®
			Move();
			break;
		case 0:
			if (_CurrentTime == 0) { _Duration = 2000; }
			_CurrentTime += _Owner->GetMode()->GetStepTm();
			if (_CurrentTime > _Duration) {
				_CurrentTime = 0;
				_Status = STATUS::SEARCH;
			}
			break;
		default:
			break;
		}
	}
	break;
	case STATUS::DISCOVER:
		_Status = STATUS::ATTACK;
		_CurrentTime = 0;
		break;

	case STATUS::ATTACK:
		break;
	default:
		break;
	}

}

bool EBoxComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		auto d = rand() % 4;
	}

	return false;
}

bool EBoxComponent::Roll() {

	return false;
}
