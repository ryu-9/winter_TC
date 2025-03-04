#pragma once
#include "appframe.h"

class PlayerMoveCollisionComponent : public MoveCollisionComponent
{
public:
	PlayerMoveCollisionComponent(class ActorClass* owner, class ModelComponent* model, VECTOR pos = VGet(0, 0, 0), VECTOR size = VGet(1, 1, 1)
		, int type = 0, bool move = FALSE, bool active = TRUE, int handle = -1);
	~PlayerMoveCollisionComponent();

	void Update() override;

private:
	class PlayerActor* _pOwner;
};

