#pragma once
#include "EnemyComponent.h"
#include "EnemyStruct.h"
class ECornComponent : public EnemyComponent {
public:
	ECornComponent(ActorClass* owner);
	~ECornComponent();
	virtual void ProcessInput() override;
	void SetAttackType(int n) { _AttackType = n; }
	void SetMoveData(Enemy::MOVE data) { _MoveData = data; }
	

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
	virtual bool CoolTime();

	virtual bool Move() override;
	virtual bool GoTo(int n);
	virtual void Jump();

	Enemy::MOVE _MoveData;
	WAIT_ACTION _WaitAction;
	int _AttackType;
	float _Rot;

};

