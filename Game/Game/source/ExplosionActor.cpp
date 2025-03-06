#include "ExplosionActor.h"
#include "EnemyActor.h"	

ExplosionActor::ExplosionActor(ModeBase* mode, VECTOR pos, float scale)
	:ActorClass(mode)
	, _LifeTime(10000)
{
	SetPosition(pos);
	VECTOR size = VScale(VGet(10, 10, 10), scale);
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), size, 2, true, true);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Explosion_Effect.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), scale);

}

ExplosionActor::~ExplosionActor()
{
}

void ExplosionActor::UpdateActor()
{
	_LifeTime -= FpsController::GetInstance()->GetDeltaTime();
	VECTOR size = _HCollision->GetRSize();
	size = VScale(size, (size.x + 10)/size.x);
	_HCollision->SetRSize(size);
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}
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
}