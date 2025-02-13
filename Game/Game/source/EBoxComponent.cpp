#include "EBoxComponent.h"
#include "PlayerActor.h"

EBoxComponent::EBoxComponent(ActorClass* owner) 
	:EnemyComponent(owner)
	, _WaitAction(WAIT_ACTION::NON)
{
	_Status = STATUS::SEARCH;
	_Duration = 1000;
	_CoolTime = 1000;
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
			// 移動
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
		// プレイヤーの方に向かう
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
	if (_CurrentTime > _Duration) {
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

		auto tmprot = 0.5 * PI;
		auto rot = _Owner->GetComponent<ModelComponent>()->GetRotation();
		rot.x += tmprot;
		// TODO:モデルの位置調整
		_Owner->GetComponent<ModelComponent>()->SetRotation(rot);
	}
	if (_CurrentTime > _Duration) {
		VECTOR pos = _Owner->GetPosition();


		auto vel = _En->GetInput()->GetVelocity();
		vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, 3));
		_En->GetInput()->SetVelocity(vel);
	//	Roll();
	}


	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration + _CoolTime) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
		_Status = STATUS::SEARCH;
	}

	return false;
}

bool EBoxComponent::Roll() {
	// 立方体の回転
	auto tmprot = 0.5 * PI;
	auto tmptm = 500 / _Owner->GetMode()->GetStepTm();
	auto rot = _Owner->GetComponent<ModelComponent>()->GetRotation();
	rot.x += tmprot * tmptm;
	_Owner->GetComponent<ModelComponent>()->SetRotation(rot);
	return false;
}
