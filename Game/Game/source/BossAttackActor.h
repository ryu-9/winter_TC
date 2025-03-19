#pragma once
#include "appframe.h"
#include "BossActor.h"

class BossAttackActor : public ActorClass{
	typedef BossActor::ACTION B_ACT;
public:
	BossAttackActor(ModeBase* mode, B_ACT type);
	virtual ~BossAttackActor();
	void UpdateActor() override;

	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }
private:
	class HitCollisionComponent* _HCollision;
	int _Time;
	B_ACT _Type;
	int _LifeTime;
};

