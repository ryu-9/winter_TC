#include "ECornComponent.h"
#include "BulletComponent.h"
#include "PlayerActor.h"

ECornComponent::ECornComponent(ActorClass* owner)
	:EnemyComponent(owner)
{
	_Duration = 1000;
	_Status = EnemyComponent::WAIT;
	_Target.push_back(static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer(0));
	_Target.push_back(static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer(1));

}

ECornComponent::~ECornComponent() {
}

void ECornComponent::ProcessInput() {
	bool flag = false;
	switch (_Status) {
		
	case STATUS::NON:
		break; 
	case STATUS::SEARCH: {
		if (Search(_Target)) { _Status = STATUS::DISCOVERY; }
		else { _Status = STATUS::WAIT; }

		break;
	}
	case STATUS::WAIT: {
		auto n = Drawing(4, 4, 3);
		switch (n) {
		case 1:
			// ˆÚ“®
			Move();
			break;
		case 2:
			Jump();
			break;
		case 0:
			break;
		default:
			break;
		}
	}
		break;
	case STATUS::DISCOVERY:
		_Status = STATUS::ATTACK;
		break;
	case STATUS::ATTACK: {
		flag = Attack();
		break;
	}
	default:
		break;
	}
	
	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (flag == true) {
	//	_Status = STATUS::SEARCH;
	}
}

bool ECornComponent::Attack() {
	auto ac = new ActorClass(_Owner->GetMode());
	ac->SetPosition(_Owner->GetPosition());
	auto m = new ModelComponent(ac, "res/model/Enemy_corn.mv1");
	new MoveCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true);
	new BulletComponent(ac, static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition(), 1500);
	auto game = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
	new SEComponent(ac, game->GetPlayer());
	_Status = STATUS::SEARCH;
	return true;
}

void ECornComponent::Jump() {
	if (_CurrentTime == 0) {
		_En->GetInput()->SetVelocity(VGet(0, 10, 0));
	}
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
}

