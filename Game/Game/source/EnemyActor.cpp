#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "EnemyAttackComponent.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"
#include "PlayerActor.h"

EnemyActor::EnemyActor(ModeBase* mode,VECTOR pos)
	:ActorClass(mode)
{
	SetPosition(pos);
	//_Model = new ModelComponent(this, "res/model/Enemy_Corn/Enemy_corn.mv1");
	_Model = new ModelComponent(this, "res/model/Enemy_Box/mouse_Open/Enemy_box.mv1");
	_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(5, 5, 5),2,true ,true);
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(5, 5, 5), 2, true, true);
	SetSize(VGet(2, 2, 2));

	_Model->SetPosition(VGet(0, -15, 0));
	_Input = new MoveComponent(this);
//	new EnemyAttackComponent(this);
	//new ECornComponent(this);
	new EBoxComponent(this);
}

EnemyActor::~EnemyActor() {}

void EnemyActor::Init() {


}

void EnemyActor::UpdateActor() {
	

}

