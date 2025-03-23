#include "LaserActor.h"
#include "EnemyActor.h"

LaserActor::LaserActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
{
	_Move = new MoveComponent(this, false);
	_Move->SetVelocity(move);
	SetPosition(pos);
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/Lazer_TDX.efkefc", VGet(0, 0, 0), rot, scale, true);

	//EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(sp);
	}
	_LifeTime = 2000;
	//auto model = new ModelComponent(this, "nomodel");
	float size = scale;
	size *= 3;
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 2, true, true);
	//auto model = new ModelComponent(this, "res/Stage/model/Cube.mv1");
	//MV1SetVisible(model->GetHandle(), FALSE);
}

LaserActor::~LaserActor()
{
}

void LaserActor::UpdateActor()
{
	_LifeTime -= GetMode()->GetStepTm();
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->Death();
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a,true);
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}
	}
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}
}
