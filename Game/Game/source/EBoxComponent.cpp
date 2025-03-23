#include "EBoxComponent.h"
#include "PlayerActor.h"

EBoxComponent::EBoxComponent(ActorClass* owner) 
	:EnemyComponent(owner)
	, _WaitAction(WAIT_ACTION::NON)
	, _AddRot(VGet(0, 0, 0))
	, _MoveDir(VGet(0, 0, 0))
	, _StartRot(VGet(0, 0, 0))
{
	_SearchRef.dist = 500;
	_Status = STATUS::SEARCH;
	_Duration = 1000;
	_CoolTime = 1000;
	_MFront = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
	_MLeft = VCross(_MFront, VGet(0, 1, 0));
	_RollAngle = 0.0f;
	_Target.push_back(_En->GetMode()->GetPlayer(0));
	_Target.push_back(_En->GetMode()->GetPlayer(1));
	_Weight.push_back(3);
	_Weight.push_back(4);
	_HalfSize = 50;
	_Slanting = sqrtf(_HalfSize * _HalfSize + _HalfSize * _HalfSize);
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
		// 移動方向をランダムに決定
		auto dir = rand() % 4;
		_StartRot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
		switch (dir % 2) {
		case 0:		// 左右
			_RollAngle = 0;
			_AddRot = VScale(_MFront, (dir - 1) * 0.5 * PI);
			_MLeft = VTransformSR(_MLeft, MGetRotAxis(_MFront, -1 * (dir - 1) * 0.5 * PI));
			_MLeft = VNorm(_MLeft);

			if (dir == 0) {
				_MoveDir = VGet(1, 0, 0);
			} else {
				_MoveDir = VGet(-1, 0, 0);
			}
			break;
		case 1:		// 前後
			_AddRot = VScale(_MLeft, (dir - 2) * 0.5 * PI);

			if (dir == 1) {
				_MoveDir = VGet(0, 0, 1);
			} else {
				_MoveDir = VGet(0, 0, -1);
			}
			break;
		}

		_MoveDist = ((float)_HalfSize * 2.0f) / (float)_Duration;
	}

	if (_CurrentTime < _Duration) {
		auto tm = _Owner->GetMode()->GetStepTm();
		// イージング回転

		auto rot = VAdd(_StartRot, VScale(_AddRot, (float)_CurrentTime / (float)_Duration));
		_RollAngle = ((float)_CurrentTime / (float)_Duration) * (0.5 * PI);
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);


		// 移動
		auto vel = _En->GetInput()->GetVelocity();
		// 回転移動分のベクトルを加算
		auto y = _Slanting * (sinf(_RollAngle + (0.25 * PI)));

		vel = VAdd(VGet(0, vel.y, 0), VScale(_MoveDir, (_MoveDist * (float)tm)));
		_En->GetInput()->SetVelocity(vel);
		

	}
	if (_CurrentTime > _Duration) {
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
	}

	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration + _CoolTime) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(VAdd(_StartRot, _AddRot));
		_Status = STATUS::SEARCH;
	}

	return false;
}

bool EBoxComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		// 移動方向をMfront基準でランダムに決定
		auto dir = rand() % 4;
		_StartRot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
		switch (dir % 2) {
		case 0:		// 左右
			_RollAngle = 0;
			_AddRot = VScale(_MFront, (dir - 1) * 0.5 * PI);
			_MLeft = VTransformSR(_MLeft, MGetRotAxis(_MFront, -1 * (dir - 1) * 0.5 * PI));
			_MLeft = VNorm(_MLeft);

			if (dir == 0) {
				_MoveDir = VGet(1, 0, 0);
			} else {
				_MoveDir = VGet(-1, 0, 0);
			}
			break;
		case 1:		// 前後
			_AddRot = VScale(_MLeft, (dir - 2) * 0.5 * PI);

			if (dir == 1) {
				_MoveDir = VGet(0, 0, 1);
			} else {
				_MoveDir = VGet(0, 0, -1);
			}
			break;
		}
		_MoveDist = ((float)_HalfSize * 2.0f) / (float)_Duration;
	}

	if (_CurrentTime < _Duration) {
		auto tm = _Owner->GetMode()->GetStepTm();
		// イージング回転

		auto rot = VAdd(_StartRot, VScale(_AddRot, (float)_CurrentTime / (float)_Duration));
		_RollAngle = ((float)_CurrentTime / (float)_Duration) * (0.5 * PI);
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);


		// 移動
		auto vel = _En->GetInput()->GetVelocity();
		// 回転移動分のベクトルを加算
		auto y = _Slanting * (sinf(_RollAngle + (0.25 * PI)))-_HalfSize;
		vel = VAdd(VGet(0, y / tm, 0), VScale(_MoveDir, (_MoveDist * (float)tm)));
		_En->GetInput()->SetVelocity(vel);
		
		// TODO: 落下中対応
	}
	if (_CurrentTime > _Duration) {
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
	}

	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration + _CoolTime) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, _En->GetInput()->GetVelocity().y, 0));
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(VAdd(_StartRot, _AddRot));
		auto n = rand() % 2;
		std::string se = "boxwalk";
		se += std::to_string(n);
		SoundServer::GetInstance()->Create(_Owner, se, "AttackSE", se);
		SoundServer::GetInstance()->GetSourceVoice(_Owner, se)->Play();
		_Status = STATUS::SEARCH;
	}

	return false;
}


