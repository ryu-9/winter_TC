#include "BossActor.h"

BossActor::BossActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
	, _Action(WAIT)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _AnimIndex(-1)
	, _AnimChangingflag(false)
{
	SetPosition(pos);
	_Input = new MoveComponent(this);
	SetSize(VGet(20, 20, 20));

	// アニメーション取得用モデルの読み込み
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/reiizo-beam.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dankanha_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));

	_Model[0] = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");
	_Model[1] = new ModelComponent(this, "res/model/Sundercross/Sundercross_Arm.mv1");
	_Model[1]->SetVisible(false);

	auto index = MV1GetAnimIndex(_AnimMV1[_Action], "idle");
	_AnimIndex = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[_Action],TRUE);

	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex);
	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex, _AnimTime);
	_Input->SetGravity(false);
//	ChangeAnim(ANIM::PUNCH);
	_TimelineIndex = 0;
	_ActionIndex =0;
	std::vector<ACTION_TIMELINE> timeline;
	timeline.push_back({ ACTION::BULLET, 5000 });
	timeline.push_back({ ACTION::BEAM, 5000 });
	timeline.push_back({ ACTION::PUNCH, 5000 });

	_ActionTimeline.push_back(timeline);
}

BossActor::~BossActor() {
}

void BossActor::Init() {

}

void BossActor::UpdateActor() {
	(this->*_ActionFunc[(int)_Action])();

	_CurrentTime += (float)GetMode()->GetStepTm();
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
	
	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex, _AnimTime);
	MV1SetAttachAnimTime(_Model[1]->GetHandle(), _AnimIndex, _AnimTime);
}



bool BossActor::ChangeAnim(ACTION a) {
	if (_Action == a) { return false; }
	if (_AnimChangingflag == true) { return false; }
	MV1DetachAnim(_Model[0]->GetHandle(), _AnimIndex);
	MV1DetachAnim(_Model[1]->GetHandle(), _AnimIndex);
	_AnimChangingflag = true;
	int oldindex = _AnimIndex;
	int index = -1;
	bool changeSucFlag = false;
	switch (a) {
	case ACTION::WAIT:
		index = MV1GetAnimIndex(_AnimMV1[a], "idle");
		_AnimIndex = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;
	
	case ACTION::PUNCH:
		index = MV1GetAnimIndex(_AnimMV1[a], "reizo-kou_motion");
		_AnimIndex = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		MV1AttachAnim(_Model[1]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	case ACTION::BULLET:
		index = MV1GetAnimIndex(_AnimMV1[a], "beemmotion");
		_AnimIndex = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;
	case ACTION::BEAM:
		index = MV1GetAnimIndex(_AnimMV1[a], "dankanha_motion");
		_AnimIndex = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	}
	_AnimChangingflag = false;
	if (changeSucFlag) {
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
	case BossActor::NUM:
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
	// イージングでサイズ変更
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

bool BossActor::CoolDown() {
	auto t = (float)GetMode()->GetStepTm();
		_AnimTime +=  t / 20.f;
	if (_AnimTime > _AnimTotalTime) {
		_AnimTime -= _AnimTotalTime;
	}
	if (_ActTime >= _ActTotalTime) {
		_ActTime = 0;
		ChangeAnim(ACTION::WAIT);
		ChangeAction(ACTION::WAIT);
		return true;
	}
	return false;
}
	
