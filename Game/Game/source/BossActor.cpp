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

	// アニメーション取得用モデルの読み込み
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/reiizo-beam.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dankanha_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));		// TODO: ひるみモーションに差し替え
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_yarare.mv1"));

	// モデルの読み込み
	_Model[0] = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");		// 本体
	_Model[1] = new ModelComponent(this, "res/model/Sundercross/Sundercross_Arm.mv1");		// 腕
	_Model[1]->SetVisible(false);

	{
		auto index = MV1GetAnimIndex(_AnimMV1[_Action], "idle");
		_AnimIndex.push(MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[_Action], TRUE));
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.front(), _AnimTime);
		_Input->SetGravity(false);
	}

	// タイムラインの設定
	std::vector<ACTION_TIMELINE> timeline;
	timeline.push_back({ ACTION::PUNCH, 10000 });
	timeline.push_back({ ACTION::BEAM, 10000 });
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
	MV1SetAttachAnimTime(_Model[0]->GetHandle(), _AnimIndex.front(), _AnimTime);
	MV1SetAttachAnimTime(_Model[1]->GetHandle(), _AnimIndex.front(), _AnimTime);

	{ // デバッグ用
	//	_HitPoint--;
		if (_HitPoint == 0) {
			ChangeAnim(ACTION::DIE);
			ChangeAction(ACTION::DIE);
		}
	}
}



bool BossActor::ChangeAnim(ACTION a) {
	if (_Action == a) { return false; }
	MV1DetachAnim(_Model[0]->GetHandle(), _AnimIndex.front());
	MV1DetachAnim(_Model[1]->GetHandle(), _AnimIndex.front());
	int oldindex = _AnimIndex.front();
	int index = -1;
	bool changeSucFlag = false;
	switch (a) {
	case ACTION::WAIT:
		index = MV1GetAnimIndex(_AnimMV1[a], "idle");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;
	
	case ACTION::PUNCH:
		index = MV1GetAnimIndex(_AnimMV1[a], "reizo-kou_motion");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		MV1AttachAnim(_Model[1]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;

	case ACTION::BULLET:
		index = MV1GetAnimIndex(_AnimMV1[a], "beemmotion");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;
	case ACTION::BEAM:
		index = MV1GetAnimIndex(_AnimMV1[a], "dankanha_motion");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;
	case ACTION::DAMAGE:
		index = MV1GetAnimIndex(_AnimMV1[a], "yarare");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;
	case ACTION::DIE:
		index = MV1GetAnimIndex(_AnimMV1[a], "yarare");
		_AnimIndex.front() = MV1AttachAnim(_Model[0]->GetHandle(), index, _AnimMV1[a], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model[0]->GetHandle(), _AnimIndex.front());
		changeSucFlag = true;
		break;
	default:
		break;
	}
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

bool BossActor::Change() {
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
	
