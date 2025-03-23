#include "DaikanpaActor.h"
#include "EnemyActor.h"
#include "StageBox.h"
#include "BreakableBoxActor.h"
#include "PlayerActor.h"
#include "TreeActor.h"



DaikanpaActor::DaikanpaActor(ModeBase* mode, PlayerActor* owner, VECTOR pos, VECTOR dir, float size)
	:ActorClass(mode)
	, _pOwner(owner)
	, _Pos(pos)
	, _Dir(VScale(dir, -1))
	, _LifeTime(6000)
{
	SetPosition(VAdd(_Pos, _pOwner->GetPosition()));
	new EffectSpriteComponent(this, "res/model/Sundercross/Daikanpa_Thunder.efkefc", VGet(0, 0, 0), dir, size);
	float Size = size;
	Size *= 10;
	_HCollision = new HitCollisionComponent(this, nullptr, VGet(0, 400, 0), VGet(Size, Size, Size), 2, true, true);

	SoundServer::GetInstance()->Create(this, "daikanpa", "AttackSE", "daikanpa");

	SoundServer::GetInstance()->GetSourceVoice(this, "daikanpa")->Play();
}


DaikanpaActor::~DaikanpaActor()
{
}

void DaikanpaActor::UpdateActor()
{

	_LifeTime -= GetMode()->GetStepTm();
	SetPosition(VAdd(_Pos, _pOwner->GetPosition()));
	if (_LifeTime > 2000 && _LifeTime < 3750) {
		VECTOR old = VAdd(GetPosition(), VScale(_Dir, _pOwner->GetSize().x * 5000));
		_HCollision->SetOldPosition(old);
	}

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto enemy = dynamic_cast<EnemyActor*>(h->GetOwner());
		if (enemy != nullptr) {
			enemy->Death(1);
		}


		auto box = dynamic_cast<StageBox*>(h->GetOwner());
		if (box != nullptr) {
			box->StartBreak();
		}

		auto ice = dynamic_cast<BreakableBoxActor*>(h->GetOwner());
		if (ice != nullptr) {
			ice->StartBreak();
		}

		auto tree = dynamic_cast<TreeActor*>(h->GetOwner());
		if (tree != nullptr) {
			tree->DropItem();
			tree->SetState(State::eDead);
		}



	}
	if (_LifeTime <= 0||_pOwner->GetModeNum() <=0) {
		SetState(State::eDead);
	}
}
