#include "BossAttackActor.h"
#include "PlayerActor.h"

BossAttackActor::BossAttackActor(ModeBase* mode, BossActor::ACTION type)
	: ActorClass(mode)
	, _HCollision(nullptr)
	, _Time(0)
	, _Type(type) {
	switch (type) {
	case BossActor::PUNCH:
		_LifeTime = 5000;
		break;
	case BossActor::BULLET:
		_LifeTime = 5000;
		break;
	case BossActor::BEAM:
		_LifeTime = 4000;
		break;
	case BossActor::PUNCH_FALL:
		_LifeTime = 5000;
		break;
	default:
		_LifeTime = 0;
		break;
	}

}

BossAttackActor::~BossAttackActor() {
}

void BossAttackActor::UpdateActor() {
	_Time += GetMode()->GetStepTm();
	if (_Type == B_ACT::BEAM) {
		if (_Time > 1200) {
			auto pos = _HCollision->GetPosition();
			pos.z -= 5000;
			_HCollision->SetOldPosition(pos);
		}
	}

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto player = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (player != nullptr) {
			player->Damage(0.3);
			if (_Type == B_ACT::PUNCH) {
				SetState(State::eDead);
			}
		}
	}
	
	if (_Time > _LifeTime) {
		SetState(State::eDead);
	}
}
