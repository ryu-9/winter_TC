#pragma once
#include "EnemyComponent.h"
class ECornComponent : public EnemyComponent {
public:
	ECornComponent(ActorClass* owner);
	~ECornComponent();
	virtual void ProcessInput() override;

	enum WAIT_ACTION {
		NON,
		MOVE,
		JUMP,
	};
protected:
	virtual bool Attack() override;

	virtual bool Move() override;
	virtual void Jump();

	WAIT_ACTION _WaitAction;

};

