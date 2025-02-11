#pragma once
#include "appframe.h"

class StageBox : public ActorClass
{
public:
	StageBox(class ModeBase* mode);
	virtual ~StageBox();

	void UpdateActor() override;

	class MoveCollisionComponent* GetMCollision() { return _MCollision; }

private:
	class MoveCollisionComponent* _MCollision;
};

