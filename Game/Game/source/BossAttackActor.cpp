#include "BossAttackActor.h"
#include "PlayerActor.h"
#include "SnowComponent.h"

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
	_MCollision = new MoveCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(500, 500, 500), 2, true, true);
	_MCollision->SetIsMove(false);

}

BossAttackActor::~BossAttackActor() {
}

void BossAttackActor::UpdateActor() {
	for (auto mc : _MCollision->GetCollResult()) {
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			s->AddMoveCollision2(_MCollision);
		}
	}
	_Time += GetMode()->GetStepTm();
	if (_Type == B_ACT::BEAM) {
		if (_Time > 1200) {
			auto pos = _HCollision->GetPosition();
			pos.z -= 5000;
			_HCollision->SetOldPosition(pos);
		}
	}
	
	if (_Time > _LifeTime) {
		SetState(State::eDead);
	}
}
