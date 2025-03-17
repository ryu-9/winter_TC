#pragma once
#include "appframe.h"

class DaikanpaActor : public ActorClass
{
public:
	DaikanpaActor(class ModeBase* mode, class PlayerActor* owner, VECTOR pos, VECTOR dir, float size);
	virtual ~DaikanpaActor();
	void UpdateActor() override;

private:

	int _LifeTime;
	HitCollisionComponent* _HCollision;
	class PlayerActor* _pOwner;
	VECTOR _Pos;
	VECTOR _Dir;

};

