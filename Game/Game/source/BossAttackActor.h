#pragma once
#include "appframe.h"

class BossAttackActor : public ActorClass{
public:
	BossAttackActor(ModeBase* mode);
	virtual ~BossAttackActor();
	void UpdateActor() override;

	void SetHitCollision(class HitCollisionComponent* hcol) { delete _HCollision; _HCollision = hcol; }
private:
	class HitCollisionComponent* _HCollision;
};

