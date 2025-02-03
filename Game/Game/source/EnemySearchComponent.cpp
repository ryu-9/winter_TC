#include "EnemySearchComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"

EnemySearchComponent::EnemySearchComponent(ActorClass* owner,int updateOrder)
	: Component(owner,updateOrder)
{
	_SearchRef.dist = 1000.0f;
	_SearchRef.angle = 60.0f;
}

EnemySearchComponent::~EnemySearchComponent()
{

}

void EnemySearchComponent::ProcessInput() {

}

