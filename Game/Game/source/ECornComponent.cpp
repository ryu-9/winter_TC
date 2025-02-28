#include "ECornComponent.h"
#include "BulletComponent.h"
#include "PlayerActor.h"

ECornComponent::ECornComponent(ActorClass* owner)
	:EnemyComponent(owner)
{
	_Duration = 1000;
	_Status = EnemyComponent::SEARCH;

	_Target.push_back(_En->GetMode()->GetPlayer(0));
	_Target.push_back(_En->GetMode()->GetPlayer(1));
	_Weight.push_back(4);
	_Weight.push_back(4);
	_Weight.push_back(2);

	_AttackType = rand() % 3;
}

ECornComponent::~ECornComponent() {
}

void ECornComponent::ProcessInput() {
	bool flag = false;
	switch (_Status) {
		
	case STATUS::NON:
		break; 
	case STATUS::SEARCH: {
		if (Search(_Target)) { _Status = STATUS::DISCOVER; }
		else { _Status = STATUS::WAIT;
		auto n = Drawing(_Weight);
		_WaitAction = static_cast<WAIT_ACTION>(n);
		}

		break;
	}
	case STATUS::WAIT: {
		switch (_WaitAction) {
		case 1:
			// 移動
			Move();
			break;
		case 2:
			Jump();
			break;
		case 0:
			if (_CurrentTime == 0) { _Duration = 2000; }
			_CurrentTime += _Owner->GetMode()->GetStepTm();
			if (_CurrentTime > _Duration) {
				_CurrentTime = 0;
				auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
				_Owner->GetComponent<ModelComponent>()[0]->SetRotation(VGet(0, rot.y + 0.3, 0));
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
	case STATUS::ATTACK: {
		switch (_AttackType) {
		case 0:
			NomalAttack();
			break;
		case 1:
			JumpAttack();
			break;
		case 2:
			FrontAttack();
			break;
		case 3:
			BackAttack();
			break;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();
	
}

bool ECornComponent::Attack(int n) {
	
		auto ac = new ActorClass(_Owner->GetMode());
		ac->SetPosition(_Owner->GetPosition());
		auto m = new ModelComponent(ac, "res/model/Enemy_corn/Enemy_corn.mv1");
		new HitCollisionComponent(ac, m, VGet(0, 0, 0), VGet(30, 30, 30), 2, true);
		new MoveCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true);
		new BulletComponent(ac, _Target[_Index[n]]->GetPosition(), 1000);
		auto game = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
		if (game != nullptr) { new SEComponent(ac, game->GetPlayer()); }
		// TODO: new SEComponent(ac,cam);


	return true;
}

bool ECornComponent::NomalAttack() {
	if (_CurrentTime == 0) {
		_Duration = 4000;
		Attack(0);
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool ECornComponent::JumpAttack() {
	if (_CurrentTime == 0) {
		_Duration = 4000;
		Jump();
		Attack(0);
	}
	

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool ECornComponent::FrontAttack() {
	if (_CurrentTime == 0) {
		_Duration = 4000;
	}
	if (_CurrentTime < 2000) {
		if (GoTo(0)) { Attack(0); }
	}
	
	

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool ECornComponent::BackAttack() {
	if (_CurrentTime == 0) {
		_Duration = 4000;
	}
	if (_CurrentTime < 2000) {
		if (GoTo(1)) { Attack(0); }
	}
	

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
	return true;
}

bool ECornComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		_MoveDist = 200 / (float)_Duration;
	}
	auto t = _Owner->GetMode()->GetStepTm();
	auto d = _MoveDist * t;
	auto front = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
	// TODO: モデルの向き調整
	// 仮でフロントを反転
	front = VScale(front, -1);
	
	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0,vel.y,0), VScale(front, d));
	_En->GetInput()->SetVelocity(vel);

	_CurrentTime += t;
	if (_CurrentTime >= _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::SEARCH;
		return true;
	}
	

	return false;
}

bool ECornComponent::GoTo(int n) {
	if (_CurrentTime == 0) {
		_Duration = 1000;
		_MoveDist = 200 / (float)_Duration;

		// プレイヤーの方を向く
		auto pos = _Owner->GetPosition();
		auto target = _Target[n]->GetPosition();
		auto dir = VSub(target, pos);
		auto front = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
		auto rot = VGet(0, atan2(dir.x, dir.z), 0);
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);
	}

	auto t = _Owner->GetMode()->GetStepTm();
	auto d = _MoveDist * t;
	auto front = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
	if (n == 0) { front = VScale(front, -1); }
	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0, vel.y, 0), VScale(front, d));
	_En->GetInput()->SetVelocity(vel);

	
	if (_CurrentTime >= _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::SEARCH;
		return true;
	}

	return false;
}

void ECornComponent::Jump() {
	if (_CurrentTime == 0) {
		_En->GetInput()->SetVelocity(VGet(0, 3, 0));
	}


	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
}

