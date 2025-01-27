#pragma once
#include "appframe.h"

class BulletComponent : public MoveComponent{
public:
	BulletComponent(class ActorClass* owner,VECTOR pos,float speed);
	virtual ~BulletComponent();

	void ProcessInput() override;
	void Update() override;

	void SetSpeed(float speed) { _Speed = speed; }
	void SetGoalPos(const VECTOR& pos) { _GoalPos = pos; }
	void UpdateDir();
private:
	float _Speed;
	VECTOR _Dir;
	VECTOR _StartPos;
	VECTOR _GoalPos;
	int _Cnt;
};

