#include "BulletComponent.h"
#include "EnemyActor.h"

BulletComponent::BulletComponent(ActorClass* owner)
	:Component(owner)
{
	_StartPos = owner->GetPosition();
}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::ProcessInput() {
	// ƒS[ƒ‹‚ÉŒü‚¯‚ÄˆÚ“®
	VECTOR dir = VSub(_GoalPos, _StartPos);
	_Dir = VNorm(dir);
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(_Dir, (_Speed * _Owner->GetMode()->GetStepTm())/1000)));


}

void BulletComponent::Update() {
}
