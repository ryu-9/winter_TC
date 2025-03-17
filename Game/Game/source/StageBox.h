#pragma once
#include "appframe.h"

class StageBox : public ActorClass
{
public:
	StageBox(class ModeBase* mode);
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

