#include "StageBox.h"

StageBox::StageBox(ModeBase* mode)
	:ActorClass(mode)
{
	auto m = new ModelComponent(this, "res/cube.mv1");
	auto coll = new MoveCollisionComponent(this, VGet(0,0,0), VGet(1,1,1),6);

}

StageBox::~StageBox()
{
}

void StageBox::UpdateActor()
{
	int test = 0;
}
