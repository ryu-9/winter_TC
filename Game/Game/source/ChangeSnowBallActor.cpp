#include "ChangeSnowBallActor.h"

ChangeSnowBallActor::ChangeSnowBallActor(ModeBase* mode, ActorClass* owner, float size)
	:ActorClass(mode)
	, _Owner(owner)
	, _AnimTime(30)
	, _LifeTime(1500)
	, _Flag(false)
{
	SetPosition(owner->GetPosition());
	SetSize(VGet(size, size, size));
	_Model = new ModelComponent(this, "res/model/SnowBall/gattai_ball.mv1");
	_Model->SetPosition(VGet(0, - 25, 0));
	MV1AttachAnim(_Model->GetHandle(), 0, _Model->GetHandle(), TRUE);
	MV1SetAttachAnimTime(_Model->GetHandle(), 0, 30);
}

ChangeSnowBallActor::~ChangeSnowBallActor()
{
}

void ChangeSnowBallActor::UpdateActor()
{
	SetPosition(_Owner->GetPosition());
	if (_Flag) {
		_AnimTime += (float)GetMode()->GetStepTm() / 10;
	}
	else {
		_AnimTime -= (float)GetMode()->GetStepTm() / 35;
		auto ms = GetComponent<ModelSpriteComponent>();
		if (ms.size() > 0) {
			float value = (float)(-15 - _AnimTime) / 10;
			COLOR_F color = GetColorF(value, value, value ,1);
			//color = GetColorF(0, 0, 0, 1);
			ms[0]->AddMaterial("specular", color);
		}
	}

	if (_AnimTime > 30) {
		SetState(ActorClass::State::eDead);
	}
	if (_AnimTime < 0) {
		MV1SetAttachAnimTime(_Model->GetHandle(), 0, 0);
		if (_AnimTime < -29) {
			_Flag = true;
			_AnimTime = 0;
		}
	}
	else {
		MV1SetAttachAnimTime(_Model->GetHandle(), 0, _AnimTime);
	}

	_LifeTime -= GetMode()->GetStepTm();
	if (_LifeTime < 0) {
		//SetState(ActorClass::State::eDead);
	}
}

