#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"

PlayerActor::PlayerActor(ModeBase* mode)
	:ActorClass(mode)
	,_Status(STATUS::NONE)
	,_OldStatus(STATUS::NONE)
{
	auto m =new ModelComponent(this);
	auto m2 = new MoveCollisionComponent(this);
	new ModelSpriteComponent(this, m);
	int n = rand() % 2;
	
		_Input = new PlayerMoveComponent(this);
	
	
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor()
{
	SetOldStatus();
}
