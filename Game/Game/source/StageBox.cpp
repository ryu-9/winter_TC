#include "StageBox.h"
#include "SnowComponent.h"
#include "ApplicationGlobal.h"

StageBox::StageBox(ModeBase* mode, VECTOR pos, VECTOR rot, VECTOR scale, int type)
	:ActorClass(mode)
{
	SetPosition(pos);
	SetDirection(rot);
	SetSize(scale);
	switch (type) {

		case 0:
		{
			switch (gGlobal._SelectStage) {
			case 0:
				_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip_Broken1.mv1", 101);
				break;
			case 1:
				_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip_Broken2.mv1", 101);
				break;
			case 2:
				_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip_Broken3.mv1", 101);
				break;

			default:
				_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip_Broken1.mv1", 101);
				break;

			}

			_Model->SetScale(VGet(2, 2, 2));
			_Model->SetPosition(VGet(0, -50, 0));
			int index = MV1GetAnimIndex(_Model->GetHandle(), "Ice_Broken");
			_AnimIndex = MV1AttachAnim(_Model->GetHandle(), index, _Model->GetHandle(), TRUE);
			_AnimTotalTime = MV1GetAttachAnimTotalTime(_Model->GetHandle(), _AnimIndex);		
			int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
			_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(0.5, 0.5, 0.5), 6, false, true, handle);
			handle = ModelServer::GetInstance()->Add("res/cube.mv1");
			_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(0.5, 0.5, 0.5), 6, false, true, handle);
			break;
		}


		case 1:
		{
			switch (gGlobal._SelectStage) {
			case 0:
				_Model = new ModelComponent(this, "res/Stage/model/Ramp1.mv1");
				break;
			case 1:
				_Model = new ModelComponent(this, "res/Stage/model/Ramp2.mv1");
				break;
			case 2:
				_Model = new ModelComponent(this, "res/Stage/model/Ramp3.mv1");
				break;
			default:
				_Model = new ModelComponent(this, "res/Stage/model/Ramp1.mv1");
				break;
			
			}
			//_Model = new ModelComponent(this, "res/Stage/model/Ramp.mv1");
			_Model->SetScale(VGet(0.5, 0.5, 0.5));
			_AnimTotalTime = 0;;
			int handle = ModelServer::GetInstance()->Add("res/Stage/model/Ramp.mv1");
			_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(0.5, 0.5, 0.5), 6, false, true, handle);
			handle = ModelServer::GetInstance()->Add("res/Stage/model/Ramp.mv1");
			_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(0.5, 0.5, 0.5), 6, false, true, handle);
			break;
		}
	
	
	case 2:
		{
			_Model = new ModelComponent(this, "res/Stage/model/laststage.mv1");
			_AnimTotalTime = 0;
			int handle = ModelServer::GetInstance()->Add("res/Stage/model/laststage.mv1");
			_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(1, 1, 1), 6, false, true, handle);
			handle = ModelServer::GetInstance()->Add("res/Stage/model/laststage.mv1");
			_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 1, 0), VGet(1, 1, 1), 6, false, true, handle);
			break;
		}
	}

	Init();

}

StageBox::~StageBox()
{
}


void StageBox::UpdateActor()
{
	if (_AnimCount > 0)
	{
		_AnimCount += GetMode()->GetStepTm() / 5;
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

void StageBox::Init()
{
	auto sc = GetComponent<SnowComponent>();
	for(auto s : sc)
	{
		delete s;
	}
	MV1_COLL_RESULT_POLY_DIM p = MV1CollCheck_Sphere(_MCollision->GetHandle(), -1, _MCollision->GetPosition(), 10000);
	for (int i = 0; i < p.HitNum; i++)
	{
		if (p.Dim[i].Normal.y > 0.5)
		{
			_Poly.push_back(p.Dim[i]);
			new SnowComponent(this, p.Dim[i], true, true, true);
		}
	}
	MV1CollResultPolyDimTerminate(p);
	_MCollision->RefleshCollInfo();
	_HCollision->RefleshCollInfo();
}

void StageBox::StartBreak()
{
	_MCollision->SetIsActive(false);
	_HCollision->SetIsActive(false);
	_AnimCount = 1;
}
