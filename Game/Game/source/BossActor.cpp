#include "BossActor.h"

BossActor::BossActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
	, _Action(ACTION::WAIT)
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

	_Model = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");
	
	auto index = MV1GetAnimIndex(_AnimMV1[_Action], "idle");
	_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[_Action],TRUE);

	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
	_Input->SetGravity(false);
//	ChangeAnim(ANIM::PUNCH);
}

BossActor::~BossActor() {
}

void BossActor::Init() {

}

void BossActor::UpdateActor() {
	switch (_Action) {
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
	
	_AnimTime += (float)GetMode()->GetStepTm() / 10.f;
	if (_AnimTime > _AnimTotalTime) {
		_AnimTime-= _AnimTotalTime;
	}

	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
}

bool BossActor::Punch() {
	
	return false;
}

bool BossActor::ChangeAnim(ACTION a) {
	if (_Action == a) { return false; }
	if (_AnimChangingflag == true) { return false; }

	_AnimChangingflag = true;
	int oldindex = _AnimIndex;
	int index = -1;
	bool changeSucFlag = false;
	switch (a) {
	case ACTION::WAIT:
		index = MV1GetAnimIndex(_AnimMV1[a], "idle");
		_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[1], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;
	
	case ACTION::PUNCH:
		index = MV1GetAnimIndex(_AnimMV1[4], "reizo-kou_motion");
		_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[4], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
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

bool BossActor::ChangeMotion(ACTION a) {
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
	return false;
}

bool BossActor::CoolDown() {
	if (_ActTime >= _ActTotalTime) {
		_ActTime = 0;
		return true;
	}
	return false;
}
	
