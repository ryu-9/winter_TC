#pragma once
#include "appframe.h"

class PunchActor : public ActorClass
{
public:
	PunchActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale);
	~PunchActor();
	void UpdateActor() override;


private:

	class MoveComponent* _Move;
	int _LifeTime;
	MoveCollisionComponent* _MCollision;
	HitCollisionComponent* _HCollision;

};

