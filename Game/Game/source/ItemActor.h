#pragma once
#include "appframe.h"

class ItemActor : public ActorClass
{
public:
	ItemActor(class ModeBase* mode, int type, int life = 7000);
	virtual ~ItemActor();
	void UpdateActor() override;

	int GetType() const { return _Type; }

private:
	HitCollisionComponent* _HCollision;
	int _Type;
	int _Life;
	ModelComponent* _Model;
};

