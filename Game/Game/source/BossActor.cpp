#include "BossActor.h"

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
	_Model[1] = new ModelComponent(this, "res/model/Sundercross/Sundercross_Arm.mv1");		// �r
	_Model[1]->SetVisible(false);

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
	timeline.push_back({ ACTION::BEAM, 10000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });

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
			} else {
				_ActionIndex = 0;
			}
			_CurrentTime = 0;
		}
	}

	if (_AnimIndex.size() > 1) {
		_AnimRate -= 0.01f;
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.front(),_AnimRate);
		MV1SetAttachAnimBlendRate(_Model[0]->GetHandle(), _AnimIndex.back(),1 - _AnimRate);
		if (_AnimRate <= 0) {
			MV1DetachAnim(_Model[0]->GetHandle(), _AnimIndex.front());
			MV1DetachAnim(_Model[1]->GetHandle(), _AnimIndex.front());
			_AnimIndex.pop();
			_AnimRate = 1.f;
		}
	}


	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.back(), _AnimTime);
	MV1SetAttachAnimTime(_Model[1]->GetHandle(), _AnimIndex.back(), _AnimTime);
	

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
	_AnimTime += t / 25.f;
	if (_AnimTime > 40 && !_Model[1]->GetVisible()) {
		_Model[1]->SetVisible(true);
	}
	// �C�[�W���O�ŃT�C�Y�ύX
	if (_AnimTime - 40 <= 20) {
		auto s = (1 * ((_AnimTime - 40) / 20)) + 1;
		_Model[1]->SetScale(VGet(s, s, s));
	}
		
	
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		_Model[1]->SetVisible(false);
		_Model[1]->SetScale(VGet(1, 1, 1));
		return true;
	}
	return false;
}

bool BossActor::Bullet() {
	auto t = (float)GetMode()->GetStepTm();
	_AnimTime += t / 20.f;
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		return true;
	}
	return false;
}

bool BossActor::Beam() {
	auto t = (float)GetMode()->GetStepTm();
	_AnimTime += t / 20.f;
	if (_AnimTime > _AnimTotalTime) {
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		return true;
	}
	return false;
}

bool BossActor::PunchFall() {
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


	
