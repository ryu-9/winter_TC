#pragma once
#include "EnemyComponent.h"
class EBoxComponent : public EnemyComponent {
public:
	EBoxComponent(ActorClass* owner);
	~EBoxComponent();
	void ProcessInput() override;
	enum WAIT_ACTION {
		NON,
		MOVE,
	};

protected:
	bool Attack() override;
	bool Move() override;
	bool Roll();
	WAIT_ACTION _WaitAction;
	VECTOR _MoveDir;
};

