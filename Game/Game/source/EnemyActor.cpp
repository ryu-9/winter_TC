#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "EnemyAttackComponent.h"
#include "EnemyComponent.h"
#include "PlayerActor.h"

EnemyActor::EnemyActor(ModeBase* mode)
	:ActorClass(mode)
{
	_Model = new ModelComponent(this, "res/model/Enemy_corn.mv1");
	_MCollision = new MoveCollisionComponent(this, _Model, VGet(0, 5, 0), VGet(10, 10, 10),2,true ,true);
	
	SetPosition(VGet(1000, 300, 1000));
	SetSize(VGet(2, 2, 2));
	new MoveComponent(this);
	new EnemyAttackComponent(this);
	new EnemyComponent(this);
}

EnemyActor::~EnemyActor() {}

void EnemyActor::Init() {
	_Status = STATUS::SEARCH;
	_OldStatus = STATUS::NONE;

	_SearchRef.dist = 1000;
	_SearchRef.angle = 60;
}

void EnemyActor::UpdateActor() {
	SetOldStatus();


}

bool EnemyActor::Search() {
	auto epos = GetPosition();
	auto ppos = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition();

	auto dist = VSize(VSub(ppos, epos));

	if (dist < _SearchRef.dist) {
		if (VDot(VGet(0, 0, 1), VGet(ppos.x - epos.x, 0, ppos.z - epos.z)) < cosf(_SearchRef.angle)) {
			
		}
	}
	return false;
}

