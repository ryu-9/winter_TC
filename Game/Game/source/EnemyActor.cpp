#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "EnemyAttackComponent.h"
#include "ECornComponent.h"
#include "PlayerActor.h"

EnemyActor::EnemyActor(ModeBase* mode)
	:ActorClass(mode)
{
	_Model = new ModelComponent(this, "res/model/Enemy_corn.mv1");
	_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 5, 0), VGet(10, 10, 10),2,true ,true);
	
	SetPosition(VGet(1000, 300, 1000));
	SetSize(VGet(2, 2, 2));
	_Input = new MoveComponent(this);
//	new EnemyAttackComponent(this);
	new ECornComponent(this);
}

EnemyActor::~EnemyActor() {}

void EnemyActor::Init() {


}

void EnemyActor::UpdateActor() {


}

