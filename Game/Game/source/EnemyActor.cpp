#include "EnemyActor.h"
#include "EnemyMoveComponent.h"

EnemyActor::EnemyActor(ModeBase* mode)
	:ActorClass(mode)
	, _Status(STATUS::NONE)
	, _OldStatus(STATUS::NONE)
{
	_Model = new ModelComponent(this, "res/model/Enemy_corn.mv1");
	_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(50, 50, 50),2,true ,true);
	
	SetPosition(VGet(1000, 200, 100));
	SetSize(VGet(2, 2, 2));
	_Input = new EnemyMoveComponent(this);
}

EnemyActor::~EnemyActor()
{
}

void EnemyActor::UpdateActor() {
	SetOldStatus();

	
}

