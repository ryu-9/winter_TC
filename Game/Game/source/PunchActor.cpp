#include "PunchActor.h"
#include "EnemyActor.h"

PunchActor::PunchActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
{
	_Move = new MoveComponent(this, false);	
	_Move->SetVelocity(move);
	SetPosition(pos);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Panch.efkefc", VGet(0, 0, 0), rot, scale, 100000);
	EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	_LifeTime = 2000;
	//auto model = new ModelComponent(this, "nomodel");
	float size = scale * 5;
	_MCollision = new MoveCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 2, true, true);
	size *= 5;
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 2, true, true);
}

PunchActor::~PunchActor()
{
}

void PunchActor::UpdateActor()
{
	_LifeTime -= FpsController::GetInstance()->GetDeltaTime();

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->SetState(State::eDead);
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a);
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}
	}
	/*

	*/


	if (_LifeTime <= 0 || _MCollision->GetCollResult().size()) {
		SetState(State::eDead);
	}
}
