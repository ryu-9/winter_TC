#pragma once
#include "appframe.h"

class LaserActor : public ActorClass
{
public:
	LaserActor(ModeBase* mode, VECTOR pos, VECTOR move, VECTOR rot, float scale);
	~LaserActor();
	void UpdateActor() override;


private:

	class MoveComponent* _Move;
	int _LifeTime;
	HitCollisionComponent* _HCollision;

};

