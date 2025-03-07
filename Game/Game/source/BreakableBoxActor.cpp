#include "BreakableBoxActor.h"

BreakableBoxActor::BreakableBoxActor(ModeBase* mode, VECTOR pos, VECTOR scale)
	: ActorClass(mode)
	,_AnimCount(0)
{
	SetPosition(pos);
	SetSize(scale);
	auto m = new ModelComponent(this, "res/model/Ice_Broken/Ice_Broken_Opacity.mv1");
	m->SetScale(VGet(2, 2, 2));
	m->SetPosition(VGet(0, -50, 0));
	_Handle = m->GetHandle();
	int index = MV1GetAnimIndex(_Handle, "Ice_Broken");
	_AnimIndex = MV1AttachAnim(_Handle, index, _Handle, TRUE);
	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Handle, _AnimIndex);
	SetDirection(VGet(0, 0, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this, m, VGet(0, 0, 0), VGet(0.5, 0.5, 0.5), 6, false, true, handle);
	_HCollision = new HitCollisionComponent(this, m, VGet(0, 0, 0), VGet(75, 75, 75), 6, false, true);
}

BreakableBoxActor::~BreakableBoxActor()
{
}

void BreakableBoxActor::UpdateActor()
{
	if (_AnimCount > 0)
	{
		_AnimCount+=FpsController::GetInstance()->GetDeltaTime()/5;
		MV1SetAttachAnimTime(_Handle, _AnimIndex, _AnimCount);
		if (_AnimCount >= _AnimTotalTime)
		{
		}
	}
}

void BreakableBoxActor::Init()
{
}

void BreakableBoxActor::StartBreak()
{
	_MCollision->SetIsActive(false);
	_HCollision->SetIsActive(false);
	_AnimCount = 1;
}
