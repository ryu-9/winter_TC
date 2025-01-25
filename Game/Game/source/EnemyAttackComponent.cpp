#include "EnemyAttackComponent.h"
#define INTERVAL (2000)
#include "ModeGame.h"
#include "PlayerActor.h"
#include "BulletComponent.h"

EnemyAttackComponent::EnemyAttackComponent(ActorClass* owner)
	:Component(owner)
	, _Interval(0)
{
}

EnemyAttackComponent::~EnemyAttackComponent()
{
}

void EnemyAttackComponent::ProcessInput()
{

}

void EnemyAttackComponent::Update() {
	if (_Interval > 0) {
		_Interval -= _Owner->GetMode()->GetStepTm();
	}
	else {
		_Interval = INTERVAL;
		auto ac = new ActorClass(_Owner->GetMode());
		ac->SetPosition(_Owner->GetPosition());
		new ModelComponent(ac, "res/model/Enemy_corn.mv1");
		auto b = new BulletComponent(ac);
		b->SetSpeed(100);
		b->SetGoalPos(static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition());
	}
}
