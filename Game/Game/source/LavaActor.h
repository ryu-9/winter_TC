#pragma once
#include "appframe.h"

class LavaActor : public ActorClass
{
public:
	LavaActor(class ModeBase* mode, VECTOR pos, VECTOR scale);
	~LavaActor();

private:
	ModelComponent* _Model;
	HitCollisionComponent* _HCollision;
};

