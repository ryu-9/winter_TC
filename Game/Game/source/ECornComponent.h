#pragma once
#include "EnemyComponent.h"
class ECornComponent : public EnemyComponent {
public:
	ECornComponent(ActorClass* owner);
	~ECornComponent();
	void ProcessInput() override;

	enum WAIT_ACTION {
		NON,
		MOVE,
		JUMP,
	};
protected:
	virtual bool Attack() override;

	bool Move() override;
	void Jump();

	WAIT_ACTION _WaitAction;

};

