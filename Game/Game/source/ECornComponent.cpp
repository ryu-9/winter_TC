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
			// ˆÚ“®
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
				auto rot = _Owner->GetComponent<ModelComponent>()->GetRotation();
				_Owner->GetComponent<ModelComponent>()->SetRotation(VGet(0, rot.y + 0.3, 0));
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
		flag = Attack();
		break;
	}
	default:
		break;
	}
	
	
	if (flag == true) {
	//	_Status = STATUS::SEARCH;
	}
}

bool ECornComponent::Attack() {
	if (_CurrentTime == 0) {
		_Duration = 4000;
		auto ac = new ActorClass(_Owner->GetMode());
		ac->SetPosition(_Owner->GetPosition());
		auto m = new ModelComponent(ac, "res/model/Enemy_corn/Enemy_corn.mv1");
		new MoveCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true);
		new BulletComponent(ac, _Target[_Index[0]]->GetPosition(), 1500);
		auto game = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
		if (game != nullptr) { new SEComponent(ac, game->GetPlayer()); }
		// TODO: new SEComponent(ac,cam);
		
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();

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
	auto front = _Owner->GetComponent<ModelComponent>()->GetFront();
	// TODO: ƒ‚ƒfƒ‹‚ÌŒü‚«’²®
	// ‰¼‚Åƒtƒƒ“ƒg‚ð”½“]
	front = VScale(front, -1);
	
	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0,vel.y,0), VScale(front, d));
	_En->GetInput()->SetVelocity(vel);

	_CurrentTime += t;
	if (_CurrentTime >= _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::SEARCH;
	}
	

	return true;
}

void ECornComponent::Jump() {
	if (_CurrentTime == 0) {
		_En->GetInput()->SetVelocity(VGet(0, 10, 0));
	}

	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
}

