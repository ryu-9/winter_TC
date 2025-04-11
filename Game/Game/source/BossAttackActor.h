#pragma once
#include "appframe.h"
#include "BossActor.h"

class BossAttackActor : public ActorClass{
public:
	BossAttackActor(ModeBase* mode, BossActor::ACTION type);
	virtual ~BossAttackActor();
	void UpdateActor() override;

	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }
private:
	class HitCollisionComponent* _HCollision;
	class MoveCollisionComponent* _MCollision;
	int _Time;					// ŠÔ
	BossActor::ACTION _Type;	// UŒ‚‚Ìí—Ş
	int _LifeTime;				// UŒ‚‚Ì‘±ŠÔ
};

