#pragma once
#include "appframe.h"

class GoalItemActor : public ActorClass{
public:
	GoalItemActor(ModeBase* mode, VECTOR pos,bool flag = false);
	~GoalItemActor();
	void UpdateActor() override;

private:
	ModelComponent* _Model;
	HitCollisionComponent* _HCollision;
	int _AnimCount;
	float _PosY;
	bool _IsHitf;
	bool _Fallf;
};

