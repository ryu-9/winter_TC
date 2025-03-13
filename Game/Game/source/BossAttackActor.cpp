#include "BossAttackActor.h"
#include "PlayerActor.h"

BossAttackActor::BossAttackActor(ModeBase* mode) 
	:ActorClass(mode)
{
}

BossAttackActor::~BossAttackActor() {
}

void BossAttackActor::UpdateActor() {
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto player = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (player != nullptr) {
			player->Damage(0.3);
		}
	}
	_Time += GetMode()->GetStepTm();
	if (_Time > 10000) {
		SetState(State::eDead);
	}
}
