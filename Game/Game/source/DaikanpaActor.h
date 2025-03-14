#pragma once
#include "appframe.h"

class DaikanpaActor : public ActorClass
{
public:
	DaikanpaActor(class ModeBase* mode, ActorClass* owner, VECTOR pos, VECTOR dir, float size);
	virtual ~DaikanpaActor();
	void UpdateActor() override;

private:

	int _LifeTime;
	HitCollisionComponent* _HCollision;
	ActorClass* _Owner;
	VECTOR _Pos;
	VECTOR _Dir;

};

