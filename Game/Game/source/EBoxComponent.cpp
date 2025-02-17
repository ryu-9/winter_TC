#include "EBoxComponent.h"
#include "PlayerActor.h"

EBoxComponent::EBoxComponent(ActorClass* owner) 
	:EnemyComponent(owner)
	, _WaitAction(WAIT_ACTION::NON)
{
	_SearchRef.dist = 10;
	_Status = STATUS::SEARCH;
	_Duration = 1000;
	_CoolTime = 1000;
	_MFront = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
	_MUp = _Owner->GetComponent<ModelComponent>()[0]->GetUp();
	_RollAngle = 0.0f;
	_Target.push_back(_En->GetMode()->GetPlayer(0));
	_Target.push_back(_En->GetMode()->GetPlayer(1));
	_Weight.push_back(3);
	_Weight.push_back(4);
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
			auto n = Drawing(_Weight);
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
		Attack();
		break;
	default:
		break;
	}

}

bool EBoxComponent::Attack() {
	if (_CurrentTime == 0) {
		_Duration = 500;
		// ƒvƒŒƒCƒ„[‚Ì•û‚ÉŒü‚©‚¤
		auto dir = VSub(_Target[_Index[0]]->GetPosition(), _Owner->GetPosition());
		dir = VNorm(dir);
		if (fabs(dir.x) < fabs(dir.z)) {
			if (dir.x > 0) {
				_MoveDir = VGet(1, 0, 0);
			} else {
				_MoveDir = VGet(-1, 0, 0);
			}
		} else {
			if (dir.z > 0) {
				_MoveDir = VGet(0, 0, 1);
			} else {
				_MoveDir = VGet(0, 0, -1);
			}
		}

	}
	if (_CurrentTime < _Duration) {
		VECTOR pos = _Owner->GetPosition();
		auto vel = _En->GetInput()->GetVelocity();
		vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, 3));
		_En->GetInput()->SetVelocity(vel);
		//Roll();
	}

	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration + _CoolTime) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool EBoxComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = 500;
		// ˆÚ“®•ûŒü‚ðMfrontŠî€‚Åƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
		auto dir = rand() % 4;
		auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
		switch (dir %2) {
		case 0:
			rot = VAdd(rot, VScale(_MFront, (dir - 1) *0.5 * PI));
			_MUp = VTransform(_MUp, MGetRotAxis(_MFront, (dir - 1) * 0.5 * PI));
			break;
		case 1:
			auto v = VCross(_MFront, _MUp);
			rot = VAdd(rot, VScale(v, (dir - 2) * 0.5 * PI));
			break;
		}
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);

	}
	if (_CurrentTime < _Duration) {
		VECTOR pos = _Owner->GetPosition();


		auto vel = _En->GetInput()->GetVelocity();
		vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, 3));
		_En->GetInput()->SetVelocity(vel);
//		Rotate_Move();
	}


	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration + _CoolTime) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
		_Status = STATUS::SEARCH;
	}

	return false;
}


bool EBoxComponent::Rotate_Move() {
	// —§•û‘Ì‚Ì‰ñ“]
	auto tmprot = 0.5 * PI * 0.002;
	// TODO: ‰ñ“]•b”‚ð‰Â•Ï‚É
	auto tmptm = _Owner->GetMode()->GetStepTm();
	auto tmpangle = _RollAngle + (tmprot * tmptm);
	if (tmpangle > 0.5 * PI) {
		_RollAngle = 0.0f;
	} else {
		auto vecAxiz = VGet(0,0,0);
		if (_MFront.x != 0) {
			vecAxiz = VGet(0, 0, 1);
		}
		if (_MFront.z != 0) {
			vecAxiz = VGet(1, 0, 0);
		}
		auto rot = VTransform(_Owner->GetComponent<ModelComponent>()[0]->GetFront(), MGetRotAxis(vecAxiz, tmprot));
		_RollAngle = tmpangle;
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);
	}
	
	return false;
}
