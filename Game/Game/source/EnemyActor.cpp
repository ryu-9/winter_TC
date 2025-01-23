#include "EnemyActor.h"

EnemyActor::EnemyActor(ModeBase* mode)
	:ActorClass(mode)
	, _Status(STATUS::NONE)
	, _OldStatus(STATUS::NONE)
{
	_Model = new ModelComponent(this, "res/model/Enemy_corn.mv1");

	SetPosition(VGet(1000, 200, 100));
}

EnemyActor::~EnemyActor()
{
}

void EnemyActor::UpdateActor()
{
}

