#pragma once
#include "appframe.h"

class ItemActor : public ActorClass
{
public:
	ItemActor(class ModeBase* mode, int type);
	virtual ~ItemActor();
	void UpdateActor() override;

	int GetType() const { return _Type; }

private:
	HitCollisionComponent* _HCollision;
	int _Type;
};

