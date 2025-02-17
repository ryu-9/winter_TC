#pragma once
#include "appframe.h"

class StageBox : public ActorClass
{
public:
	StageBox(class ModeBase* mode);
	virtual ~StageBox();

	void UpdateActor() override;

	class MoveCollisionComponent* GetMCollision() { return _MCollision; }
	
	void Init();

private:
	class MoveCollisionComponent* _MCollision;
	std::deque<MV1_COLL_RESULT_POLY> _Poly;
};

