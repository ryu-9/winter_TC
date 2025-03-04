#include "BossActor.h"

BossActor::BossActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
	, _Animation(ANIM::WAIT)
	, _AnimTime(0)
	, _AnimTotalTime(0)
	, _AnimIndex(-1)
	, _AnimChangingflag(false)
{
	SetPosition(pos);
	_Input = new MoveComponent(this);
	SetSize(VGet(20, 20, 20));

	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_move_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_dash_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_reizo-kou.mv1"));

	_Model = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");
	
	auto index = MV1GetAnimIndex(_AnimMV1[1], "idle");
	_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[1],TRUE);

	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
	_Input->SetGravity(false);
}

BossActor::~BossActor() {
}

void BossActor::Init() {

}

void BossActor::UpdateActor() {
	
	ChangeAnim(ANIM::PUNCH);
	_AnimTime += (float)GetMode()->GetStepTm() / (float)10;
	//_AnimTime += (float)FpsController::GetInstance()->GetDeltaTime() / 10;
	if (_AnimTime > _AnimTotalTime) {
		_AnimTime-= _AnimTotalTime;
	}

	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
}

bool BossActor::ChangeAnim(ANIM a) {
	if (_Animation == a) { return false; }
	if (_AnimChangingflag == true) { return false; }

	_AnimChangingflag = true;
	int oldindex = _AnimIndex;
	int index = -1;
	bool changeSucFlag = false;
	switch (a) {
	case ANIM::PUNCH:
		index = MV1GetAnimIndex(_AnimMV1[4], "reizo-kou_motion");
		_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[4], TRUE);
		_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
		changeSucFlag = true;
		break;

	}
	_AnimChangingflag = false;
	if (changeSucFlag) {
		_Animation = a;
		_AnimTime = 0;
		return true;
	}

	return false;
}
	
