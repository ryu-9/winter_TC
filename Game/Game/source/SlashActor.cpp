#include "SlashActor.h"
#include "EnemyActor.h"

SlashActor::SlashActor(ModeBase* mode, ActorClass* owner, VECTOR pos, VECTOR move, VECTOR rot, float scale)
	:ActorClass(mode)
	, _Owner(owner)
	, _Pos(pos)
{
	_Move = new MoveComponent(this, false);
	_Move->SetVelocity(move);
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	auto sp = new EffectSpriteComponent(this, "res/model/Sundercross/slash.efkefc", VGet(0, 0, 0), rot, scale);

	//EffectController::GetInstance()->GetShadowMap(0)->AddRemoveSprite(sp);
	auto sm = EffectController::GetInstance()->GetEffect<ShadowMapSpriteComponent>();
	if (sm.size() > 0) {
		sm[0]->AddRemoveSprite(sp);
	}
	_LifeTime = 2000;
	//auto model = new ModelComponent(this, "nomodel");
	float size = scale;
	size *= 5;
	int handle = ModelServer::GetInstance()->Add("res/Stage/model/Sundercross/slashHit.mv1");
	auto model = new ModelComponent(this, "res/Stage/model/Cube.mv1");
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(size, size, size), 6, true, true, handle);

	MV1SetVisible(model->GetHandle(), FALSE);
}

SlashActor::~SlashActor()
{
}

void SlashActor::UpdateActor()
{
	_LifeTime -= GetMode()->GetStepTm();
	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->SetState(State::eDead);
			auto a = new ActorClass(GetMode());
			a->SetPosition(enemy->GetPosition());
			auto s = new SoundComponent(a,true);
			s->SetSourceVoice(new SourceVoiceItem("KillEnemy"));
			s->Play(0);
			s->SetTimer(500);
		}
	}
	SetPosition(VAdd(_Pos, _Owner->GetPosition()));
	if (_LifeTime <= 0) {
		SetState(State::eDead);
	}

}
