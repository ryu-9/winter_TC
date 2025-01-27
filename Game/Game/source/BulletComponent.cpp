#include "BulletComponent.h"
#include "EnemyActor.h"

BulletComponent::BulletComponent(ActorClass* owner, VECTOR pos, float speed)
	:MoveComponent(owner)
	, _Speed(speed), _GoalPos(pos)
	, _Cnt(10000)
{
	_StartPos = owner->GetPosition();

	UpdateDir();
}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::ProcessInput() {
	// ƒS[ƒ‹‚ÉŒü‚¯‚ÄˆÚ“®
	
	auto v = VScale(_Dir, (_Speed * _Owner->GetMode()->GetStepTm()) / 1000);
	SetVelocity(v);

}


void BulletComponent::Update() {
	// —Ž‰ºˆ—
	//// ‰¼‚Å’x‚­‚µ‚Ä‚é
	_Cnt -= _Owner->GetMode()->GetStepTm();
	if (_Cnt <= 0) {
		_Owner->SetState(ActorClass::State::eDead);
		return;
	}
	
	if (GetStand() == TRUE) {
		return;
	}
	if (GetStand() == FALSE) {
		SetVelocity(VAdd(GetVelocity(), VGet(0, -0.5, 0)));
	}
	SetStand(FALSE);
	SetOldPosition(_Owner->GetPosition());
	_Owner->SetPosition(VAdd(_Owner->GetPosition(), GetVelocity()));

	
}

void BulletComponent::UpdateDir() {
	VECTOR dir = VSub(_GoalPos, _StartPos);
	_Dir = VNorm(dir);
}