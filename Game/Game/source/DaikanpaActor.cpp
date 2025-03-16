#include "DaikanpaActor.h"
#include "EnemyActor.h"


DaikanpaActor::DaikanpaActor(ModeBase* mode, ActorClass* owner, VECTOR pos, VECTOR dir, float size)
	:ActorClass(mode)
	, _Owner(owner)
	, _Pos(pos)
	, _Dir(VScale(dir, -1))
	, _LifeTime(6000)
{
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	new EffectSpriteComponent(this, "res/model/Sundercross/Daikanpa_Thunder.efkefc", GetPosition(), dir, size);
	float Size = size;
	Size *= 10;
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(Size, Size, Size), 2, true, true);

}


DaikanpaActor::~DaikanpaActor()
{
}

void DaikanpaActor::UpdateActor()
{

	_LifeTime -= GetMode()->GetStepTm();
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	VECTOR old = VAdd(GetPosition(), VScale(_Dir, _Owner->GetSize().x * 5000));
	_HCollision->SetOldPosition(old);
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->SetState(State::eDead);
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a, true);
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}
	}
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}
}
