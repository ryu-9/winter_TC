#include "BulletComponent.h"
#include "EnemyActor.h"

BulletComponent::BulletComponent(ActorClass* owner)
	:MoveComponent(owner)
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
	auto v = VScale(_Dir, (_Speed * _Owner->GetMode()->GetStepTm()) / 1000);
	SetVelocity(v);
	// ‚±‚Ì‚Ü‚Ü‚¾‚Æ‚ß‚Á‚¿‚á—‰º‚·‚é‚Ì‚Å—vC³
	// Update‚ğoverride‚·‚ê‚Î‚¢‚¢
}

