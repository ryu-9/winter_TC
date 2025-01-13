#include "StageBox.h"

StageBox::StageBox(ModeBase* mode)
	:ActorClass(mode)
{
	auto m = new ModelComponent(this, "res/cube.mv1");
	auto coll = new MoveCollisionComponent(this, m, VGet(0,0,0), VGet(1,1,1),6);

}

StageBox::~StageBox()
{
}

void StageBox::UpdateActor()
{
	//auto model = GetComponent<ModelComponent>();
	//model->SetRotation(VAdd(model->GetRotation(), VGet(0, 0.001, 0)));
}
