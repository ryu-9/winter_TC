#include "BulletComponent.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

BulletComponent::BulletComponent(ActorClass* owner, VECTOR pos, float speed)
	:MoveComponent(owner)
	, _Speed(speed), _GoalPos(pos)
	, _Cnt(10000)
{
	_StartPos = owner->GetPosition();
	SetGravity(0.1);
	UpdateDir();
}

BulletComponent::~BulletComponent()
{
}

void BulletComponent::ProcessInput() {
	// ƒS[ƒ‹‚ÉŒü‚¯‚ÄˆÚ“®
	
	auto v = VScale(_Dir, (_Speed * _Owner->GetMode()->GetStepTm()) / 2000);
	SetVelocity(v);

}


void BulletComponent::Update() {

	auto mc = _Owner->GetComponent<MoveCollisionComponent>();
	if (mc.size() > 0) {
		if (mc[0]->GetCollResult().size() > 0) {
			_Speed = 0;
			SetVelocity(VGet(0, 0, 0));
		}
	}
	MoveComponent::Update();
	_Cnt -= _Owner->GetMode()->GetStepTm();
	if (_Cnt <= 0) {
		_Owner->SetState(ActorClass::State::eDead);
		return;
	}


	for (auto h : _Owner->GetComponent<HitCollisionComponent>()[0]->IsHit()) {
		auto player = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (player != nullptr) {
			player->Damage(0.1, 100);
			_Owner->SetState(ActorClass::State::eDead);
		}
	}
}

void BulletComponent::UpdateDir() {
	VECTOR dir = VSub(_GoalPos, _StartPos);
	_Dir = VNorm(dir);
	_Dir.y *= 0.5;
}