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
	virtual bool Attack(int n);
	virtual bool NomalAttack();
	virtual bool JumpAttack();
	virtual bool FrontAttack();
	virtual bool BackAttack();

	virtual bool Move() override;
	virtual bool GoTo(int n);
	virtual void Jump();

	WAIT_ACTION _WaitAction;
	int _AttackType;

};

