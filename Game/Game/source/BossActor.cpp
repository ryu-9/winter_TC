#include "BossActor.h"
#include "PlayerActor.h"
#include "BossAttackActor.h"
#include "GoalItemActor.h"
#include "ApplicationGlobal.h"
#include "LaserActor.h"
#include "SlashActor.h"
#include "PunchActor.h"
#include "DaikanpaActor.h"
#include "ItemActor.h"
#include "EnemyActor.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"
#include "EnemyCreator.h"

BossActor::BossActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
	, _Action(WAIT)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _CurrentTime(0)
	, _TimelineIndex(0)
	, _ActionIndex(0)
	, _ActTime(0)
	, _ActTotalTime(0)
	, _HitPoint(5000)
	, _Invincible(-1)
{
	SetPosition(pos);
	_Input = new MoveComponent(this);
	SetSize(VGet(20, 20, 20));

	// �A�j���[�V�����擾�p���f���̓ǂݍ���
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/reiizo-beam.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dankanha_motion.mv1"));
	_AnimMV1.push_back(0);
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/hirumimotion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));

	// �A�j���[�V�������̐ݒ�
	for (auto i = 0; i < _AnimMV1.size(); i++) {
		_AnimName.push_back(MV1GetAnimName(_AnimMV1[i], 0));
	}

	// ���f���̓ǂݍ���
	_Model[0] = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");
	

	{
		auto index = MV1GetAnimIndex(_AnimMV1[_Action], _AnimName[0]);
		_AnimIndex.push(MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[_Action], TRUE));
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.front(), _AnimTime);
		_Input->SetGravity(false);
	}

	// �^�C�����C���̐ݒ�
	std::vector<ACTION_TIMELINE> timeline;
	timeline.push_back({ ACTION::PUNCH, 10000 });
	timeline.push_back({ ACTION::PUNCH, 10000 });
	timeline.push_back({ ACTION::BULLET, 12000 });
	_ActionTimeline.push_back(timeline);

	timeline.clear();
	timeline.push_back({ ACTION::PUNCH_FALL, 8000 });
	timeline.push_back({ ACTION::PUNCH, 17000 });
	timeline.push_back({ ACTION::PUNCH, 9000 });
	timeline.push_back({ ACTION::BULLET, 8000 });
	_ActionTimeline.push_back(timeline);

	timeline.clear();
	timeline.push_back({ ACTION::BEAM, 8000 });
	timeline.push_back({ ACTION::PUNCH, 7000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });
	timeline.push_back({ ACTION::PUNCH_FALL, 5000 });
	timeline.push_back({ ACTION::PUNCH, 8000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });
	_ActionTimeline.push_back(timeline);

	timeline.clear();
	timeline.push_back({ ACTION::BEAM, 7000 });
	timeline.push_back({ ACTION::PUNCH, 7000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });
	timeline.push_back({ ACTION::BULLET, 5000 });

	_ActionTimeline.push_back(timeline);

	gGlobal._BossHP = _HitPoint;

	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 100, 0), VGet(50, 50, 50), 2, true, true);
}

BossActor::~BossActor() {
}

void BossActor::UpdateActor() {
	(this->*_ActionFunc[(int)_Action])();
	if (_Invincible > 0) {
		_Invincible -= (float)GetMode()->GetStepTm();
	}
	if (_Action != ACTION::DAMAGE) {
		_CurrentTime += (float)GetMode()->GetStepTm();
	}
	
	if (_HitPoint > 0 ) {

		if (_CurrentTime >= _ActionTimeline[_TimelineIndex][_ActionIndex].time) {
			ChangeAnim(_ActionTimeline[_TimelineIndex][_ActionIndex].action);
			ChangeAction(_ActionTimeline[_TimelineIndex][_ActionIndex].action);
			if (_ActionIndex < _ActionTimeline[_TimelineIndex].size() - 1) {
				_ActionIndex++;
				_CurrentTime = 0;
			} else {
				_ActionIndex = 0;
				_CurrentTime = 0;

			}

		}
	}

	if (_AnimIndex.size() > 1) {
		_AnimRate -= 0.01f;
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.front(), _AnimRate);
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.back(), 1 - _AnimRate);
		if (_AnimRate <= 0) {
			MV1DetachAnim(_Model[0]->GetHandle(), _AnimIndex.front());
			_AnimIndex.pop();
			_AnimRate = 1.f;
		}
	}


	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.back(), _AnimTime);

	if (_Action == DIE || _Action == DAMAGE) {
		return;
	}

	// 当たり判定
	
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto punch = dynamic_cast<PunchActor*>(h->GetOwner());
		if (punch != nullptr) {
			_HitPoint -= 20;
			gGlobal._BossHP = _HitPoint;
			punch->SetState(State::eDead);
			break;
		}
		auto laser = dynamic_cast<LaserActor*>(h->GetOwner());
		if (laser != nullptr) {
			_HitPoint -= 1;
			gGlobal._BossHP = _HitPoint;
			laser->SetState(State::eDead);
			break;
		}
		auto slash = dynamic_cast<SlashActor*>(h->GetOwner());
		if (slash != nullptr) {
			_HitPoint -= 20;
			gGlobal._BossHP = _HitPoint;
			slash->SetState(State::eDead);
			break;
		}
		auto daik = dynamic_cast<DaikanpaActor*>(h->GetOwner());
		if (daik != nullptr) {
			_HitPoint -= 20;
			gGlobal._BossHP = _HitPoint;
			_Invincible = 400;
			break;
		}
		auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (p != nullptr) {
			p->Damage(0.05);
		}
	}
	if (_HitPoint <= 4000 && _TimelineIndex==0) {
		ChangeAnim(ACTION::DAMAGE);
		ChangeAction(ACTION::DAMAGE);
		_TimelineIndex = 1;
		_HitPoint = 4000;
		auto pos = GetPosition();
		pos.y  = 300;
		pos.z = 600;
		new ItemActor(GetMode(), pos, 11);
		pos.x += 100;
		new ItemActor(GetMode(), pos, 11);
		pos.x -= 200;
		new ItemActor(GetMode(), pos, 11);
		_ActionIndex = 0;
		_CurrentTime = 0;
		GenerateEnemy(7);
	} else if (_HitPoint <= 2500 && _TimelineIndex == 1) {
		ChangeAnim(ACTION::DAMAGE);
		ChangeAction(ACTION::DAMAGE);
		_TimelineIndex = 2;
		_HitPoint = 2500;
		auto pos = GetPosition();
		pos.y = 300;
		pos.z = 600;
		new ItemActor(GetMode(), pos, 12);
		pos.x += 100;
		new ItemActor(GetMode(), pos, 12);
		pos.x -= 200;
		new ItemActor(GetMode(), pos, 12);
		_ActionIndex = 0;
		_CurrentTime = 0;
		GenerateEnemy(7);
	} else if (_HitPoint <= 500 && _TimelineIndex == 2) {
		ChangeAnim(ACTION::DAMAGE);
		ChangeAction(ACTION::DAMAGE);
		_TimelineIndex = 3;
		_HitPoint = 500;

		GetMode()->GetPlayer(0)->SetItemNum(3);
		_ActionIndex = 0;
		_CurrentTime = 0;
		GenerateEnemy(7);
	}
	if (_HitPoint == 0) {
		ChangeAnim(ACTION::DIE);
		ChangeAction(ACTION::DIE);
		
	}

}



bool BossActor::ChangeAnim(ACTION a) {
	if (_Action == a) { return false; }
	int index = -1;
	bool changeSucFlag = false;
	index = MV1GetAnimIndex(_AnimMV1[a], _AnimName[a]);
	_AnimIndex.push(MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE));
	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.back());
	changeSucFlag = true;
	if (changeSucFlag) {
		_AnimRate = 1.f;
		_Action = a;
		_AnimTime = 0;
		return true;
	}

	return false;
}

bool BossActor::ChangeAction(ACTION a) {
	if (_Action == a) { return false; }
	
	switch (a) {
	case BossActor::WAIT:
		break;
	case BossActor::PUNCH:
		break;
	case BossActor::BULLET:
		break;
	case BossActor::BEAM:
		break;
	case BossActor::DAMAGE:
		break;
	case BossActor::ACTION_COUNT:
		break;
	default:
		break;
	}
	_Action = a;
	_GenerateCnt = 0;
	return false;
}

bool BossActor::Rotate() {

	return false;
}

bool BossActor::Wait() {
	auto t = (float)GetMode()->GetStepTm();
	_AnimTime += t / 20.f;
	if (_AnimTime > _AnimTotalTime) {
		_AnimTime -= _AnimTotalTime;
	}
	return false;
}

bool BossActor::Punch() {
	auto t = (float)GetMode()->GetStepTm();
	if (_AnimTime < 23) {
		_AnimTime += t / 25.f;
	} else if (_AnimTime < 25) {
		_AnimTime += t / 50.f;
	} else {
		_AnimTime += t / 25.f;
	}
	
	if (_AnimTime > 25 && _CurrentTime <3000) {
		_AnimTime = 23;
	}
	if (_AnimTime > 28 && _GenerateCnt==0) {
		GeneratePunch();
	}
	
	
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		_GenerateCnt = 0;
		return true;
	}
	return false;
}

bool BossActor::Bullet() {
	auto t = (float)GetMode()->GetStepTm();
	if (_AnimTime < 50 && _CurrentTime < 3500) {
		_AnimTime += t / 20.f;
	} else {
		_AnimTime += t / 20.f;
	}
	if (_AnimTime > 50 && _CurrentTime < 3000) {
		_AnimTime = 47;
	}
	if (_CurrentTime > 2000 && _GenerateCnt == 0) {
		GenerateBullet();
	}
	if (_CurrentTime > 2500 && _GenerateCnt == 1) {
		GenerateBullet();
	}


	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT); _GenerateCnt = 0;
		return true;
	}
	return false;
}

bool BossActor::Beam() {
	auto t = (float)GetMode()->GetStepTm();
	_AnimTime += t / 40.f;
	if (_CurrentTime > 2800 && _GenerateCnt == 0) {
		GenerateBeam();
	}
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		_GenerateCnt = 0;
		return true;
	}
	return false;
}

bool BossActor::PunchFall() {
	float c = _CurrentTime / 100;
	if (_CurrentTime < 2000) {
		SetPosition(VGet(GetPosition().x, GetPosition().y + (c * c) * 20, GetPosition().z));
	}
	if (_CurrentTime >= 2000 && _CurrentTime <= 15000) {
		if (((_CurrentTime - 2000) % 1800) <  10) {
			GeneratePunchFall();
		}
	} else {
		SetPosition(VGet(GetPosition().x, GetPosition().y - 800, GetPosition().z));
		if (GetPosition().y < -1800) {
			SetPosition(VGet(GetPosition().x, -1800, GetPosition().z));
		}
	}
	if (_CurrentTime > 17000) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		SetPosition(VGet(GetPosition().x, -1800, GetPosition().z));
		_GenerateCnt = 0;
		return true;
	}
	return false;
}

bool BossActor::Damage() {
	if (_AnimTime < _AnimTotalTime) {
		auto t = (float)GetMode()->GetStepTm();
		_AnimTime += t / 18.f;
		if (_AnimTime > _AnimTotalTime ) {
			ChangeAnim(ACTION::WAIT);
			ChangeAction(ACTION::WAIT);
			return true;
		}
	}
	return false;
}

bool BossActor::Die() {
	if (_AnimTime < _AnimTotalTime) {
		auto t = (float)GetMode()->GetStepTm();
		_AnimTime += t / 20.f;
		if (_AnimTime > _AnimTotalTime) {
			_AnimTime = _AnimTotalTime;
			auto g = new GoalItemActor(GetMode(), VGet(0,500,0),true);
			g->SetSize(VGet(3, 3, 3));
		}
	}
	
	return false;
}

void BossActor::GeneratePunch() {
	auto ac = new BossAttackActor(GetMode(),_Action);
	auto n = rand() % 2;
	auto p = GetMode()->GetPlayer(n);
	auto dir = VGet(p->GetPosition().x - GetPosition().x, 0, p->GetPosition().z - GetPosition().z);
	ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
	
	auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
	ac->SetHitCollision(h);
	auto mv = new MoveComponent(ac);
	mv->SetGravity(false);
	dir = VScale(VNorm(dir), -1);
	auto effect = new EffectSpriteComponent(ac, "res/model/Sundercross/Panch_enemy_2.efkefc", VGet(0, 0, 0),dir, 8, true);
	dir = VScale(VNorm(dir), -20);
	mv->SetVelocity(dir);

	_GenerateCnt++;
}

void BossActor::GenerateBullet() {
	if (_GenerateCnt == 0) {
		auto ac = new BossAttackActor(GetMode(), _Action);
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		
		auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		auto mv = new MoveComponent(ac);
		mv->SetGravity(false);
		auto dir = VGet(0,0,-30);
		mv->SetVelocity(dir);
		auto ef = new EffectSpriteComponent(ac, "res/model/Sundercross/Boss/Laser_Boss.efkefc", VGet(0, 0, 0), VGet(0,0,-1), 8, true);
		_GenerateCnt++;
	}
	if (_GenerateCnt == 1) {
		auto ac = new BossAttackActor(GetMode(), _Action);
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		
		auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		auto mv = new MoveComponent(ac);
		mv->SetGravity(false);
		auto rot = DEG2RAD(105);
		auto dir = VGet(0, 0, -30);
		dir = VScale(VGet(cos(rot), 0, sin(rot)), -30);
		mv->SetVelocity(dir);
		auto ef = new EffectSpriteComponent(ac, "res/model/Sundercross/Boss/Laser_Boss.efkefc", VGet(0, 0, 0), VNorm(dir), 8, true);
		

		// 2個目
		ac = new BossAttackActor(GetMode(), _Action);
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		
		h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		mv = new MoveComponent(ac);
		mv->SetGravity(false);
		rot = DEG2RAD(75);
		auto dir2 = VGet(0, 0, -30);
		dir2 = VScale(VGet(cos(rot), 0, sin(rot)), -30);
		mv->SetVelocity(dir2);
		 ef = new EffectSpriteComponent(ac, "res/model/Sundercross/Boss/Laser_Boss.efkefc", VGet(0, 0, 0), VNorm(dir2), 8, true);

		_GenerateCnt+=2;
	}
}

void BossActor::GenerateBeam() {
	auto ac = new BossAttackActor(GetMode(), _Action);
	auto p = GetMode()->GetPlayer(0);
	auto pos = VGet(GetPosition().x, 300, GetPosition().z);
	ac->SetPosition(pos);
	auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
	ac->SetHitCollision(h);
	auto mv = new MoveComponent(ac);
	mv->SetGravity(false);
	auto tmpos = VSub( GetPosition(), p->GetPosition());
	auto dir = VNorm(VGet(tmpos.x, 0, tmpos.z));
	//auto dir = VGet(0, 0, 1);
	new EffectSpriteComponent(this, "res/model/Sundercross/Daikanpa_Boss.efkefc", VGet(0, 2000, -800), dir,50,false,0.7);
	

	_GenerateCnt++;
}

void BossActor::GeneratePunchFall() {
	auto ac = new BossAttackActor(GetMode(), _Action);
	auto n = rand() % 2;
	auto p = GetMode()->GetPlayer(n)->GetPosition();
	ac->SetPosition(VGet(p.x, 1000, p.z));
	auto m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
	m->SetIndipendent(true);
	m->SetScale(VGet(10, 10, 10));
	m->SetRotation(VGet(DEG2RAD(-90), 0, 0));
	auto h = new HitCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true, true);
	ac->SetHitCollision(h);
	auto mv = new MoveComponent(ac);
	mv->SetGravity(false);
	mv->SetVelocity(VGet(0, -15, 0));

	_GenerateCnt++;
}

void BossActor::GenerateEnemy(int n ) {
	auto pos = VGet(-500,500,200);
	for (auto i = 0; i < n; i++) {
		EnemyCreator::GetInstance()->Create(GetMode(), 0, pos, nullptr);
		pos.x += 150;
	}

}


	
