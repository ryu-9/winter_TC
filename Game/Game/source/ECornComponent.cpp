#include "ECornComponent.h"
#include "BulletComponent.h"
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "appframe.h"
#include <iostream>
#include "ModeGame.h"
#include "CameraActor.h"
#include "GroupAttackActor.h"


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

	_AttackType = 2;

	_MoveData.duration = 1000;
	_MoveData.dur_rand = 500;
	_MoveData.cooltime = 1000;
	_MoveData.cool_rand = 500;
	_MoveData.dist = 200;
	_MoveData.dist_rand = 100;

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
		_CurrentTime = 0;
		}

		break;
	}
	case STATUS::WAIT: {
		switch (_WaitAction) {
		case 1:
			// �ړ�
			Move();
			break;
		case 2:
			Jump();
			break;
		case 0:
			{	if (_CurrentTime == 0) {
				_Duration = 2000;
				auto p = _En->GetGroupSpawner();
				if (p != nullptr) {
					// ���S�����

					auto pos = _Owner->GetPosition();
					auto target = p->GetPosition();
					auto dir = VSub(pos, target);
					auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
					_Rot = atan2(dir.x, dir.z);
					_Rot = _Rot - rot.y;
				}
			}
			auto tm = _Owner->GetMode()->GetStepTm();
			_CurrentTime += tm;
			auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
			rot.y += _Rot * ((float)tm / (float)_Duration);
			_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);
			if (_CurrentTime > _Duration) {
				_CurrentTime = 0;
				_Status = STATUS::SEARCH;
			}
			break;
		}
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
	case STATUS::COOLTIME:
		CoolTime();
		break;
	default:
		break;
	}
	
	
}

bool ECornComponent::Attack(int n) {

	
		auto ac = new ActorClass(_Owner->GetMode());
		ac->SetPosition(_Owner->GetPosition());
		auto m = new ModelComponent(ac, "res/model/Enemy_corn/Enemy_corn.mv1");
		m->SetVisible(false);
		new EffectSpriteComponent(ac, "res/model/Fire_MINI/fire_mini_002.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), 50, false, 0.3);
		new HitCollisionComponent(ac, m, VGet(0, 0, 0), VGet(30, 30, 30), 2, true);
		new MoveCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true);
		new BulletComponent(ac, _Target[_Index[n]]->GetPosition(), 1000);
		auto pos = _Owner->GetPosition();
		auto target = _Target[n]->GetPosition();
		auto dir = VSub(pos, target);
		auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
		rot.y = atan2(dir.x, dir.z);
		_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);
		SoundServer::GetInstance()->Create(ac, "cornfire", "AttackSE", "cornfire");
		auto sv = SoundServer::GetInstance()->GetSourceVoice(ac, "cornfire");
		auto p = new SVItemPanning(sv);
		auto g = dynamic_cast<ModeGame*>(_Owner->GetMode());
		p->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
		auto di = new SVItemDistanceDecay(sv);
		di->SetListener(dynamic_cast<ActorClass*>(g->GetCamera()));
		sv->Play();
		

	return true;
}

bool ECornComponent::NomalAttack() {
	if (_CurrentTime == 0) {
		auto r = rand() % 1000;
		_Duration = 2000 + r;
		Attack(0);
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::COOLTIME;
	}
	return true;
}

bool ECornComponent::JumpAttack() {
	if (_CurrentTime == 0) {
		auto r = rand() % 1000;
		_Duration = 2000 + r;
		Jump();
		
		Attack(0);
	}
	
	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::COOLTIME;
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
	
	auto tm = _Owner->GetMode()->GetStepTm();
	_CurrentTime += tm;
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::COOLTIME;
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
	_CurrentTime += _Owner->GetMode()->GetStepTm();

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::COOLTIME;
	}
	return true;
}

bool ECornComponent::CoolTime() {
	if (_CurrentTime == 0) {
		_Duration = _MoveData.cooltime + rand() % _MoveData.cool_rand;
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();
	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::SEARCH;
	}
	return false;
}

bool ECornComponent::Move() {
	if (_CurrentTime == 0) {
		_Duration = _MoveData.duration + rand() % _MoveData.dur_rand;
		float dur = static_cast<float>(_Duration);
		_MoveDist = (_MoveData.dist + (rand() % _MoveData.dist_rand)) / dur;

	}
	auto t = _Owner->GetMode()->GetStepTm();
	auto d = _MoveDist * t;
	auto front = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
	// TODO: ���f���̌�������
	// ���Ńt�����g�𔽓]
	front = VScale(front, -1);

	auto vel = _En->GetInput()->GetVelocity();
	vel = VAdd(VGet(0, vel.y, 0), VScale(front, d));
	_En->GetInput()->SetVelocity(vel);

	_CurrentTime += t;
	if (_CurrentTime >= _Duration) {
		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
		_Status = STATUS::COOLTIME;
		return true;
	}

	return false;
}


bool ECornComponent::GoTo(int n) {
	if (_CurrentTime == 0) {
		_Duration = _MoveData.duration + rand() % _MoveData.dur_rand;
		float dur = static_cast<float>(_Duration);
		_MoveDist = (_MoveData.dist + (rand() % _MoveData.dist_rand)) / dur;

		// �v���C���[�̕������
		auto pos = _Owner->GetPosition();
		auto target = _Target[n]->GetPosition();
		auto dir = VSub(pos,target);
		auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();
		rot.y = atan2(dir.x, dir.z);
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
//		_CurrentTime = 0;
		_En->GetInput()->SetVelocity(VGet(0, 0, 0));
//		_Status = STATUS::COOLTIME;
		return true;
	}

	return false;
}

void ECornComponent::Jump() {
	if (_CurrentTime == 0) {
		_En->GetInput()->SetVelocity(VGet(0, 4, 0));
	}
	_CurrentTime += _Owner->GetMode()->GetStepTm();

	if (_CurrentTime > _Duration) {
		_CurrentTime = 0;
		_Status = STATUS::COOLTIME;
	}
}

