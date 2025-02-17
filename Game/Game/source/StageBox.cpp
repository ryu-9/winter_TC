#include "StageBox.h"
#include "SnowComponent.h"

StageBox::StageBox(ModeBase* mode)
	:ActorClass(mode)
{
	auto m = new ModelComponent(this, "res/model/Mapchip/Mapchip.mv1");
	m->SetScale(VGet(2,2,2));
	m->SetPosition(VGet(0, -50, 0));
	SetDirection(VGet(0, 0, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this, m, VGet(0,0,0), VGet(0.5,0.5,0.5),6, false, true, handle);
	//coll->SetRSize(VGet(0.1, 0.1, 0.1));

}

StageBox::~StageBox()
{
}


void StageBox::UpdateActor()
{
	//auto model = GetComponent<ModelComponent>();
	//model->SetRotation(VAdd(model->GetRotation(), VGet(0, 0.001, 0)));
}

void StageBox::Init()
{
	MV1_COLL_RESULT_POLY_DIM p = MV1CollCheck_Sphere(_MCollision->GetHandle(), -1, _MCollision->GetPosition(), 10000);
	for (int i = 0; i < p.HitNum; i++)
	{
		if (p.Dim[i].Normal.y > 0.5)
		{
			_Poly.push_back(p.Dim[i]);
			new SnowComponent(this, p.Dim[i]);
		}
	}
}
