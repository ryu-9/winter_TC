#pragma once
#include "appframe.h"

class ExplosionActor : public ActorClass
{
public:
	ExplosionActor(class ModeBase* mode, VECTOR pos, float scale = 1);
	virtual ~ExplosionActor();
	void UpdateActor() override;


private:
	int _LifeTime;
	HitCollisionComponent* _HCollision;
};

