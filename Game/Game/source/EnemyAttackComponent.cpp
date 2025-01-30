#include "EnemyAttackComponent.h"
#define INTERVAL (2000)
#include "ModeGame.h"
#include "PlayerActor.h"
#include "BulletComponent.h"
#include "CameraActor.h"

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
		//auto m = new ModelComponent(ac, "res/model/Enemy_corn.mv1");
		auto m = new ModelComponent(ac, "");
		//new EffectComponent(ac, "res/effect/kannpekikamo.efkefc",10);
		new MoveCollisionComponent(ac, m, VGet(0, 0, 0), VGet(10, 10, 10), 2, true);
		new BulletComponent(ac, static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition(),1500);
		auto game = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"));
		//auto cam = static_cast<ActorClass*>(game->GetCamera());
		
		//new SEComponent(ac,cam);
		new SEComponent(ac, game->GetPlayer());
	}
}
