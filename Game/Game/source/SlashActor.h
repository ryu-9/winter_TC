#pragma once
#include "appframe.h"

class SlashActor : public ActorClass
{
public:
	SlashActor(class ModeBase* mode, ActorClass* owner, VECTOR pos, VECTOR move, VECTOR rot, float scale);
	~SlashActor();
	void UpdateActor() override;

private:
	class MoveComponent* _Move;
	int _LifeTime;
	HitCollisionComponent* _HCollision;
	MoveCollisionComponent* _MCollision;
	ActorClass* _Owner;
	VECTOR _Pos;
};

