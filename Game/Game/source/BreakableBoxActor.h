#pragma once
#include "appframe.h"

class BreakableBoxActor : public ActorClass
{
public:
	BreakableBoxActor(class ModeBase* mode, VECTOR pos, VECTOR scale);
	virtual ~BreakableBoxActor();

	void UpdateActor() override;

	class MoveCollisionComponent* GetMCollision() { return _MCollision; }

	void Init();

	void StartBreak();

private:
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;
	class ModelComponent* _Model;
	class ModelComponent* _Model2;
	int _AnimTotalTime;
	float _AnimCount;
	int _AnimIndex;
	float _Life;
};

