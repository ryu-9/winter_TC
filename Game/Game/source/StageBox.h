#pragma once
#include "appframe.h"

class StageBox : public ActorClass
{
public:
	StageBox(class ModeBase* mode, VECTOR pos = VGet(0, 0, 0), VECTOR rot = VGet(0, 0, 0), VECTOR scale = VGet(1, 1, 1), int type = 0);
	virtual ~StageBox();

	void UpdateActor() override;

	class MoveCollisionComponent* GetMCollision() { return _MCollision; }
	class HitCollisionComponent* GetHCollision() { return _HCollision; }
	
	void Init();

	void StartBreak();

private:
	class MoveCollisionComponent* _MCollision;
	class HitCollisionComponent* _HCollision;
	class ModelComponent* _Model;
	int _AnimTotalTime;
	float _AnimCount;
	int _AnimIndex;
	float _Life;
	std::deque<MV1_COLL_RESULT_POLY> _Poly;
};

