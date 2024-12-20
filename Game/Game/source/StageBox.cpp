#include "StageBox.h"

StageBox::StageBox(ModeBase* mode)
	:ActorClass(mode)
{
	auto m = new ModelComponent(this, "res/cube.mv1");
	new MoveCollisionComponent(this);
}

StageBox::~StageBox()
{
}

void StageBox::UpdateActor()
{
}
