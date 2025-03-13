#include "BossActor.h"
#include "PlayerActor.h"
#include "BossAttackActor.h"

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
	, _HitPoint(500)
{
	SetPosition(pos);
	_Input = new MoveComponent(this);
	SetSize(VGet(20, 20, 20));

	// �A�j���[�V�����擾�p���f���̓ǂݍ���
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/reiizo-beam.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dankanha_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));			// TODO: �Ђ�݃��[�V�����ɍ����ւ�
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));

	// �A�j���[�V�������̐ݒ�
	for (auto i = 0; i < _AnimMV1.size(); i++) {
		_AnimName.push_back(MV1GetAnimName(_AnimMV1[i], 0));
	}

	// ���f���̓ǂݍ���
	_Model[0] = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");		// �{��

	{
		auto index = MV1GetAnimIndex(_AnimMV1[_Action], _AnimName[0]);
		_AnimIndex.push(MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[_Action], TRUE));
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.front(), _AnimTime);
		_Input->SetGravity(false);
	}

	// �^�C�����C���̐ݒ�
	std::vector<ACTION_TIMELINE> timeline;
	timeline.push_back({ ACTION::PUNCH_FALL, 2000 });
	timeline.push_back({ ACTION::PUNCH, 18000 });
	timeline.push_back({ ACTION::BULLET, 10000 });
	timeline.push_back({ ACTION::PUNCH, 10000 });

	_ActionTimeline.push_back(timeline);
}

BossActor::~BossActor() {
}

void BossActor::UpdateActor() {
	(this->*_ActionFunc[(int)_Action])();

	_CurrentTime += (float)GetMode()->GetStepTm();
	if (_HitPoint > 0) {

		if (_CurrentTime >= _ActionTimeline[_TimelineIndex][_ActionIndex].time) {
			ChangeAnim(_ActionTimeline[_TimelineIndex][_ActionIndex].action);
			ChangeAction(_ActionTimeline[_TimelineIndex][_ActionIndex].action);
			if (_ActionIndex < _ActionTimeline[_TimelineIndex].size() - 1) {
				_ActionIndex++;
				_CurrentTime = 0;
			} else {
				_ActionIndex = 0;
				
			}
			
		}
	}

	if (_AnimIndex.size() > 1) {
		_AnimRate -= 0.01f;
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.front(),_AnimRate);
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.back(),1 - _AnimRate);
		if (_AnimRate <= 0) {
			MV1DetachAnim(_Model[0]->GetHandle(), _AnimIndex.front());
			_AnimIndex.pop();
			_AnimRate = 1.f;
		}
	}


	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.back(), _AnimTime);
	
	{ // �f�o�b�O�p
	//	_HitPoint--;
		if (_HitPoint == 0) {
			ChangeAnim(ACTION::DIE);
			ChangeAction(ACTION::DIE);
		}
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
	if (_AnimTime > 50) {
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
		ChangeAction(ACTION::WAIT);
		return true;
	}
	return false;
}

bool BossActor::Beam() {
	auto t = (float)GetMode()->GetStepTm();
	_AnimTime += t / 40.f;
	if (_CurrentTime > 3000 && _GenerateCnt == 0) {
		GenerateBeam();
	}
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
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
	return false;
}

bool BossActor::Die() {
	if (_AnimTime <= _AnimTotalTime) {
		auto t = (float)GetMode()->GetStepTm();
		_AnimTime += t / 20.f;
	}
	return false;
}

void BossActor::GeneratePunch() {
	auto ac = new BossAttackActor(GetMode());
	auto p = GetMode()->GetPlayer(0);
	auto dir = VGet(p->GetPosition().x - GetPosition().x, 0, p->GetPosition().z - GetPosition().z);
	ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
	auto m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
	m->SetIndipendent(true);
	m->SetScale(VGet(25, 25, 25));
	auto h = new HitCollisionComponent(ac, m, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
	ac->SetHitCollision(h);
	auto mv = new MoveComponent(ac);
	mv->SetGravity(false);
	dir = VScale(VNorm(dir), 20);
	mv->SetVelocity(dir);

	_GenerateCnt++;
}

void BossActor::GenerateBullet() {
	if (_GenerateCnt == 0) {
		auto ac = new BossAttackActor(GetMode());
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		auto m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
		m->SetIndipendent(true);
		m->SetScale(VGet(25, 25, 25));
		
		auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		auto mv = new MoveComponent(ac);
		mv->SetGravity(false);
		auto dir = VGet(0,0,-30);
		mv->SetVelocity(dir);
		_GenerateCnt++;
	}
	if (_GenerateCnt == 1) {
		auto ac = new BossAttackActor(GetMode());
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		auto m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
		m->SetIndipendent(true);
		m->SetScale(VGet(25, 25, 25));
		auto h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		auto mv = new MoveComponent(ac);
		mv->SetGravity(false);
		auto rot = DEG2RAD(105);
		auto dir = VGet(0, 0, -30);
		dir = VScale(VGet(cos(rot), 0, sin(rot)), -30);
		mv->SetVelocity(dir);
		m->SetRotation(VGet(0, DEG2RAD(-15), 0));

		// 2個目
		ac = new BossAttackActor(GetMode());
		ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
		m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
		m->SetIndipendent(true);
		m->SetScale(VGet(25, 25, 25));
		h = new HitCollisionComponent(ac, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
		ac->SetHitCollision(h);
		mv = new MoveComponent(ac);
		mv->SetGravity(false);
		rot = DEG2RAD(75);
		auto dir2 = VGet(0, 0, -30);
		dir2 = VScale(VGet(cos(rot), 0, sin(rot)), -30);
		mv->SetVelocity(dir2);
		m->SetRotation(VGet(0, DEG2RAD(15), 0));

		_GenerateCnt+=2;
	}
}

void BossActor::GenerateBeam() {
	auto ac = new BossAttackActor(GetMode());
	auto p = GetMode()->GetPlayer(0);
	ac->SetPosition(VGet(GetPosition().x, 300, GetPosition().z));
	auto m = new ModelComponent(ac, "res/model/Sundercross/untitled.mv1");
	m->SetIndipendent(true);
	m->SetScale(VGet(25, 25, 25));
	auto h = new HitCollisionComponent(ac, m, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
	ac->SetHitCollision(h);
	auto mv = new MoveComponent(ac);
	mv->SetGravity(false);
	mv->SetVelocity(VGet(0, 0, -30));

	

	_GenerateCnt++;
}

void BossActor::GeneratePunchFall() {
	auto ac = new BossAttackActor(GetMode());
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


	
