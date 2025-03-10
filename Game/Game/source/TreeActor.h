#pragma once
#include "appframe.h"

class TreeActor : public ActorClass
{
public:
	TreeActor(class ModeBase* mode, VECTOR pos);
	virtual ~TreeActor();
	void UpdateActor() override;

	void SetItem(class ItemActor* item);

	void DropItem();

private:
	HitCollisionComponent* _HCollision;
	ModelComponent* _Model;
	std::vector<class ItemActor*> _Item;
};

