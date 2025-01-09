#include "StageBox.h"

StageBox::StageBox(ModeBase* mode)
	:ActorClass(mode)
{
	auto m = new ModelComponent(this, "res/cube.mv1");
	auto coll = new MoveCollisionComponent(this);
}

StageBox::~StageBox()
{
}

void StageBox::UpdateActor()
{
	int test = 0;
}
