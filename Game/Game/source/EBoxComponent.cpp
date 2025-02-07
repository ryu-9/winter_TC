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
			auto n = Drawing(4, 3);
			_WaitAction = static_cast<WAIT_ACTION>(n);
		}
		break;
	}
	case STATUS::WAIT: {
		switch (_WaitAction) {
		case 0:
			if (_CurrentTime == 0) { _Duration = 2000; }
			_CurrentTime += _Owner->GetMode()->GetStepTm();
			if (_CurrentTime > _Duration) {
				_CurrentTime = 0;
				_Status = STATUS::SEARCH;
			}
			break;
		case 1:
			// ˆÚ“®
			Move();
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

bool EBoxComponent::Attack() {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		// ƒvƒŒƒCƒ„[‚Ì•û‚ÉŒü‚©‚¤
		auto dir = VSub(_Target[0]->GetPosition(), _Owner->GetPosition());
		dir = VNorm(dir);
		if()
	}

	VECTOR pos = _Owner->GetPosition();


	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, 3));
	_En->GetInput()->SetVelocity(vel);



	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool EBoxComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		switch (rand() % 4) {
		case 0:
			_MoveDir = VGet(0, 0, 1);
			break;
		case 1:
			_MoveDir = VGet(0, 0, -1);
			break;
		case 2:
			_MoveDir = VGet(1, 0, 0);
			break;
		case 3:
			_MoveDir = VGet(-1, 0, 0);
			break;
		}
	}

	VECTOR pos = _Owner->GetPosition();
	

	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, 3));
	_En->GetInput()->SetVelocity(vel);



	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::SEARCH;
	}

	return false;
}

bool EBoxComponent::Roll() {

	return false;
}
