#pragma once
#include "AppFrame.h"

class MoveCollisionComponent : public	Component
{
	MoveCollisionComponent(class ActorClass* owner, int updateOrder = 100);
	~MoveCollisionComponent() override;
};

