#include "EnemyMoveComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"
#include "EnemyActor.h"


EnemyMoveComponent::EnemyMoveComponent(ActorClass* owner)
	:MoveComponent(owner)
{
}

EnemyMoveComponent::~EnemyMoveComponent() {}

void EnemyMoveComponent::ProcessInput() {
	/*
	auto pPos = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition();
	auto ePos = _Owner->GetPosition();
	VECTOR move = VSub(pPos, ePos);
	move = VGet(move.x * 0.1, 0, move.z * 0.1);
	_Owner->SetPosition(VAdd(ePos, move));
	*/

}

