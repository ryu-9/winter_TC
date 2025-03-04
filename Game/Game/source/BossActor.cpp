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
	SetSize(VGet(3, 3, 3));

	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/gattaimotion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_idle_motion.mv1"));
	_AnimMV1.push_back(ModelServer::GetInstance()->Add("res/model/Sundercross/motion/SK_move_motion.mv1"));
	_Model = new ModelComponent(this, "res/model/Sundercross/B_Sundercross.mv1");
	
	auto index = MV1GetAnimIndex(_AnimMV1[1], "idle");
	_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _AnimMV1[1],TRUE);

	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
}

BossActor::~BossActor() {
}

void BossActor::Init() {

}

void BossActor::UpdateActor() {
	//_AnimTime += (float)GetMode()->GetStepTm() / (float)10000;
	_AnimTime += (float)FpsController::GetInstance()->GetDeltaTime() / 10;
	if (_AnimTime > _AnimTotalTime) {
		_AnimTime-= _AnimTotalTime;
	}

	MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimTime);
}
	
