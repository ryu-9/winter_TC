#include "BreakableBoxActor.h"

BreakableBoxActor::BreakableBoxActor(ModeBase* mode, VECTOR pos, VECTOR scale)
	: ActorClass(mode)
	,_AnimCount(0)
	, _Life(8000)
{
	SetPosition(pos);
	SetSize(scale);
	_Model = new ModelComponent(this, "res/model/Ice_Broken/Ice_Broken_Opacity.mv1", 102);
	_Model->SetScale(VGet(2, 2, 2));
	_Model->SetPosition(VGet(0, -50, 0));
	int index = MV1GetAnimIndex(_Model->GetHandle(), "Ice_Broken");
	_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _Model->GetHandle(), TRUE);
	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);
	SetDirection(VGet(0, 0, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(0.4, 0.5, 0.4), 6, false, true, handle);
	handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(1, 1,1), 6, false, true,handle);
	_Model->SetVisible(false);


	_Model2 = new ModelComponent(this, "res/model/Ice_Broken/Ice_Broken_Opacity_NoCrack.mv1", 102);
	_Model2->SetScale(VGet(2, 2, 2));
	_Model2->SetPosition(VGet(0, -50, 0));
	_AnimIndex = MV1AttachAnim(_Model2->GetHandle(), index, _Model->GetHandle(), TRUE);
	_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model2->GetHandle(), _AnimIndex);
	//_Model2->SetVisible(false);
	
}

BreakableBoxActor::~BreakableBoxActor()
{
}

void BreakableBoxActor::UpdateActor()
{
	if (_AnimCount > 0)
	{
		
		_AnimCount+= GetMode()->GetStepTm() /5;
		MV1SetAttachAnimTime(_Model->GetHandle(), _AnimIndex, _AnimCount);
		if (_AnimCount >= _AnimTotalTime)
		{
			_Life -= GetMode()->GetStepTm();
			auto s = -_Model->GetScale().y * (8000 - _Life) / 8000;
			_Model->SetScale(VAdd(_Model->GetScale(), VGet(0, -_Model->GetScale().y * (8000 - _Life) / 8000, 0)));
			if (_Life <= 0)
			{
				SetState(State::eDead);
			}
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
	_Model->SetVisible(true);
	_Model2->SetVisible(false);
}
